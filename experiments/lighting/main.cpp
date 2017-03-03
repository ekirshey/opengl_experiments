#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL.h>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 960;

std::string programName = "Headerphile SDL2 - OpenGL thing";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

void CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SDL Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

// The MAIN function, from here we start the application and run the game loop
int main(int argc, char *argv[])
{
	std::string shaderpath = "F:/github/opengl_experiments/shaders/";
	std::string mediapath = "F:/github/opengl_experiments/media/";
	int screenWidth = WIDTH;
	int screenHeight = HEIGHT;
	Camera c(screenWidth, screenHeight);

	Shader shader;
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	// Create our window centered at 512x512 resolution
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screenWidth, screenHeight, SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n" << std::endl;;
		CheckSDLError(__LINE__);
		return false;
	}

	SDL_CaptureMouse(SDL_TRUE);
	SDL_ShowCursor(0);
	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Enable Z Buffer
	glEnable(GL_DEPTH_TEST);

	SDL_GL_SwapWindow(mainWindow);
	
	int lightingShader = shader.BuildNewProgram(shaderpath + "lighting.vert", shaderpath + "lighting.frag", DEFAULTSHADER);
	int lampShader = shader.BuildNewProgram(shaderpath + "lamp.vert", shaderpath + "lamp.frag", DEFAULTSHADER);

	// Load Texture
	GLuint diffuseMap = LoadTexture(mediapath + "container2.png", GL_RGBA);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint light2VAO;
	glGenVertexArrays(1, &light2VAO);
	glBindVertexArray(light2VAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	// Light attributes
	glm::vec3 lightPos(1.2f, 0.75f, 2.0f);
	glm::vec3 light2Pos(1.2f, 0.0f, 2.0f);

	bool keys[1024] = { false };
	bool quit = false;
	while (!quit)
	{
		// Render
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get SDL Input
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// handle your event here
			switch (event.type) {
			case SDL_QUIT:
				/* Quit */
				quit = true;
				break;
			case SDL_KEYDOWN:
				keys[event.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				keys[event.key.keysym.scancode] = false;
				break;
			case SDL_MOUSEWHEEL:
				c.SetFov(event.wheel.y);
				break;
			default:
				break;
			}
		}

		// Mouse handling for yaw, pitch, zoom
		int xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);
		c.Update(xpos, ypos, keys);

		GLfloat time = SDL_GetTicks() / 1000.0f;
		//lightPos = glm::vec3(lightPos.x, glm::cos(time)*0.5f, glm::sin(time)*1.0f);
		shader.UseProgram(lightingShader);
		GLint objectColorLoc = shader.GetUniformlocation(lightingShader, "objectColor");
		GLint lightColorLoc = shader.GetUniformlocation(lightingShader, "lightColor");
		GLint lightPosLoc = shader.GetUniformlocation(lightingShader, "lightPos");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		// Set Material properties
		GLint matAmbientLoc = shader.GetUniformlocation(lightingShader, "material.ambient");
		GLint matDiffuseLoc = shader.GetUniformlocation(lightingShader, "material.diffuse");
		GLint matSpecularLoc = shader.GetUniformlocation(lightingShader, "material.specular");
		GLint matShineLoc = shader.GetUniformlocation(lightingShader, "material.shininess");

		glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
//		glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
		glUniform1i(matDiffuseLoc, 0);
		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 32.0f);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
/*
		// Cyan Plastic
		glUniform3f(matAmbientLoc, 0.0f, 0.1f, 0.06f);
		glUniform3f(matDiffuseLoc, 0.0f, 0.50980392f, 0.50980392f);
		glUniform3f(matSpecularLoc, 0.50196078f, 0.50196078f, 0.50196078f);
		glUniform1f(matShineLoc, 0.25f * 128.0f);
*/

		// Set Light Properties
		glm::vec3 lightColor;
		lightColor.x = sin(time * 2.0f);
		lightColor.y = sin(time * 0.7f);
		lightColor.z = sin(time * 1.3f);

		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence

		GLint lightAmbientLoc = shader.GetUniformlocation(lightingShader, "light.ambient");
		GLint lightDiffuseLoc = shader.GetUniformlocation(lightingShader, "light.diffuse");
		GLint lightSpecularLoc = shader.GetUniformlocation(lightingShader, "light.specular");

		glUniform3f(lightAmbientLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

/*
		glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
*/
		// Create camera transformations
		glm::mat4 view;
		view = c.GetViewMatrix();
		glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = shader.GetUniformlocation(lightingShader, "model");
		GLint viewLoc = shader.GetUniformlocation(lightingShader, "view");
		GLint projLoc = shader.GetUniformlocation(lightingShader, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the container (using container's vertex attributes)
		glBindVertexArray(containerVAO);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		shader.UseProgram(lampShader);
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = shader.GetUniformlocation(lampShader, "model");
		viewLoc = shader.GetUniformlocation(lampShader, "view");
		projLoc = shader.GetUniformlocation(lampShader, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
/*
		// Second light
		model = glm::mat4();
		model = glm::translate(model, light2Pos);
		model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(light2VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
*/
		// Swap the screen buffers
		SDL_GL_SwapWindow(mainWindow);
	}

	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
	return 0;
}
