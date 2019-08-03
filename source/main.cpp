#include "core/Window/Window.h"
#include <stdio.h>
#include <math.h>
#include "character/Character.h"
#include <fstream>
#include <iomanip>
#include "StateMediator.h"
#include "SelectCharacterWindow.h"
#include "Shaders.h"

using namespace CharacterTypes;
using namespace std;

GLFWwindow *window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	StateMediator::SendKeyboardEvent(window, key, scancode, action, mods);
}

void mouse_btn_callback(GLFWwindow* window, int button, int action, int mods)
{
	StateMediator::SendMouseBtnEvent(window, button, action, mods);
}

void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	StateMediator::SendMousePosEvent(window, xpos, ypos);
}

bool Setup()
{
	//Initialize GLFW and create a window
	window = Window::Create_window(640, 640, "Triangle_app");
	if (window == nullptr)
	{
		printf("Window: Failed to create window\nEnter any button to continue ");
		scanf("%*s");
		return false;
	}

	//Register inputs
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_btn_callback);
	glfwSetCursorPosCallback(window, mouse_pos_callback);

	//Set startup state
	SelectCharacterWindow::SelectCharacterState();
	
	//Set clear color to gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setup shaders
	Shaders::SetupShaders();

	return true;
}

void Application_loop()
{
	//Program loop
	while (!glfwWindowShouldClose(window))
	{
        glClear(GL_COLOR_BUFFER_BIT);
		
		//Rotate then draw triangle
		// rot += 0.01f;
		// Set_matrix_rot(rot, rot_matrix);
		// glUniformMatrix4fv(glGetUniformLocation(program, "rot"), 1, GL_FALSE, rot_matrix);
		// glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		StateMediator::SendDrawEvent();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main()
{
	if (Setup() == true)
		Application_loop();
	Window::Destroy_window(window);
	return 0;
}