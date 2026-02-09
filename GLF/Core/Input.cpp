#include "Input.h"
#include <GLFW/glfw3.h>

void Input::Init(void* windowhandle)
{
    s_WindowHandle = windowhandle;
}

bool Input::IsKeyPressed(KeyCode keycode)
{
    return glfwGetKey((GLFWwindow*)s_WindowHandle, (int)keycode) == GLFW_PRESS;
}

bool Input::IsKeyReleased(KeyCode keycode)
{
    return glfwGetKey((GLFWwindow*)s_WindowHandle, (int)keycode) == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(MouseButton mousebutton)
{
    return glfwGetMouseButton((GLFWwindow*)s_WindowHandle, mousebutton) == GLFW_PRESS;
}

bool Input::IsMouseButtonReleased(MouseButton mousebutton)
{
    return glfwGetMouseButton((GLFWwindow*)s_WindowHandle, mousebutton) == GLFW_RELEASE;
}

void Input::ResetInput()
{
    MouseOffset = { 0.0f, 0.0f };
    MouseScroll = { 0.0f, 0.0f };

    for (auto& [key, state] : s_LastFrameKeys)
    {
        state = IsKeyPressed((KeyCode)key);
    }
}

bool Input::IsKeyPressedOnce(KeyCode keycode)
{
    int key = (int)keycode;

    bool isPressed = IsKeyPressed(keycode);

    bool wasPressed = s_LastFrameKeys.count(key) ? s_LastFrameKeys[key] : false;

    s_LastFrameKeys[key] = isPressed;

    return isPressed && !wasPressed;
}
