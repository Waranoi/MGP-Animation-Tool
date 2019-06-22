Tidy up the following fixed bug: Imported sprite sheets are not copied to the sprite sheets folder if it does not exist

Add functions to manipulate sprite sheets in the program.

Tidy upp the add and load sprite sheet code.

// The character's sprite sheets
std::vector<std::shared_ptr<SpriteSheet>> spriteSheets;
It keeps sprite sheet in a shared pointer but sprite sheet contains a raw pointer to data. Will this data be released when sprite sheet is destroyed? I doubt this, possibly a memory leak.

Remove external console and replace it with a console within the opengl window. https://github.com/ocornut/imgui