#include "StateMediator.h"

KeyboardCallback StateMediator::keyboardEvent = nullptr;
MouseBtnCallback StateMediator::mouseBtnEvent = nullptr;
MousePosCallback StateMediator::mousePosEvent = nullptr;
DrawCallback StateMediator::drawEvent = nullptr;

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