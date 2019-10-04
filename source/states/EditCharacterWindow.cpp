#include "EditCharacterWindow.h"
#include "StateMediator.h"
#include "ConsoleUtils.h"
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <memory>
#include <windows.h>
#include <iterator>
extern "C"
{
    #include "tinyfiledialogs.h"
}

#define NONE -1

using namespace CharacterTypes;
using namespace ValidateCharacter;

Character EditCharacterWindow::character;
std::string EditCharacterWindow::rootDir("");

void EditCharacterWindow::EditCharacterState(Character target, std::string targetRootDir)
{
    character = target;
    rootDir = targetRootDir;

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
                    // List all sprite sheets to pick from
                    printf("List of sprite sheets:\n");
                    int spriteSheetIndex = 0;
                    for (auto it = character.spriteSheets.begin(); it != character.spriteSheets.end(); it++)
                    {
                        printf("%d. %s\n", spriteSheetIndex, it->first.c_str());
                        spriteSheetIndex++;
                    }
                    
                    printf("Set sprite sheet for animation (-1 to cancel) --> ");
                    int selected = ConsoleUtils::GetIntegerInput(-1, character.spriteSheets.size()-1);
                    if (selected != -1)
                    {
                        // Create animation
                        Animation animation;

                        // Set name of animation
                        animation.name = "animation" + std::to_string(character.animations.size());
                        
                        // Set sprite sheet of animation
                        auto spriteSheet = character.spriteSheets.begin();
                        std::advance(spriteSheet, selected); 
                        animation.spriteSheet = spriteSheet->first;
                        
                        // Save sprite sheet and go to state
                        character.animations.emplace_back(animation);
                        SaveCharacter();
                        EditAnimationState(character.animations.size()-1, 0);
                    }
                    else
                        printf("Cancelled create new animation\n\n");

                    break;
                }
                case 1:
                {
                    SpriteSheet newSpriteSheet;

                    // Select import target
                    char const * filterPatterns[2] = { "*.jpeg", "*.png" };
                    char const * addSpriteSheetFilepath = tinyfd_openFileDialog("Select import target", "", 2, filterPatterns, NULL, 0);

                    if (addSpriteSheetFilepath == NULL)
                    {
                        printf("Sprite sheet import aborted\n\n");
                        break;
                    }
                    
                    // Get filename, copy pasted from https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
                    std::string file(addSpriteSheetFilepath);
                    const size_t last_slash_idx = file.find_last_of("\\/");
                    if (std::string::npos != last_slash_idx)
                    {
                        file.erase(0, last_slash_idx + 1);
                    }

                    // Set sprite source
                    std::string sourceLocation = "sprite sheets/" + file;

                    // Check if a sprite sheet with this sourceLocation already exists
                    if (character.spriteSheets.find(sourceLocation) != character.spriteSheets.end())
                    {
                        printf("Can't import %s, a sprite sheet with this name already exists\n\n", file.c_str());
                        break;
                    }

                    // Check if imported image will conflict with existing image
                    std::ifstream copyTargetTest(rootDir + sourceLocation, std::ios::binary);
                    if (!copyTargetTest.good())
                    {
                        // create folders if they are missing
                        DWORD ftyp = GetFileAttributesA((rootDir + "sprite sheets").c_str());
                        if (ftyp == INVALID_FILE_ATTRIBUTES)
                        {
                            DWORD err = GetLastError();
                            if (err == ERROR_FILE_NOT_FOUND)
                            {
                                if (!CreateDirectoryA((rootDir + "sprite sheets").c_str(), NULL))
                                {
                                    // failed to create missing directory!
                                    printf("Aborting import while creating sprite sheets folder. Failed to create directory %s\n", (rootDir + "sprite sheets").c_str());
                                    break;
                                }
                            }
                            else
                            {
                                // Unexpected error
                                printf("Aborting import while looking for sprite sheets folder. Error code %lu\n", err);
                                break;
                            }
                        }
                        else if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            // this is not a directory!
                            printf("Aborting import while looking for sprite sheets folder. Something that is not a directory is occupying the sprite sheets folder name\n");
                            break;
                        }

                        // open image file
                        std::ifstream infile(addSpriteSheetFilepath, std::ios::binary);

                        // read data to buffer vector
                        std::vector<char> buffer(std::istreambuf_iterator<char>(infile), {});

                        //close image file
                        infile.close();

                        // save image locally in the mpgchar folder
                        std::ofstream copyTarget(rootDir + sourceLocation, std::ios::binary);
                        copyTarget.write(buffer.data(), buffer.size());
                        copyTarget.close();

                        // Create render object
                        newSpriteSheet.texQuadObj = TexturedQuad::CreateQuad(rootDir + sourceLocation);
                    }
                    else
                        printf("No sprite sheet named %s exists, but a sprite sheet source image with this name already exists.\nUsing existing source image for new sprite sheet.\n", file.c_str());
                
                    copyTargetTest.close();

                    // Create render object
                    newSpriteSheet.texQuadObj = TexturedQuad::CreateQuad(rootDir + sourceLocation);

                    printf("Set image width: ");
                    newSpriteSheet.texDim.x = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                    printf("Set image height: ");
                    newSpriteSheet.texDim.y = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                    printf("Set sprite sheet cell width: ");
                    newSpriteSheet.cellDim.x = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                    printf("Set sprite sheet cell height: ");
                    newSpriteSheet.cellDim.y = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                    printf("\n");
                    character.spriteSheets.insert(std::pair<std::string, SpriteSheet>(sourceLocation, newSpriteSheet));
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
            int spriteSheetIndex = 0;
            for (auto it = character.spriteSheets.begin(); it != character.spriteSheets.end(); it++)
            {
                printf("%d. %s\n", spriteSheetIndex, it->first.c_str());
                spriteSheetIndex++;
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

void EditCharacterWindow::EditSpriteSheetState(int spriteSheetIndex)
{
    if (!ValidateSpriteSheet(character, spriteSheetIndex))
        return;

    auto spriteSheet = character.spriteSheets.begin();
    std::advance(spriteSheet, spriteSheetIndex); 

    KeyboardCallback newKeyboardEvent = nullptr;
    DrawCallback newDrawEvent = nullptr;
    if (!spriteSheet->second.texQuadObj)
    {
        printf("Warning: Sprite sheet is broken\n");
        printf("R: Remove sprite sheet\n");
        printf("ESC: Go to Edit Character state\n\n");

        newKeyboardEvent = [spriteSheet] (GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_R && action == GLFW_PRESS)
            {
                // Delete the selected sprite sheet
                int err = std::remove((rootDir + spriteSheet->first).c_str());
                if (err == 0 || errno == ENOENT)
                {
                    // Remove sprite sheet if sprite sheet image was succesfully removed or if it didn't exist to begin with
                    character.spriteSheets.erase(spriteSheet);
                    SaveCharacter();
                    EditCharacterState(character, rootDir);
                }
                else
                {
                    // Failed to remove sprite sheet image
                    std::perror("Failed to remove sprite sheet");
                    printf("Error code: %d", err);
                }
            }
            else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                // Go to EditCharacterState
                EditCharacterState(character, rootDir);
            }
        };
    }
    else
    {
        printf("\nState: Edit Sprite Sheet\n");
        printf("I: Edit image source width and height\n");
        printf("C: Edit cell width and height\n");
        printf("R: Remove sprite sheet\n");
        printf("ESC: Go to Edit Character state\n\n");

        // Keyboard action event for this State.
        newKeyboardEvent = [spriteSheet] (GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_I && action == GLFW_PRESS)
            {
                printf("Set image width: ");
                spriteSheet->second.texDim.x = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                printf("Set image height: ");
                spriteSheet->second.texDim.y = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                // Save changes
                SaveCharacter();
            }
            else if (key == GLFW_KEY_C && action == GLFW_PRESS)
            {
                printf("Set sprite sheet cell width: ");
                spriteSheet->second.cellDim.x = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                printf("Set sprite sheet cell height: ");
                spriteSheet->second.cellDim.y = ConsoleUtils::GetIntegerInput(0, INT_MAX - 1);

                // Save changes
                SaveCharacter();
            }
            else if (key == GLFW_KEY_R && action == GLFW_PRESS)
            {
                // Delete the selected sprite sheet
                int err = std::remove((rootDir + spriteSheet->first).c_str());
                if (err == 0 || errno == ENOENT)
                {
                    // Remove sprite sheet if sprite sheet image was succesfully removed or if it didn't exist to begin with
                    character.spriteSheets.erase(spriteSheet);
                    SaveCharacter();
                    EditCharacterState(character, rootDir);
                }
                else
                {
                    // Failed to remove sprite sheet image
                    std::perror("Failed to remove sprite sheet");
                    printf("Error code: %d", err);
                }
            }
            else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                // Go to EditCharacterState
                EditCharacterState(character, rootDir);
            }
        };

        // Draw event for this State
        newDrawEvent = [spriteSheet] {
            TexturedQuad::InitQuadDrawing();
            TexturedQuad::BindQuad(spriteSheet->second.texQuadObj);
            TexturedQuad::DrawQuad();
        };
    }

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
            EditCharacterState(character, rootDir);
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
    
    auto spriteSheet = character.spriteSheets.find(character.animations[animation].spriteSheet);
    KeyboardCallback newKeyboardEvent = nullptr;
    DrawCallback newDrawEvent = nullptr;

    if (spriteSheet == character.spriteSheets.end() || !spriteSheet->second.texQuadObj)
    {
        printf("Warning: Referenced sprite sheet is broken\n");
        printf("\nState: Edit Animation\n");
        printf("R: Remove animation\n");
        printf("ESC: Go to Edit Character state\n\n");

        // Keyboard action event for this State.
        newKeyboardEvent = [animation, sprite, spriteSheet] (GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_R && action == GLFW_PRESS)
            {
                // Delete the selected animation
                character.animations.erase(character.animations.begin() + animation);
                SaveCharacter();
                EditCharacterState(character, rootDir);
            }
            else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                // Go to EditCharacterState
                EditCharacterState(character, rootDir);
            }
        };
    }
    else
    {
        printf("\nState: Edit Animation\n");
        printf("P: Playback animation\n");
        printf("E: Select sprite\n");
        printf("Right arrow: Next sprite\n");
        printf("Left arrow: Prev sprite\n");
        printf("N: Create new sprite\n");
        printf("R: Remove animation\n");
        printf("ESC: Go to Edit Character state\n\n");

        // Keyboard action event for this State.
        newKeyboardEvent = [animation, sprite, spriteSheet] (GLFWwindow* window, int key, int scancode, int action, int mods) {
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
                if (prev < 0)
                    prev = character.animations[animation].sprites.size()-1;
                EditAnimationState(animation, prev);
            }
            else if (key == GLFW_KEY_N && action == GLFW_PRESS)
            {
                // Create a new sprite
                int next = character.animations[animation].sprites.size();
                character.animations[animation].sprites.emplace_back();

                printf("Select sprite cell: ");
                character.animations[animation].sprites[next].cell = ConsoleUtils::GetIntegerInput(true);

                // Create texture quad for sprite
                character.animations[animation].sprites[next].texQuadObj = TexturedQuad::CreateQuad(
                    rootDir + spriteSheet->first, 
                    spriteSheet->second.texDim, 
                    spriteSheet->second.cellDim, 
                    character.animations[animation].sprites[next].cell);

                SaveCharacter();
                EditAnimationState(animation, next);
            }
            else if (key == GLFW_KEY_R && action == GLFW_PRESS)
            {
                // Delete the selected animation
                character.animations.erase(character.animations.begin() + animation);
                SaveCharacter();
                EditCharacterState(character, rootDir);
            }
            else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                // Go to EditCharacterState
                EditCharacterState(character, rootDir);
            }
        };

        // Draw event for this State
        newDrawEvent = [animation, sprite, spriteSheet] {
            if (sprite != NONE)
            {
                std::shared_ptr<TextureQuadObject> texQuadObj = character.animations[animation].sprites[sprite].texQuadObj;
                if (texQuadObj)
                {
                    TexturedQuad::InitQuadDrawing();
                    TexturedQuad::BindQuad(texQuadObj);
                    TexturedQuad::DrawQuad();
                }
            }
        };
    }

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
    std::string characterPath = rootDir + character.name + std::string(".mgpchar");
    nlohmann::json j = character;
    std::ofstream outfile;
    outfile.open(characterPath, std::ios::out | std::ios::trunc);
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