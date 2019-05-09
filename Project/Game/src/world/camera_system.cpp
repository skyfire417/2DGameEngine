#include "world/camera_system.h"

void CameraSystem::ResetAndInit() {
    level_manager_->ResetLevel();
    cur_tilemap_->InitTiles(level_manager_->ShareCurrentLevel());
    std::pair<int, int> map_dimen = cur_tilemap_->tiles_dimen();
    border_manager_->ResetPosition({0, 0, map_dimen.first, map_dimen.second});
    player_->SetPos(cur_tilemap_->PlayerInitRelPosMap());
}

void CameraSystem::CenterCamera() {
    cam_relpos_map_ = {
        player_->pos_x() -
            (cam_dimen_.first - player_->GetTextureDimen().first) / 2,
        player_->pos_y() -
            (cam_dimen_.second - player_->GetTextureDimen().second) / 2};
    // Keep camera in bound
    auto border_boundary = border_manager_->boundary();
    if (cam_relpos_map_.x < border_boundary.x) {
        cam_relpos_map_.x = border_boundary.x;
    } else if (cam_relpos_map_.x >
               border_boundary.PosTopRight().x - cam_dimen_.first) {
        cam_relpos_map_.x = border_boundary.PosTopRight().x - cam_dimen_.first;
    }
    if (cam_relpos_map_.y < border_boundary.y) {
        cam_relpos_map_.y = border_boundary.y;
    } else if (cam_relpos_map_.y >
               border_boundary.PosBottomLeft().y - cam_dimen_.second) {
        cam_relpos_map_.y =
            border_boundary.PosBottomLeft().y - cam_dimen_.second;
    }
}

void CameraSystem::Update(int ms_passed) {
    player_->Update(ms_passed);
    if (prev_tilemap_ != nullptr) prev_tilemap_->Update(ms_passed);
    cur_tilemap_->Update(ms_passed);
    CenterCamera();
    RemovePreviousTilemapIfOutOfCamera();
}

void CameraSystem::Render(int ms_passed) {
    Math::Vec2<double> pos_offset = Math::Vec2<double>(viewport_relpos_win_);
    auto tiles_orig_point =
        Math::Add(ConvertFromMapRelToViewportRelPos({0, 0}), pos_offset);

    if (prev_tilemap_ != nullptr) {
        prev_tilemap_->Render(tiles_orig_point, ms_passed);
    }
    cur_tilemap_->Render(tiles_orig_point, ms_passed);
    player_->Render(
        Math::Add(ConvertFromMapRelToViewportRelPos(player_->GetPos()),
                  pos_offset),
        ms_passed);
}

void CameraSystem::MoveToNextLevel(World& world,
                                   std::shared_ptr<TileManager> new_tilemap) {
    level_manager_->IncLevel();

    RemovePreviousTilemapIfOutOfCamera();
    prev_tilemap_ = cur_tilemap_;
    cur_tilemap_ = new_tilemap;
    cur_tilemap_->InitTiles(level_manager_->ShareCurrentLevel());
    std::pair<int, int> cur_map_dimen = cur_tilemap_->tiles_dimen();
    std::pair<int, int> prev_map_dimen = prev_tilemap_->tiles_dimen();
    int cur_tilemap_offset_y = prev_map_dimen.second - cur_map_dimen.second;
    cur_tilemap_->AdjustAllObjectsPosition(
        Math::Vec2<double>(prev_map_dimen.first, cur_tilemap_offset_y));
    border_manager_->ResetPosition({0, std::min(0, cur_tilemap_offset_y),
                                    TileMapDimen().first,
                                    TileMapDimen().second});
}

void CameraSystem::RemovePreviousTilemapIfOutOfCamera() {
    if (prev_tilemap_ != nullptr &&
        cam_relpos_map_.x >= prev_tilemap_->tiles_dimen().first) {
        // Remove previous tilemap
        auto adjustment = Math::Scale(cur_tilemap_->tiles()[0]->GetPos(), -1.0);
        cur_tilemap_->AdjustAllObjectsPosition(adjustment);
        prev_tilemap_->Clear();
        prev_tilemap_ = nullptr;
        // Adjust player's position in map
        player_->AdjustPos(adjustment);
        // Recenter camera
        CenterCamera();
        // Reset borders
        border_manager_->ResetPosition(
            {0, 0, TileMapDimen().first, TileMapDimen().second});
    }
}
