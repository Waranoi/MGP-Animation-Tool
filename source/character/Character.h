#pragma once
#include <string>
#include <vector>
#include <memory>
#include "vmath.h"
#include "../nlohmann/json.hpp"

namespace CharacterTypes
{
    enum HitboxType
    {
        HITBOX,
        HURTBOX
    };

    struct Version
    {
        int major;
        int minor;
        int patch;
    };

    struct Hitbox
    {
        HitboxType type;
        Aabb3f hitbox; 
    };

    struct Sprite
    {
        // The cell of the sprite sheet used for this sprite
        int cell;

        // The hitboxes of this sprite
        std::vector<Hitbox> hitboxes;
    };

    struct SpriteSheet
    {
        // Name of the sprite sheet, should be unique to the character
        std::string name;

        // Filepath to the sprite sheet source
        std::string sourceLocation;

        // Width of sprite sheet
        int width;
        
        // Height of sprite sheet
        int height;

        // Width of sprite cell
        int cellWidth;

        // Height of sprite cell
        int cellHeight;

        // Byte array and array size of the sprite sheet
        char *data;
        int size;
    };

    struct Animation
    {
        // Name of the animation, should be unique to the character
        std::string name;
        
        // Weak pointer to the sprite sheet this animation depends on. Validity of ptr should be checked regularly
        std::weak_ptr<SpriteSheet> spriteSheet;

        // The sprites in this animation
        std::vector<Sprite> sprites;
    };

    struct Character
    {
        // Which version of the .mgpa file this character is made with
        Version version;

        // The frame rate of this character's animations
        int fps;

        // Name of the character, should be unique
        std::string name;

        // The character's sprite sheets
        std::vector<std::shared_ptr<SpriteSheet>> spriteSheets;

        // The character's animations
        std::vector<Animation> animations;
    };

    // Serialize Version
    void to_json(nlohmann::json& j, const Version& v);
    void from_json(const nlohmann::json& j, Version& v);

    // Serialize Sprite Sheet
    void to_json(nlohmann::json& j, const std::shared_ptr<SpriteSheet>& s);
    void from_json(const nlohmann::json& j, std::shared_ptr<SpriteSheet>& s);

    // Serialize Hitbox
    void to_json(nlohmann::json& j, const Hitbox& h);
    void from_json(const nlohmann::json& j, Hitbox& h);

    // Serialize Sprite
    void to_json(nlohmann::json& j, const Sprite& s);
    void from_json(const nlohmann::json& j, Sprite& s);

    // Serialize Animation
    void to_json(nlohmann::json& j, const Animation& a);
    void from_json(const nlohmann::json& j, Animation& a);

    // Serialize Character
    void to_json(nlohmann::json& j, const Character& c);
    void from_json(const nlohmann::json& j, Character& c);

    // Return current version of the Character format
    Version LatestCharacterVersion();
};