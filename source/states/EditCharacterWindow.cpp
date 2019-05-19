#include "EditCharacterWindow.h"
#include "StateMediator.h"
#include "ConsoleUtils.h"
#include <stdio.h>
#include <fstream>

#define NONE -1

using namespace CharacterTypes;
using namespace ValidateCharacter;

Character EditCharacterWindow::character;
std::string EditCharacterWindow::saveLocation("");

void EditCharacterWindow::EditCharacterState(Character target, std::string targetFilepath)
{
    character = target;
    saveLocation = targetFilepath;

    printf("\nState: Edit Character\n");
    printf("A: Select animation\n");
    printf("S: Select sprite sheet\n");
    printf("N: Create new animation or sprite sheet\n");
    printf("L: List all sprite sheets and animations\n\n");

    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
        {
            // Select animation and go to EditAnimationState
            printf("Select animation (-1 to cancel) --> ");
            int selected = ConsoleUtils::GetIntegerInput(-1, character.animations.size()-1);
            if (selected != -1)
                EditAnimationState(selected, 0);
            else
                printf("Cancelled select animation\n\n");
        }
        else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            // Select sprite sheet and go to EditSpriteSheetState
            printf("Select sprite sheet (-1 to cancel) --> ");
            int selected = ConsoleUtils::GetIntegerInput(-1, character.spriteSheets.size()-1);
            if (selected != -1)
                EditSpriteSheetState(selected);
            else
                printf("Cancelled select sprite sheet\n\n");
        }
        else if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            // Create a new sprite sheet or animation and go to either EditAnimationState or EditSpriteSheetState
            printf("-1: Cancel\n0: Add a new animation\n1: Add a new sprite sheet\n");
            int selected = ConsoleUtils::GetIntegerInput(-1, 1);
            switch (selected)
            {
                case -1:
                    printf("Cancelled create new resource\n\n");
                    return;
                case 0:
                {
                    Animation animation;
                    animation.name = "animation" + std::to_string(character.animations.size());

                    character.animations.emplace_back(animation);
                    SaveCharacter();
                    EditAnimationState(character.animations.size()-1, 0);
                    break;
                }
                case 1:
                {
                    SpriteSheet *newSheet = new SpriteSheet();
                    newSheet->name = "sheet" + std::to_string(character.spriteSheets.size());
                    newSheet->sourceLocation = "none";
                    newSheet->data = nullptr;
                    newSheet->size = 0;

                    character.spriteSheets.emplace_back(newSheet);
                    SaveCharacter();
                    EditSpriteSheetState(character.spriteSheets.size()-1);
                    break;
                }
            }
        }
        else if (key == GLFW_KEY_L && action == GLFW_PRESS)
        {
            // List all sprite sheets and animations
            printf("List of sprite sheets:\n");
            for (int i = 0; i < character.spriteSheets.size(); i++)
            {
                printf("%d. %s\n", i, character.spriteSheets[i]->name.c_str());
            }

            printf("----\nList of animations:\n");
            for (int i = 0; i < character.animations.size(); i++)
            {
                printf("%d. %s\n", i, character.animations[i].name.c_str());
            }
            printf("\n");
        }
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditSpriteSheetState(int spriteSheet)
{
    if (!ValidateSpriteSheet(character, spriteSheet))
        return;

    printf("\nState: Edit Sprite Sheet\n");
    printf("R: Remove sprite sheet\n");
    printf("ESC: Go to Edit Character state\n\n");

    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [spriteSheet] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            // Delete the selected sprite sheet
            character.spriteSheets.erase(character.spriteSheets.begin() + spriteSheet);
            SaveCharacter();
            EditCharacterState(character, saveLocation);
        }
        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            // Go to EditCharacterState
            EditCharacterState(character, saveLocation);
        }
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::PlayAnimationState(int animation)
{
    if (!ValidateAnimation(character, animation))
        return;

    printf("\nState: Play Animation\n");
    printf("E: Edit animation\n");
    printf("ESC: Go to Edit Character state\n\n");

    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [animation] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            // This is a bug TODO
            EditAnimationState(animation, 0);
        }
        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            // Go to EditCharacterState
            EditCharacterState(character, saveLocation);
        }
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditAnimationState(int animation, int sprite)
{
    // Make sure the animation exists
    if (!ValidateAnimation(character, animation))
        return;

    // If sprite list is empty, set selected sprite to NONE, otherwise check if sprite exists
    if (character.animations[animation].sprites.empty())
        sprite = NONE;
    else if (!ValidateSprite(character, animation, sprite))
        return;
        
    printf("\nState: Edit Animation\n");
    printf("P: Playback animation\n");
    printf("E: Select sprite\n");
    printf("Right arrow: Next sprite\n");
    printf("Left arrow: Prev sprite\n");
    printf("N: Create new sprite\n");
    printf("R: Remove animation\n");
    printf("ESC: Go to Edit Character state\n\n");
    
    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [animation, sprite] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            // Animate selected animation
            PlayAnimationState(animation);
        }
        else if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            // Edit selected sprite
            EditSpriteState(animation, sprite, 0);
        }
        else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        {
            // Select next sprite
            int next = sprite + 1;
            if (next >= character.animations[animation].sprites.size())
                next = 0;
            EditAnimationState(animation, next);
        }
        else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        {
            // Select previous sprite
            int prev = sprite - 1;
            if (prev <= 0)
                prev = character.animations[animation].sprites.size()-1;
            EditAnimationState(animation, prev);
        }
        else if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            // Create a new sprite
            int next = character.animations[animation].sprites.size();
            character.animations[animation].sprites.emplace_back();
            SaveCharacter();
            EditAnimationState(animation, next);
        }
        else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            // Delete the selected animation
            character.animations.erase(character.animations.begin() + animation);
            SaveCharacter();
            EditCharacterState(character, saveLocation);
        }
        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            // Go to EditCharacterState
            EditCharacterState(character, saveLocation);
        }
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [animation, sprite] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditSpriteState(int animation, int sprite, int hitbox)
{
    // Make sure the sprite exists
    if (!ValidateSprite(character, animation, sprite))
        return;

    // If hitbox list is empty, set selected hitbox to NONE, otherwise check if hitbox exists
    if (character.animations[animation].sprites[sprite].hitboxes.empty())
        hitbox = NONE;
    else if (!ValidateHitbox(character, animation, sprite, hitbox))
        return;
        
    printf("\nState: Edit Sprite\n");
    printf("E: Select hitbox\n");
    printf("Right arrow: Next hitbox\n");
    printf("Left arrow: Prev hitbox\n");
    printf("N: Create new hitbox\n");
    printf("R: Remove sprite\n");
    printf("ESC: Go to Edit Animation state\n\n");

    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [animation, sprite, hitbox] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
             // Edit selected hitbox
            EditHitboxState(animation, sprite, hitbox);
        }
        else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        {
            // Select next hitbox
            int next = hitbox + 1;
            if (next >= character.animations[animation].sprites[sprite].hitboxes.size())
                next = 0;
            EditSpriteState(animation, sprite, next);
        }
        else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        {
            // Select previous hitbox
            int prev = hitbox - 1;
            if (prev <= 0)
                prev = character.animations[animation].sprites[sprite].hitboxes.size()-1;
            EditSpriteState(animation, sprite, prev);
        }
        else if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            // Create a new hitbox
            int next = character.animations[animation].sprites[sprite].hitboxes.size();
            character.animations[animation].sprites[sprite].hitboxes.emplace_back();
            SaveCharacter();
            EditSpriteState(animation, sprite, next);
        }
        else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            // Delete the selected sprite
            character.animations[animation].sprites.erase(character.animations[animation].sprites.begin() + sprite);
            SaveCharacter();

            if (sprite < character.animations[animation].sprites.size())
                EditAnimationState(animation, sprite);
            else
                EditAnimationState(animation, sprite-1);
        }
        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            // Go to EditAnimationState
            EditAnimationState(animation, sprite);
        }
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditHitboxState(int animation, int sprite, int hitbox)
{
    if (!ValidateHitbox(character, animation, sprite, hitbox))
        return;
        
    printf("\nState: Edit Hitbox\n");
    printf("Left click: Drag the corners to change the dimension\n");
    printf("Left click: Drag the center to change the position\n");
    printf("T: Set hitbox type\n");
    printf("R: Remove hitbox\n");
    printf("ESC: Go to Edit Sprite state\n\n");

    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [animation, sprite, hitbox] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_T && action == GLFW_PRESS)
        {
            // Set the type of hitbox
            printf("-1: Cancel\n0: Hitbox\n1: Hurtbox\n");
            int selected = ConsoleUtils::GetIntegerInput(-1, 1);
            switch (selected)
            {
                case -1:
                    printf("Cancelled set hitbox type\n\n");
                    return;
                case 0:
                    character.animations[animation].sprites[sprite].hitboxes[hitbox].type = HitboxType::HITBOX;
                    SaveCharacter();
                    break;
                case 1:
                    character.animations[animation].sprites[sprite].hitboxes[hitbox].type = HitboxType::HURTBOX;
                    SaveCharacter();
                    break;
            }
        }
        else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            // Delete the selected hitbox
            character.animations[animation].sprites[sprite].hitboxes.erase(character.animations[animation].sprites[sprite].hitboxes.begin() + hitbox);
            SaveCharacter();

            if (hitbox < character.animations[animation].sprites[hitbox].hitboxes.size())
                EditSpriteState(animation, sprite, hitbox);
            else
                EditSpriteState(animation, sprite, hitbox-1);
        }
        else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            // Go to EditAnimationState
            EditSpriteState(animation, sprite, hitbox);
        }
    };

    // Mouse button action event for this State.
    MouseBtnCallback newMouseBtnEvent = [] (GLFWwindow* window, int button, int action, int mods) {
    
    };

    // Mouse move event for this State.
    MousePosCallback newMousePosEvent = [] (GLFWwindow* window, double xpos, double ypos) {
    
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, newMouseBtnEvent, newMousePosEvent, newDrawEvent);
}

void EditCharacterWindow::SaveCharacter()
{
    nlohmann::json j = character;
    std::ofstream outfile;
    outfile.open(saveLocation, std::ios::out | std::ios::trunc);
    outfile << j.dump(4);
    outfile.close();
}

bool ValidateCharacter::ValidateSpriteSheet(Character character, int spriteSheet)
{
    if (spriteSheet < 0 || spriteSheet >= character.spriteSheets.size())
    {
        printf("Selected sprite sheet does not exist!\n");
        return false;
    }
    else
        return true;
}

bool ValidateCharacter::ValidateAnimation(Character character, int animation)
{
    if (animation < 0 || animation >= character.animations.size())
    {
        printf("Selected animation does not exist!\n");
        return false;
    }
    else
        return true;
}

bool ValidateCharacter::ValidateSprite(Character character, int animation, int sprite)
{
    if (animation < 0 || animation >= character.animations.size())
    {
        printf("Selected animation does not exist!\n");
        return false;
    }
    else if (sprite < 0 || sprite >= character.animations[animation].sprites.size())
    {
        printf("Selected sprite does not exist!\n");
        return false;
    }
    else
        return true;
}

bool ValidateCharacter::ValidateHitbox(Character character, int animation, int sprite, int hitbox)
{
    if (animation < 0 || animation >= character.animations.size())
    {
        printf("Selected animation does not exist!\n");
        return false;
    }
    else if (sprite < 0 || sprite >= character.animations[animation].sprites.size())
    {
        printf("Selected sprite does not exist!\n");
        return false;
    }
    else if (hitbox < 0 || hitbox >= character.animations[animation].sprites[sprite].hitboxes.size())
    {
        printf("Selected hitbox does not exist!\n");
        return false;
    }
    else
        return true;
}

#undef NONE