Tidy up the following fixed bug: Imported sprite sheets are not copied to the sprite sheets folder if it does not exist

Tidy upp the add and load sprite sheet code.

Fix TexturedQuad::IsValidTexQuad function.
Add loading times to LoadCharacter function.
Select .mpgchar root directory instead of file.
Change sprite cell ordering from "bottom to top" to "top to bottom".
Draw some noticeable quad when rendering a texquadobj with default value to notify the user that something is wrong.
Release buffer objects when deleting objects with texquadobjects. Maybe with shared pointers and custom destructors?

Render animation hitboxes
Animate animation

Version 2 goals:
Remove console or replace it with an internal console. https://github.com/ocornut/imgui
