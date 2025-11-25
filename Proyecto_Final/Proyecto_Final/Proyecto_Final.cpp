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
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-1.0f, 2.0f, 9.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-1.0f,3.0f, 0.0f),
	glm::vec3(0.0f,1.0f, 0.0f),
	glm::vec3(0.0f,0.5f,  0.0f),
	glm::vec3(1.0f,0.25f, 0.0f)
};
//Modelado de Objetos
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


#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;

// ============================================================
// Funciones para Transformaciones Geometricas
// ============================================================
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
//caja
//dibujar con rotacion
glm::mat4 DibujarC(glm::mat4 model, glm::vec3 escala, glm::vec3 traslado, glm::vec3 rotacion, float angulo, GLint uniformModel, int cara) {
	glm::mat4 modelTemp = model; // copia la transformación actual
	modelTemp = model = glm::translate(modelTemp, traslado);
	model = glm::rotate(model, glm::radians(angulo), rotacion);
	model = glm::scale(model, escala);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, cara);

	return modelTemp; // devolvemos la transformación
}
//dibujar Modelo con rotacion
glm::mat4 DibujarMu(glm::mat4 model, glm::vec3 traslado, glm::vec3 escala, glm::vec3 rotacion, float angulo, GLint modelLoc) {
	glm::mat4 modelTemp = model; // copia la transformación actual
	modelTemp = model = glm::translate(modelTemp, traslado);
	modelTemp = model = glm::rotate(model, glm::radians(angulo), rotacion);
	model = glm::scale(model, escala);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	return modelTemp; // devolvemos la transformación
}
glm::mat4 DibujarM(glm::mat4 model, glm::vec3 traslado, glm::vec3 escala, glm::vec3 rotacion, float angulo, GLint modelLoc) {
	glm::mat4 modelTemp = model; // copia la transformación actual
	modelTemp = model = glm::translate(modelTemp, traslado);
	model = glm::rotate(model, glm::radians(angulo), rotacion);
	model = glm::scale(model, escala);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	return modelTemp; // devolvemos la transformación
}
//dibujar Modelo con rotacion
glm::mat4 DibujarMu2(glm::mat4 model, glm::vec3 traslado, glm::vec3 escala, glm::vec3 rotacion, float angulo, GLint modelLoc) {
	glm::mat4 modelTemp = model; // copia la transformación actual
	model = glm::translate(modelTemp, traslado);
	model = glm::rotate(model, glm::radians(angulo), rotacion);
	model = glm::scale(model, escala);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	return model; // devolvemos la transformación
}
// ============================================================
// FUNCIÓN PARA CARGA DE TEXTURAS
// ============================================================
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

//Animacion sillas
float desplazamientoSilla = 0.0f;
int direccionSilla = 0; // 1 = hacia +X, -1 = hacia -X, 0 = quieto
//float dCajon = 0.0f;
//int direccionCajon= 0; // 1 = hacia +X, -1 = hacia -X, 0 = quieto

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

//animacion pendulo
float penduloAngulo = 0.0f;    // ángulo actual
float penduloVelocidad = 2.0f; // velocidad de oscilación
float penduloAmplitud = 15.0f; // amplitud máxima del movimiento (grados)

// Variables de animación, murciélago
float bodyY = 0.0f;
float wingRot = 0.0f;
float headRot = 0.0f;
bool up = true;
bool wingUp = true;
float bodyX = 0.0f;
float bodyZ = 0.0f;
float bodyRotY = 0.0f;
// Máquina de estados
int batState = 0;  // 0: idle, 1: volando
float timeElapsed = 0.0f;
float ellipseA = 2.5f;   // Radio en eje X
float ellipseB = 1.5f;   // Radio en eje Z
//animacion puerta
float anguloPuerta = 0.0f;     // ángulo actual de la puerta
float velocidadPuerta = 90.0f; // grados por segundo
bool abrirPuerta = false;
bool cerrarPuerta = false;
//animacion cabeza
float tiempoTotal = 0.0f;

//escalas de modelos de OpenGL
float escalaModelo1 = 0.5f;  // aumentar o disminuir la mesa
float escalaModelo2 = 0.5f;
float escalaModelo3 = 0.25f;
float escalaModelo4 = 0.25f;
float escalaModelo5 = 0.5f;
float escalaModelo6 = 0.25f;
float escalaModelo7 = 0.25f;
float escalaModelo8 = 0.3f;

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
	Shader skyboxshader("Shader/SkyBox.vs","Shader/SkyBox.frag");


	//models
	Model Piso((char*)"Models/piso.obj");
	// Modelos del murciélago
	Model Body((char*)"Models/mur-cuerpo.obj");
	Model Head((char*)"Models/mur-cabeza.obj");
	Model LeftWing((char*)"Models/mur-alaizq.obj");
	Model RightWing((char*)"Models/mur-alader.obj");
	//modelos de decoracion
	/*Model tumba1((char*)"Models/tumba1.obj");
	Model tumba2((char*)"Models/tumba2.obj");
	Model hongo((char*)"Models/hongo.obj");
	Model foco3((char*)"Models/foco3.obj");*/
	/*Model repisas1((char*)"Models/repisas.obj");*/
	Model repisas2((char*)"Models/repisas2.obj");
	/*Model repisas3((char*)"Models/repisas3.obj");
	Model mesa1((char*)"Models/mesa.obj");
	Model mesa2((char*)"Models/mesa2.obj");
	Model mesa3((char*)"Models/mesa3.obj");
	Model basura((char*)"Models/basura.obj");
	Model ropero((char*)"Models/ropa.obj");*/
	//Objetos Habitación 1
	Model sofah1((char*)"Models/sofah.obj");
	Model sofah2((char*)"Models/sofah2.obj");
	Model amp((char*)"Models/amplificador.obj");
	Model toc((char*)"Models/tocadiscos.obj");
	Model banco((char*)"Models/bancos.obj");
	//Objetos Habitación 2
	Model tv((char*)"Models/tv.obj");
	Model sof((char*)"Models/sof.obj");
	Model cama((char*)"Models/cama.obj");

	Model cabe1((char*)"Models/cara1.obj");
	Model cabe2((char*)"Models/cara2.obj");
	Model cabe3((char*)"Models/cara3.obj");
	Model cabe4((char*)"Models/cara4.obj");
	Model cabe5((char*)"Models/cara5.obj");
	
	//Modelo de la casa
	Model casa((char*)"Models/mmarceline.obj");
	Model ventana((char*)"Models/ventanas.obj");
	Model puertah1((char*)"Models/puertaAnim.obj");
	//Model relo((char*)"Models/reloj.obj");
	//Model luna((char*)"Models/luna.obj");
	
	//Skybox
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint index[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(2);


	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	// ============================================================
    // Skybox
    // ============================================================
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);

	// Cierra el VAO aquí
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//cargando texturas skybox
	vector<const GLchar*>faces;

	faces.push_back("SkyBox/px.jpg");
	faces.push_back("SkyBox/nx.jpg");
	faces.push_back("SkyBox/py.jpg");
	faces.push_back("SkyBox/ny.jpg");
	faces.push_back("SkyBox/pz.jpg");
	faces.push_back("SkyBox/nz.jpg");
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Carga de textura
	GLuint texture1 = loadTexture("models/mesa.jpg");
	GLuint texture4 = loadTexture("models/marceline.png");
	GLuint texture13 = loadTexture("models/cocina2.jpg");
	GLuint texture11 = loadTexture("models/cocina.jpg");
	GLuint texture3 = loadTexture("models/libro.jpg");
	GLuint texture5 = loadTexture("models/marcos.png");
	GLuint texture14 = loadTexture("models/v.jpg");
	GLuint texture9 = loadTexture("models/mueble2.jpg");
	/*GLuint texture2 = loadTexture("images/vhs1.jpg");
	GLuint texture3 = loadTexture("images/libro.jpg");
	GLuint texture4 = loadTexture("images/marceline.png");
	GLuint texture5 = loadTexture("images/marcos.png");
	GLuint texture6 = loadTexture("images/calavera.png");
	GLuint texture7 = loadTexture("images/soporte.jpg");
	GLuint texture8 = loadTexture("images/mueble1.png");
	GLuint texture9 = loadTexture("images/mueble2.jpg");
	GLuint texture10 = loadTexture("images/caja.jpg");
	GLuint texture11 = loadTexture("images/cocina.jpg");
	GLuint texture12 = loadTexture("images/pendulo.jpg");
	GLuint texture13 = loadTexture("images/cocina2.jpg");
	GLuint texture14 = loadTexture("images/v.jpg");*/

	float velocidad = 0.5f; // velocidad de movimiento (unidades por segundo)

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

		float tiempo = glfwGetTime(); // tiempo en segundos desde que inició el programa
		penduloAngulo = sin(tiempo * penduloVelocidad) * penduloAmplitud;

		//Animacion de silla 

		if (direccionSilla == 1 && desplazamientoSilla < 1.0f) {
			desplazamientoSilla += velocidad * deltaTime;
		}
		if (direccionSilla == -1 && desplazamientoSilla > 0.0f) {
			desplazamientoSilla -= velocidad * deltaTime;
		}
		/*if (direccionCajon == 1 && dCajon < 0.7f) {
			dCajon += velocidad * deltaTime;
		}
		if (direccionCajon == -1 && dCajon > 0.0f) {
			dCajon -= velocidad * deltaTime;
		}*/
		// Animación suave
		if (abrirPuerta) {
			anguloPuerta += velocidadPuerta * deltaTime;
			if (anguloPuerta >= 90.0f) {
				anguloPuerta = 90.0f;
				abrirPuerta = false;
			}
		}

		if (cerrarPuerta) {
			anguloPuerta -= velocidadPuerta * deltaTime;
			if (anguloPuerta <= 0.0f) {
				anguloPuerta = 0.0f;
				cerrarPuerta = false;
			}
		}
		//animacion cabeza
		tiempoTotal += deltaTime;
		float anguloCuernos = sin(tiempoTotal * 2.0f) * 5.0f;   // ±5° en X
		float anguloColmillos = sin(tiempoTotal * 3.0f) * 3.0f; // ±3° en Y


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
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		//// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 1.8f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Spotlight suave desde la cámara
		/*glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.9f, 0.9f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));*/








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
		model = DibujarMu(model, glm::vec3(0.0, 0.0, -0.0), glm::vec3(1.1f, 1.1f, 1.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		Piso.Draw(lightingShader);

		// ============================================================
		// CASA
		// ============================================================
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-6.8, -0.4, -3.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		casa.Draw(lightingShader);

		// ============================================================
		// ANIMACIÓN DE LA PUERTA
		// ============================================================
		//Puerta
		model = glm::mat4(1.0f);
		//model = DibujarMu(model, glm::vec3(-6.8, -0.4, -3.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		model = glm::translate(model, glm::vec3(-0.13192, -0.4, 1.30454));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		float offsetPuertaX = -1.3f;
		// mover hacia pivote
		model = glm::translate(model, glm::vec3(offsetPuertaX, 0.0f, 0.0f));
		// aplicar la rotación animada
		model = glm::rotate(model, glm::radians(anguloPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
		// regresar a la posición original
		model = glm::translate(model, glm::vec3(-offsetPuertaX, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertah1.Draw(lightingShader);

		// ============================================================
		// ANIMACIÓN MURCIÉLAGOS
		// ============================================================
		//murciélago 1
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		// Cuerpo del murciélago (padre)
		modelTemp = model = DibujarMu(model, glm::vec3(bodyX, 5.0 + bodyY, -2.0 + bodyZ), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 1.0f, 0.0f), bodyRotY, modelLoc);
		Body.Draw(lightingShader);
		// Cabeza (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(1.0f, 0.0f, 0.0f), headRot, modelLoc);
		Head.Draw(lightingShader);
		// Ala izquierda (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 1.0f), -wingRot, modelLoc);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LeftWing.Draw(lightingShader);
		// Ala derecha (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 1.0f), wingRot, modelLoc);
		RightWing.Draw(lightingShader);

		//murciélago 2
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		// Cuerpo del murciélago (padre)
		modelTemp = model = DibujarMu(model, glm::vec3(2.0 + bodyX, 5.0 + bodyY, 1.0 + bodyZ), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 1.0f, 0.0f), bodyRotY, modelLoc);
		Body.Draw(lightingShader);
		// Cabeza (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(1.0f, 0.0f, 0.0f), headRot, modelLoc);
		Head.Draw(lightingShader);
		// Ala izquierda (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 1.0f), -wingRot, modelLoc);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LeftWing.Draw(lightingShader);
		// Ala derecha (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 1.0f), wingRot, modelLoc);
		RightWing.Draw(lightingShader);

		//murciélago 3
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		// Cuerpo del murciélago (padre)
		modelTemp = model = DibujarMu(model, glm::vec3(-2.0 + bodyX, 5.0 + bodyY, -2.0 + bodyZ), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 1.0f, 0.0f), bodyRotY, modelLoc);
		Body.Draw(lightingShader);
		// Cabeza (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(1.0f, 0.0f, 0.0f), headRot, modelLoc);
		Head.Draw(lightingShader);
		// Ala izquierda (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 1.0f), -wingRot, modelLoc);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LeftWing.Draw(lightingShader);
		// Ala derecha (hijo)
		model = modelTemp;
		modelTemp = model = DibujarM(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.07f, 0.07f, 0.07f), glm::vec3(0.0f, 0.0f, 1.0f), wingRot, modelLoc);
		RightWing.Draw(lightingShader);

		// ============================================================
		// MODELOS DE DECORACIÓN
		// ============================================================
		//model = glm::mat4(1.0f);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		//////Tumba1
		//modelTemp=model = DibujarMu(model, glm::vec3(1.0, -0.58, 0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, modelLoc);
		//tumba1.Draw(lightingShader);
		//////Tumba2
		//model = DibujarMu(model, glm::vec3(-0.9, 0, 0.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//tumba2.Draw(lightingShader);
		//model = DibujarMu(modelTemp, glm::vec3(0.9, 0, 0.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//tumba2.Draw(lightingShader);
		//model = DibujarMu(model, glm::vec3(0.9, 0, 0.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//tumba1.Draw(lightingShader);

		//////Hongo
		//model = DibujarMu(model, glm::vec3(0.8, 0.19, 1.4), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//hongo.Draw(lightingShader);
		//model = DibujarMu(model, glm::vec3(-4.6, 0,0), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//hongo.Draw(lightingShader);
		//model = DibujarMu(model, glm::vec3(0, 0, -4.0), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//hongo.Draw(lightingShader);

		////Basura
		//model = DibujarMu(model, glm::vec3(4.05, 0.4, 1.15f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//basura.Draw(lightingShader);





		////Foco
		//model = DibujarMu(model, glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//foco3.Draw(lightingShader);

		////Repisa1
		//model = DibujarMu(model, glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//repisas1.Draw(lightingShader);
		//Repisa2
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(0.4, 1.0, 0.0), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, modelLoc);
		repisas2.Draw(lightingShader);
		////Repisa3
		//model = DibujarMu(model, glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//repisas3.Draw(lightingShader);

		////Mesa 1
		//model = DibujarMu(model, glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//mesa1.Draw(lightingShader);
		//Mesa 2
		/*model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(0.19, 0.3, 0), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, modelLoc);
		mesa2.Draw(lightingShader);*/
		////Mesa 3
		//model = DibujarMu(model, glm::vec3(0, 5.0, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		//mesa3.Draw(lightingShader);

		//ropero
		/*model = DibujarMu(model, glm::vec3(0, 5.0, 0), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		ropero.Draw(lightingShader);*/
		/*model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(4.05, 0.4, 1.15f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		relo.Draw(lightingShader);*/
		/*model = glm::mat4(1.0f);
		model = DibujarMu(modelTemp, glm::vec3(-1.02, 1.6, -3.35f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		relo.Draw(lightingShader);*/
		// ============================================================
		// OBJETOS DE HABITACIÓN 1
		// ============================================================
		//SOFA 1
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(0.19f, 0.2f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, modelLoc);
		sofah1.Draw(lightingShader);
		//SOFA 2
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-1.7f, 0.2f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, modelLoc);
		sofah2.Draw(lightingShader);
		//AMPLIFICDOR
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-2.4f, 0.1f, -0.3f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, modelLoc);
		amp.Draw(lightingShader);
		//TOCADISCOS
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-2.4f, 0.1, 0.3f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, modelLoc);
		toc.Draw(lightingShader);
		//BANCOS
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-0.8f, 0.1f, -0.8f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0, modelLoc);
		banco.Draw(lightingShader);

		// ============================================================
		// OBJETOS DE HABITACIÓN 2
		// ============================================================
		//1. TV
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-1.3, 1.8f, -1.8f), glm::vec3(0.028f, 0.028f, 0.028f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0, modelLoc);
		tv.Draw(lightingShader);
		//2. SOFA
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-1.65f, 1.91f, -1.93f), glm::vec3(0.16f, 0.16f, 0.16f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, modelLoc);
		sof.Draw(lightingShader);
		//3. CAMA
		model = glm::mat4(1.0f);
		model = DibujarMu(model, glm::vec3(-1.05f, 1.865f, 1.0f), glm::vec3(0.04f, 0.04f, 0.04f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0, modelLoc);
		cama.Draw(lightingShader);

		//4. Cabeza animada
		model = glm::mat4(1.0f);
		modelTemp = model = DibujarMu(model, glm::vec3(-1.6f, 2.2f, -1.93f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, modelLoc);
		cabe1.Draw(lightingShader);
		//colmillo1
		model = modelTemp;
		model = glm::rotate(model, glm::radians(anguloColmillos), glm::vec3(0, 1, 0));
		model = DibujarMu(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, modelLoc);
		cabe2.Draw(lightingShader);
		//colmillo2
		model = modelTemp;
		model = glm::rotate(model, glm::radians(-anguloColmillos), glm::vec3(0, 1, 0));
		model = DibujarMu(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, modelLoc);
		cabe3.Draw(lightingShader);
		//cuerno1
		model = modelTemp;
		model = glm::rotate(model, glm::radians(anguloCuernos), glm::vec3(0, 0, 1));
		model = DibujarMu(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, modelLoc);
		cabe4.Draw(lightingShader);
		//cuerno2
		model = modelTemp;
		model = glm::rotate(model, glm::radians(anguloCuernos), glm::vec3(0, 0, 1));
		model = DibujarMu(model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, modelLoc);
		cabe5.Draw(lightingShader);


		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		model = DibujarMu2(model, glm::vec3(-6.8, -0.4, -3.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f), 0, modelLoc);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ventana.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);


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
		model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ============================================================
        // MODELADO CON OPENGL
        // ============================================================
		
		//// ============================================================
		////1. Mesa y Sillas
		//// ============================================================
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Bind diffuse map 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1.0f);
		modelTemp = glm::mat4(1.0f);
		modelTemp1 = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(escalaModelo1));
		model= glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//Tabla
		modelTemp1=modelTemp=model= glm::translate(model, glm::vec3(1.0f, 1.0f, -8.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.1f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		////Patas Tabla
		modelTemp=Dibujar(modelTemp, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(1.45f, -0.5f, 0.95f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(-1.45f, -0.5f, 0.95f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(-1.45, -0.5f, -0.95f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 0.1f), glm::vec3(1.45f, -0.5f, -0.95f), modelLoc);
		////Silla 1
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(1.5f + desplazamientoSilla, -0.7f, 0.35f), modelLoc);//patas
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(1.5f + desplazamientoSilla, -0.7f, -0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(2.1f + desplazamientoSilla, -0.7f, 0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(2.1f + desplazamientoSilla, -0.7f, -0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.7f, 0.1f, 0.8f), glm::vec3(1.8f + desplazamientoSilla, -0.4f, 0.0f), modelLoc);//asiento
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.7f, 0.8f), glm::vec3(2.1f + desplazamientoSilla, 0.0f, 0.0f), modelLoc);//respaldo
        //silla2
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-1.5f - desplazamientoSilla, -0.7f, -0.35f), modelLoc);//patas
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-1.5f - desplazamientoSilla, -0.7f, 0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-2.1f - desplazamientoSilla, -0.7f, 0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-2.1f - desplazamientoSilla, -0.7f, -0.35f), modelLoc);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.7f, 0.1f, 0.8f), glm::vec3(-1.8f - desplazamientoSilla, -0.4f, 0.0f), modelLoc);//asiento
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.7f, 0.8f), glm::vec3(-2.1f - desplazamientoSilla, 0.0f, 0.0f), modelLoc);//respaldo

		////// ============================================================
		//////2. Caja con VHS
		////// ============================================================
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(escalaModelo2));
		////model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//modelTemp=modelTemp1= Dibujar(model, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(-6.0f, 0.3f, 1.5f), modelLoc);//VHS1
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.0f, 0.1f, 0.0f), modelLoc);//VHS2
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.3f, 0.0f, 0.0f), modelLoc);//VHS3
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.f, -0.1f, 0.0f), modelLoc);//VHS4
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(-0.3f, 0.0f, 0.0f), modelLoc);//VHS5
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.0f, 0.1f, 0.0f), modelLoc);//VHS6
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.0f, 0.1f, 0.0f), modelLoc);//VHS7
		//modelTemp = DibujarR(modelTemp, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(0.455f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),45.0f,modelLoc);//VHS8
		//modelTemp = DibujarR(modelTemp1, glm::vec3(0.3f, 0.1f, 0.6f), glm::vec3(-0.3f, 0.2f, 0.4f), glm::vec3(1.0f, 0.0f, 0.0f), 45.0f, modelLoc);//VHS9
		////Caja
		//glBindTexture(GL_TEXTURE_2D, texture10);
		//glm::mat4(1.0f);
		//model=glm::translate(modelTemp1, glm::vec3(0.0f, 0.1f, 0.15f));
		//model = glm::scale(model, glm::vec3(1.0f, 0.4f, 1.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 30);
		//modelTemp = DibujarC(modelTemp1, glm::vec3(1.0f, 0.4f, 0.5f), glm::vec3(0.0f, 0.33f, 0.965f), glm::vec3(1.0f, 0.0f, 0.0f), -45.0, modelLoc,6);
		//modelTemp = DibujarC(modelTemp1, glm::vec3(1.0f, 0.4f, 0.5f), glm::vec3(0.0f, -0.03f, -0.315f), glm::vec3(1.0f, 0.0f, 0.0f), 45.0, modelLoc, 6);

		//// ============================================================
		////3. Librero
		//// ============================================================
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(escalaModelo3));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//modelTemp1=modelTemp = Dibujar(model, glm::vec3(2.0f, 0.1f, 1.0f), glm::vec3(-2.0f, 1.5f, -9.5f), modelLoc);
		//glBindTexture(GL_TEXTURE_2D, texture3);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(-0.8f, 0.45f, 0.0f), modelLoc);//libro1
		//glBindTexture(GL_TEXTURE_2D, texture8);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.2f, 0.0f, 0.0f), modelLoc);//libro2
		//glBindTexture(GL_TEXTURE_2D, texture3);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.2f, 0.0f, 0.0f), modelLoc);//libro3
		//glBindTexture(GL_TEXTURE_2D, texture8);
		//modelTemp = DibujarR(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.41f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),45.0,modelLoc);//libro4
		////mueble completo
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//modelTemp = Dibujar(modelTemp1, glm::vec3(2.0f, 0.1f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), modelLoc);//repisa1
		//modelTemp = Dibujar(modelTemp1, glm::vec3(2.0f, 0.1f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), modelLoc);//repisa2
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 2.1f, 1.0f), glm::vec3(-1.05f, 0.0f, 0.0f), modelLoc);//lateral1
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 2.1f, 1.0f), glm::vec3(1.05f, 0.0f, 0.0f), modelLoc);//lateral2
		//modelTemp = Dibujar(modelTemp1, glm::vec3(2.2f, 2.1f, 0.1f), glm::vec3(0.0f, 0.0f, -0.55f), modelLoc);//parte trasera
		////cajon
		//modelTemp = Dibujar(modelTemp1, glm::vec3(2.1f, 1.0f, 0.1f), glm::vec3(0.0f, -0.5f, 0.55f + dCajon), modelLoc);//puerta
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 1.0f), glm::vec3(-0.95f, -0.5f, 0.0f + dCajon), modelLoc);//cajon1
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.9f, 1.0f), glm::vec3(0.95f, -0.5f, 0.0f + dCajon), modelLoc);//cajon2
		//modelTemp = Dibujar(modelTemp1, glm::vec3(1.8f, 0.9f, 0.1f), glm::vec3(0.0f, -0.5f, -0.45f + dCajon), modelLoc);//cajon3
		//modelTemp = Dibujar(modelTemp1, glm::vec3(1.8f, 0.1f, 0.9f), glm::vec3(0.0f, -0.9f, 0.05f + dCajon), modelLoc);//cajon4
		//glBindTexture(GL_TEXTURE_2D, texture12);
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.2f, 0.2f, 0.1f), glm::vec3(0.0f, -0.5f, 0.6f + dCajon), modelLoc);//boton
		////detalles
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.3f, 0.1f), glm::vec3(0.9f, -1.2f, 0.4f), modelLoc);//pata1
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.3f, 0.1f), glm::vec3(-0.9f, -1.2f, 0.4f), modelLoc);//pata2
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.3f, 0.1f), glm::vec3(0.9f, -1.2f, -0.4f), modelLoc);//pata3
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.3f, 0.1f), glm::vec3(-0.9f, -1.2f, -0.4f), modelLoc);//pata4

		//// ============================================================
		////4. Cuadro y Estandarte
		//// ============================================================
		glBindTexture(GL_TEXTURE_2D, texture4);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(escalaModelo4));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp1 = modelTemp = model = glm::translate(model, glm::vec3(-4.0f, 5.0f, -1.8f));
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
		////Estandarte
		//glBindTexture(GL_TEXTURE_2D, texture6);
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(escalaModelo5));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//modelTemp1 = modelTemp = model = glm::translate(model, glm::vec3(14.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 1.5f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		////marcos
		//glBindTexture(GL_TEXTURE_2D, texture7);
		//modelTemp = Dibujar(modelTemp1, glm::vec3(1.0f, 0.1f, 0.1f), glm::vec3(0.0f, 2.55f, 0.0f), modelLoc);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(0.65f, 0.0f, 0.0f), modelLoc);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(-1.3f, 0.0f, 0.0f), modelLoc);
		//glBindTexture(GL_TEXTURE_2D, texture7);
		//modelTemp = Dibujar(modelTemp1, glm::vec3(1.0f, 0.1f, 0.1f), glm::vec3(0.0f, -2.55f, 0.0f), modelLoc);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(0.65f, 0.0f, 0.0f), modelLoc);
		//modelTemp = Dibujar(modelTemp, glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(-1.3f, 0.0f, 0.0f), modelLoc);
		//
		//// ============================================================
		////5. Mueble con microondas
		//// ============================================================
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(escalaModelo6));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, texture11); // textura blanca
		//glBindTexture(GL_TEXTURE_2D, texture8);//textura roja
		modelTemp1=modelTemp = Dibujar(model, glm::vec3(3.0f, 1.5f, 1.0f), glm::vec3(-4.0f,1.5f, -8.2f), modelLoc);//base
		glBindTexture(GL_TEXTURE_2D, texture13);
		modelTemp = Dibujar(modelTemp, glm::vec3(3.5f, 0.1f, 1.5f), glm::vec3(0.0f, 0.8f, 0.0f), modelLoc);//tabla
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.2f, 0.5f, 0.1f), glm::vec3(0.75f, 0.38f, 0.55f), modelLoc);//cajon1
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.2f, 0.5f, 0.1f), glm::vec3(0.75f, -0.38f, 0.55f), modelLoc);//cajon2
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.2f, 1.2f, 0.1f), glm::vec3(-0.75f, 0.0f, 0.55f), modelLoc);//cajon3
		glBindTexture(GL_TEXTURE_2D, texture9);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(1.4f, -0.95f, 0.4f), modelLoc);//pata1
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(1.4f, -0.95f, -0.4f), modelLoc);//pata2
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(-1.4f, -0.95f, 0.4f), modelLoc);//pata3
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(-1.4f, -0.95f, -0.4f), modelLoc);//pata4
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.75f, 0.38f, 0.65f), modelLoc);//boton1
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.75f, -0.38f, 0.65f), modelLoc);//boton2
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(-0.45f, 0.0f, 0.65f), modelLoc);//boton3
		//microondas
		glBindTexture(GL_TEXTURE_2D, texture13); // textura metálica clara
		modelTemp=Dibujar(modelTemp1, glm::vec3(1.6f, 0.9f, 0.8f), glm::vec3(0.0f, 1.3f, 0.0f), modelLoc); // cuerpo principal
		glBindTexture(GL_TEXTURE_2D, texture14);
		modelTemp1= Dibujar(modelTemp, glm::vec3(1.2f, 0.6f, 0.05f), glm::vec3(-0.08f, 0.0f, 0.425f), modelLoc);//puerta
		glBindTexture(GL_TEXTURE_2D, texture13);
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.2f, 0.05f, 0.05f), glm::vec3(0.0f, 0.325f, 0.0f), modelLoc);//marco superior
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.2f, 0.05f, 0.05f), glm::vec3(0.0f, -0.325f, 0.0f), modelLoc);//marco inferior
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.05f, 0.7f, 0.05f), glm::vec3(-0.625f, 0.0f, 0.0f), modelLoc);//marco izquierdo
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.05f, 0.7f, 0.05f), glm::vec3(0.625f, 0.0f, 0.0f), modelLoc);//marco derecho

		//// ============================================================
		////6. refrigerador:
		//// ============================================================
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(escalaModelo7));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, texture11); // textura blanca
		modelTemp1 = modelTemp = Dibujar(model, glm::vec3(1.5f, 3.0f, 1.2f), glm::vec3(-7.1f, 2.0f, -1.2f), modelLoc); // cuerpo principal
		glBindTexture(GL_TEXTURE_2D, texture13);
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.45f, 1.4f, 0.05f), glm::vec3(0.0f, 0.8f, 0.625f), modelLoc);// Puerta superior
		modelTemp = Dibujar(modelTemp1, glm::vec3(1.45f, 1.4f, 0.05f), glm::vec3(0.0f, -0.8f, 0.625f), modelLoc);// Puerta inferior
		glBindTexture(GL_TEXTURE_2D, texture9);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.05f, 0.8f, 0.05f), glm::vec3(0.7f, 0.8f, 0.65f), modelLoc);// Mango puerta superior
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.05f, 0.8f, 0.05f), glm::vec3(0.7f, -0.8f, 0.65f), modelLoc);// Mango puerta inferior
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.15f, 0.3f, 0.15f), glm::vec3(0.6f, -1.65f, 0.5f), modelLoc);// pata frontal derecha
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.15f, 0.3f, 0.15f), glm::vec3(-0.6f, -1.65f, 0.5f), modelLoc);// pata frontal izquierda
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.15f, 0.3f, 0.15f), glm::vec3(0.6f, -1.65f, -0.5f), modelLoc);// pata trasera derecha
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.15f, 0.3f, 0.15f), glm::vec3(-0.6f, -1.65f, -0.5f), modelLoc);// pata trasera izquierda
		glBindTexture(GL_TEXTURE_2D, texture3);
		modelTemp = Dibujar(modelTemp1, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(-0.6f, 1.9f, 0.0f), modelLoc);//cereal1
		modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.2f, 0.0f, 0.0f), modelLoc);//cerealo2
		modelTemp = Dibujar(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.2f, 0.0f, 0.0f), modelLoc);//cereal3
		modelTemp = DibujarR(modelTemp, glm::vec3(0.15f, 0.8f, 0.6f), glm::vec3(0.41f, -0.05f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 45.0, modelLoc);//cereal4


		////// ============================================================
		//////7. reloj
		////// ============================================================
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(escalaModelo8));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//modelTemp1 = modelTemp = Dibujar(model, glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(-3.4f, 0.5f, -8.0f), modelLoc);//base1
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 3.0f, 0.6f), glm::vec3(0.4f, 1.8f, 0.0f), modelLoc);//soporte1
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.1f, 3.0f, 0.6f), glm::vec3(-0.4f, 1.8f, 0.0f), modelLoc);//soporte2
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.7f, 3.0f, 0.1f), glm::vec3(0.0f, 1.8f, -0.25f), modelLoc);//soporte3
		//modelTemp = Dibujar(modelTemp1, glm::vec3(1.0f, 0.1f, 1.0f), glm::vec3(0.0f, 2.5f, 0.0f), modelLoc);//base2
		//modelTemp = Dibujar(modelTemp1, glm::vec3(1.0f, 0.1f, 1.0f), glm::vec3(0.0f, 3.3f, 0.0f), modelLoc);//base3
		//modelTemp = Dibujar(modelTemp1, glm::vec3(0.8f, 0.7f, 0.1f), glm::vec3(0.0f, 2.9f, 0.0f), modelLoc);//base3
		////pendulo animado
		//glBindTexture(GL_TEXTURE_2D, texture12);
		//glm::mat4 pendulo = modelTemp1;
		//pendulo = glm::translate(pendulo, glm::vec3(0.0f, 3.f, 0.0f)); // punto donde cuelga
		//pendulo = glm::rotate(pendulo, glm::radians(penduloAngulo), glm::vec3(0.0f, 0.0f, 1.0f)); // rotación Z
		//pendulo = glm::translate(pendulo, glm::vec3(0.0f, -1.0f, 0.0f)); // bajar el péndulo después de rotar
		//pendulo = glm::scale(pendulo, glm::vec3(0.1f, 1.0f, 0.1f)); // tamaño del péndulo
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pendulo));
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		

		glBindVertexArray(0);


		// ============================================================
        // Dibujando Skybox
        // ============================================================
		glDepthFunc(GL_LEQUAL);//funcion de profundidad
		skyboxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program,"view"),1,GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

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

	// ROTACIÓN CON Q Y E
	float giroVel = 120.0f;  // grados por segundo

	if (keys[GLFW_KEY_Q])
		camera.ProcessMouseMovement(-giroVel * deltaTime, 0.0f);  // rota hacia la izquierda

	if (keys[GLFW_KEY_E])
		camera.ProcessMouseMovement(giroVel * deltaTime, 0.0f);   // rota hacia la derecha

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

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

	//Animar sillas
	if (keys[GLFW_KEY_Z]) {
		direccionSilla = +1; // mover hacia +X
	}
	if (keys[GLFW_KEY_X]) {
		direccionSilla = -1; // mover hacia -X
	}
	//Animar puerta
	if (keys[GLFW_KEY_C]) {
		abrirPuerta = true;
		cerrarPuerta = false;
	}

	if (keys[GLFW_KEY_V]) {
		cerrarPuerta = true;
		abrirPuerta = false;
	}


}
void Animation() {

	timeElapsed += deltaTime;

	// Movimiento elíptico en XZ
	bodyX = ellipseA * cos(timeElapsed * 0.8f);
	bodyZ = ellipseB * sin(timeElapsed * 0.8f);

	// Movimiento vertical oscilante (vuelo)
	//bodyY = sin(timeElapsed * 2.0f) * 0.1f;

	// Dirección de movimiento(derivadas)
	float dx = -ellipseA * sin(timeElapsed * 0.8f);
	float dz = ellipseB * cos(timeElapsed * 0.8f);

	// Ángulo en radianes -> grados
	bodyRotY = glm::degrees(atan2(dx, dz)); // dirección tangencial a la elipse

	// Movimiento vertical del cuerpo (sube y baja)
	if (up) {
		bodyY += 0.002f;
		if (bodyY > 0.1f) up = false;
	}
	else {
		bodyY -= 0.002f;
		if (bodyY < -0.1f) up = true;
	}
	// Movimiento de las alas
	if (wingUp) {
		wingRot += 2.0f;
		if (wingRot > 45.0f) wingUp = false;
	}
	else {
		wingRot -= 2.0f;
		if (wingRot < -10.0f) wingUp = true;
	}

	// Oscilación suave de la cabeza
	headRot = sin(glfwGetTime() * 2.0f) * 10.0f;

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



