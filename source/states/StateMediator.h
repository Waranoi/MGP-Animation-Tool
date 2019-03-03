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
    extern KeyboardCallback keyboardEvent;
    extern MouseBtnCallback mouseBtnEvent;
    extern MousePosCallback mousePosEvent;
    extern DrawCallback drawEvent;

    // Parameters can be null
    void SetEventCallbacks(
        KeyboardCallback newKeyboardEvent, 
        MouseBtnCallback newMouseBtnEvent,
        MousePosCallback newMousePosEvent,
        DrawCallback newDrawEvent);

    // State events
    void SendKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    void SendMouseBtnEvent(GLFWwindow* window, int button, int action, int mods);
    void SendMousePosEvent(GLFWwindow* window, double xpos, double ypos);
    void SendDrawEvent();
};