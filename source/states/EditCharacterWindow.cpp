#include "EditCharacterWindow.h"
#include "StateMediator.h"

void EditCharacterWindow::EditCharacterState(Character target)
{
    // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditAnimationState(int animation)
{
        // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::PlayAnimationState(int animation)
{
        // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditSpriteSheetState(int spriteSheet)
{
        // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditSpriteState(int animation, int sprite)
{
        // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    };

    // Draw event for this State
    DrawCallback newDrawEvent = [] {
        
    };

    StateMediator::SetEventCallbacks(newKeyboardEvent, nullptr, nullptr, newDrawEvent);
}

void EditCharacterWindow::EditHitboxState(int animation, int sprite, int hitbox)
{
        // Keyboard action event for this State.
    KeyboardCallback newKeyboardEvent = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
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