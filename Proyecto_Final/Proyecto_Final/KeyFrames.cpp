#include <iostream>
#include <cmath>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include "stb_image.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//Load Models
#include "SOIL2/SOIL2.h"
// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//arco
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//frente
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//atras
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//izquierda
	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//derecha
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//abajo
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//arriba
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f
};

GLuint indices[] = {
	0,1,2, 2,3,0,   // frente
	4,5,6, 6,7,4,   // atrás
	8,9,10, 10,11,8, // izquierda
	12,13,14, 14,15,12, // derecha
	16,17,18, 18,19,16, // arriba
	20,21,22, 22,23,20  // abajo
};


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;

//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float FLegs;
	float RLegs;
	float head;
	float headInc;
	float tail;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;
	rotDog = KeyFrame[0].rotDog;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

}

//dibujar
glm::mat4 Dibujar(glm::mat4 model, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel) {
	glm::mat4 modelTemp = model; // copia la transformación actual
	modelTemp= model = glm::translate(modelTemp, traslado);
	model = glm::scale(model, escala);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	return modelTemp; // devolvemos la transformación
}
//dibujar con rotacion
glm::mat4 DibujarR(glm::mat4 model, glm::vec3 escala, glm::vec3 traslado, glm::vec3 rotacion, float angulo, GLint uniformModel) {
	glm::mat4 modelTemp = model; // copia la transformación actual
	modelTemp = model = glm::translate(modelTemp, traslado);
	model = glm::rotate(model, glm::radians(angulo), rotacion);
	model = glm::scale(model, escala);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	return modelTemp; // devolvemos la transformación
}

// FUNCIÓN PARA CARGA DE TEXTURAS
GLuint loadTexture(const char* path) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load(path, &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error al cargar textura: " << path << std::endl;
	}
	stbi_image_free(image);
	return textureID;
}

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//Arco
	std::vector<float> verticesArco;
	float radio = 0.5f;
	int segmentos = 40;

	for (int i = 0; i <= segmentos; i++) {
		float theta = M_PI * i / segmentos; // 0 a 180 grados
		float x = radio * cos(theta);
		float y = radio * sin(theta);
		float z = 0.0f;

		// posición (x,y,z), color (blanco), coordenadas textura (x,y)
		verticesArco.insert(verticesArco.end(), { x, y, z, 1.0f, 1.0f, 1.0f, x, y });
	}
	// ============================================================
	// CONFIGURAR VAO/VBO DEL ARCO
	// ============================================================
	GLuint VAOarco, VBOarco;
	glGenVertexArrays(1, &VAOarco);
	glGenBuffers(1, &VBOarco);

	glBindVertexArray(VAOarco);
	glBindBuffer(GL_ARRAY_BUFFER, VBOarco);
	glBufferData(GL_ARRAY_BUFFER, verticesArco.size() * sizeof(float), verticesArco.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);



	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].headInc = 0;
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Carga de textura
	GLuint texture1 = loadTexture("images/mesa.jpg");
	GLuint texture2 = loadTexture("images/vhs1.jpg");
	GLuint texture3 = loadTexture("images/libro.jpg");
	GLuint texture4 = loadTexture("images/marceline.png");
	GLuint texture5 = loadTexture("images/marcos.png");

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//glm::mat4 model(1);


		//Modelando los 7 objetos
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelTemp = glm::mat4(1.0f);
		glm::mat4 modelTemp1 = glm::mat4(1.0f);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glBindVertexArray(0);

		// Bind diffuse map 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// ============================================================
        // MODELADO CON OPENGL
        // ============================================================
		//1. Mesa y 2. Sillas
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		modelTemp = glm::mat4(1.0f);
		modelTemp1 = glm::mat4(1.0f);
		//Tabla
		modelTemp1=modelTemp=model= glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.1f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		////Patas Tabla
		modelTemp=Dibujar(modelTemp, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(1.45f, -0.5f, 0.95f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(-1.45f, -0.5f, 0.95f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(-1.45, -0.5f, -0.95f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(1.45f, -0.5f, -0.95f), modelLoc);
		////Silla 1
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(2.0f, -0.7f, 0.35f), modelLoc);//patas
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(2.0f, -0.7f, -0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(2.6f, -0.7f, 0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(2.6f, -0.7f, -0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.7f, 0.1f, 0.8f), glm::vec3(2.3f, -0.4f, 0.0f), modelLoc);//asiento
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.7f, 0.8f), glm::vec3(2.6f, 0.0f, 0.0f), modelLoc);//respaldo
        //silla2
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-2.0f, -0.7f, -0.35f), modelLoc);//patas
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-2.0f, -0.7f, 0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-2.6f, -0.7f, 0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-2.6f, -0.7f, -0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.7f, 0.1f, 0.8f), glm::vec3(-2.3f, -0.4f, 0.0f), modelLoc);//asiento
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.7f, 0.8f), glm::vec3(-2.6f, 0.0f, 0.0f), modelLoc);//respaldo
		
		//3. VHS
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1.0f);
		//modelTemp = glm::mat4(1.0f);
		//modelTemp1 = glm::mat4(1.0f);
		modelTemp=modelTemp1= Dibujar(model, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(4.0f, 0.0f, 0.0f), modelLoc);
		//VHS2
		modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.0f, 0.1f, 0.0f), modelLoc);
		//VHS3
		modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.3f, 0.0f, 0.0f), modelLoc);
		//VHS4
		modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.f, -0.1f, 0.0f), modelLoc);
		//VHS5
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(-0.3f, 0.0f, 0.0f), modelLoc);
		//VHS6
		modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.0f, 0.1f, 0.0f), modelLoc);
		//VHS7
		modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.0f, 0.1f, 0.0f), modelLoc);
		//VHS8
		modelTemp = DibujarR(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.455f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),45.0f,modelLoc);
		//VHS9
		modelTemp = DibujarR(modelTemp1, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(-0.3f, 0.2f, 0.4f), glm::vec3(1.0f, 0.0f, 0.0f), 45.0f, modelLoc);

		//4. Repisa
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1.0f);
		modelTemp = Dibujar(model, glm::vec3(2.0f, 0.1f, 1.0f), glm::vec3(7.0f, 2.0f, 0.0f), modelLoc);
		//libro1
		glBindTexture(GL_TEXTURE_2D, texture3);
		modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(-0.8f, 0.45f, 0.0f), modelLoc);
		//libro2
		modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.2f, 0.0f, 0.0f), modelLoc);
		//libro3
		modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.2f, 0.0f, 0.0f), modelLoc);
		//libro4
		modelTemp = DibujarR(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.41f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),45.0,modelLoc);

		//5. Cuadro
		glBindTexture(GL_TEXTURE_2D, texture4);
		model = glm::mat4(1.0f);
		modelTemp1 = modelTemp = model = glm::translate(model, glm::vec3(11.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.8f, 1.4f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//marcos
		glBindTexture(GL_TEXTURE_2D, texture5);
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.35f, 0.1f, 0.1f), glm::vec3(0.0f, 0.9f, 0.0f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.35f, 0.1f, 0.1f), glm::vec3(0.0f, -0.9f, 0.0f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 1.9f, 0.1f), glm::vec3(0.7f, 0.0f, 0.0f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 1.9f, 0.1f), glm::vec3(-0.7f, 0.0f, 0.0f), modelLoc);
		//VHS8

		//6. Estandarte

		//

		glBindVertexArray(0);







		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls
	if (keys[GLFW_KEY_4])
	{

		head += 1.0f;

	}

	if (keys[GLFW_KEY_5])
	{

		head -= 1.0f;

	}

	if (keys[GLFW_KEY_2])
	{

		rotDog += 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{

		rotDog -= 1.0f;

	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.01;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.01;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}



	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].headInc;
			rotDog += KeyFrame[playIndex].rotDogInc;

			i_curr_steps++;
		}

	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}



