#include <glad/glad.h> 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Image.hpp> //window icon
#include <iostream>
#include <unordered_map>
#include <string>
#include "iRender.h"


struct action
{
	std::string name;
	bool isActive = false;

	void Activate()
	{
		isActive = true;
	}

	void Deactivate()
	{
		isActive = false;
	}

	void Perform()
	{
		std::cerr << name << " activated " << std::endl;
	}
};


//set of actions - mapped to keys
std::unordered_map<int, action> keymap;


void InitInput()
{
	//set defaults
	action up;
	up.name = "Up";

	action down;
	down.name = "Down";

	action left;
	left.name = "Left";

	action right;
	right.name = "Right";




	keymap[sf::Keyboard::W] = up;
	keymap[sf::Keyboard::S] = down;
	keymap[sf::Keyboard::A] = left;
	keymap[sf::Keyboard::D] = right;
}

void HandleInput(sf::Event& event, bool& running)
{
	//use a boolean that you set on KeyPressed and clear on KeyReleased


	if (event.type == sf::Event::KeyPressed)
	{
		//QUIT
		if (event.key.code == sf::Keyboard::Escape)
		{
			running = false;
			return;
		}

		for (auto& input : keymap) 
		{
			if (event.key.code == input.first)
			{
				input.second.Activate();
			}
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		for (auto& input : keymap)
		{
			if (event.key.code == input.first)
			{
				input.second.Deactivate();
			}
		}
	}
}

void HandleWindowEvents(sf::Window* window, bool& running)
{
	// handle events
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			// end the program
			running = false;
		}
		else if (event.type == sf::Event::Resized)
		{
			// adjust the viewport when the window is resized
			//TODO: abstract ogl
			glViewport(0, 0, event.size.width, event.size.height);
		}
		
		HandleInput(event, running);
		
	}
}

sf::Window* CreateWindowContext()
{
	// create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 4;
	settings.minorVersion = 6;

	//TODO: abstract ogl
	sf::Window* window = new sf::Window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
	window->setVerticalSyncEnabled(true);
	window->setTitle("Moon Renderer");

	

	// activate the window
	window->setActive(true);


	//DEBUG
	sf::ContextSettings context = window->getSettings();
	std::cout << "OpenGL Context Settings Loaded: " << std::endl;
	std::cout << " *	version:" << context.majorVersion << "." << context.minorVersion << std::endl;
	std::cout << " * 	depth bits:" << context.depthBits << std::endl;
	std::cout << " * 	stencil bits:" << context.stencilBits << std::endl;
	std::cout << " *	antialiasing level:" << context.antialiasingLevel << std::endl;

	return window;
}




int main()
{
	//Init Glad  TODO:without glfw??
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}

	//Init Window
	sf::Window* window = CreateWindowContext();

	InitInput();

	// load resources, initialize the OpenGL states, ...
	// Load an image file from a file
	sf::Image icon;
	if (!icon.loadFromFile("../../../../Media/Icon/MoonWindowIcon.png"))
	{
		std::cerr << "Failed to Load Icon!!" << std::endl;
	}
	window->setIcon(25, 25, icon.getPixelsPtr());


	//TODO: build and link GLFW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//Build Shaders
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	
	
	
	// run the main loop
	bool running = true;
	while (running)
	{
		//handle events
		HandleWindowEvents(window, running);

		//perform actions
		for (auto& action : keymap)
		{
			if (action.second.isActive)
			{
				action.second.Perform();
			}
		}

		//clear back buffer to preset color
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 

		// draw...
		Render();
		

		// end the current frame (internally swaps the front and back buffers)
		window->display();

		
	}

	// release resources...
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete window;
	return 0;
}