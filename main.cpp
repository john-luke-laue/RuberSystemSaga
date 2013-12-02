# define __Windows__ // define your target operating system
# include "includes465/include465.h"
# define __INCLUDES465__  

# include "Shape3D.hpp"


const int nModels = 8, missileRange = 500;
Shape3D * modelObjArr[nModels];
char * modelFile[nModels] = {"ball.tri", "Planet2.tri", "Planet3.tri", "Planet4.tri", "Planet5.tri", "Warbird2.0.tri", "Missle.tri", "Missle.tri"};
//char * modelFile[nModels] = {"ball.tri"};

float modelBR[nModels];
float scaleValue[nModels];
const int nVertices[nModels] = { 4900 * 3, 79600 * 3, 79600 * 3, 79600 * 3, 79600 * 3, 39880 * 3, 96 *3, 96 *3};
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
int tempCounter = 0;

// vectors and values for lookAt
glm::vec3 eye, at, up;
glm::vec3 eye_war, at_war, up_war;

bool moveMissle = false;


// angle of rotation for the camera direction
float angle_cam = 0.0;
// actual vector representing the camera's direction
float lx_cam = 0.0f, lz_cam = -1.0f;
// XZ position of the camera
float x_cam = 0.0f, z_cam = 100.0f;

bool trailingCamera = false, fire = false;

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
		int ID = modelObjArr[m]->getID();
		if(parentID > 1 && parentID < 5) {
			modelMatrix = modelObjArr[m] -> getModelMatrix(modelObjArr[parentID]->getPositioning());
		}	
		else if(parentID == 5)
		{
			if(fire)
			{
				if(tempCounter < missileRange)
				{
					printf("\nMissile Fired!");
					modelObjArr[m] ->  updateWarbirdPosition('u');
					tempCounter ++;
					printf("\nTemp Counter: %d", tempCounter);
				}
				else
				{
					fire = false;
					tempCounter = 0;
					modelObjArr[m]->setMatricies(modelObjArr[parentID] ->getTranslationMatrix(), modelObjArr[parentID] -> getRotationMatrix());
				}
				modelMatrix = modelObjArr[m] -> getModelMatrix();
			}
			else
			{
				modelMatrix = (modelObjArr[parentID] ->getPositioning()) * modelObjArr[m]->getScaleMatrix();
				modelObjArr[m]->setMatricies(modelObjArr[parentID] ->getTranslationMatrix(), modelObjArr[parentID] -> getRotationMatrix());
			}
		}
		else {

			if(ID == 7) { //if its the smart missle
				glm::mat4 m_war, m_missle;
				const float *pSource;
				double missle_Array[16] = {0.0};
				double war_Array[16] = {0.0};
				glm::vec3 pos_war, pos_missle;

				glm::vec3 dir;


				// Smart Missle //
				if(moveMissle == true) {
					m_missle = modelObjArr[7] -> getModelMatrix_MissleTracking();
				}
				else {
					m_missle = modelObjArr[7] -> getModelMatrix();
				}
				pSource = (const float*)glm::value_ptr(m_missle);
			
				for (int i = 0; i < 16; ++i)
					missle_Array[i] = pSource[i];
				// end Smart Missle//

			

				// warbird //
				m_war = modelObjArr[5] -> getModelMatrix();
				pSource = (const float*)glm::value_ptr(m_war);
			
				for (int i = 0; i < 16; ++i)
					war_Array[i] = pSource[i];
				// end warbird //


				pos_war = glm::vec3(war_Array[12],war_Array[13],war_Array[14]);
				pos_missle = glm::vec3(missle_Array[12],missle_Array[13],missle_Array[14]);



				float x = pos_war.x - pos_missle.x;
				float y = pos_war.y - pos_missle.y;
				float z = pos_war.z - pos_missle.z;

				float distance = glm::sqrt(((x) * (x)) + ((y) * (y)) + ((z) * (z)));
			
				//printf("DISTANCE$$$$$$$$$$$$ %f\n\n", distance);

				if(distance <= 3000.0) {
					moveMissle = true;
				}

				if(moveMissle == true) {
					dir.x = pos_war.x - pos_missle.x;
					dir.y = pos_war.y - pos_missle.y;
					dir.z = pos_war.z - pos_missle.z;

					dir = glm::normalize(dir);

					//printf("DIR::::::::::::::::::::: %f, %f, %f \n\n", dir.x, dir.y, dir.z); 

					pos_missle.x += dir.x*4;
					pos_missle.y += dir.y*4;
					pos_missle.z += dir.z*4;

					modelObjArr[7] ->updateSmartMisslePosition(pos_missle);
					//-----------

					modelObjArr[7] ->updateSmartMissleRotation();

					modelMatrix = modelObjArr[m] -> getModelMatrix_MissleTracking();
				}
				else {
					modelMatrix = modelObjArr[m] -> getModelMatrix();
				}
			}
			else {
				modelMatrix = modelObjArr[m] -> getModelMatrix(); 
			}
		}


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
	glClearColor(0.5f, 0.2f, 0.3f, 1.0f);
	
	
	for(int i = 0; i < nModels; i++) {
		modelObjArr[i] = new Shape3D(i);
	}
	printf("%d Models created \n", nModels);
}


// to set rotation
void animate(void){
  frameCount++;
  
  for(int i = 0; i < nModels; i++) {  
	  if(i != 5) {
		  modelObjArr[i] -> update();
	  }
	  else {
		  //modelObjArr[i] -> updateWarbirdPosition();
	  }
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
		trailingCamera = false;
        eye = glm::vec3(0.0f, 0.0f, 10000.0f);   // eye is 2000 "out of screen" from origin
        at  = glm::vec3(0.0f, 0.0f,    0.0f);   // looking at origin
        up  = glm::vec3(0.0f, 1.0f,    0.0f);   // camera'a up vector
        //strcpy(viewStr, " front view");
		break;
    case 'r' : case 'R' :  // bottom view
		trailingCamera = false;
        eye = glm::vec3(10000.0f, 0.0f, 0.0f);   // eye is 1000 right from origin
        at  = glm::vec3(   0.0f, 0.0f, 0.0f);   // looking at origin
        up  = glm::vec3(   0.0f, 1.0f, 0.0f);   // camera'a up vector
        //strcpy(viewStr, " right view");
		break;
    case 't' : case 'T' :  // top view
		trailingCamera = false;
        eye = glm::vec3(0.0f, 10000.0f,  0.0f);   // eye is 3000 up from origin
        at  = glm::vec3(0.0f,    0.0f,  0.0f);   // looking at origin  
        up  = glm::vec3(0.0f,    0.0f, -1.0f);   // camera's up is looking towards -Z vector
        //strcpy(viewStr, " top view"); 
		break;
	case 'k' : case 'K' :
		trailingCamera = false;
		eye = glm::vec3(0.0f, 5000.0f,  10000.0f);//0, 1000, 10950
		at  = glm::vec3(0.0f, 0.0f,  0.0f);
		up  = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 'i' : case 'I' :
		fire = true;
		break;
	case 'j' : case 'J':
		trailingCamera = true;
		eye = eye_war;
		at = at_war;
		up = up_war;
		//viewMatrix = glm::lookAt(eye_war, at_war, up_war);
		break;

    }
  viewMatrix = glm::lookAt(eye, at, up);
  }


void special_key_press( int key, int xx, int yy ) 
{
	glm::vec3 m_pos, m_rot, m_z;
	float fraction = 50.0f;
	double dArray[16] = {0.0};
	glm::mat4 m_war;
	const float *pSource;

    switch( key )
    {
		case GLUT_KEY_LEFT :
			modelObjArr[5]->updateWarbirdRotation('l');

			m_war = modelObjArr[5] -> getModelMatrix(); 

			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war;

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
		case GLUT_KEY_RIGHT :
			modelObjArr[5]->updateWarbirdRotation('r');

			m_war = modelObjArr[5] -> getModelMatrix(); 

			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war;

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
		case GLUT_KEY_UP :
			modelObjArr[5]->updateWarbirdPosition('u');
			
			m_war = modelObjArr[5] -> getModelMatrix(); 
			
			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war;

			if(trailingCamera)
				keyboard('j', 0, 0);

			break;
		case GLUT_KEY_DOWN :
			modelObjArr[5]->updateWarbirdPosition('d');
			
			m_war = modelObjArr[5] -> getModelMatrix(); 
			
			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war;

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
		case GLUT_KEY_F1:
			modelObjArr[5]->updateWarbirdRotation('u');

			m_war = modelObjArr[5] -> getModelMatrix(); 

			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war; 

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
		case GLUT_KEY_F2:
			modelObjArr[5]->updateWarbirdRotation('d');

			m_war = modelObjArr[5] -> getModelMatrix(); 

			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war; 

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
		case GLUT_KEY_F3:
			modelObjArr[5]->updateWarbirdRotation('s');

			m_war = modelObjArr[5] -> getModelMatrix(); 

			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war; 

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
		case GLUT_KEY_F4:
			modelObjArr[5]->updateWarbirdRotation('o');

			m_war = modelObjArr[5] -> getModelMatrix(); 

			pSource = (const float*)glm::value_ptr(m_war);
			
			for (int i = 0; i < 16; ++i)
				dArray[i] = pSource[i];

			up_war = glm::normalize(glm::vec3(dArray[4],dArray[5],dArray[6])); //up of camera is normalized up of warbird

			//position of camera is position of warbird plus a factor of the normalized warbird's up vector
			eye_war = glm::vec3(dArray[12]+(up_war.x*300.0f),dArray[13]+(up_war.y*300.0f),dArray[14]+(up_war.z*300.0f));
			
			at_war = glm::vec3(dArray[8],dArray[9],dArray[10]);
			at_war = glm::normalize(at_war);

			at_war = eye_war - at_war; 

			if(trailingCamera)
				keyboard('j', 0, 0);
			break;
	}

	
	printf("at_war ~~~~~~~~~~~~~~~~ : %f, %f, %f \n", at_war.x, at_war.y, at_war.z);
	
	//viewMatrix = glm::lookAt(eye_war, at_war, up_war);
		
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(880, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Options: R, T, F, K, J");

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
	glutSpecialFunc( special_key_press );

	glutIdleFunc(animate);
	glutTimerFunc(timerDelay, intervalTimer, 1);

	glutMainLoop();
	printf("done\n");
	return 0;	
}










