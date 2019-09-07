Tidy up the following fixed bug: Imported sprite sheets are not copied to the sprite sheets folder if it does not exist

Tidy upp the add and load sprite sheet code.

Add loading times to LoadCharacter function.
Select .mpgchar root directory instead of file.
Change sprite cell ordering from "bottom to top" to "top to bottom".
Replace SpriteSheet.sourceLocation usage with its map key equivalent.
Gracefully handle if spriteSheet texquad can't be generated because image does not exist. (How to deal with dependant sprites?).
Currently sprites depending on a broken spriteSheet are handled by a newly made TexQuad::init boolean. If a texQuad is not set then the init boolean is false and won't be drawn. This is an ugly emergency fix because there will be no error message if the user tries to draw a sprite using a broken spriteSheet reference. Handle broken sprites better.
Add a reload option to spriteSheet so that you can fix missing spriteSheet texquads and dependant sprites without restarting.
Create sprite texquads anew when switching spriteSheet reference for an animation.
Gracefully handle out of bounds error with animation sprites in case of a sprite sheet reference replacement.
Draw some noticeable quad when rendering a texquadobj with default value to notify the user that something is wrong.

Render animation hitboxes
Animate animation

Version 2 goals:
Remove console or replace it with an internal console. https://github.com/ocornut/imgui
