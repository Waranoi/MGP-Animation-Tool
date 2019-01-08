#include "Character.h"

using nlohmann::json;

// Serialize Version
void CharacterTypes::to_json(json& j, const Version& v)
{
    j["major"] = v.major;
    j["minor"] = v.minor;
    j["patch"] = v.patch;
}

void CharacterTypes::from_json(const json& j, Version& v)
{
    j.at("major").get_to(v.major);
    j.at("minor").get_to(v.minor);
    j.at("patch").get_to(v.patch);
}

// Serialize Hitbox
void CharacterTypes::to_json(json& j, const Hitbox& h)
{
    j["type"] = h.type;
    j["min"] = { h.hitbox.min.x, h.hitbox.min.y, h.hitbox.min.z };
    j["max"] = { h.hitbox.max.x, h.hitbox.max.y, h.hitbox.max.z };
}

void CharacterTypes::from_json(const json& j, Hitbox& h)
{
    j.at("type").get_to(h.type);

    std::vector<float> min, max;
    j.at("min").get_to(min);
    j.at("max").get_to(max);

    if (min.size() == 3 && max.size() == 3)
        h.hitbox = Aabb3f(min[0], min[1], min[2], max[0], max[1], max[2]);
    else
        ;// Todo: Print error
}

// Serialize Sprite
void CharacterTypes::to_json(json& j, const Sprite& s)
{
    j["cell"] = s.cell;
    j["hitboxes"] = s.hitboxes;
}

void CharacterTypes::from_json(const json& j, Sprite& s)
{
    j.at("cell").get_to(s.cell);
    j.at("hitboxes").get_to(s.hitboxes);
}

// Serialize Animation
void CharacterTypes::to_json(json& j, const Animation& a)
{
    j["name"] = a.name;
    j["sprites"] = a.sprites;
}

void CharacterTypes::from_json(const json& j, Animation& a)
{
    j.at("name").get_to(a.name);
    j.at("sprites").get_to(a.sprites);
}

// Serialize Character
void CharacterTypes::to_json(json& j, const Character& c)
{
    j["version"] = c.version;
    j["fps"] = c.fps;
    j["name"] = c.name;
    j["animations"] = c.animations;
}

void CharacterTypes::from_json(const json& j, Character& c)
{
    j.at("version").get_to(c.version);
    j.at("fps").get_to(c.fps);
    j.at("name").get_to(c.name);
    j.at("animations").get_to(c.animations);
}