#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Headers.h>
#include <Headerc.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <exampleLoader.h>
#include <string>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;



GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void getDataImage(unsigned char* data, int& width, int& height, string& format);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(unsigned char* path,int &size,const char *format,int width, int height);

int a = 5;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float deltaTime = 0.0f;	// время между текущим и последним кадрами
float lastFrame = 0.0f;// время последнего кадра
bool firstMouse = true;

float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float yaw = -90.0f;	// рыскание инициализируется значением -90.0 градусов, так как значние рыскания равное 0.0 приводит к тому, что вектор направления будет указывать вправо, поэтому мы изначально поворачиваемся немного влево.
float pitch = 0.0f;
float fov = 45.0f;

Camera cam(cameraPos, cameraUp, cameraFront, yaw, pitch, fov);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "Open Project", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	//stbi_set_flip_vertically_on_load(true);
	
	glEnable(GL_DEPTH_TEST);
	
	Shader ourShader("E:/cpp_projects/oglProject/tempProjectOgl/modelvs.txt",
		"E:/cpp_projects/oglProject/tempProjectOgl/modelfs.txt");


	//string path_obj ="E:/OpenGL/backpack/backpack.obj";
	string path_obj = "F:/examples_obj/bugatti.obj";
	//string path_obj = "F:/blendfiles/examplecube.obj";
	string file_mtl_path;
	/*for (fs::path p : { "E:/OpenGL/backpack/backpack.obj", "/", "E:/OpenGL/backpack/" })
	{
		path_obj = p.parent_path().string();
		objname = p.filename().string();
		file_mtl_path = p.parent_path().string();
	}*/
	
	file_mtl_path = fs::path(path_obj).parent_path().string();

	exampleLoader el(path_obj,file_mtl_path);

	//Model ourModel("F:/blendfiles/naruto_modelling.obj");
	
	/*Shader lightShader("E:/cpp_projects/oglProject/oglProject/shaderv.txt",
		"E:/cpp_projects/oglProject/oglProject/shaderf.txt");

	Shader lampShader("E:/cpp_projects/oglProject/oglProject/lightShaderv.txt",
		"E:/cpp_projects/oglProject/oglProject/lightShaderf.txt");

	Shader surfShader("E:/cpp_projects/oglProject/oglProject/shaderv.txt",
		"E:/cpp_projects/oglProject/oglProject/groundShaderf.txt");

	Model myModel("E:/OpenGL/backpack/backpack.obj");


	float vertices[]
	{
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
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
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float surface[]{
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f
	};

	/*unsigned int indices[] = {
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};



	glm::vec3 cubePositions[] =
	{

		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(0.0f,2.0f,-3.0f),
			glm::vec3(2.0f,-4.0f,-2.0f),
			glm::vec3(-2.0f,3.0f,-5.0f),
			glm::vec3(5.0f,-1.0f,-2.0f),
			glm::vec3(1.0f,4.0f,-7.0f),
			glm::vec3(0.0f,7.0f,-9.0f)
	};

	glm::vec4 pointLightPositions[] = {
		glm::vec4(0.7f,  0.2f,  2.0f,1.0f),
		glm::vec4(2.3f, -3.3f, -4.0f,1.0f),
		glm::vec4(-4.0f,  2.0f, -12.0f,1.0f),
		glm::vec4(0.0f,  0.0f, -3.0f,1.0f)
	};


	int width, height, nrchannels;



	unsigned int cubeVAO, cubeVBO, surfVAO, surfVBO, lightVAO, EBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	//glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &surfVAO);
	glGenBuffers(1, &surfVBO);

	glBindBuffer(GL_ARRAY_BUFFER, surfVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(surface), surface, GL_STATIC_DRAW);
	glBindVertexArray(surfVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	int sizeTex = 1;


	unsigned char* data = stbi_load("E:/tree.png", &width, &height, &nrchannels, 0);

	unsigned int diffuseMap = loadTexture(data, sizeTex, "png", width, height);
	data = stbi_load("E:/steel.png", &width, &height, &nrchannels, 0);
	unsigned int specularMap = loadTexture(data, sizeTex, "png", width, height);
	data = stbi_load("E:/surface_tex.jpg", &width, &height, &nrchannels, 0);
	unsigned int groundTex = loadTexture(data, sizeTex, "jpg", width, height);

	lightShader.use();
	lightShader.setInt("material.diffuse", 0);
	lightShader.setInt("material.specular", 1);
	surfShader.use();
	surfShader.setInt("material.mars_texture", 0);


	stbi_image_free(data);

	struct Lighting_vec4
	{
		glm::vec4 position = glm::vec4(cam.Position, 1.0f);
		glm::vec4 direction = glm::vec4(cam.Front, 1.0f);
	};
	Lighting_vec4 light_v4;

	
	struct PointLightUBlock {

		//glm::vec4 position[sizeof(pointLightPositions)] ;
		glm::vec4 position;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular; 
		float constant;
		float linear;
		float quadratic;
		float padd;
		
	};

	PointLightUBlock po_light[4]{};

	for (int i = 0; i < 4; i++)
	{
		po_light[i].position = pointLightPositions[i];
		po_light[i].ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
		po_light[i].diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		po_light[i].specular = glm::vec4(1.0f);
		po_light[i].constant = 1.0f;
		po_light[i].linear = 0.09f;
		po_light[i].quadratic = 0.032f;
		po_light[i].padd = 0.0f;
	}

	int lengthOfPointLight = sizeof(po_light) / sizeof(po_light[0]);
	

	glUniformBlockBinding(lightShader.shaderProgram, glGetUniformBlockIndex(lightShader.shaderProgram, "Lighting_vec4"), 0);
	glUniformBlockBinding(surfShader.shaderProgram, glGetUniformBlockIndex(surfShader.shaderProgram, "Lighting_vec4"), 0);
	glUniformBlockBinding(lightShader.shaderProgram, glGetUniformBlockIndex(lightShader.shaderProgram, "PointLightUBlock"), 1);
	glUniformBlockBinding(surfShader.shaderProgram, glGetUniformBlockIndex(surfShader.shaderProgram, "PointLightUBlock"), 1);

	unsigned int uboLightingv4, uboPointLight;

	glGenBuffers(1, &uboLightingv4);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightingv4);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(light_v4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboLightingv4,0,sizeof(light_v4));
	
	glGenBuffers(1, &uboPointLight);
	glBindBuffer(GL_UNIFORM_BUFFER, uboPointLight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(po_light)* lengthOfPointLight, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboPointLight, 0, sizeof(po_light)*lengthOfPointLight);
	
	*/
	int count = 0;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = (float)glfwGetTime();

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//glm::mat4 model = glm::mat4(1.0f);
		//glm::mat4 view = glm::mat4(1.0f);
		//glm::mat4 projection = glm::mat4(1.0f);




		/*glBindBuffer(GL_UNIFORM_BUFFER, uboLightingv4);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(light_v4), &light_v4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);



		glBindBuffer(GL_UNIFORM_BUFFER, uboPointLight);
		int offset = 0;
		for (int i = 0; i < 4; i++)
		{

			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(po_light)*4, po_light);

		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		lightShader.use();

		lightShader.setVec3("dir_light.ambient", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("dir_light.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("dir_light.specular", 1.0f, 1.0f, 1.0f);



		/*lightShader.setVec3("po_light[0].position", pointLightPositions[0]);
		lightShader.setVec3("po_light[0].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("po_light[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("po_light[0].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("po_light[0].constant", 1.0f);
		lightShader.setFloat("po_light[0].linear", 0.09);
		lightShader.setFloat("po_light[0].quadratic", 0.032);

		// Точечный источник света №2
		lightShader.setVec3("po_light[1].position", pointLightPositions[1]);
		lightShader.setVec3("po_light[1].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("po_light[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("po_light[1].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("po_light[1].constant", 1.0f);
		lightShader.setFloat("po_light[1].linear", 0.09);
		lightShader.setFloat("po_light[1].quadratic", 0.032);

		// Точечный источник света №3
		lightShader.setVec3("po_light[2].position", pointLightPositions[2]);
		lightShader.setVec3("po_light[2].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("po_light[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("po_light[2].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("po_light[2].constant", 1.0f);
		lightShader.setFloat("po_light[2].linear", 0.09);
		lightShader.setFloat("po_light[2].quadratic", 0.032);

		// Точечный источник света №4
		lightShader.setVec3("po_light[3].position", pointLightPositions[3]);
		lightShader.setVec3("po_light[3].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("po_light[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("po_light[3].specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("po_light[3].constant", 1.0f);
		lightShader.setFloat("po_light[3].linear", 0.09);
		lightShader.setFloat("po_light[3].quadratic", 0.032);
		*/

		/*lightShader.setVec3("proj_light.ambient", 0.0f, 0.0f, 0.0f);
		lightShader.setVec3("proj_light.diffuse", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("proj_light.specular", 1.0f, 1.0f, 1.0f);
		lightShader.setFloat("proj_light.constant", 1.0f);
		lightShader.setFloat("proj_light.linear", 0.09);
		lightShader.setFloat("proj_light.quadratic", 0.032);
		lightShader.setFloat("proj_light.cutOff", glm::cos(glm::radians(12.5f)));


		lightShader.setFloat("material.shininess", 32.0f);



		lightShader.setVec4("viewPos", glm::vec4(cam.Position,1.0f));
		projection = glm::perspective(glm::radians(cam.GetFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
		view = cam.GetViewMat();

		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(cubeVAO);
		float angel;
		for (unsigned int i = 0; i <sizeof(cubePositions); i++)
		{
			model = glm::translate(model, cubePositions[i]);
			angel = 20.0f * i;
			model = glm::rotate(model, glm::radians(angel), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		surfShader.use();


		surfShader.setVec3("dir_light.ambient", 0.05f, 0.05f, 0.0f);
		surfShader.setVec3("dir_light.diffuse", 0.5f, 0.4f, 0.4f);
		surfShader.setVec3("dir_light.specular", 0.7f,0.04f,0.04f);

		/*surfShader.setFloat("po_light.constant", 1.0f);
		surfShader.setFloat("po_light.linear", 0.09f);
		surfShader.setFloat("po_light.quadratic", 0.032f);
		surfShader.setVec3("po_light[0].position", pointLightPositions[0]);
		surfShader.setVec3("po_light[0].ambient", 0.05f, 0.05f, 0.05f);
		surfShader.setVec3("po_light[0].diffuse", 0.8f, 0.8f, 0.8f);
		surfShader.setVec3("po_light[0].specular", 1.0f, 1.0f, 1.0f);
		surfShader.setFloat("po_light[0].constant", 1.0f);
		surfShader.setFloat("po_light[0].linear", 0.09);
		surfShader.setFloat("po_light[0].quadratic", 0.032);

		// Точечный источник света №2
		surfShader.setVec3("po_light[1].position", pointLightPositions[1]);
		surfShader.setVec3("po_light[1].ambient", 0.05f, 0.05f, 0.05f);
		surfShader.setVec3("po_light[1].diffuse", 0.8f, 0.8f, 0.8f);
		surfShader.setVec3("po_light[1].specular", 1.0f, 1.0f, 1.0f);
		surfShader.setFloat("po_light[1].constant", 1.0f);
		surfShader.setFloat("po_light[1].linear", 0.09);
		surfShader.setFloat("po_light[1].quadratic", 0.032);

		// Точечный источник света №3
		surfShader.setVec3("po_light[2].position", pointLightPositions[2]);
		surfShader.setVec3("po_light[2].ambient", 0.05f, 0.05f, 0.05f);
		surfShader.setVec3("po_light[2].diffuse", 0.8f, 0.8f, 0.8f);
		surfShader.setVec3("po_light[2].specular", 1.0f, 1.0f, 1.0f);
		surfShader.setFloat("po_light[2].constant", 1.0f);
		surfShader.setFloat("po_light[2].linear", 0.09);
		surfShader.setFloat("po_light[2].quadratic", 0.032);

		// Точечный источник света №4
		surfShader.setVec3("po_light[3].position", pointLightPositions[3]);
		surfShader.setVec3("po_light[3].ambient", 0.05f, 0.05f, 0.05f);
		surfShader.setVec3("po_light[3].diffuse", 0.8f, 0.8f, 0.8f);
		surfShader.setVec3("po_light[3].specular", 1.0f, 1.0f, 1.0f);
		surfShader.setFloat("po_light[3].constant", 1.0f);
		surfShader.setFloat("po_light[3].linear", 0.09);
		surfShader.setFloat("po_light[3].quadratic", 0.032);
		*/

		/*surfShader.setVec3("proj_light.ambient", 0.0f, 0.0f, 0.0f);
		surfShader.setVec3("proj_light.diffuse", 1.0f, 1.0f, 1.0f);
		surfShader.setVec3("proj_light.specular", 1.0f, 1.0f, 1.0f);
		surfShader.setFloat("proj_light.constant", 1.0f);
		surfShader.setFloat("proj_light.linear", 0.09);
		surfShader.setFloat("proj_light.quadratic", 0.032);
		surfShader.setFloat("proj_light.cutOff", glm::cos(glm::radians(12.5f)));

		surfShader.setFloat("material.shininess", 0.7f);

		surfShader.setVec4("viewPos", glm::vec4(cam.Position, 1.0f));
		surfShader.setMat4("projection", projection);
		surfShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, groundTex);

		surfShader.setMat4("model", model);
		glBindVertexArray(surfVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);



		lampShader.use();

		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		glBindVertexArray(lightVAO);

		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			glm::vec3 posTemp = glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y,pointLightPositions[i].z);
			model = glm::translate(model, posTemp);
			model = glm::scale(model, glm::vec3(0.2f)); // куб, меньшего размера
			lampShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		*/

		ourShader.use();

		// Преобразования Вида/Проекции
		glm::mat4 projection = glm::perspective(glm::radians(cam.GetFOV()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = cam.GetViewMat();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// Рендеринг загруженной модели
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // смещаем вниз чтобы быть в центре сцены
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
		ourShader.setMat4("model", model);
		//int size = el.GetMatSize();
			el.Draw(ourShader);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &surfVAO);
	//glDeleteBuffers(1, &cubeVBO);
	//glDeleteBuffers(1, &surfVBO);
	//glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // перевернуто, так как Y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;
	cam.CamDirectionMouse(xoffset, yoffset, true);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.CamZoomFOV(yoffset);
}
