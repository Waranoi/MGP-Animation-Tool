#pragma once
#include <string>
#include <vector>
#include <memory>
#include "vmath.h"
// Convert to json https://github.com/nlohmann/json

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