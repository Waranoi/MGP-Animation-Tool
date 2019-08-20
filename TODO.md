Tidy up the following fixed bug: Imported sprite sheets are not copied to the sprite sheets folder if it does not exist

Tidy upp the add and load sprite sheet code.

Load sprite textures when loading characters in Character.cpp
Add loading times to LoadCharacter function
Select .mpgchar root directory instead of file
Handle broken sprite sheet reference in animation edit state gracefully
Change sprite cell ordering from "bottom to top" to "top to bottom"

Render animation hitboxes
Animate animation

Version 2 goals:
Remove external console or replace it with a console within the opengl window. https://github.com/ocornut/imgui
