#include "SelectCharacterWindow.h"
#include "EditCharacterWindow.h"
#include "Character.h"
#include "StateMediator.h"
#include <stdio.h>
#include <windows.h>
#include <fstream>
extern "C"
{
    #include "tinyfiledialogs.h"
}

using namespace CharacterTypes;

void SelectCharacterWindow::SelectCharacterState()
{
    printf("\nState: Select Character\n");
    printf("N: Create new Character\n");
    printf("L: Load an existing Character\n\n");

    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            // Select animation and go to EditAnimationState
            int parentFolderLen = 0;
            const char *parentFolder = tinyfd_selectFolderDialog("Character storage location", NULL);
            if (parentFolder == NULL)
            {
                printf("Folder selection cancelled or failed\n");
                return;
            }

            // maxLen is the max length for windows file paths
            std::string test;
            const int maxLen = 260;
            char name[maxLen];
            printf("Character name: ");
            if (scanf("%260s", name) < 0)
            {
                printf("Name selection failed\n");
                return;
            }

            // Get length of name
            int nameLen = strlen(name);

            // Copy parent folder path to character folder path
            int charFolderLen = 0;
            char charFolder[maxLen];
            for (int i = 0; i < maxLen; i++)
            {
                if (parentFolder[i] == '\0')
                {
                    charFolder[i] = '\\';
                    parentFolderLen = i + 1;
                    break;
                }
                else
                    charFolder[i] = parentFolder[i];
            }

            // Check that parent folder length + name length doesn't exceed max length
            if (parentFolderLen + nameLen > maxLen)
            {
                printf("Combination of path and character name is too long\n");
                return;
            }

            // Copy name to the end of the character path
            for (int i = 0; i < nameLen; i++)
            {
                charFolder[parentFolderLen + i] = name[i];
            }

            // Handle create directory failing
            if (!CreateDirectoryA(charFolder, NULL))
            {
                if (GetLastError() == ERROR_ALREADY_EXISTS)
                {
                    printf("Folder already exists\n");
                    return;
                }
                else
                {
                    printf("Create directory failed\n");
                    return;
                }
            }

            int extensionLen = 6;
            char extension[extensionLen] = ".json";
            // Check that character folder length + name length + .json extension doesn't exceed max length
            if (charFolderLen + nameLen + extensionLen > maxLen)
            {
                printf("Combination of path and filename is too long\n");
                return;
            }

            // copy character folder path to the start of the character filepath
            int filepathLen = 0;
            char filepath[maxLen];
            for (int i = 0; i < maxLen; i++)
            {
                if (charFolder[i] == '\0')
                {
                    filepath[i] = '\\';
                    charFolderLen = i + 1;
                    break;
                }
                else
                    filepath[i] = charFolder[i];
            }

            // Copy name to the end of the filepath
            for (int i = 0; i < nameLen; i++)
            {
                filepath[charFolderLen + i] = name[i];
            }

            // Copy extension to the end of the filepath
            for (int i = 0; i < extensionLen; i++)
            {
                filepath[charFolderLen + nameLen + i] = extension[i];
            }

            printf("filepath: %s\n", filepath);
        	Character c;
        	c.version = LatestCharacterVersion();
        	c.fps = 60;
        	c.name = std::string(name);
        
        	nlohmann::json j = c;

            std::ofstream outfile;
           	outfile.open(filepath, std::ios::out | std::ios::trunc);
        	outfile << j.dump(4);
           	outfile.close();

            EditCharacterWindow::EditCharacterState(c);
        }
        else if (key == GLFW_KEY_L && action == GLFW_PRESS)
        {
            // Select sprite sheet and go to EditSpriteSheetState
            printf("This feature does not exist yet\n");
        }
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, nullptr);
}