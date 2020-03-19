#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Shader.h"  

#include "Camera.h"
#include "Player.h"
#include "Terrain.h"
#include "Rocket.h"
#include "Block.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


float WIDTH = 1420.0f;
float HEIGHT = 810.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float mixValue = 0.2f;
float force = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void windowSize_callback(GLFWwindow* window, int width, int height);

void nextPlayerTurn();
void do_movement();
void do_mixTexture();
void framesPerSecond(int& counter, float& counterS, bool log = true);
bool hit(glm::vec3 objA, glm::vec3 objB);
bool hitBlock(glm::vec3 objA, glm::vec3 objB);

Camera camera(glm::vec3(0.0f, 1.0f, 1.0f));
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;
bool firstMouse = true;

Player* players[2];
std::vector<Rocket*> rockets;
std::vector<Block*> blocks;

bool keys[1024];
int numOfKeysPressed = 0;

bool FOLLOWING_ROCKET = false;
Rocket* FOLLOWING_ROCKET_PTR;

int ACTIVE_PLAYER = 0;

bool GAME_STATE = true;
int fpstmp = 0;

void printVectorLocation(glm::vec3 loc)
{
	std::cout << loc.x << " " << loc.y << " " << loc.z << std::endl;
}

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "ooga booga", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowSizeCallback(window, windowSize_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	

	std::cout << glGetString(GL_VERSION) << std::endl;

	{		
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(6.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(10.8f, 2.0f, -12.3f),
			glm::vec3(4.4f, -0.4f, -3.5f),
			glm::vec3(-4.7f,  3.0f, -7.5f),
			glm::vec3(2.3f, -2.0f, -2.5f),
			glm::vec3(0.5f,  2.0f, -2.5f),
			glm::vec3(-2.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};			

		glm::mat4 proj = glm::perspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture t_Player("res/textures/player.jpg");
		Texture t_Terrain("res/textures/wall.jpg", true);
		Texture t_Rocket("res/textures/rocket.jpg");
		Texture t_Container("res/textures/container.jpg");

		shader.SetUniform1i("u_Texture", 0);

		shader.Unbind();
		
		Renderer renderer;

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		

		// For FPS
		int counter = 0;
		float counterS = 0;			

		players[0] = new Player();
		players[0]->setPosition(glm::vec3(0.0f, 0.501f, -20.0f));
		players[1] = new Player();
		players[1]->setPosition(glm::vec3(0.0f, 0.501f, 20.0f));


		glm::vec3 blockPositions[] = {
			glm::vec3(20.0f,  1.0f, 20.0f),
			glm::vec3(5.0f,  1.0f, 5.0f),

			glm::vec3(-20.0f,  1.0f, 20.0f),
			glm::vec3(-5.0f,  1.0f, 5.0f),

			glm::vec3(20.0f,  1.0f, -20.0f),
			glm::vec3(5.0f,  1.0f, -5.0f),

			glm::vec3(-20.0f,  1.0f, -20.0f),
			glm::vec3(-5.0f,  1.0f, -5.0f),

			glm::vec3(0.0f,  1.0f, 0.0f),					
		};

		for (int i = 0; i < 9; i++)
		{
			Block* b = new Block(blockPositions[i]);
			blocks.push_back(b);
		}

		Terrain t;			
		
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();			

			do_movement();
			do_mixTexture();

			renderer.Clear();

			GLfloat timeValue = glfwGetTime();
			deltaTime = timeValue - lastFrame;
			lastFrame = timeValue;

			framesPerSecond(counter, counterS, false);	
					   			

			proj = glm::perspective(camera.GetZoom(), WIDTH / HEIGHT, 0.1f, 100.0f);


			shader.Bind();

			view = camera.GetViewMatrix();

			players[0]->Draw(shader, t_Player, proj, view);
			players[1]->Draw(shader, t_Player, proj, view);


			int index = 0;
			for (Rocket* r : rockets)
			{
				r->Update(deltaTime);					
				r->Draw(shader, t_Rocket, proj, view, deltaTime);									

				if (!r->m_Done)
				{
					if (r->GetLocation().y < 0.1f)
					{							
						r->m_Done = true;
						FOLLOWING_ROCKET = false;
						nextPlayerTurn();							
						camera.otherWay();
					}
					else if (r->GetLocation().x < -32.0f || r->GetLocation().x > 32.0f ||
								r->GetLocation().z < -32.0f || r->GetLocation().z > 32.0f)
					{
						rockets.erase(rockets.begin() + index);
						index--;
						r->m_Done = true;
						FOLLOWING_ROCKET = false;
						nextPlayerTurn();
						delete r;
						camera.otherWay();
					}
					else
					{
						for (int i = 0; i < 2; i++)
						{
							if (hit(r->GetLocation(), players[i]->GetLocation()))
							{
								r->m_Done = true;
								std::cout << "HIT" << std::endl;
								if (players[i]->add(r))
								{
									std::cout << "KONEC IGRE" << std::endl;
									GAME_STATE = false;
								}
								nextPlayerTurn();
								FOLLOWING_ROCKET = false;
								camera.otherWay();
							}
						}
						if (!r->m_Done)
						{
							for (Block* b : blocks)
							{
								if (hitBlock(r->GetLocation(), b->GetLocation()))
								{
									r->m_Done = true;
									nextPlayerTurn();
									FOLLOWING_ROCKET = false;
									camera.otherWay();
								}
							}
						}							
					}						
				}
				index++;
			}

			for (Block* b : blocks)
			{
				b->Draw(shader, t_Container, proj, view);
			}
				
			t.Draw(shader, t_Terrain, proj, view);

			if (!FOLLOWING_ROCKET)
			{
				camera.SetFollow(players[ACTIVE_PLAYER]->GetLocation());
			}
			else
			{
				camera.SetFollow(FOLLOWING_ROCKET_PTR->GetLocation());
			}

			shader.Unbind();			

			glfwSwapBuffers(window);
		}
		delete players[0];
		delete players[1];

		for (Rocket* r : rockets)
			delete r;
	}
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS) {
		keys[key] = true;

		if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_A || key == GLFW_KEY_D)
			numOfKeysPressed++;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
		if (key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_A || key == GLFW_KEY_D)
			numOfKeysPressed--;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE && !FOLLOWING_ROCKET && GAME_STATE)
	{
		glm::vec3 direction = camera.GetFront() + glm::vec3(0.0f, 0.2f, 0.0f);
		direction.y *= 1.5;
		Rocket* r = new Rocket(direction);		
		r->setForce(force);
		r->setPosition(players[ACTIVE_PLAYER]->GetLocation() + glm::vec3(0.0f, 1.1f, 0.0f));
		rockets.push_back(r);			

		FOLLOWING_ROCKET = true;
		FOLLOWING_ROCKET_PTR = r;
		force = 0;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (fpstmp == 0)
		{
			glfwSwapInterval(3);
			fpstmp = 2;
		}
		else
		{
			fpstmp = 0;
			glfwSwapInterval(0);
		}
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS && !GAME_STATE)
	{
		std::cout << "RESETING GAME" << std::endl;
		for (Rocket* r : rockets) {
			delete r;
		}
		rockets = {};
		
		players[0]->setPosition(glm::vec3(0.0f, 0.501f, -20.0f));		
		players[0]->m_Appended = {};
		players[0]->m_Color = glm::vec3(0.0f, 1.0f, 0.0f);

		players[1]->setPosition(glm::vec3(0.0f, 0.501f, 20.0f));
		players[1]->m_Appended = {};
		players[1]->m_Color = glm::vec3(0.0f, 1.0f, 0.0f);

		ACTIVE_PLAYER = 0;
		players[ACTIVE_PLAYER]->turn();
		
		FOLLOWING_ROCKET = false;
		//camera.SetFollow(players[ACTIVE_PLAYER]->GetLocation());

		GAME_STATE = true;
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void windowSize_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void do_mixTexture()
{
	float add = 1.0f * deltaTime;
	if (keys[GLFW_KEY_UP])
	{
		if (mixValue < 1)
			mixValue += add;
	}
	else if (keys[GLFW_KEY_DOWN])
	{
		if (mixValue > 0)
			mixValue -= add;
	}
}

void do_movement()
{
	float cameraSpeed = 5.0f * deltaTime;

	if (!FOLLOWING_ROCKET && GAME_STATE)
	{
		if (keys[GLFW_KEY_UP])
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (keys[GLFW_KEY_DOWN])
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (keys[GLFW_KEY_LEFT])
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (keys[GLFW_KEY_RIGHT])
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (keys[GLFW_KEY_W])
			players[ACTIVE_PLAYER]->ProcessKeyboard(camera.GetFront(), deltaTime, blocks, numOfKeysPressed);
		if (keys[GLFW_KEY_S])
			players[ACTIVE_PLAYER]->ProcessKeyboard(camera.GetFront() * -1.0f, deltaTime, blocks, numOfKeysPressed);
		if (keys[GLFW_KEY_A])	  
			players[ACTIVE_PLAYER]->ProcessKeyboard(camera.GetRight() * -1.0f, deltaTime, blocks, numOfKeysPressed);
		if (keys[GLFW_KEY_D])	  
			players[ACTIVE_PLAYER]->ProcessKeyboard(camera.GetRight(), deltaTime, blocks, numOfKeysPressed);
		if (keys[GLFW_KEY_SPACE])
			force += 0.4f;
	}
}

void framesPerSecond(int& counter, float& counterS, bool log)
{	
	counter++;
	counterS += deltaTime;
	if (counterS >= 1)
	{
		if (log)
			std::cout << "FPS: " << counter / counterS << std::endl;
		counterS = 0;
		counter = 0;
	}	
}

bool hit(glm::vec3 objA, glm::vec3 objB)
{
	if ((objB.x - 0.35f <= objA.x && objA.x <= objB.x +0.35f) && 
		(objB.y - 0.5f <= objA.y  && objA.y <= objB.y + 0.55f) && 
		(objB.z - 0.3f <= objA.z && objA.z <= objB.z + 0.3f))
		return true;
	return false;
}

bool hitBlock(glm::vec3 objA, glm::vec3 objB)
{
	if ((objB.x - 1.05f < objA.x && objA.x < objB.x + 1.05f) &&
		(objB.y - 1.05f < objA.y && objA.y < objB.y + 1.05f) &&
		(objB.z - 1.05f < objA.z && objA.z < objB.z + 1.05f))
		return true;
	return false;
}

void nextPlayerTurn()
{
	ACTIVE_PLAYER++;
	if (ACTIVE_PLAYER > 1)
	{
		ACTIVE_PLAYER = 0;
	}
	players[ACTIVE_PLAYER]->turn();
}
