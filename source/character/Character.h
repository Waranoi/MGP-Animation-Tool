#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "vmath.h"
#include "../nlohmann/json.hpp"
#include "TexturedQuad.h"

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
        
        // Texture Quad Object
        TexturedQuad::TexQuadObj texQuadObj;

        // The hitboxes of this sprite
        std::vector<Hitbox> hitboxes;
    };

    // Filepath to the sprite sheet source is defined as the key for the sprite sheet in the Character struct
    struct SpriteSheet
    {
        // Width and height of sprite sheet
        Vector2i texDim;

        // Width and height of sprite cell
        Vector2i cellDim;

        // Texture Quad Object
        TexturedQuad::TexQuadObj texQuadObj;
    };

    struct Animation
    {
        // Name of the animation, should be unique to the character
        std::string name;
        
        // Key to the sprite sheet this animation depends on
        std::string spriteSheet;

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
        //std::vector<std::shared_ptr<SpriteSheet>> spriteSheets;

        std::map<std::string, SpriteSheet> spriteSheets;

        // The character's animations
        std::vector<Animation> animations;
    };

    // Serialize Version
    void to_json(nlohmann::json& j, const Version& v);
    void from_json(const nlohmann::json& j, Version& v);

    // Serialize Sprite Sheet
    void to_json(nlohmann::json& j, const SpriteSheet& s);
    void from_json(const nlohmann::json& j, SpriteSheet& s);

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
    Character LoadCharacter(std::string character);
};