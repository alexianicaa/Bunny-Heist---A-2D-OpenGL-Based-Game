// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"



//variables
GLFWwindow* window;
const int width = 1220, height = 1024;
float sx = 1.5f, sy = 0.5f, sz = 0;

glm::vec4 bunnyColor(1.0f, 1.0f, 1.0f, 1.0f); // bunny color
glm::vec4 bearColor(0.3f, 0.2f, 0.1f, 1.0f);  // bear color
glm::vec4 carrotColor(1.0f, 0.5f, 0.0f, 1.0f); // carrot color
glm::vec4 leafColor(0.0f, 1.0f, 0.0f, 1.0f); // leaf color

glm::vec3 position(-0.7f, -0.2f, 0.0f); // bunny position
glm::vec3 position2(0.4f, -0.1f, 0.0f); // bear position
glm::vec3 carrotPosition(-0.55, -0.35f, 0.0f); // carrot position that will keep changing
glm::vec3 position5(0.7f, -0.6f, 0.0f); // treasure position
glm::vec3 position6(-0.75f, 0.5f, 0.0f); // HealthBar Rabbit
glm::vec3 position7(-0.75f, 0.5f, 0.0f); // HealthBar Dead rabbit
glm::vec3 position8(0.75f, 0.5f, 0.0f); // HealthBar Dead Bear
glm::vec3 position9(0.75f, 0.5f, 0.0f);// HealthBar Bear
glm::vec3 goldenCarrrot_position(0.7f, -0.3f, 0.0f); // golden carrot
glm::vec3 finalPosition(0.0f, 0.0f, 0.0f);



bool pressJump = false;
float jumpVelocity = 0.0f;
const float gravity_jump = -0.000001f;
bool pressShoot = false;

float bearMaxHealth = 100;
float bunnyMaxHealth = 100;
float bearHealth = 100;
float bunnyHealth = 100;



const float carrotSpeed = 0.1f;
const float gravityCarrot = -1.1f;
bool didRabbitwin = false;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (position.x <= 0.1 ) {
		if (key == GLFW_KEY_W && action == GLFW_PRESS && pressJump == false)
		{
			position.y += 0.1f;
			pressJump = true;
			jumpVelocity = 0.000015f;
		}
	}
	else
	{
		if (bearHealth > 0) {
			position.x = -0.7;
			bunnyHealth -= 25;
			position6.x -= 0.2;
			carrotPosition.y = position.y - 0.25;
			carrotPosition.x = position.x + 0.15;
		}
		else
		{
			if (key == GLFW_KEY_W && action == GLFW_PRESS && pressJump == false)
			{
				position.y += 0.1f;
				pressJump = true;
				jumpVelocity = 0.000015f;
			}
		}
	}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			position.x -= 0.1f;
			if(pressShoot == false)
				carrotPosition.x -= 0.1f;
		}

		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			if (position.y - 0.1f >= -0.2f)  // Constrain to ground level
				position.y -= 0.1f;
		}

		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			position.x += 0.1f;
			if (pressShoot == false)
				carrotPosition.x += 0.1f;
			position.x = glm::clamp(position.x, -1.0f, 1.0f);
		}
		
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && pressShoot == false)
		{			
			pressShoot = true;
			carrotPosition.y = position.y - 0.25;
			carrotPosition.x = position.x + 0.15;
		}
	
	std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";

}

int main(void) {

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Bunny game", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//bunny
	float g_vertex_buffer_bunny[] = {
			0.1f, 0.0f, 0.0f,
			0.0f, -0.1f, 0.0f,
			-0.1f,  0.0f, 0.0f, // head
			0.0f, 0.1f, 0.0f,

			-0.1f, -0.1f, 0.0f,
			0.1f, -0.1f, 0.0f,
			-0.1f, -0.4f, 0.0f, //body
			0.1f, -0.4f, 0.0f,

			0.0f,-0.4f, 0.0f,
			0.0f,-0.5f, 0.0f, //legs
			0.1f,-0.5f, 0.0f,

			-0.1f, -0.3f, 0.0f, //tail
			-0.15f, -0.35f, 0.0f,

			-0.2f, 0.3f, 0.0f, //ear up
			-0.3f, 0.2f, 0.0f,

			-0.3f, 0.1f, 0.0f, //ear down
			-0.2f, 0.0f, 0.0f,

			0.0f, 0.01f, 0.0f,
			0.04f, 0.01f,0.0f,
			0.02f, 0.03f,0.0f,
			-0.17f, 0.155,0.0f,
			-0.17f,0.1f,0.0f,

	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,
		1, 2, 3,
		4,5,6,
		5,6,7,
		8,9,10,
		6,11,12,
		13,14,3,
		3,15,16,

		0,2,3,
		20,3,13,
		21,3,16,
		17,18,19,

	};
	//bear
	float g_vertex_buffer_bear[] = {
		0.1f, 0.1f, 0.0f,
		0.1f, -0.1f, 0.0f, //head
		-0.1f, -0.1f, 0.0f,
		-0.1f, 0.1f, 0.0f,

		-0.15f, -0.1f, 0.0f,
		0.15f, -0.1f, 0.0f, //body
		-0.15f, -0.5f,0.0f,
		0.15f, -0.5f,0.0f,

		-0.15f, -0.5f, 0.0f,
		-0.05f, -0.5f, 0.0f, //left leg
		-0.15f, -0.6f, 0.0f,
		-0.05f, -0.6f, 0.0f,

		0.15f, -0.5f, 0.0f,
		0.05f, -0.5f, 0.0f, //right leg
		0.15f, -0.6f, 0.0f,
		0.05f, -0.6f, 0.0f,

		-0.2f, -0.1f, 0.0f,
		-0.2f, -0.3f, 0.0f, //left arm
		-0.15f, -0.3f, 0.0f,

		0.2f, -0.1f, 0.0f,
		0.2f, -0.3f, 0.0f,//right arm
		0.15f, -0.3f, 0.0f,

		-0.1f, 0.15f,0.0f,
		-0.05f, 0.15f,0.0f,//ear
		-0.05f, 0.1f,0.0f,

		0.1f, 0.15f,0.0f,
		0.05f, 0.15f,0.0f,//ear
		0.05f, 0.1f,0.0f,

		-0.1f, -0.15f, 0.0f,
		0.1f, -0.15f, 0.0f, //belly
		-0.1f, -0.45f,0.0f,
		0.1f, -0.45f,0.0f,

		-0.07f, 0.0f,0.0f,
		-0.02f, 0.0f,0.0f,
		0.07f, 0.0f,0.0f,
		0.02f, 0.0f,0.0f,	//eyes
		-0.07f, 0.04f,0.0f,
		0.07f, 0.04f,0.0f,

		-0.04f,-0.07f,0.0f,
		0.04f,-0.07f,0.0f,
		-0.04f,-0.04f,0.0f,
		0.04f,-0.04f,0.0f,


	};

	unsigned int indices2[] = {
		0, 1, 2,
		0,2,3,

		4,5,6,
		5,6,7,

		8,9,10,
		9,10,11,

		12,13,14,
		13,14,15,

		4,16,17,
		4,17,18,

		5,19,20,
		5,20,21,

		3,22,23,
		3,23,24,

		0,25,26,
		0,26,27,

		28,29,30,
		29,30,31,

		32,33,36,
		34,35,37,

		38,39,40,
		39,40,41,
	};
	
	//ground
	float g_vertex_buffer_ground[] = {
			-1.0f, -0.7f, 0.0f,
			1.0f, -0.7f, 0.0f,
			1.0f,  -1.0f, 0.0f, // dreptunghi
			-1.0f, -1.0f, 0.0f,

	};

	unsigned int indices3[] = {  // note that we start from 0!
		0, 1, 2,
		2, 3, 0,
	};

	//carrot
	float g_vertex_buffer_carrot[] = {
		0.0f,  -0.2f, 0.0f,
		-0.05f, 0.0f, 0.0f,     //body carrot
		0.05f, 0.0f, 0.0f,

		-0.03f, 0.0f, 0.0f,
		-0.01f, 0.0f, 0.0f,
		0.01f, 0.0f, 0.0f,	// leafs
		0.03f, 0.0f, 0.0f,

		-0.05f, 0.1f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.05f, 0.1f, 0.0f,

	};

	unsigned int indices4[] = {  // note that we start from 0!
		0,1,2,

		3,4,7,
		4,5,8,
		5,6,9,
	};



	//treasure
	float g_vertex_buffer_treasure[] = {
			-0.1f, -0.1f,0.0f,
			0.1f, -0.1f, 0.0f, //bottom brown
			-0.1f, 0.0f, 0.0f,
			0.1f, 0.0f, 0.0f,

			-0.1f, 0.02f, 0.0f,
			0.1f, 0.02f, 0.0f, //top brown
			-0.1f, 0.1f, 0.0f,
			0.1f, 0.1f, 0.0f,

			-0.12f, 0.02f ,0.0f,
			0.12f, 0.02f, 0.0f, //top left/right gold
			-0.12f, 0.1f, 0.0f,
			0.12f, 0.1f, 0.0f,

			-0.08f, -0.1f ,0.0f,
			0.08f, -0.1f ,0.0f,	//bot left/right gold
			-0.08f, 0.0f ,0.0f,
			0.08f, 0.0f ,0.0f,

			-0.12f, 0.0f ,0.0f, //middle gold line
			0.12f, 0.0f, 0.0f,

			-0.02f, 0.03f, 0.0f,
			0.02f, 0.03f, 0.0f,
			-0.02f, -0.03f, 0.0f,
			0.02f, -0.03f, 0.0f,

	};

	unsigned int indices5[] = {  // note that we start from 0!
		0, 1, 2,
		2, 3, 1,

		4,5,6,
		5,6,7,

		8,4,6,
		8,6,10,

		9,5,7,
		9,7,11,

		12,14,0,
		14,0,2,

		13,15,1,
		15,1,3,

		16,17,8,
		8,9,17,

		18,19,20,
		19,20,21

	};

	//health bar rabbit

	float g_vertex_buffer_healthbar[] = {
		-0.3f, 0.5f, 0.0f,  //0
		0.5f, 0.5f, 0.0f,  //1
		-0.3f, 0.4f, 0.0f,  //2
		0.5f, 0.4f, 0.0f	//3
	};

	unsigned int indices6[] = {
		0, 2, 1, 
		1, 2, 3  
	};

	//health bar rabbit dead

	float g_vertex_buffer_healthbarwhite[] = {
		-0.3f, 0.5f, 0.0f,  //0
		0.5f, 0.5f, 0.0f,  //1
		-0.3f, 0.4f, 0.0f,  //2
		0.5f, 0.4f, 0.0f	//3
	};

	unsigned int indices7[] = {
		0, 2, 1,
		1, 2, 3
	};


	//health bar bear

	float g_vertex_buffer_healthbarbear[] = {
		-0.5f, 0.5f, 0.0f,  //0
		0.5f, 0.5f, 0.0f,  //1
		-0.5f, 0.4f, 0.0f,  //2
		0.5f, 0.4f, 0.0f	//3
	};

	unsigned int indices8[] = {
		0, 2, 1,
		1, 2, 3
	};

	//health bar bear dead

	float g_vertex_buffer_healthbarwhitebear[] = {
		-0.5f, 0.5f, 0.0f,  //0
		0.5f, 0.5f, 0.0f,  //1
		-0.5f, 0.4f, 0.0f,  //2
		0.5f, 0.4f, 0.0f	//3
	};

	unsigned int indices9[] = {
		0, 2, 1,
		1, 2, 3
	};

	//golden carrot
	float g_vertex_buffer_golden_carrot[] = {
		0.0f,  -0.2f, 0.0f,
		-0.05f, 0.0f, 0.0f,     //body carrot
		0.05f, 0.0f, 0.0f,

		-0.03f, 0.0f, 0.0f,
		-0.01f, 0.0f, 0.0f,
		0.01f, 0.0f, 0.0f,	// leafs
		0.03f, 0.0f, 0.0f,

		-0.05f, 0.1f, 0.0f,
		0.0f, 0.1f, 0.0f,
		0.05f, 0.1f, 0.0f,

	};

	unsigned int indices10[] = {  // note that we start from 0!
		0,1,2,

		3,4,7,
		4,5,8,
		5,6,9,
	};

	glfwSetKeyCallback(window, keyboard_callback);

	//SET COLOR FOR OBJECTS
	glm::vec4 bunnyColor(1.0f, 1.0f, 1.0f, 1.0f); // White for bunny
	glm::vec4 bearColor(0.3f, 0.2f, 0.1f, 1.0f);  // Brown for bear

	GLuint colorLoc = glGetUniformLocation(programID, "objectColor");

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 

	//BUNNY
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	
	//bind VAO
	glBindVertexArray(vao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_bunny), g_vertex_buffer_bunny, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);




	//BEAR
	GLuint vbo2, vao2, ibo2;
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ibo2);

	// Bind VAO2
	glBindVertexArray(vao2);

	// Bind VBO2
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_bear), g_vertex_buffer_bear, GL_STATIC_DRAW);

	// Bind IBO2
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);




	//GROUND
	GLuint vbo3, vao3, ibo3;
	glGenVertexArrays(1, &vao3);
	glGenBuffers(1, &vbo3);
	glGenBuffers(1, &ibo3);

	// Bind VAO3
	glBindVertexArray(vao3);

	// Bind VBO3
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_ground), g_vertex_buffer_ground, GL_STATIC_DRAW);

	// Bind IBO3
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	// CARROT
	GLuint vbo4, vao4, ibo4;
	glGenVertexArrays(1, &vao4);
	glGenBuffers(1, &vbo4);
	glGenBuffers(1, &ibo4);

	// Bind VAO4
	glBindVertexArray(vao4);

	// Bind VBO4
	glBindBuffer(GL_ARRAY_BUFFER, vbo4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_carrot), g_vertex_buffer_carrot, GL_STATIC_DRAW);

	// Bind IBO4
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices4), indices4, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);





	//TREASURE
	GLuint vbo5, vao5, ibo5;
	glGenVertexArrays(1, &vao5);
	glGenBuffers(1, &vbo5);
	glGenBuffers(1, &ibo5);

	// Bind VAO5
	glBindVertexArray(vao5);

	// Bind VBO5
	glBindBuffer(GL_ARRAY_BUFFER, vbo5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_treasure), g_vertex_buffer_treasure, GL_STATIC_DRAW);

	// Bind IBO4
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices5), indices5, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);





	// HealthBar Dead rabbit

	GLuint vbo7, vao7, ibo7;
	glGenVertexArrays(1, &vao7);
	glGenBuffers(1, &vbo7);
	glGenBuffers(1, &ibo7);

	// Bind VAO7
	glBindVertexArray(vao7);

	// Bind VBO7
	glBindBuffer(GL_ARRAY_BUFFER, vbo7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_healthbarwhite), g_vertex_buffer_healthbarwhite, GL_STATIC_DRAW);

	// Bind IBO7
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices7), indices7, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	//HealthBar Rabbit

	GLuint vbo6, vao6, ibo6;
	glGenVertexArrays(1, &vao6);
	glGenBuffers(1, &vbo6);
	glGenBuffers(1, &ibo6);

	// Bind VAO6
	glBindVertexArray(vao6);

	// Bind VBO6
	glBindBuffer(GL_ARRAY_BUFFER, vbo6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_healthbar), g_vertex_buffer_healthbar, GL_STATIC_DRAW);

	// Bind IBO6
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices6), indices6, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	// HealthBar Dead Bear

	GLuint vbo8, vao8, ibo8;
	glGenVertexArrays(1, &vao8);
	glGenBuffers(1, &vbo8);
	glGenBuffers(1, &ibo8);

	// Bind VAO8
	glBindVertexArray(vao8);

	// Bind VBO8
	glBindBuffer(GL_ARRAY_BUFFER, vbo8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_healthbarwhitebear), g_vertex_buffer_healthbarwhitebear, GL_STATIC_DRAW);

	// Bind IBO8
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices8), indices8, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	//HealthBar Bear

	GLuint vbo9, vao9, ibo9;
	glGenVertexArrays(1, &vao9);
	glGenBuffers(1, &vbo9);
	glGenBuffers(1, &ibo9);

	// Bind VAO9
	glBindVertexArray(vao9);

	// Bind VBO9
	glBindBuffer(GL_ARRAY_BUFFER, vbo9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_healthbarbear), g_vertex_buffer_healthbarbear, GL_STATIC_DRAW);

	// Bind IBO9
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices9), indices9, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	//Golden carrot

	GLuint vbo10, vao10, ibo10;
	glGenVertexArrays(1, &vao10);
	glGenBuffers(1, &vbo10);
	glGenBuffers(1, &ibo10);
	
		// Bind VAO10
		glBindVertexArray(vao10);

		// Bind VBO10
		glBindBuffer(GL_ARRAY_BUFFER, vbo10);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_golden_carrot), g_vertex_buffer_golden_carrot, GL_STATIC_DRAW);

		// Bind IBO10
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo10);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices10), indices10, GL_STATIC_DRAW);

		//set attribute pointers
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size of each attribute
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			3 * sizeof(float),  // stride
			(void*)0            // array buffer offset
		);


	glEnableVertexAttribArray(0);

	//BUNNY
	GLuint vbo11, vao11, ibo11;
	glGenVertexArrays(1, &vao11);
	glGenBuffers(1, &vbo11);
	glGenBuffers(1, &ibo11);


	//bind VAO
	glBindVertexArray(vao11);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_bunny), g_vertex_buffer_bunny, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo11);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);




	//create matrices for transforms
	glm::mat4 trans(1.0f);
	trans = glm::scale(trans, glm::vec3(1.0f));
	trans = glm::rotate(trans, 45.0f, glm::vec3(0.0, 0.0, 1.0));



	//vec4 for color
	glm::vec4 transColor(1.0f);
	transColor = glm::vec4(1.0, 1.0, 0.0, 1.0);


	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Check for events
		glfwPollEvents();

		// Use our shader
		glUseProgram(programID);

		GLuint transformLoc = glGetUniformLocation(programID, "transform");

		if (bunnyHealth > 0) {

			// Render BUNNY
			glm::mat4 bunnyTransform = glm::translate(glm::mat4(1.0f), position); //matrix set dynamically
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(bunnyTransform));

			//own unique transf matrix, position, transformation and color
			glUniform4fv(colorLoc, 1, glm::value_ptr(bunnyColor));
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

			glm::vec4 bunnyColor2(0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4fv(colorLoc, 1, glm::value_ptr(bunnyColor2));

			glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int)));

			glm::vec4 bunnyColor(1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4fv(colorLoc, 1, glm::value_ptr(bunnyColor));

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(33 * sizeof(unsigned int)));



			if (bearHealth > 0) {
				// Render BEAR
				glm::mat4 bearTransform = glm::translate(glm::mat4(1.0f), position2);//matrix set dynamically
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(bearTransform));

				glUniform4fv(colorLoc, 1, glm::value_ptr(bearColor));
				glBindVertexArray(vao2);
				glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

				glm::vec4 bearColor2(0.45f, 0.29f, 0.14f, 1.0f);
				glUniform4fv(colorLoc, 1, glm::value_ptr(bearColor2));

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(48 * sizeof(unsigned int)));

				glm::vec4 bearColor3(0.47f, 0.0f, 0.0f, 1.0f);
				glUniform4fv(colorLoc, 1, glm::value_ptr(bearColor3));

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(54 * sizeof(unsigned int)));


				glm::vec4 bearColor4(0.77f, 0.61f, 0.43f, 1.0f);
				glUniform4fv(colorLoc, 1, glm::value_ptr(bearColor4));

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(60 * sizeof(unsigned int)));

			}
			else
			{
				if (didRabbitwin == false) {

					if (position.x >= position5.x - 0.1) {

						didRabbitwin = true;
						// Render GOLDEN CARROT
						glm::mat4 goldenCarrotTransform = glm::translate(glm::mat4(1.0f), goldenCarrrot_position);
						glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(goldenCarrotTransform));

						glm::vec4 goldenCarrotColor(1.00f, 0.84f, 0.00f, 1.0f);
						glUniform4fv(colorLoc, 1, glm::value_ptr(goldenCarrotColor));

						glBindVertexArray(vao10);
						glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

						glm::vec4 goldenCarrotColor2(0.93f, 0.79f, 0.00f, 1.0f);
						glUniform4fv(colorLoc, 1, glm::value_ptr(goldenCarrotColor2));

						glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

					}

				}
				else {
					// Render GOLDEN CARROT
					glm::mat4 goldenCarrotTransform = glm::translate(glm::mat4(1.0f), goldenCarrrot_position);
					glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(goldenCarrotTransform));

					glm::vec4 goldenCarrotColor(1.00f, 0.84f, 0.00f, 1.0f);
					glUniform4fv(colorLoc, 1, glm::value_ptr(goldenCarrotColor));

					glBindVertexArray(vao10);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

					glm::vec4 goldenCarrotColor2(0.93f, 0.79f, 0.00f, 1.0f);
					glUniform4fv(colorLoc, 1, glm::value_ptr(goldenCarrotColor2));

					glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

				}


			}


			// Render GROUND
			glm::mat4 groundTransform = glm::mat4(1.0f); // Identity matrix
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(groundTransform));

			glm::vec4 groundColor(0.3f, 0.6f, 0.3f, 1.0f); // Green ground
			glUniform4fv(colorLoc, 1, glm::value_ptr(groundColor));

			glBindVertexArray(vao3);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



			// Render CARROT
			glm::mat4 carrotTransform = glm::translate(glm::mat4(1.0f), carrotPosition);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(carrotTransform));

			glm::vec4 carrotColor(0.96f, 0.58f, 0.11f, 1.0f);
			glUniform4fv(colorLoc, 1, glm::value_ptr(carrotColor));

			glBindVertexArray(vao4);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

			glm::vec4 carrotColor2(0.0f, 0.65f, 0.31f, 1.0f); // Gold color
			glUniform4fv(colorLoc, 1, glm::value_ptr(carrotColor2));

			glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));



			// Render TREASURE
			glm::mat4 treasureTransform = glm::translate(glm::mat4(1.0f), position5);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(treasureTransform));

			glm::vec4 treasureColor(0.6f, 0.3f, 0.1f, 1.0f);
			glUniform4fv(colorLoc, 1, glm::value_ptr(treasureColor));

			glBindVertexArray(vao5);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

			glm::vec4 treasureColor2(1.0f, 0.85f, 0.0f, 1.0f); // Gold color
			glUniform4fv(colorLoc, 1, glm::value_ptr(treasureColor2));

			glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));






			// Render HEALTHBARDEAD
			glm::mat4 healthdeadTransform = glm::translate(glm::mat4(1.0f), position7);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(healthdeadTransform));

			glm::vec4 healthdeadColor(0.0f, 0.0f, 0.0f, 0.0f); // White for health
			glUniform4fv(colorLoc, 1, glm::value_ptr(healthdeadColor));

			glBindVertexArray(vao7);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			// Render HEALTHBAR
			glm::mat4 healthTransform = glm::translate(glm::mat4(1.0f), position6);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(healthTransform));

			glm::vec4 healthColor(0.8f, 0.1f, 0.1f, 1.0f); // Red for health
			glUniform4fv(colorLoc, 1, glm::value_ptr(healthColor));

			glBindVertexArray(vao6);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);






			// Render HEALTHBARDEAD Bear
			glm::mat4 healthdeadbearTransform = glm::translate(glm::mat4(1.0f), position8);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(healthdeadbearTransform));

			glm::vec4 healthdeadbearColor(0.0f, 0.0f, 0.0f, 0.0f); // White for health
			glUniform4fv(colorLoc, 1, glm::value_ptr(healthdeadbearColor));

			glBindVertexArray(vao8);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			// Render HEALTHBARBear
			glm::mat4 healthbearTransform = glm::translate(glm::mat4(1.0f), position9);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(healthbearTransform));

			glm::vec4 healthbearColor(0.8f, 0.1f, 0.1f, 1.0f); // Red for health
			glUniform4fv(colorLoc, 1, glm::value_ptr(healthbearColor));

			glBindVertexArray(vao9);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



			// Simulate jump physics
			if (pressJump == true) {
				position.y += jumpVelocity;       // position updated based on velocity
				jumpVelocity += gravity_jump;          // gravity - each frame=> the velocity gets smaller so the bunny goes up/down slower until jumpvelocity is 0 and it goes to -

				// Check if bunny lands back on the ground
				if (position.y <= -0.2f) {
					position.y = -0.2f;           // Reset to ground level
					pressJump = false;            // Allow another jump
					jumpVelocity = 0.0f;          // Reset jump velocity
				}
			}

			position.x = glm::clamp(position.x, -1.0f, 1.0f);

			//Shooting carrots

			if (pressShoot == true) {
				carrotPosition.x += 0.001;

				if (carrotPosition.x >= position2.x) {

					carrotPosition.y = position.y - 0.25;
					carrotPosition.x = position.x + 0.15;

					bearHealth -= 20;
					position9.x += 0.15;
					pressShoot = false;

					if (bearHealth <= 0) {
						glDeleteBuffers(1, &vbo2);
						glDeleteBuffers(1, &ibo2);
						glDeleteVertexArrays(1, &vao2);

						glDeleteBuffers(1, &vbo4);
						glDeleteBuffers(1, &ibo4);
						glDeleteVertexArrays(1, &vao4);

					}
				}


				if (carrotPosition.y <= -0.7f) {
					pressShoot = false;                      // Stop shooting
					carrotPosition.y = position.y - 0.25;
					carrotPosition.x = position.x + 0.15;
				}
			}
		}
		else {

			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ibo);
			glDeleteVertexArrays(1, &vao);

			glDeleteBuffers(1, &vbo2);
			glDeleteBuffers(1, &ibo2);
			glDeleteVertexArrays(1, &vao2);

			glDeleteBuffers(1, &vbo4);
			glDeleteBuffers(1, &ibo4);
			glDeleteVertexArrays(1, &vao4);

			glDeleteBuffers(1, &vbo5);
			glDeleteBuffers(1, &ibo5);
			glDeleteVertexArrays(1, &vao5);

			glDeleteBuffers(1, &vbo6);
			glDeleteBuffers(1, &ibo6);
			glDeleteVertexArrays(1, &vao6);

			glDeleteBuffers(1, &vbo7);
			glDeleteBuffers(1, &ibo7);
			glDeleteVertexArrays(1, &vao7);

			glDeleteBuffers(1, &vbo8);
			glDeleteBuffers(1, &ibo8);
			glDeleteVertexArrays(1, &vao8);

			glDeleteBuffers(1, &vbo9);
			glDeleteBuffers(1, &ibo9);
			glDeleteVertexArrays(1, &vao9);

			glDeleteBuffers(1, &vbo10);
			glDeleteBuffers(1, &ibo10);
			glDeleteVertexArrays(1, &vao10);

			// Render BUNNY
			glm::mat4 bunnyTransform1 = glm::translate(glm::mat4(1.0f), finalPosition); 
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(bunnyTransform1));


			glm::vec4 bunnyColor3 (0.47f, 0.0f, 0.0f, 0.0f);
			glUniform4fv(colorLoc, 1, glm::value_ptr(bunnyColor3));
			glBindVertexArray(vao11);

			glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

		}

		position.x = glm::clamp(position.x, -1.0f, 1.0f);

		// Swap buffers
		glfwSwapBuffers(window);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &ibo2);
	glDeleteVertexArrays(1, &vao2);

	glDeleteBuffers(1, &vbo3);
	glDeleteBuffers(1, &ibo3);
	glDeleteVertexArrays(1, &vao3);

	glDeleteBuffers(1, &vbo4);
	glDeleteBuffers(1, &ibo4);
	glDeleteVertexArrays(1, &vao4);

	glDeleteBuffers(1, &vbo5);
	glDeleteBuffers(1, &ibo5);
	glDeleteVertexArrays(1, &vao5);

	glDeleteBuffers(1, &vbo6);
	glDeleteBuffers(1, &ibo6);
	glDeleteVertexArrays(1, &vao6);

	glDeleteBuffers(1, &vbo7);
	glDeleteBuffers(1, &ibo7);
	glDeleteVertexArrays(1, &vao7);

	glDeleteBuffers(1, &vbo8);
	glDeleteBuffers(1, &ibo8);
	glDeleteVertexArrays(1, &vao8);

	glDeleteBuffers(1, &vbo9);
	glDeleteBuffers(1, &ibo9);
	glDeleteVertexArrays(1, &vao9);

	glDeleteBuffers(1, &vbo10);
	glDeleteBuffers(1, &ibo10);
	glDeleteVertexArrays(1, &vao10);

	glDeleteBuffers(1, &vbo11);
	glDeleteBuffers(1, &ibo11);
	glDeleteVertexArrays(1, &vao11);

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}