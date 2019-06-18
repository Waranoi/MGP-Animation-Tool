#pragma once
#include "Character.h"
#include <iostream>

namespace EditCharacterWindow
{
    // The character being edited
    extern CharacterTypes::Character character;
    extern std::string rootDir;

    // The state used to enter this Window, the 'character' value will be replaced by the 'target' value.
    // Features: Edit name of character, edit fps of animations, create new animations/sprite sheets, print list of animations/sprite sheets, select animation/sprite sheet.
    // Connections: EditAnimationState(a -> enter animation index or n -> new animation), EditSpriteSheetState(s -> enter sprite sheet index or n -> new sprite)
    void EditCharacterState(CharacterTypes::Character target, std::string targetRootDir);

    // Features: Remove sprite sheet
    // Future features: Set horizontal and vertical reading direction of sprite sheet, set sprite sheet margins, set padding between cells
    // Connections: EditCharacterState(esc, 'r' to remove sprite sheet)
    void EditSpriteSheetState(int spriteSheet);

    // Features: Play animation, edit play state between one-pass and loop.
    // Connections: EditAnimationState(e), EditCharacterState(esc)
    void PlayAnimationState(int animation);

    // Features: Edit name of animation, change sprite sheet used, create new sprite, cycle through list of sprites, select sprite, remove animation
    // Connections: PlayAnimationState(p), EditSpriteState(e, 'n' for new sprite), EditCharacterState(esc, 'r' to remove animation)
    void EditAnimationState(int animation, int sprite);

    // Features: Change sprite sheet cell used, create new hitboxes, cycle through hitboxes, select hitbox, remove sprite
    // Connections: EditHitboxState(e, 'n' for new hitbox), EditAnimationState(esc, 'r' to remove sprite) 
    void EditSpriteState(int animation, int sprite, int hitbox);

    // Features: Change hitbox type, change hitbox dimensions, change hitbox position, remove hitbox
    // Connections: EditSpriteState(esc, 'r' to remove hitbox)
    void EditHitboxState(int animation, int sprite, int hitbox);

    void SaveCharacter();
};

// Makes sure that the input values given are valid
namespace ValidateCharacter
{
    bool ValidateSpriteSheet(CharacterTypes::Character character, int spriteSheet);
    bool ValidateAnimation(CharacterTypes::Character character, int animation);
    bool ValidateSprite(CharacterTypes::Character character, int animation, int sprite);
    bool ValidateHitbox(CharacterTypes::Character character, int animation, int sprite, int hitbox);
};