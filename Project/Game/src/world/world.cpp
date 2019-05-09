#include "world/world.h"
#include <SDL.h>
#include <SDL2pp/Renderer.hh>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include "resource/font.h"
#include "resource/game_image.h"
#include "resource/image.h"
#include "resource/text.h"
#include "utility.h"

bool World::instantiated_ {false};

World::World(std::shared_ptr<Game> game, Math::Rect<int> area) : game_ {game} {
    assert(!instantiated_);

    area_ = area;
    area_info_ = {area.x, area.y, area.w, (int)(area.h * 0.07)};
    area_main_ = {area.x, area.y + area_info_.h, area.w, area.h - area_info_.h};

    camera_system_ = std::make_shared<CameraSystem>(
        cur_tilemap_, level_manager_, border_manager_, player_, area_main_);

    /// Set default collision handling
    collision_->ObserveCollision(CollidableObject::Type::kPlayer,
                                 CollidableObject::Type::kObstacle);
    collision_->ObserveCollision(CollidableObject::Type::kPlayer,
                                 CollidableObject::Type::kBorder);
    collision_->ObserveCollision(CollidableObject::Type::kPlayer,
                                 CollidableObject::Type::kCoin);
    collision_->ObserveCollision(CollidableObject::Type::kEnemy,
                                 CollidableObject::Type::kObstacle);
    collision_->ObserveCollision(CollidableObject::Type::kEnemy,
                                 CollidableObject::Type::kPlayer);
    // Top border is not an obstacle
    border_manager_->SetOnCollisionFunc(Border::Side::kTop,
                                        [](auto& o, auto v) {});
    // Player loses 1 life when touching bottom border
    border_manager_->SetOnCollisionFunc(
        Border::Side::kBottom, [this](auto& o, auto v) {
            if (o.CollidObjType() == CollidableObject::Type::kPlayer) {
                player_->Minus1LiveCount();
                if (player_->live_count() == 0) {
                    state_ = std::make_unique<LostGameState>();
                } else {
                    state_ = std::make_unique<LostLevelState>();
                }
            }
        });

    collision_->AddCollidableObject(
        border_manager_->ShareBorderCollidableObjects());

    collision_->AddCollidableObject(player_);

    cur_tilemap_->AddEnemyOnCollisionFunction([this](auto o, auto v) {
        if (o.CollidObjType() == CollidableObject::Type::kPlayer) {
            if (v.y > 0) {
                // Player is on top of this enemy
                player_->Jump();
            } else {
                player_->Minus1LiveCount();
                if (player_->live_count() == 0) {
                    state_ = std::make_unique<LostGameState>();
                } else {
                    state_ = std::make_unique<LostLevelState>();
                }
            }
        }
    });

    player_->AddOnCollisionFunc([this](auto& o, auto v) {
        if (o.CollidObjType() == CollidableObject::Type::kEnemy) {
            if (v.y < 0) {
                collision_->MarkCollidableObjectAsRemoved(&o);
                cur_tilemap_->RemovePatrolEnemy(dynamic_cast<PatrolEnemy&>(o));
            }
        }
    });

    instantiated_ = true;
}

Math::Rect<int> World::area() { return area_; }

void World::Update(int ms_passed) {
    auto next = state_->Update(*this, ms_passed);
    if (next != nullptr) {
        next->Enter(*this);
        state_ = std::move(next);
    }
}

std::unique_ptr<World::State> World::StartedState::Update(World& world,
                                                          int ms_passed) {
    world.camera_system_->Update(ms_passed);
    world.collision_->Update();

    if (world.CheckIfWinLevel()) {
        return std::make_unique<World::WonState>();
    }

    return nullptr;
}

std::unique_ptr<World::State> World::LostLevelState::Update(World& world,
                                                            int ms_passed) {
    world.player_->SetPos(world.cur_tilemap_->PlayerInitRelPosMap());
    world.camera_system_->Update(ms_passed);
    return std::make_unique<World::StartedState>();
}

bool World::CheckIfWinLevel() {
    return cur_tilemap_->CollectedCoinCount() == cur_tilemap_->TotalCoinCount();
}

void World::Render(int ms_passed, float fps) {
    SDL2pp::Renderer& renderer {GetImageProvider().GetRenderer()};
    Text& text_provider {GetTextProvider()};
    Text::Lang cur_lang = game_->lang();
    // Render background color
    renderer.SetDrawColor(0x00, 0x00, 0x00, 0xFF);
    renderer.FillRect(area_main_.toSDLRect());
    // Render tile map
    camera_system_->Render(ms_passed);
    // Render info area
    int padding_x = 10;
    renderer.SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
    renderer.FillRect(area_info_.toSDLRect());
    // Render lives count
    std::string live_count_text = Utility::ToUpper(text_provider.GetText(
                                      Text::ID::kLifeCountLabel, cur_lang)) +
                                  ":  " + std::to_string(player_->live_count());
    SDL2pp::Texture info_texture = SDL2pp::Texture(
        renderer,
        GetFontProvider()
            .GetFont(Font::ID::kInfo)
            .RenderText_Blended(live_count_text, SDL_Color {0, 0, 0, 0xFF}));
    renderer.Copy(
        info_texture, SDL2pp::NullOpt,
        SDL2pp::Rect(
            int(area_info_.x + padding_x + area_info_.w * 0.1),
            area_info_.y + (area_info_.h - info_texture.GetHeight()) / 2,
            info_texture.GetWidth(), info_texture.GetHeight()));

    // Render coin text
    char coin_text_buf[100];
    snprintf(coin_text_buf, sizeof(coin_text_buf), "%s: %02d/%02d",
             Utility::ToUpper(
                 text_provider.GetText(Text::ID::kCoinLabel, cur_lang).c_str())
                 .c_str(),
             cur_tilemap_->CollectedCoinCount(),
             cur_tilemap_->TotalCoinCount());
    SDL2pp::Texture coin_texture = SDL2pp::Texture(
        renderer,
        GetFontProvider()
            .GetFont(Font::ID::kInfo)
            .RenderText_Blended(coin_text_buf, SDL_Color {0, 0, 0, 0xFF}));
    renderer.Copy(
        coin_texture, SDL2pp::NullOpt,
        SDL2pp::Rect(
            int(area_info_.x + padding_x + area_info_.w * 0.2),
            area_info_.y + (area_info_.h - coin_texture.GetHeight()) / 2,
            coin_texture.GetWidth(), coin_texture.GetHeight()));

    // Render level text
    char level_text_buf[100];
    snprintf(level_text_buf, sizeof(level_text_buf), "%02d/%02d",
             level_manager_->CurrentLevelNo(), level_manager_->TotalLevel());
    std::string level_text = Utility::ToUpper(text_provider.GetText(
                                 Text::ID::kLevelNumLabel, cur_lang)) +
                             ": " + std::string(level_text_buf);
    SDL2pp::Texture level_texture = SDL2pp::Texture(
        renderer,
        GetFontProvider()
            .GetFont(Font::ID::kInfo)
            .RenderText_Blended(level_text, SDL_Color {0, 0, 0, 0xFF}));
    renderer.Copy(
        level_texture, SDL2pp::NullOpt,
        SDL2pp::Rect(
            area_info_.x + padding_x,
            area_info_.y + (area_info_.h - level_texture.GetHeight()) / 2,
            level_texture.GetWidth(), level_texture.GetHeight()));

    // Render FPS
    char fps_text_buf[100];
    snprintf(fps_text_buf, sizeof(fps_text_buf), "FPS: %03.2f", fps);
    SDL2pp::Texture fps_texture = SDL2pp::Texture(
        renderer,
        GetFontProvider()
            .GetFont(Font::ID::kInfo)
            .RenderText_Blended(fps_text_buf, SDL_Color {0, 0, 0, 0xFF}));
    renderer.Copy(
        fps_texture, SDL2pp::NullOpt,
        SDL2pp::Rect(
            area_info_.x + area_info_.w - fps_texture.GetWidth() - padding_x,
            area_info_.y + (area_info_.h - fps_texture.GetHeight()) / 2,
            fps_texture.GetWidth(), fps_texture.GetHeight()));

    state_->Render(*this);
}

void World::LostGameState::Render(World& world) {
    world.RenderGameLostMessage();
}

void World::WonState::Render(World& world) {
    if (world.level_manager_->IsFinalLevel()) {
        world.RenderGameWonMessage();
    } else {
        world.RenderLevelWonMessage();
    }
}

void World::ProcessInput(SDL_Event e) {
    auto next = state_->ProcessInput(*this, e);
    if (next != nullptr) {
        state_ = std::move(next);
    }
}

void World::ProcessMouseInput(SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int cameraTopX = camera_system_->GetCameraTop().x;
        int cameraTopY = camera_system_->GetCameraTop().y;
        double cameraPosX = camera_system_->GetCameraPos().x;
        double cameraPosY = camera_system_->GetCameraPos().y;
        int mouseX = e.button.x + cameraTopX + cameraPosX;
        int mouseY = e.button.y - cameraTopY + cameraPosY;
        int row = int(double(mouseY) / cur_tilemap_->TileHeight());
        int col = int(double(mouseX) / cur_tilemap_->TileWidth());

        cur_tilemap_->SetTile(row, col, mouseX, mouseY);
    }
}

Json::Value World::ProcessSocketCall(std::string action_name,
                                     Json::Value args) {
    if (action_name == "change_player_image") {
        if (args.isMember("choice")) {
            std::string choice = args["choice"].asString();
            try {
                Player::ChangeImageSet(choice);
            } catch (std::runtime_error& e) {
                return Utility::CreateSocketCallResponse(
                    false,
                    Utility::StringFormat("Runtime error: %s", e.what()));
            }
            return Utility::CreateSocketCallResponse(
                true, "Player image set to " + choice);
        }
        return Utility::CreateSocketCallResponse(
            false, "Wrong arguments, expecting 'choice'");
    } else if (action_name == "change_tile_map") {
        if (args.isMember("choice")) {
            std::string choice = args["choice"].asString();
            try {
                cur_tilemap_->MarkNextTileChange(choice);
            } catch (std::runtime_error& e) {
                return Utility::CreateSocketCallResponse(
                    false,
                    Utility::StringFormat("Runtime error: %s", e.what()));
            }
            return Utility::CreateSocketCallResponse(
                true, "Next tile change set to" + choice);
        }
        return Utility::CreateSocketCallResponse(
            false, "Wrong arguments, expecting 'choice'");
    }

    if (action_name == "restart_game_setting") {
        if (args.isMember("lives")) {
            int lives = args["lives"].asInt();
            Json::Value start_position = args["start_position"];
            int startX = start_position[0].asInt();
            int startY = start_position[1].asInt();
            std::vector<std::shared_ptr<Tile>> tiles_ =
                cur_tilemap_->GetTiles();
            int t_size = tiles_.size();
            int cols = level_manager_->CurrentLevel().tile_num_col();
            int rows = t_size / cols;
            int index = (rows - startY - 1) * cols + startX;

            try {
                player_->ChangeMaxLives(lives);
                for (auto tile : tiles_) {
                    if (tile->mark_type() == "player_init_pos") {
                        tile->set_mark_type(Tile::kNullMarkType);
                        break;
                    }
                }

                tiles_[index]->set_mark_type("player_init_pos");
                player_->SetPos(tiles_[index]->pos_x(), tiles_[index]->pos_y());

            } catch (std::runtime_error& e) {
                return Utility::CreateSocketCallResponse(
                    false,
                    Utility::StringFormat("Runtime error: %s", e.what()));
            }
            return Utility::CreateSocketCallResponse(true,
                                                     "Player image set to ");
        }

        return Utility::CreateSocketCallResponse(
            false, "Wrong arguments, expecting 'lives'");
    }

    if (action_name == "load_from_json") {
        if (args.isMember("load_json_path")) {
            Json::Value msg;
            std::istringstream(Utility::ReadFileToString(
                args["load_json_path"].asString().c_str(), "rb")) >>
                msg;
            int maxLives = msg["player"]["max_lives"].asInt();
            std::string player_image =
                msg["player"]["init_image_selected"].asString();
            auto map = msg["levels"]["map"][0];
            int num_col = map["num_col"].asInt();
            std::vector<std::string> tileSet;
            int t_size = map["tiles"].size();
            for (int i = 0; i < t_size; i++) {
                tileSet.push_back(map["tiles"][i].asString());
            }
            std::string background_image_id =
                map["background_img_id"].asString();

            auto level = std::make_shared<Level>(background_image_id, num_col,
                                                 tileSet, 0);
            cur_tilemap_->InitTiles(level);
            auto tiles = cur_tilemap_->tiles();
            Math::Vec2<double> init_player_pos;
            for (int i = 0; i < tiles.size(); ++i) {
                auto tile = tiles[i];
                if (tile->mark_type() == "player_init_pos") {
                    init_player_pos = cur_tilemap_->TileRelPosMap(
                        i, cur_tilemap_->TileWidth(),
                        cur_tilemap_->TileHeight());
                    break;
                }
            }

            player_->ChangeMaxLives(maxLives);
            Player::ChangeImageSet(player_image);
            player_->SetPos(init_player_pos);

            return Utility::CreateSocketCallResponse(true,
                                                     "Config file loaded.");
        }

        return Utility::CreateSocketCallResponse(
            false, "Wrong arguments, expecting 'load_json_path'");
    }

    if (action_name == "save_to_json") {
        if (args.isMember("save_json_path")) {
            std::string fullpath =
                args["save_json_path"].asString() + "/savefile.json";
            printf("path: %s\n", fullpath.c_str());
            std::vector<std::shared_ptr<Tile>> tiles_ =
                cur_tilemap_->GetTiles();
            int t_size = tiles_.size();

            try {
                Json::Value msg;
                Json::Value msgMap;

                msg["player"]["init_image_selected"] =
                    player_->GetImageSelected();
                msg["player"]["max_lives"] = player_->GetMaxLives();
                msgMap["background_img_id"] = "background_default";
                msgMap["num_col"] =
                    level_manager_->CurrentLevel().tile_num_col();

                for (int i = 0; i < t_size; i++) {
                    if (tiles_[i]->mark_type() == "player_init_pos")
                        msgMap["tiles"][i] = "0@";

                    if (tiles_[i]->tile_type() == "none" &&
                        tiles_[i]->mark_type() == "enemy_fly")
                        msgMap["tiles"][i] = "0f";

                    if (tiles_[i]->tile_type() == "none" &&
                        tiles_[i]->mark_type() == "enemy_slime")
                        msgMap["tiles"][i] = "0s";

                    if (tiles_[i]->tile_type() == "none" &&
                        tiles_[i]->mark_type() == "coin_gold")
                        msgMap["tiles"][i] = "0c";

                    if (tiles_[i]->tile_type() == "none" &&
                        tiles_[i]->mark_type() == "level_text")
                        msgMap["tiles"][i] = "0t";

                    if (tiles_[i]->tile_type() == "none" &&
                        tiles_[i]->mark_type() == "none")
                        msgMap["tiles"][i] = "0";

                    if (tiles_[i]->tile_type() == "box")
                        msgMap["tiles"][i] = "b";

                    if (tiles_[i]->tile_type() == "grass")
                        msgMap["tiles"][i] = "g";
                }

                msg["levels"]["map"][0] = msgMap;
                // to string
                Json::StreamWriterBuilder
                    builder;  // If you want whitespace-less output
                builder["indentation"] = "";
                const std::string output = Json::writeString(builder, msg);
                // write to file
                std::ofstream os(fullpath);
                if (os.is_open()) {
                    os << output;
                    os.close();
                }

            } catch (std::runtime_error& e) {
                return Utility::CreateSocketCallResponse(
                    false,
                    Utility::StringFormat("Runtime error: %s", e.what()));
            }
            return Utility::CreateSocketCallResponse(true, "Save to json");
        }
        return Utility::CreateSocketCallResponse(
            false, "Wrong arguments, expecting 'lives'");
    }
    if (action_name == "resize_tile_map") {
        if (args.isMember("size")) {
        }
        return Utility::CreateSocketCallResponse(
            false, "Wrong arguments, expecting 'size'");
    }

    return Utility::CreateSocketCallResponse(
        false,
        Utility::StringFormat("Unknown action name: %s", action_name.c_str()));
}

std::unique_ptr<World::State> World::StartedState::ProcessInput(World& world,
                                                                SDL_Event e) {
    world.player_input_.ProcessInput(e, *world.player_);
    return nullptr;
}

std::unique_ptr<World::State> World::LostGameState::ProcessInput(World& world,
                                                                 SDL_Event e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
        world.game_->Lose();
    }
    return nullptr;
}

std::unique_ptr<World::State> World::WonState::ProcessInput(World& world,
                                                            SDL_Event e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) {
        if (world.level_manager_->IsFinalLevel()) {
            world.game_->Win();
        } else {
            auto next_tilemap =
                std::make_shared<TileManager>(*world.cur_tilemap_);
            world.cur_tilemap_ = next_tilemap;
            world.camera_system_->MoveToNextLevel(world, next_tilemap);
            return std::make_unique<StartedState>();
        }
    }
    return nullptr;
}

void World::Start() {
    GetMixer().PlayMusic(GetAudioProvider().GetMusic(Audio::ID::kBackground),
                         -1);
    state_ = std::make_unique<World::StartedState>();
    state_->Enter(*this);
    camera_system_->ResetAndInit();
}

void World::RenderLevelEndMessage(std::string title, std::string body) {
    SDL2pp::Renderer& renderer {GetImageProvider().GetRenderer()};
    renderer.SetDrawColor(0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    int gap_between_lines = 20;
    int pos_x = area_main_.x;
    int pos_y = area_main_.y;

    // Render message area
    pos_x += (area_main_.w - area_win_lose_.w) / 2;
    pos_y += (area_main_.h - area_win_lose_.h) / 2;
    renderer.FillRect(
        SDL2pp::Rect(pos_x, pos_y, area_win_lose_.w, area_win_lose_.h));
    // title message
    SDL2pp::Texture title_texture = SDL2pp::Texture(
        renderer,
        GetFontProvider()
            .GetFont(Font::ID::kWinLose)
            .RenderText_Blended(title, SDL_Color {0, 0, 0, SDL_ALPHA_OPAQUE}));
    // Body message
    SDL2pp::Texture body_texture = SDL2pp::Texture(
        renderer,
        GetFontProvider()
            .GetFont(Font::ID::kWinLose)
            .RenderText_Blended(body, SDL_Color {0, 0, 0, SDL_ALPHA_OPAQUE}));
    // Calculate positions
    int pos_x_main = pos_x + (area_win_lose_.w - title_texture.GetWidth()) / 2;
    int pos_y_main = pos_y + (area_win_lose_.h - title_texture.GetHeight() -
                              gap_between_lines - body_texture.GetHeight()) /
                                 2;
    int pos_x_prompt = pos_x + (area_win_lose_.w - body_texture.GetWidth()) / 2;
    int pos_y_prompt =
        pos_y_main + title_texture.GetHeight() + gap_between_lines;

    // Render messages
    renderer.Copy(title_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(pos_x_main, pos_y_main, title_texture.GetWidth(),
                               title_texture.GetHeight()));
    renderer.Copy(
        body_texture, SDL2pp::NullOpt,
        SDL2pp::Rect(pos_x_prompt, pos_y_prompt, body_texture.GetWidth(),
                     body_texture.GetHeight()));
}

void World::RenderLevelWonMessage() {
    RenderLevelEndMessage(Utility::ToUpperFirst(GetTextProvider().GetText(
                              Text::ID::kLevelWinMsg, game_->lang())),
                          Utility::ToUpperFirst(GetTextProvider().GetText(
                              Text::ID::kNextLevelPrompt, game_->lang())));
}

void World::RenderGameWonMessage() {
    RenderLevelEndMessage(Utility::ToUpperFirst(GetTextProvider().GetText(
                              Text::ID::kGameWinMsg, game_->lang())),
                          Utility::ToUpperFirst(GetTextProvider().GetText(
                              Text::ID::kQuitGamePrompt, game_->lang())));
}

void World::RenderGameLostMessage() {
    RenderLevelEndMessage(Utility::ToUpperFirst(GetTextProvider().GetText(
                              Text::ID::kGameLoseMsg, game_->lang())),
                          Utility::ToUpperFirst(GetTextProvider().GetText(
                              Text::ID::kQuitGamePrompt, game_->lang())));
}
