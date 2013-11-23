# define __Windows__ // define your target operating system
# include "../includes465/include465.h"
# define __INCLUDES465__  

# include "Shape3D.hpp"

const int nModels = 5;
Shape3D * modelObjArr[nModels];
char * modelFile[nModels] = {"ball.tri", "Warbird2.0.tri", "ball.tri", "Warbird2.0.tri", "ball.tri"};
//char * modelFile[nModels] = {"ball.tri"};

float modelBR[nModels];
float scaleValue[nModels];
const int nVertices[nModels] = { 4900 * 3, 39880 * 3, 4900 * 3, 39880 * 3, 4900 * 3 };
//const int nVertices[nModels] = {4900 * 3};
char * vertexShaderFile = "viewVertex.glsl";
char * fragmentShaderFile = "viewFragment.glsl";
GLuint shaderProgram;
GLuint VAO[nModels];
GLuint buffer[nModels];

GLuint model, view, project;
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];

float modelSize[nModels] = { 25.0f, 50.0f, 100.0f };
//float modelSize[nModels] = {25.0f};
glm::vec3 scale[nModels];
glm::vec3 translate[nModels];
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

char titleStr[50] = "jl_main ";

int frameCount = 0;
int timerDelay = 1000;

// vectors and values for lookAt
glm::vec3 eye, at, up;

void reshape(int width, int height) {
	float aspectRatio = (float) width / (float) height;
	float FOVY = glm::radians(100.0f);
	glViewport(0, 0, width, height);
	printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n", FOVY, width, height, aspectRatio);
	projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f); 
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int m = 0; m < nModels; m++) {
		int parentID = modelObjArr[m]->getParentID();
		if(parentID > 1)		
			modelMatrix = modelObjArr[m] -> getModelMatrix(modelObjArr[parentID]->getPositioning());
		else
			modelMatrix = modelObjArr[m] -> getModelMatrix(); 

		glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
		glBindVertexArray(VAO[m]);
		glEnableVertexAttribArray( vPosition[m] );
		glEnableVertexAttribArray( vColor[m] );
		glEnableVertexAttribArray( vNormal[m] );
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m] ); 
	}
	glutSwapBuffers();
	frameCount++;
}

void init() {
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	glGenVertexArrays( nModels, VAO);
	glGenBuffers( nModels, buffer);

	for(int i = 0; i < nModels; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram, vPosition[i], 
			vColor[i], vNormal[i], "vPosition", "vColor", "vNormal"); 
		scale[i] = glm::vec3( modelSize[i] * 1.0f/modelBR[i]);
	}

	model =   glGetUniformLocation(shaderProgram, "Model");
	view =    glGetUniformLocation(shaderProgram, "View");
	project = glGetUniformLocation(shaderProgram, "Project");

	printf("model %d, view %d, project %d\n", model, view, project);

	  viewMatrix = glm::lookAt(
	  glm::vec3(0.0f, 10000.0f,  0.0f),
      glm::vec3(0.0f,    0.0f,  0.0f),
      glm::vec3(0.0f,    0.0f, -1.0f));   // camera's up is looking towards -Z vector

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	
	for(int i = 0; i < nModels; i++) {
		modelObjArr[i] = new Shape3D(i);
	}
	printf("%d Models created \n", nModels);
}


// to set rotation
void animate(void){
  frameCount++;
  
  for(int i = 0; i < nModels; i++) {  
	  modelObjArr[i] -> update();
  }

  glutPostRedisplay();
}



// Estimate FPS, use fixed interval timer to measure every second
// timerDelay = 1000
void intervalTimer (int i) { 
	glutTimerFunc(timerDelay, intervalTimer, 1);
	frameCount = 0;
	animate();
}

void keyboard (unsigned char key, int x, int y) {
  

	switch(key) {
    case 033 : case 'q' :  case 'Q' : exit(EXIT_SUCCESS); break;
    case 'f' : case 'F' :  // front view
        eye = glm::vec3(0.0f, 0.0f, 10000.0f);   // eye is 2000 "out of screen" from origin
        at  = glm::vec3(0.0f, 0.0f,    0.0f);   // looking at origin
        up  = glm::vec3(0.0f, 1.0f,    0.0f);   // camera'a up vector
        //strcpy(viewStr, " front view");
		break;
    case 'r' : case 'R' :  // bottom view
        eye = glm::vec3(10000.0f, 0.0f, 0.0f);   // eye is 1000 right from origin
        at  = glm::vec3(   0.0f, 0.0f, 0.0f);   // looking at origin
        up  = glm::vec3(   0.0f, 1.0f, 0.0f);   // camera'a up vector
        //strcpy(viewStr, " right view");
		break;
    case 't' : case 'T' :  // top view
        eye = glm::vec3(0.0f, 10000.0f,  0.0f);   // eye is 3000 up from origin
        at  = glm::vec3(0.0f,    0.0f,  0.0f);   // looking at origin  
        up  = glm::vec3(0.0f,    0.0f, -1.0f);   // camera's up is looking towards -Z vector
        //strcpy(viewStr, " top view"); 
		break;
	case 'k' : case 'K' :
		eye = glm::vec3(0.0f, 5000.0f,  10000.0f);//0, 1000, 10950
		at  = glm::vec3(0.0f, 0.0f,  0.0f);
		up  = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
    }
  viewMatrix = glm::lookAt(eye, at, up);
  }

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(880, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Options: R, T, F, K");

	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("GLEW Error: %s \n", glewGetErrorString(err)); 
	}
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n", 
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	glutIdleFunc(animate);
	glutTimerFunc(timerDelay, intervalTimer, 1);

	glutMainLoop();
	printf("done\n");
	return 0;	
}










