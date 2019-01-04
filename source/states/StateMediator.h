#pragma once
#include <functional>
#include <GLFW/glfw3.h>

// Typedefs to make callbacks less verbose and more descriptive
typedef std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> KeyboardCallback;
typedef std::function<void(GLFWwindow* window, int button, int action, int mods)> MouseBtnCallback;
typedef std::function<void(GLFWwindow* window, double xpos, double ypos)> MousePosCallback;
typedef std::function<void()> DrawCallback;

namespace StateMediator
{
    KeyboardCallback keyboardEvent = nullptr;
    MouseBtnCallback mouseBtnEvent = nullptr;
    MousePosCallback mousePosEvent = nullptr;
    DrawCallback drawEvent = nullptr;

    // Parameters can be null
    inline void SetEventCallbacks(
        KeyboardCallback newKeyboardEvent, 
        MouseBtnCallback newMouseBtnEvent,
        MousePosCallback newMousePosEvent,
        DrawCallback newDrawEvent);

    // State events
    inline void SendKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    inline void SendMouseBtnEvent(GLFWwindow* window, int button, int action, int mods);
    inline void SendMousePosEvent(GLFWwindow* window, double xpos, double ypos);
    inline void SendDrawEvent();
};

void StateMediator::SetEventCallbacks(
    KeyboardCallback newKeyboardEvent, 
    MouseBtnCallback newMouseBtnEvent,
    MousePosCallback newMousePosEvent,
    DrawCallback newDrawEvent)
{
    keyboardEvent = newKeyboardEvent;
    mouseBtnEvent = newMouseBtnEvent;
    mousePosEvent = newMousePosEvent;
    drawEvent = newDrawEvent;
}

void StateMediator::SendKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (keyboardEvent)
        keyboardEvent(window, key, scancode, action, mods);
}

void StateMediator::SendMouseBtnEvent(GLFWwindow* window, int button, int action, int mods)
{
    if (mouseBtnEvent)
        mouseBtnEvent(window, button, action, mods);
}

void StateMediator::SendMousePosEvent(GLFWwindow* window, double xpos, double ypos)
{
    if (mousePosEvent)
        mousePosEvent(window, xpos, ypos);
}

void StateMediator::SendDrawEvent()
{
    if (drawEvent)
        drawEvent();
}