#include "object/tile_manager.h"
#include "utility.h"

double TileManager::coin_wobble_vel_;
double TileManager::coin_wobble_dist_;
double TileManager::slime_init_vel_;
double TileManager::fly_init_vel_;

void TileManager::InitTiles(std::shared_ptr<Level> level) {
    Clear();
    cur_level_ = level;
    auto tile_layout = level->tile_layout();
    for (int i = 0; i < tile_layout.size(); ++i) {
        std::string tile_str = tile_layout[i];
        assert(tile_str.size() > 0);
        auto tile_type_abbr = tile_str.substr(0, 1);
        std::string mark_type_abbr {""};
        if (tile_str.size() > 1) {
            mark_type_abbr = tile_str.substr(1, 1);
        }

        auto tile = CreateTile(i, tile_type_abbr, mark_type_abbr);
        int tile_width = tile->GetTextureDimen().first;
        int tile_height = tile->GetTextureDimen().second;

        auto mark_type = tile->mark_type();
        if (mark_type != Tile::kNullMarkType) {
            if (mark_type == "player_init_pos") {
                init_player_pos_ = TileRelPosMap(i, tile_width, tile_height);
            } else if (mark_type == "level_text") {
                level_text_pos_ = TileRelPosMap(i, tile_width, tile_height);
            } else if (mark_type == "coin_gold") {
                coins_.push_back(CreateCoin(i, tile_width, tile_height));
            } else if (mark_type == "enemy_slime" || mark_type == "enemy_fly") {
                patrol_enemies_.push_back(
                    CreatePatrolEnemy(mark_type, i, tile_width, tile_height));
            }
        }

        tiles_.push_back(tile);
    }
    int num_col = level->tile_num_col();
    tiles_dimen_.first = num_col * TileWidth();
    tiles_dimen_.second = tiles_.size() / num_col * TileHeight();
}

void TileManager::Update(int ms_passed) {
    for (auto &tile : tiles_) {
        tile->Update(ms_passed);
    }
    for (auto &coin : coins_) {
        coin->Update(ms_passed);
    }
    for (auto &patrol : patrol_enemies_) {
        patrol->Update(ms_passed);
    }
}

void TileManager::Render(Math::Vec2<double> start_point, int ms_passed) {
    auto offset = Math::Add(start_point, pos_offset_);

    RenderBackground(offset);

    RenderLevelText(offset);

    for (auto &tile : tiles_) {
        tile->Render(Math::Add(tile->GetPos(), start_point), ms_passed);
    }
    for (auto &coin : coins_) {
        coin->Render(Math::Add(coin->GetPos(), start_point), ms_passed);
    }
    for (auto &patrol : patrol_enemies_) {
        patrol->Render(Math::Add(patrol->GetPos(), start_point), ms_passed);
    }
}

std::vector<std::shared_ptr<CollidableObject>>
TileManager::ShareCollidableObjects() {
    std::vector<std::shared_ptr<CollidableObject>> collidables;
    for (auto &tile : tiles_) {
        if (tile->IsCollidable()) {
            collidables.push_back(tile);
        }
    }
    return collidables;
}

void TileManager::RenderBackground(Math::Vec2<double> start_point) {
    auto &image_provider = GetImageProvider();

    auto bg_id = cur_level_->background_img_id();
    auto bg_dimen = image_provider.GetImageDimen(bg_id);
    for (int x = start_point.x; x <= tiles_dimen_.first; x += bg_dimen.first) {
        for (int y = start_point.y; y <= tiles_dimen_.second;
             y += bg_dimen.second) {
            image_provider.RenderImage(bg_id, {(double)x, (double)y});
        }
    }
}

void TileManager::ReadConfiguration() {
    Json::Value root = GetConfigProvider().GetRoot();
    auto enemies = root["levels"]["shared"]["enemies"];
    slime_init_vel_ = enemies["slime"]["init_speed"].asDouble();
    fly_init_vel_ = enemies["fly"]["init_speed"].asDouble();
    auto coin = root["levels"]["shared"]["items"]["coin"];
    coin_wobble_vel_ = coin["wobble_speed"].asDouble();
    coin_wobble_dist_ = coin["wobble_distance"].asDouble();

    Tile::ReadConfiguration();
}

void TileManager::RenderLevelText(Math::Vec2<double> start_point) {
    Text &text_provider {GetTextProvider()};
    auto &renderer = GetImageProvider().GetRenderer();

    char level_text_buf[100];
    snprintf(level_text_buf, sizeof(level_text_buf), "LEVEL %02d",
             cur_level_->level_no());
    SDL2pp::Texture level_texture = SDL2pp::Texture(
        renderer, GetFontProvider()
                      .GetFont(Font::ID::kLevelText)
                      .RenderText_Blended(level_text_buf,
                                          SDL_Color {0xFF, 0xFF, 0xFF, 0}));
    renderer.Copy(
        level_texture, SDL2pp::NullOpt,
        SDL2pp::Rect(level_text_pos_.x + start_point.x,
                     level_text_pos_.y + start_point.y,
                     level_texture.GetWidth(), level_texture.GetHeight()));
}

Tile::TileType TileManager::GetTileType(int row, int col) {
    int index = row * cur_level_->tile_num_col() + col;
    assert(index < tiles_.size());
    return tiles_[index]->tile_type();
}

Tile::MarkType TileManager::GetTileMarkType(int row, int col) {
    int index = row * cur_level_->tile_num_col() + col;
    assert(index < tiles_.size());
    return tiles_[index]->mark_type();
}

void TileManager::MarkNextTileChange(std::string selection) {
    if (Tile::IsTileType(selection)) {
        next_tile_change_type_ = TileChangeType::kTile;
        next_tile_change_value_ = selection;
    } else if (Tile::IsMarkType(selection)) {
        next_tile_change_type_ = TileChangeType::kMark;
        next_tile_change_value_ = selection;
    } else {
        throw std::runtime_error("Unknown type: " + selection);
    }
}

void TileManager::SetTile(int row, int col, int mouse_x, int mouse_y) {
    if (next_tile_change_value_ == "") {
        return;
    }

    // First check if mouse click at any item or enemy
    if (next_tile_change_value_ == "none") {
        if (RemoveCoinAt({mouse_x, mouse_y})) {
            return;
        }
        if (RemovePatrolEnemyAt({mouse_x, mouse_y})) {
            return;
        }
    }

    int index = row * cur_level_->tile_num_col() + col;
    auto tile = tiles_[index];

    auto overridden_tile_type = GetTileType(row, col);
    auto overridden_mark_type = GetTileMarkType(row, col);
    if (overridden_tile_type != "none") {
        if (next_tile_change_type_ != TileChangeType::kTile) {
            fprintf(stderr,
                    "[TileManager][SetTile] Only another tile or none can "
                    "override a tile.\n");
            return;
        }
        // Remove the tile
        if (tile->IsCollidable()) {
            collision_->RemoveCollidableObject(*tile);
        }
        tiles_[index]->set_tile_type(next_tile_change_value_);
        // Add the tile
        if (tile->IsCollidable()) {
            collision_->AddCollidableObject(tile);
        }
    } else if (overridden_mark_type != "none") {
        if (next_tile_change_value_ != "none") {
            fprintf(stderr,
                    "[TileManager][SetTile] Only none can override a mark.\n");
            return;
        }
        // Remove the mark
        // player_init_pos or level_text cannot be removed here
        if (next_tile_change_value_ == "player_init_pos" ||
            next_tile_change_value_ == "level_text") {
            fprintf(stderr,
                    "[TileManager][SetTile] Mark 'player_init_pos' or "
                    "'level_text' can not be removed.\n");
            return;
        }
    } else {
        // Add whatever comes (mark/tile/none)
        if (next_tile_change_value_ != "none") {
            if (next_tile_change_type_ == TileChangeType::kTile) {
                tile->set_tile_type(next_tile_change_value_);
                if (tile->IsCollidable()) {
                    collision_->AddCollidableObject(tile);
                }
            } else {
                if (next_tile_change_value_ == "coin_gold") {
                    coins_.push_back(
                        CreateCoin(index, TileWidth(), TileHeight()));
                } else if (next_tile_change_value_ == "enemy_fly" ||
                           next_tile_change_value_ == "enemy_slime") {
                    patrol_enemies_.push_back(
                        CreatePatrolEnemy(next_tile_change_value_, index,
                                          TileWidth(), TileHeight()));
                }
            }
        }
    }
}

std::shared_ptr<Coin> TileManager::CreateCoin(int tile_map_index,
                                              int tile_width, int tile_height) {
    auto coin = std::make_shared<Coin>(coin_wobble_vel_, coin_wobble_dist_,
                                       "item_coin", tile_map_index);
    coin->AddOnCollisionFunc([this, coin](auto o, auto v) {
        if (o.CollidObjType() == CollidableObject::Type::kPlayer) {
            this->collision_->MarkCollidableObjectAsRemoved(coin.get());
        }
    });
    coin->SetPos(TileRelPosMap(tile_map_index, tile_width, tile_height));
    collision_->AddCollidableObject(coin);
    return coin;
}

bool TileManager::RemoveCoinAt(const Math::Vec2<int> point) {
    for (auto coin_it = coins_.begin(); coin_it != coins_.end(); ++coin_it) {
        auto coin = *coin_it;
        if (Math::DoRectContainPoint(coin->CollidObjRect(), point)) {
            collision_->RemoveCollidableObject(*coin);
            tiles_[coin->tile_map_index()]->set_mark_type(Tile::kNullMarkType);
            coins_.erase(coin_it);
            return true;
        }
    }
    return false;
}

std::shared_ptr<Tile> TileManager::CreateTile(int tile_map_index,
                                              std::string tile_type_abbr,
                                              std::string mark_type_abbr) {
    auto tile = std::make_shared<Tile>(tile_type_abbr, mark_type_abbr);
    int tile_width = tile->GetTextureDimen().first;
    int tile_height = tile->GetTextureDimen().second;
    // Positions relative to map are set here which are not the
    // same positions used to rendered in window
    tile->SetPos(TileRelPosMap(tile_map_index, tile_width, tile_height));
    if (tile->IsCollidable()) {
        collision_->AddCollidableObject(tile);
    }
    return tile;
}

std::shared_ptr<PatrolEnemy> TileManager::CreatePatrolEnemy(Tile::MarkType type,
                                                            int tile_map_index,
                                                            int tile_width,
                                                            int tile_height) {
    std::shared_ptr<PatrolEnemy> enemy;
    if (type == "enemy_slime") {
        enemy = std::make_shared<PatrolEnemy>(
            Math::Vec2<double>(slime_init_vel_, 0),
            slime_init_vel_ < 0 ? PatrolEnemy::MoveState::kNegative
                                : PatrolEnemy::MoveState::kPositive,
            "enemy_slime_walk", PatrolEnemy::MoveState::kNegative,
            tile_map_index);
    } else if (type == "enemy_fly") {
        enemy = std::make_shared<PatrolEnemy>(
            Math::Vec2<double>(0, fly_init_vel_),
            fly_init_vel_ < 0 ? PatrolEnemy::MoveState::kNegative
                              : PatrolEnemy::MoveState::kPositive,
            "enemy_fly_fly", PatrolEnemy::MoveState::kNegative, tile_map_index);
    }
    for (auto &func : enemy_collid_funcs_) {
        enemy->AddOnCollisionFunc(func);
    }
    int height_offset = tile_height - enemy->GetTextureDimen().second;
    enemy->SetPos(
        Math::Add(TileRelPosMap(tile_map_index, tile_width, tile_height),
                  {0, (double)height_offset}));
    collision_->AddCollidableObject(enemy);

    return enemy;
}

bool TileManager::RemovePatrolEnemyAt(const Math::Vec2<int> point) {
    for (auto enemy_it = patrol_enemies_.begin();
         enemy_it != patrol_enemies_.end(); ++enemy_it) {
        auto enemy = *enemy_it;
        if (Math::DoRectContainPoint(enemy->CollidObjRect(), point)) {
            collision_->RemoveCollidableObject(*enemy);
            tiles_[enemy->tile_map_index()]->set_mark_type(Tile::kNullMarkType);
            patrol_enemies_.erase(enemy_it);
            return true;
        }
    }
    return false;
}

void TileManager::ResizeMap(int new_row, int new_col) {
    int cur_col = cur_level_->tile_num_col();
    int cur_row = tiles_.size() / cur_col;

    if (new_row < cur_row) {
        for (int i = 0; i < cur_row - new_row; ++i) {
            for (int j = 0; j < cur_col; ++j) {
                auto tile = tiles_[j];
                collision_->MarkCollidableObjectAsRemoved(tile.get());
            }
            tiles_.erase(tiles_.begin(), tiles_.begin() + cur_col - 1);
        }
    } else {
    }
}

void TileManager::RemovePatrolEnemy(const PatrolEnemy &enemy) {
    for (auto it = patrol_enemies_.begin(); it != patrol_enemies_.end(); ++it) {
        if (it->get() == &enemy) {
            patrol_enemies_.erase(it);
            return;
        }
    }
}
