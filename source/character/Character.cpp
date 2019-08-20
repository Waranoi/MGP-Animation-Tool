#include "Character.h"
#include <fstream>

using nlohmann::json;
using namespace CharacterTypes;

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

// Serialize Sprite Sheet
void CharacterTypes::to_json(json& j, const std::shared_ptr<SpriteSheet>& s)
{
    if (s.get()) {
        j["sourceLocation"] = s->sourceLocation;
        j["width"] = s->texDim.x;
        j["height"] = s->texDim.y;
        j["cell width"] = s->cellDim.x;
        j["cell height"] = s->cellDim.y;
    } else {
        j = nullptr;
    }
}

void CharacterTypes::from_json(const json& j, std::shared_ptr<SpriteSheet>& s)
{
    SpriteSheet *newSheet = new SpriteSheet();
    j.at("sourceLocation").get_to(newSheet->sourceLocation);
    j.at("width").get_to(newSheet->texDim.x);
    j.at("height").get_to(newSheet->texDim.y);
    j.at("cell width").get_to(newSheet->cellDim.x);
    j.at("cell height").get_to(newSheet->cellDim.y);

    std::shared_ptr<SpriteSheet> tempPtr(newSheet);
    s.swap(tempPtr);
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
    j["spriteSheets"] = c.spriteSheets;
    j["animations"] = c.animations;
}

void CharacterTypes::from_json(const json& j, Character& c)
{
    j.at("version").get_to(c.version);
    j.at("fps").get_to(c.fps);
    j.at("name").get_to(c.name);
    j.at("spriteSheets").get_to(c.spriteSheets);
    j.at("animations").get_to(c.animations);
}

Version CharacterTypes::LatestCharacterVersion()
{
    return { 1, 0, 0 };
}

CharacterTypes::Character CharacterTypes::LoadCharacter(std::string character)
{
    std::ifstream infile(character);
    nlohmann::json j;
    infile >> j;
    infile.close();
    Character c = j;

    std::string rootDir(character);
    const size_t last_slash_idx_root = rootDir.find_last_of("\\/");
    if (std::string::npos != last_slash_idx_root)
    {
        rootDir.erase(rootDir.begin() + last_slash_idx_root + 1, rootDir.end());
    }
    std::replace(rootDir.begin(), rootDir.end(), '\\', '/');

    for (int i = 0; i < c.spriteSheets.size(); i++)
    {
        // Create render object
        c.spriteSheets[i]->texQuadObj = TexturedQuad::CreateQuad(rootDir + c.spriteSheets[i]->sourceLocation);
    }

    return c;
}