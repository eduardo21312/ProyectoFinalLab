/*---------------------------------------------------------*/
/* ----------------  ProyectoFinal -----------*/
/*-----------------    2021-2   ---------------------------*/
/*------------- Alumno: Martínez Matías Joan Eduardo  ---------------*/
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
//agregando libreria para audio
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		giroR2D2 = 0.0f,
		movPataIzq = 0.0f,
		movPataDer = 0.0f,
		movPataCh = 0.0f,
		movPataIzqX = 0.0f,
		movPataDerX = 0.0f,
		movPataIzqZ = 0.0f,
		movPataDerZ = 0.0f,
		movCuerpo = 0.0f,
		movCabeza = 0.0f,
		giro2R2D2 = 0.0f,
		giro3R2D2 = 0.0f,
		movGrafico = 0.0f,
		incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroR2D2Inc = 0.0f,
		giro2R2D2Inc = 0.0f,
		giro3R2D2Inc = 0.0f,
		movPataIzqInc = 0.0f,
		movPataDerInc = 0.0f,
		movPataIzqXInc = 0.0f,
		movPataDerXInc = 0.0f,
		movPataIzqZInc = 0.0f,
		movPataDerZInc = 0.0f,
		movPataChInc = 0.0f,
		movCuerpoInc = 0.0f,
		movCabezaInc = 0.0f,
		movYoda_z = 0.0f,
		movYoda_x = 0.0f,
		orientaYoda = 0.0f,
		movDroide_z = 0.0f,
		movDroide_x = 0.0f,
		orientaDroide = 0.0f;

float	posPodX = 0.0f,
		posPodY = 0.0f,
		posPodZ = 0.0f,
		rotPodX = 0.0f,
		rotPodY = 0.0f,
		rotPodZ = 0.0f,
		giroTurbina = 0.0f;
float	posPodXInc = 0.0f,
		posPodYInc = 0.0f,
		posPodZInc = 0.0f,
		rotPodXInc = 0.0f,
		rotPodYInc = 0.0f,
		rotPodZInc = 0.0f,
		giroTurbinaInc = 0.0f;

		
//Animación de Yoda
float movYodaX = 0.0;
float movYodaZ = 0.0;
float rotYoda = 0.0;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;

//Animación de Yoda
float movDroideX = 0.0;
float movDroideZ = 0.0;
float rotDroide = 0.0;

bool circuito2 = false;
bool recorrido1_1 = true;
bool recorrido2_1 = false;
bool recorrido3_1 = false;
bool recorrido4_1 = false;
bool recorrido5_1 = false;
bool recorrido6_1 = false;
bool recorrido7_1 = false;

#define MAX_FRAMES 31
int i_max_steps = 100;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float giroR2D2;
	float giro2R2D2;
	float giro3R2D2;
	float movPataIzq;
	float movPataDer;
	float movPataIzqX;
	float movPataDerX;
	float movPataIzqZ;
	float movPataDerZ;
	float movPataCh;
	float movCuerpo;
	float movCabeza;
	//potCARRERAS
	float posPodX;		//Variable para PosicionX
	float posPodY;		//Variable para PosicionY
	float posPodZ;		//Variable para PosicionZ
	float rotPodX;
	float rotPodY;
	float rotPodZ;
	float giroTurbina;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 31;		//numero de keyframes 
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].giroR2D2 = giroR2D2;
	KeyFrame[FrameIndex].giro2R2D2 = giro2R2D2;
	KeyFrame[FrameIndex].giro3R2D2 = giro3R2D2;
	KeyFrame[FrameIndex].movPataIzq = movPataIzq;
	KeyFrame[FrameIndex].movPataDer = movPataDer;
	KeyFrame[FrameIndex].movPataIzqX = movPataIzqX;
	KeyFrame[FrameIndex].movPataDerX = movPataDerX;
	KeyFrame[FrameIndex].movPataIzqZ = movPataIzqZ;
	KeyFrame[FrameIndex].movPataDerZ = movPataDerZ;
	KeyFrame[FrameIndex].movPataCh = movPataCh;
	KeyFrame[FrameIndex].movCuerpo = movCuerpo;
	KeyFrame[FrameIndex].movCabeza = movCabeza;

	KeyFrame[FrameIndex].posPodX = posPodX;
	KeyFrame[FrameIndex].posPodY = posPodY;
	KeyFrame[FrameIndex].posPodZ = posPodZ;
	KeyFrame[FrameIndex].rotPodX = rotPodX;
	KeyFrame[FrameIndex].rotPodY = rotPodY;
	KeyFrame[FrameIndex].rotPodZ = rotPodZ;
	KeyFrame[FrameIndex].giroTurbina = giroTurbina;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	//R2D2
	giroR2D2 = KeyFrame[0].giroR2D2;
	giro2R2D2 = KeyFrame[0].giro2R2D2;
	giro3R2D2 = KeyFrame[0].giro3R2D2;
	movPataIzq = KeyFrame[0].movPataIzq;
	movPataDer = KeyFrame[0].movPataDer;
	movPataIzqX = KeyFrame[0].movPataIzqX;
	movPataDerX = KeyFrame[0].movPataDerX;
	movPataIzqZ = KeyFrame[0].movPataIzqZ;
	movPataDerZ = KeyFrame[0].movPataDerZ;
	movPataCh = KeyFrame[0].movPataCh;
	movCuerpo = KeyFrame[0].movCuerpo;
	movCabeza = KeyFrame[0].movCabeza;
	//potCARRERAS
	posPodX = KeyFrame[0].posPodX;
	posPodY = KeyFrame[0].posPodY;
	posPodZ = KeyFrame[0].posPodZ;
	rotPodX = KeyFrame[0].rotPodX;
	rotPodY = KeyFrame[0].rotPodY;
	rotPodZ = KeyFrame[0].rotPodZ;
	giroTurbina = KeyFrame[0].giroTurbina;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	giroR2D2Inc = (KeyFrame[playIndex + 1].giroR2D2 - KeyFrame[playIndex].giroR2D2) / i_max_steps;
	giro2R2D2Inc = (KeyFrame[playIndex + 1].giro2R2D2 - KeyFrame[playIndex].giro2R2D2) / i_max_steps;
	giro3R2D2Inc = (KeyFrame[playIndex + 1].giro3R2D2 - KeyFrame[playIndex].giro3R2D2) / i_max_steps;
	movPataIzqInc = (KeyFrame[playIndex + 1].movPataIzq - KeyFrame[playIndex].movPataIzq) / i_max_steps;
	movPataDerInc = (KeyFrame[playIndex + 1].movPataDer - KeyFrame[playIndex].movPataDer) / i_max_steps;
	movPataIzqXInc = (KeyFrame[playIndex + 1].movPataIzqX - KeyFrame[playIndex].movPataIzqX) / i_max_steps;
	movPataDerXInc = (KeyFrame[playIndex + 1].movPataDerX - KeyFrame[playIndex].movPataDerX) / i_max_steps;
	movPataIzqZInc = (KeyFrame[playIndex + 1].movPataIzqZ - KeyFrame[playIndex].movPataIzqZ) / i_max_steps;
	movPataDerZInc = (KeyFrame[playIndex + 1].movPataDerZ - KeyFrame[playIndex].movPataDerZ) / i_max_steps;
	movPataChInc = (KeyFrame[playIndex + 1].movPataCh - KeyFrame[playIndex].movPataCh) / i_max_steps;
	movCuerpoInc = (KeyFrame[playIndex + 1].movCuerpo - KeyFrame[playIndex].movCuerpo) / i_max_steps;
	movCabezaInc = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps;	

	// PodRacer
	posPodXInc = (KeyFrame[playIndex + 1].posPodX - KeyFrame[playIndex].posPodX) / i_max_steps;
	posPodYInc = (KeyFrame[playIndex + 1].posPodY - KeyFrame[playIndex].posPodY) / i_max_steps;
	posPodZInc = (KeyFrame[playIndex + 1].posPodZ - KeyFrame[playIndex].posPodZ) / i_max_steps;
	rotPodXInc = (KeyFrame[playIndex + 1].rotPodX - KeyFrame[playIndex].rotPodX) / i_max_steps;
	rotPodYInc = (KeyFrame[playIndex + 1].rotPodY - KeyFrame[playIndex].rotPodY) / i_max_steps;
	rotPodZInc = (KeyFrame[playIndex + 1].rotPodZ - KeyFrame[playIndex].rotPodZ) / i_max_steps;
	giroTurbinaInc = (KeyFrame[playIndex + 1].giroTurbina - KeyFrame[playIndex].giroTurbina) / i_max_steps;
}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
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
			posX += incX;
			posY += incY;
			posZ += incZ;

			giroR2D2 += giroR2D2Inc;
			giro2R2D2 += giro2R2D2Inc;
			giro3R2D2 += giro3R2D2Inc;
			movPataIzq += movPataIzqInc;
			movPataDer += movPataDerInc;
			movPataIzqX += movPataIzqXInc;
			movPataDerX += movPataDerXInc;
			movPataIzqZ += movPataIzqZInc;
			movPataDerZ += movPataDerZInc;
			movPataCh += movPataChInc;
			movCuerpo += movCuerpoInc;
			movCabeza += movCabezaInc;
			i_curr_steps++;
		}
	}

	//Vehículo

	if (movCabeza <= 5.0f)
	{
		movCabeza += 10.0f;
	}
	else
	{
		movCabeza -= 0.22f;
	}

	movGrafico += 10.0f;




	if (circuito)
	{
		if (recorrido1)
		{
			movYodaX += 0.1f;
			if (movYodaX > 50)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotYoda = -90;
			movYodaZ += 0.1f;
			if (movYodaZ > 80)
			{
				recorrido2 = false;
				recorrido3 = true;

			}
		}

		if (recorrido3)
		{
			rotYoda = 135;
			movYodaX -= 0.1f;
			movYodaZ -= 0.1f;
			if (movYodaX < 0 && movYodaZ < 0)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			rotYoda = 0;
			movYodaZ += 0.5f;
			if (movYodaZ > 0)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}
	}

	if (circuito2)
	{
		if (recorrido1_1)
		{
			movDroideX -= 0.2f;
			movDroideZ -= 0.2f;
			if (movDroideX < -70 && movDroideZ < -70)
			{
				recorrido1_1 = false;
				recorrido2_1 = true;
			}
		}
		if (recorrido2_1)
		{
			rotDroide = -90;
			movDroideZ -= 0.2f;
			if (movDroideZ < -570)
			{
				recorrido2_1 = false;
				recorrido3_1 = true;

			}
		}
		if (recorrido3_1)
		{
			rotDroide = -180;
			movDroideX += 0.2f;
			if (movDroideX > 120)
			{
				recorrido3_1 = false;
				recorrido4_1 = true;
			}
		}
		if (recorrido4_1)
		{
			rotDroide = -360;
			movDroideX -= 0.2f;
			if (movDroideX < 0)
			{
				recorrido4_1 = false;
				recorrido5_1 = true;
			}
		}
		if (recorrido5_1)
		{
			rotDroide = 90;
			movDroideZ += 0.2f;
			if (movDroideZ > 5)
			{
				recorrido5_1 = false;
				recorrido6_1 = true;
			}
		}
		
		if (recorrido6_1)
		{
			rotDroide = 135;
			movDroideX += 0.2f;
			movDroideZ += 0.2f;

			if (movDroideX > 70 && movDroideZ > 70)
			{
				recorrido6_1 = false;
				recorrido7_1 = true;
			}
		}
		if (recorrido7_1)
		{
			rotDroide = -45;
			if (rotDroide = -45)
			{
				recorrido7_1 = false;
				recorrido1_1 = true;
			}
		}

	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

int main()
{
	//MUSICAFONDO
	//PlaySound(TEXT("ambienteTatooine.wav"), NULL, SND_ASYNC);
	
	
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right4.jpg",
		"resources/skybox/back4.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom2.jpg",
		"resources/skybox/left4.jpg",
		"resources/skybox/front5.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso2/piso2.obj");
	Model CasaT("resources/objects/CasaTatooine/CasaTatooine.obj");
	Model Pataizq("resources/objects/Pata izquierda (defrente)/pata izquierda color 2.obj");
	Model Patader("resources/objects/Pata derecha (defrente)R2D2/pata derecha color 2.obj");
	Model Patachica("resources/objects/Pata Chica R2D2/pata chica color 2.obj");
	Model Cuerpo("resources/objects/Cuerpo R2D2/Cuerpo Color 2.obj");
	Model Cabeza("resources/objects/Cabeza R2D2/Cabeza Color 3.obj");
	Model Base("resources/objects/Base/basehlgmcolor.obj");
	Model Grafico("resources/objects/Grafico/graficohlgmcolor.obj");
	Model CasaT2("resources/objects/Casa2Tatooine/casa2color.obj");
	Model CasaT3("resources/objects/Casa3Tatooine/casa3color.obj");
	Model CasaAnt("resources/objects/CasaAntenas/antenascolor.obj");
	Model Rampa("resources/objects/Base/rampanave.obj");
	Model Cantina("resources/objects/cantina/cantina3.obj");
	Model Casas("resources/objects/Tatoot/Tatoot.obj");
	Model Caja("resources/objects/CajaImperial/caja.obj");
	Model Caja2("resources/objects/CajaImperial/caja2.obj");
	Model Caja3("resources/objects/CajaImperial/caja3.obj");
	Model Habitante("resources/objects/Tatooiano/tatooiano.obj");
	Model Nave("resources/objects/Arc170/Arc170.obj");
	Model Accesorios("resources/objects/Accesorios/accesorios.obj");
	Model Casa("resources/objects/CasaLuke/casaluke5.obj");
	Model Casa2("resources/objects/CasaLuke/casaluke6.obj");

	ModelAnim Cayendo("resources/objects/PilotAnim/pilotanim.dae");
	Cayendo.initShaders(animShader.ID);
	ModelAnim Muriendo("resources/objects/Droide2Anim/droide2anim.dae");
	Muriendo.initShaders(animShader.ID);
	ModelAnim Caminando("resources/objects/YodaAnim/yodaanim.dae");
	Caminando.initShaders(animShader.ID);
	ModelAnim Elevacion("resources/objects/RavenAnim/ravenanim.dae");
	Elevacion.initShaders(animShader.ID);

	// PodRacer
	Model PodRacer("resources/objects/PodRacer/PodRacer.obj");
	Model Turbina1("resources/objects/PodRacer/Turbina1.obj");
	Model Turbina2("resources/objects/PodRacer/Turbina2.obj");
	Model Turbina3("resources/objects/PodRacer/Turbina3.obj");

	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;
	KeyFrame[0].giroR2D2 = 0.0f;
	KeyFrame[0].giro2R2D2 = 0.0f;
	KeyFrame[0].giro3R2D2 = 0.0f;
	KeyFrame[0].movPataIzq = 0.0f;
	KeyFrame[0].movPataDer = 0.0f;
	KeyFrame[0].movPataCh = 0.0f;
	KeyFrame[0].movCabeza = -50.0f;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 0.0f;
	KeyFrame[1].posZ = 0.0f;
	KeyFrame[1].giroR2D2 = -25.0f;
	KeyFrame[1].giro2R2D2 = 0.0f;
	KeyFrame[1].giro3R2D2 = 0.0f;
	KeyFrame[1].movPataIzq = 20.0f;
	KeyFrame[1].movPataIzqX = 0.0f;
	KeyFrame[1].movPataIzqZ = -8.8f;
	KeyFrame[1].movPataDer = 20.0f;
	KeyFrame[1].movPataDerX = 0.0f;
	KeyFrame[1].movPataDerZ = -8.8f;
	KeyFrame[1].movPataCh = -2.0f;
	KeyFrame[1].movCabeza = 50.0f;

	KeyFrame[2].posX = 0.0f;
	KeyFrame[2].posY = 0.0f;
	KeyFrame[2].posZ = 100.0f;
	KeyFrame[2].giroR2D2 = -25.0f;
	KeyFrame[2].giro2R2D2 = 0.0f;
	KeyFrame[2].giro3R2D2 = 0.0f;
	KeyFrame[2].movPataIzq = 20.0f;
	KeyFrame[2].movPataIzqX = 0.0f;
	KeyFrame[2].movPataIzqZ = -8.8f;
	KeyFrame[2].movPataDer = 20.0f;
	KeyFrame[2].movPataDerX = 0.0f;
	KeyFrame[2].movPataDerZ = -8.8f;
	KeyFrame[2].movPataCh = -2.0f;
	KeyFrame[2].movCabeza = -50.0f;

	KeyFrame[3].posX = 0.0f;
	KeyFrame[3].posY = 0.0f;
	KeyFrame[3].posZ = 100.0f;
	KeyFrame[3].giroR2D2 = 0.0f;
	KeyFrame[3].giro2R2D2 = 0.0f;
	KeyFrame[3].giro3R2D2 = 0.0f;
	KeyFrame[3].movPataIzq = 0.0f;
	KeyFrame[3].movPataIzqX = 0.0f;
	KeyFrame[3].movPataIzqZ = 0.0f;
	KeyFrame[3].movPataDer = 0.0f;
	KeyFrame[3].movPataDerX = 0.0f;
	KeyFrame[3].movPataDerZ = 0.0f;
	KeyFrame[3].movPataCh = -2.0f;
	KeyFrame[3].movCabeza = 50.0f;

	KeyFrame[4].posX = 0.0f;
	KeyFrame[4].posY = 0.0f;
	KeyFrame[4].posZ = 100.0f;
	KeyFrame[4].giroR2D2 = 0.0f;
	KeyFrame[4].giro2R2D2 = 110.0f;
	KeyFrame[4].giro3R2D2 = 0.0f;
	KeyFrame[4].movPataIzq = 0.0f;
	KeyFrame[4].movPataIzqX = 0.0f;
	KeyFrame[4].movPataIzqZ = 0.0f;
	KeyFrame[4].movPataDer = 0.0f;
	KeyFrame[4].movPataDerX = 0.0f;
	KeyFrame[4].movPataDerZ = 0.0f;
	KeyFrame[4].movPataCh = 0.0f;
	KeyFrame[4].movCabeza = -50.0f;

	KeyFrame[5].posX = 0.0f;
	KeyFrame[5].posY = 0.0f;
	KeyFrame[5].posZ = 100.0f;
	KeyFrame[5].giroR2D2 = 5.0f;
	KeyFrame[5].giro2R2D2 = 110.0f;
	KeyFrame[5].giro3R2D2 = -15.0f;
	KeyFrame[5].movPataIzq = 22.0f;
	KeyFrame[5].movPataIzqX = 0.0f;
	KeyFrame[5].movPataIzqZ = -10.0f;
	KeyFrame[5].movPataDer = 22.0f;
	KeyFrame[5].movPataDerX = 0.0f;
	KeyFrame[5].movPataDerZ = -10.0f;
	KeyFrame[5].movPataCh = -2.5f;
	KeyFrame[5].movCabeza = 50.0f;

	KeyFrame[6].posX = 250.0f;
	KeyFrame[6].posY = 0.0f;
	KeyFrame[6].posZ = 0.0f;
	KeyFrame[6].giroR2D2 = 5.0f;
	KeyFrame[6].giro2R2D2 = 110.0f;
	KeyFrame[6].giro3R2D2 = -15.0f;
	KeyFrame[6].movPataIzq = 22.0f;
	KeyFrame[6].movPataIzqX = 0.0f;
	KeyFrame[6].movPataIzqZ = -10.0f;
	KeyFrame[6].movPataDer = 22.0f;
	KeyFrame[6].movPataDerX = 0.0f;
	KeyFrame[6].movPataDerZ = -10.0f;
	KeyFrame[6].movPataCh = -2.5f;
	KeyFrame[6].movCabeza = -50.0f;

	KeyFrame[7].posX = 250.0f;
	KeyFrame[7].posY = 0.0f;
	KeyFrame[7].posZ = 0.0f;
	KeyFrame[7].giroR2D2 = 0.0f;
	KeyFrame[7].giro2R2D2 = 110.0f;
	KeyFrame[7].giro3R2D2 = 0.0f;
	KeyFrame[7].movPataIzq = 0.0f;
	KeyFrame[7].movPataIzqX = 0.0f;
	KeyFrame[7].movPataIzqZ = 0.0f;
	KeyFrame[7].movPataDer = 0.0f;
	KeyFrame[7].movPataDerX = 0.0f;
	KeyFrame[7].movPataDerZ = 0.0f;
	KeyFrame[7].movPataCh = 0.0f;
	KeyFrame[7].movCabeza = 50.0f;

	KeyFrame[8].posX = 250.0f;
	KeyFrame[8].posY = 0.0f;
	KeyFrame[8].posZ = 0.0f;
	KeyFrame[8].giroR2D2 = 0.0f;
	KeyFrame[8].giro2R2D2 = 0.0f;
	KeyFrame[8].giro3R2D2 = 0.0f;
	KeyFrame[8].movPataIzq = 0.0f;
	KeyFrame[8].movPataIzqX = 0.0f;
	KeyFrame[8].movPataIzqZ = 0.0f;
	KeyFrame[8].movPataDer = 0.0f;
	KeyFrame[8].movPataDerX = 0.0f;
	KeyFrame[8].movPataDerZ = 0.0f;
	KeyFrame[8].movPataCh = 0.0f;
	KeyFrame[8].movCabeza = -50.0f;

	KeyFrame[9].posX = 250.0f;
	KeyFrame[9].posY = 0.0f;
	KeyFrame[9].posZ = 0.0f;
	KeyFrame[9].giroR2D2 = -25.0f;
	KeyFrame[9].giro2R2D2 = 0.0f;
	KeyFrame[9].giro3R2D2 = 0.0f;
	KeyFrame[9].movPataIzq = 20.0f;
	KeyFrame[9].movPataIzqX = 0.0f;
	KeyFrame[9].movPataIzqZ = -8.8f;
	KeyFrame[9].movPataDer = 20.0f;
	KeyFrame[9].movPataDerX = 0.0f;
	KeyFrame[9].movPataDerZ = -8.8f;
	KeyFrame[9].movPataCh = -2.0f;
	KeyFrame[9].movCabeza = 50.0f;

	KeyFrame[10].posX = 250.0f;
	KeyFrame[10].posY = 0.0f;
	KeyFrame[10].posZ = 100.0f;
	KeyFrame[10].giroR2D2 = -25.0f;
	KeyFrame[10].giro2R2D2 = 0.0f;
	KeyFrame[10].giro3R2D2 = 0.0f;
	KeyFrame[10].movPataIzq = 20.0f;
	KeyFrame[10].movPataIzqX = 0.0f;
	KeyFrame[10].movPataIzqZ = -8.8f;
	KeyFrame[10].movPataDer = 20.0f;
	KeyFrame[10].movPataDerX = 0.0f;
	KeyFrame[10].movPataDerZ = -8.8f;
	KeyFrame[10].movPataCh = -2.0f;
	KeyFrame[10].movCabeza = -50.0f;

	KeyFrame[11].posX = 250.0f;
	KeyFrame[11].posY = 0.0f;
	KeyFrame[11].posZ = 100.0f;
	KeyFrame[11].giroR2D2 = 0.0f;
	KeyFrame[11].giro2R2D2 = 0.0f;
	KeyFrame[11].giro3R2D2 = 0.0f;
	KeyFrame[11].movPataIzq = 0.0f;
	KeyFrame[11].movPataIzqX = 0.0f;
	KeyFrame[11].movPataIzqZ = 0.0f;
	KeyFrame[11].movPataDer = 0.0f;
	KeyFrame[11].movPataDerX = 0.0f;
	KeyFrame[11].movPataDerZ = 0.0f;
	KeyFrame[11].movPataCh = 0.0f;
	KeyFrame[11].movCabeza = 50.0f;

	KeyFrame[12].posX = 250.0f;
	KeyFrame[12].posY = 0.0f;
	KeyFrame[12].posZ = 100.0f;
	KeyFrame[12].giroR2D2 = 0.0f;
	KeyFrame[12].giro2R2D2 = -135.0f;
	KeyFrame[12].giro3R2D2 = 0.0f;
	KeyFrame[12].movPataIzq = 0.0f;
	KeyFrame[12].movPataIzqX = 0.0f;
	KeyFrame[12].movPataIzqZ = 0.0f;
	KeyFrame[12].movPataDer = 0.0f;
	KeyFrame[12].movPataDerX = 0.0f;
	KeyFrame[12].movPataDerZ = 0.0f;
	KeyFrame[12].movPataCh = 0.0f;
	KeyFrame[12].movCabeza = -50.0f;

	KeyFrame[13].posX = 250.0f;
	KeyFrame[13].posY = 0.0f;
	KeyFrame[13].posZ = 100.0f;
	KeyFrame[13].giroR2D2 = 5.0f;
	KeyFrame[13].giro2R2D2 = -135.0f;
	KeyFrame[13].giro3R2D2 = -15.0f;
	KeyFrame[13].movPataIzq = 22.0f;
	KeyFrame[13].movPataIzqX = 0.0f;
	KeyFrame[13].movPataIzqZ = -10.0f;
	KeyFrame[13].movPataDer = 22.0f;
	KeyFrame[13].movPataDerX = 0.0f;
	KeyFrame[13].movPataDerZ = -10.0f;
	KeyFrame[13].movPataCh = -2.5f;
	KeyFrame[13].movCabeza = 50.0f;

	KeyFrame[14].posX = 175.0f;
	KeyFrame[14].posY = 0.0f;
	KeyFrame[14].posZ = 0.0f;
	KeyFrame[14].giroR2D2 = 5.0f;
	KeyFrame[14].giro2R2D2 = -135.0f;
	KeyFrame[14].giro3R2D2 = -15.0f;
	KeyFrame[14].movPataIzq = 22.0f;
	KeyFrame[14].movPataIzqX = 0.0f;
	KeyFrame[14].movPataIzqZ = -10.0f;
	KeyFrame[14].movPataDer = 22.0f;
	KeyFrame[14].movPataDerX = 0.0f;
	KeyFrame[14].movPataDerZ = -10.0f;
	KeyFrame[14].movPataCh = -2.5f;
	KeyFrame[14].movCabeza = -50.0f;

	KeyFrame[15].posX = 175.0f;
	KeyFrame[15].posY = 0.0f;
	KeyFrame[15].posZ = 0.0f;
	KeyFrame[15].giroR2D2 = 0.0f;
	KeyFrame[15].giro2R2D2 = -135.0f;
	KeyFrame[15].giro3R2D2 = 0.0f;
	KeyFrame[15].movPataIzq = 0.0f;
	KeyFrame[15].movPataIzqX = 0.0f;
	KeyFrame[15].movPataIzqZ = 0.0f;
	KeyFrame[15].movPataDer = 0.0f;
	KeyFrame[15].movPataDerX = 0.0f;
	KeyFrame[15].movPataDerZ = 0.0f;
	KeyFrame[15].movPataCh = 0.0f;
	KeyFrame[15].movCabeza = 50.0f;

	KeyFrame[16].posX = 175.0f;
	KeyFrame[16].posY = 0.0f;
	KeyFrame[16].posZ = 0.0f;
	KeyFrame[16].giroR2D2 = 0.0f;
	KeyFrame[16].giro2R2D2 = -90.0f;
	KeyFrame[16].giro3R2D2 = 0.0f;
	KeyFrame[16].movPataIzq = 0.0f;
	KeyFrame[16].movPataIzqX = 0.0f;
	KeyFrame[16].movPataIzqZ = 0.0f;
	KeyFrame[16].movPataDer = 0.0f;
	KeyFrame[16].movPataDerX = 0.0f;
	KeyFrame[16].movPataDerZ = 0.0f;
	KeyFrame[16].movPataCh = 0.0f;
	KeyFrame[16].movCabeza = -50.0f;

	KeyFrame[17].posX = 175.0f;
	KeyFrame[17].posY = 0.0f;
	KeyFrame[17].posZ = 0.0f;
	KeyFrame[17].giroR2D2 = 5.0f;
	KeyFrame[17].giro2R2D2 = -90.0f;
	KeyFrame[17].giro3R2D2 = -15.0f;
	KeyFrame[17].movPataIzq = 20.0f;
	KeyFrame[17].movPataIzqX = 0.0f;
	KeyFrame[17].movPataIzqZ = -8.8f;
	KeyFrame[17].movPataDer = 20.0f;
	KeyFrame[17].movPataDerX = 0.0f;
	KeyFrame[17].movPataDerZ = -8.8f;
	KeyFrame[17].movPataCh = -2.0f;
	KeyFrame[17].movCabeza = 50.0f;

	KeyFrame[18].posX = 0.0f;
	KeyFrame[18].posY = 0.0f;
	KeyFrame[18].posZ = 0.0f;
	KeyFrame[18].giroR2D2 = 5.0f;
	KeyFrame[18].giro2R2D2 = -90.0f;
	KeyFrame[18].giro3R2D2 = -15.0f;
	KeyFrame[18].movPataIzq = 20.0f;
	KeyFrame[18].movPataIzqX = 0.0f;
	KeyFrame[18].movPataIzqZ = -8.8f;
	KeyFrame[18].movPataDer = 20.0f;
	KeyFrame[18].movPataDerX = 0.0f;
	KeyFrame[18].movPataDerZ = -8.8f;
	KeyFrame[18].movPataCh = -2.0f;
	KeyFrame[18].movCabeza = -50.0f;

	KeyFrame[19].posX = 0.0f;
	KeyFrame[19].posY = 0.0f;
	KeyFrame[19].posZ = 0.0f;
	KeyFrame[19].giroR2D2 = 0.0f;
	KeyFrame[19].giro2R2D2 = 0.0f;
	KeyFrame[19].giro3R2D2 = 0.0f;
	KeyFrame[19].movPataIzq = 0.0f;
	KeyFrame[19].movPataIzqX = 0.0f;
	KeyFrame[19].movPataIzqZ = 0.0f;
	KeyFrame[19].movPataDer = 0.0f;
	KeyFrame[19].movPataDerX = 0.0f;
	KeyFrame[19].movPataDerZ = 0.0f;
	KeyFrame[19].movPataCh = 0.0f;
	KeyFrame[19].movCabeza = 50.0f;

	// PodRace
	KeyFrame[20].posPodX = 0.0f;
	KeyFrame[20].posPodY = 0.0f;
	KeyFrame[20].posPodZ = 0.0f;
	KeyFrame[20].rotPodX = -90.0f;
	KeyFrame[20].rotPodY = 0.0f;
	KeyFrame[20].rotPodZ = 0.0f;
	KeyFrame[20].giroTurbina = 1000.0f;

	KeyFrame[21].posPodX = -50.0f;
	KeyFrame[21].posPodY = 300.0f;
	KeyFrame[21].posPodZ = 0.0f;
	KeyFrame[21].rotPodX = -60.0f;
	KeyFrame[21].rotPodY = 0.0f;
	KeyFrame[21].rotPodZ = 0.0f;
	KeyFrame[21].giroTurbina = 2000.0f;

	KeyFrame[22].posPodX = -100.0f;
	KeyFrame[22].posPodY = 400.0f;
	KeyFrame[22].posPodZ = 350.0f;
	KeyFrame[22].rotPodX = -30.0f;
	KeyFrame[22].rotPodY = 0.0f;
	KeyFrame[22].rotPodZ = 0.0f;
	KeyFrame[22].giroTurbina = 3000.0f;

	KeyFrame[23].posPodX = -200.0f;
	KeyFrame[23].posPodY = 400.0f;
	KeyFrame[23].posPodZ = 700.0f;
	KeyFrame[23].rotPodX = 0.0f;
	KeyFrame[23].rotPodY = 0.0f;
	KeyFrame[23].rotPodZ = 720.0f;
	KeyFrame[23].giroTurbina = 4000.0f;

	KeyFrame[24].posPodX = -200.0f;
	KeyFrame[24].posPodY = 400.0f;
	KeyFrame[24].posPodZ = 1050.0f;
	KeyFrame[24].rotPodX = 0.0f;
	KeyFrame[24].rotPodY = 0.0f;
	KeyFrame[24].rotPodZ = 720.0f;
	KeyFrame[24].giroTurbina = 5000.0f;

	KeyFrame[25].posPodX = -500.0f;
	KeyFrame[25].posPodY = 200.0f;
	KeyFrame[25].posPodZ = 1400.0f;
	KeyFrame[25].rotPodX = 0.0f;
	KeyFrame[25].rotPodY = -40.0f;
	KeyFrame[25].rotPodZ = 720.0f;
	KeyFrame[25].giroTurbina = 4000.0f;

	KeyFrame[26].posPodX = -800.0f;
	KeyFrame[26].posPodY = 100.0f;
	KeyFrame[26].posPodZ = 1500.0f;
	KeyFrame[26].rotPodX = 0.0f;
	KeyFrame[26].rotPodY = -60.0f;
	KeyFrame[26].rotPodZ = 720.0f;
	KeyFrame[26].giroTurbina = 3000.0f;

	KeyFrame[27].posPodX = -1100.0f;
	KeyFrame[27].posPodY = 50.0f;
	KeyFrame[27].posPodZ = 1550.0f;
	KeyFrame[27].rotPodX = 0.0f;
	KeyFrame[27].rotPodY = -80.0f;
	KeyFrame[27].rotPodZ = 720.0f;
	KeyFrame[27].giroTurbina = 2000.0f;

	KeyFrame[28].posPodX = -1300.0f;
	KeyFrame[28].posPodY = -10.0f;
	KeyFrame[28].posPodZ = 1600.0f;
	KeyFrame[28].rotPodX = 0.0f;
	KeyFrame[28].rotPodY = -75.0f;
	KeyFrame[28].rotPodZ = 720.0f;
	KeyFrame[28].giroTurbina = 1000.0f;

	KeyFrame[29].posPodX = -1350.0f;
	KeyFrame[29].posPodY = -10.0f;
	KeyFrame[29].posPodZ = 1600.0f;
	KeyFrame[29].rotPodX = 0.0f;
	KeyFrame[29].rotPodY = -80.0f;
	KeyFrame[29].rotPodZ = 720.0f;
	KeyFrame[29].giroTurbina = 500.0f;
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();
		
		// render
		
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		//personaje animado caminando
		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -32.0f, -850.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.45f));
		animShader.setMat4("model", model);
		Elevacion.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 40.0f, -700.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.40f));
		animShader.setMat4("model", model);
		Cayendo.Draw(animShader);
		
		//DROIDE MURIENDO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(850.0f, -4.0f, 700.0f));
		model = glm::translate(model, glm::vec3(movDroideX, -12.0f, movDroideZ));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotDroide), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f));
		animShader.setMat4("model", model);
		Muriendo.Draw(animShader);

		//YODA CAMINANDO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -4.0f, 800.0f));
		model = glm::translate(model, glm::vec3(movYodaX, -22.0f, movYodaZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotYoda), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		animShader.setMat4("model", model);
		Caminando.Draw(animShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		//PISO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 19.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//CASA 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 50.0f, -980.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		Casa.Draw(staticShader);
		//CASA2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(900.0f, -42.0f, 430.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		Casa2.Draw(staticShader);
				
		//DEFRENTE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.5f, -22.0f, -1025.0f));
		model = glm::rotate(model, glm::radians(-126.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.7f));
		staticShader.setMat4("model", model);
		CasaT2.Draw(staticShader);
		//DERECHA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, -31.5f, -80.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.7f));
		staticShader.setMat4("model", model);
		CasaT3.Draw(staticShader);
		//ANTENAS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1100.0f, -20.0f, -160.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.7f));
		staticShader.setMat4("model", model);
		CasaAnt.Draw(staticShader);
		//IZQUIERDA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-850.0f, -22.0f, -670.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.022f));
		staticShader.setMat4("model", model);
		Rampa.Draw(staticShader);
		//CANTINA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -22.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.5f));
		staticShader.setMat4("model", model);
		Cantina.Draw(staticShader);
		//ACCESORIOS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(550.0f, -22.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		Accesorios.Draw(staticShader);
		//CASA C/NAVE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-600.0f, 15.0f, 900.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		Casas.Draw(staticShader);

		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, -20.0f, 450.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja.Draw(staticShader);
		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, -20.0f, -450.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja.Draw(staticShader);
		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, -20.0f, 500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja2.Draw(staticShader);
		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, -20.0f, -500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja3.Draw(staticShader);
		//TATOOIANO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, -20.0f, 800.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Habitante.Draw(staticShader);
		//NAVE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, -20.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Nave.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Holograma
		// -------------------------------------------------------------------------------------------------------------------------
		//BASE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -23.0f, -320.0f));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Base.Draw(staticShader);
		//GRAFICO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -16.0f, -320.0f));
		model = glm::rotate(model, glm::radians(movGrafico), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Grafico.Draw(staticShader);
		//BASE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -23.0f, -1020.0f));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Base.Draw(staticShader);
		//GRAFICO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -16.0f, -1020.0f));
		model = glm::rotate(model, glm::radians(-movGrafico), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Grafico.Draw(staticShader);

		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		//CUERPO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -22.0f, 100.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroR2D2), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giro2R2D2), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giro3R2D2), glm::vec3(1.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		Cuerpo.Draw(staticShader);

		//PIERNA IZQUIERDA
		model = glm::translate(tmp, glm::vec3(-8.3f, 0.0f, -1.0f));
		model = glm::translate(model, glm::vec3(movPataIzqX, 0.0f, movPataIzqZ));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPataIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		Pataizq.Draw(staticShader);

		//PIERNA DERECHA
		model = glm::translate(tmp, glm::vec3(10.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movPataDerX, 0.0f, movPataDerZ));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPataDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		Patader.Draw(staticShader);

		//PATA CHICA
		model = glm::translate(tmp, glm::vec3(0.0f, -0.8f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, movPataCh, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		Patachica.Draw(staticShader); 

		//CABEZA
		model = glm::translate(tmp, glm::vec3(0.25f, -9.0f, -0.65f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		staticShader.setMat4("model", model);
		Cabeza.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje GONK 2
		// -------------------------------------------------------------------------------------------------------------------------
		
		// -------------------------------------------------------------------------------------------------------------------------
		// PodRacer
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(850.0f + posPodX, 20.0f + posPodY, -800.0f + posPodZ));
		tmp = model = glm::rotate(model, glm::radians(rotPodX), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotPodY), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotPodZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		PodRacer.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(71.0f, -16.0f, 112.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina1.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-71.0f, -16.0f, 112.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina1.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(71.0f, -16.0f, 51.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina2.Draw(staticShader);	//Izq central

		model = glm::translate(tmp, glm::vec3(-71.0f, -16.0f, 51.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina2.Draw(staticShader);	//Der central

		model = glm::translate(tmp, glm::vec3(17.1f, 16.0f, -147.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina3.Draw(staticShader);	//Izq trasera

		model = glm::translate(tmp, glm::vec3(-17.1f, 16.0f, -147.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina3.Draw(staticShader);	//Der trasera


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		circuito = true;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		circuito = false;

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		circuito2 = true;

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		circuito2 = false;

	
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
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
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}