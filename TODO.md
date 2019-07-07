Tidy up the following fixed bug: Imported sprite sheets are not copied to the sprite sheets folder if it does not exist

Tidy upp the add and load sprite sheet code.

Handle SelectCharacterWindow trying to load sprite sheets that doesn't exist in the sprite sheets folder. Check that the ifstream is valid, if it's not then warn the user and continue to the next iteration.

Instead of breaking when trying to import an existing sprite sheet, check if the sprite sheet exists in the .mpgchar json file. If it does not exist, add it. If it exists, complain about adding an existing sprite sheet and break.

// The character's sprite sheets
std::vector<std::shared_ptr<SpriteSheet>> spriteSheets;
It keeps sprite sheet in a shared pointer but sprite sheet contains a raw pointer to data. Will this data be released when sprite sheet is destroyed? I doubt this, possibly a memory leak.

Remove external console and replace it with a console within the opengl window. https://github.com/ocornut/imgui