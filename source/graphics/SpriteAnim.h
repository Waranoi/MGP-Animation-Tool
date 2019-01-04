#pragma once
#include <vector>
#include "vmath.h"

struct Animation
{
    // spriteSheet here
    std::vector<Sprite> sprites;
};

struct Sprite
{
    int cell;
    std::vector<Aabb3f> hitboxes;
};