#ifndef PLATFORMER_OBJECT_TILE_H
#define PLATFORMER_OBJECT_TILE_H

#include <functional>
#include <memory>
#include <unordered_set>
#include "object/collidable_object.h"

//! %Base class for a tile
/*!
 * A tile is an basic object that composes the platformer's game world
 */
class Tile : public CollidableObject {
   public:
    typedef std::string TileType;
    static const TileType kNullTileType;
    typedef std::string MarkType;
    static const MarkType kNullMarkType;

    //! Read configuration
    static void ReadConfiguration();

    //! Check whether is a mark
    static bool IsMarkType(std::string type);
    //! Check whether is a tile
    static bool IsTileType(std::string type);

    Tile(std::string tile_type_abbr)
        : CollidableObject(CollidableObject::Type::kObstacle,
                           ImageType::kStatic,
                           tile_image_ids_[tile_abbrs_[tile_type_abbr]]),
          tile_type_(tile_abbrs_[tile_type_abbr]) {
        SetOnCollisionFunc([](auto& o, auto v) {
            // Displaces the incoming object
            o.AdjustPos(Math::Scale(v, -1.0));
        });
        SetCollidObjDimen(GetTextureDimen());
    }

    Tile(std::string tile_type_abbr, std::string mark_type_abbr)
        : Tile(tile_type_abbr) {
        mark_type_ = mark_abbrs_[mark_type_abbr];
    }
    //! Returen tile type
    const TileType& tile_type() const { return tile_type_; }
    //! Return mark type
    const MarkType& mark_type() const { return mark_type_; }
    //! Set tile type
    void set_tile_type(TileType new_type) { tile_type_ = new_type; }
    //! Set mark type
    void set_mark_type(MarkType new_type) { mark_type_ = new_type; }

    //! Returns true if the object is collodable, false if otherwise
    bool IsCollidable();

    //! Render the updated position of the tile
    void Render(Math::Vec2<double> pos, int ms_passed) {
        CollidableObject::Render(pos, ms_passed);
    }

    //! Return the dimension of the texture, overriding the meethod in Object
    std::pair<int, int> GetTextureDimen() const;

    //! Overrides parent Object's image ID getter method
    /*!
     * To dynamically change image.
     */
    Image::ImageID image_id() const { return tile_image_ids_[tile_type_]; }

   private:
    static std::unordered_set<TileType> tile_types_;
    static std::unordered_set<MarkType> mark_types_;
    //! Lookup table for abbreviation of a tile on map
    static std::unordered_map<std::string, std::string> tile_abbrs_;
    //! Lookup table for abbreviation of a mark on map
    static std::unordered_map<std::string, std::string> mark_abbrs_;
    static std::unordered_map<TileType, Image::ImageID> tile_image_ids_;
    static std::unordered_set<TileType> collidable_tiles_;

    TileType tile_type_ {kNullTileType};
    MarkType mark_type_ {kNullMarkType};
};

#endif
