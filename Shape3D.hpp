/*
Shape3D.cpp

Mike Barnes
9/7/2013
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.h"
# define __INCLUDES465__
# endif

class Shape3D {

private :

  int id;
  int parent;
  glm::mat4 rotationMatrix;
  glm::mat4 scaleMatrix;
  glm::mat4 translationMatrix;
  glm::vec3 rotationAxis; 
  float radians; 
  float translation;
  bool orbital;
  bool warbird;


  /*
     ----
  for moving the warbird in direction of its forward
  */
  glm::vec3 war_pos;
  glm::vec3 war_dir;
  float war_speed;
  float radiansWarRight;
  float radiansWarLeft;
  /* ---- */


public:

  Shape3D(int number) {
    id = number;  // for debugging
	warbird = false;
	if(id == 0) //Ruber
	{
		parent = -1; //No Parents
		orbital = false;
		radians = glm::radians(0.2f);
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(2000, 2000, 2000));
		rotationAxis = glm::vec3(0, 1, 0);
		printf("\nRuber created");
	}
	else if(id == 1)//Unum
	{
		parent = 0;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(200, 200, 200));  // make model larger
		rotationMatrix = glm::mat4();  // no initial orientation
		//set cube's  rotation axis and rotation radians
		rotationAxis = glm::vec3(0, 1, 0);
		// rotate between 0.1 and 2.0 degrees
		radians = glm::radians(0.40f);//0.04
		// random initial placement +/- 500 from origin in X, Y, Z
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-4000, 0, -4000));
		printf("\nUnum Created");
	}
	else if(id == 2)// Duo
	{
		parent = 0;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(400, 400, 400));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(0.6f);//0.02
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(7000, 0, 7000));
		printf("\nDuo Created");
	}
	else if(id == 3) // Primus
	{
		parent = 2;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(0.9f);//0.04
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-500, 0, -500)); //8,100
		printf("\nPrimus Created");
	}
	else if(id == 4)//Secundus
	{
		parent = 2;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(150, 150, 150));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(1.0f);//0.02f
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(700, 0, 700)); //7250
		printf("\nSecundus Created");
	}
	else if(id == 5) //warbird
	{
		parent = -2;
		warbird = true;
		orbital = false;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(1000, 1000, 1000));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radiansWarRight = glm::radians(-0.5f);
		radiansWarLeft = glm::radians(0.5f);
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-4000, 0, -4000));

		war_pos = glm::vec3(0, 0, 0);
		war_dir = glm::vec3(1, 0, 0);
		war_speed = 35.0f;


		printf("\nWarbird Created");
	}
	else if(id == 6) //One Missile
	{
		parent = 5;
		orbital = false;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(150, 150, 150));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(0.0f);//0.02f
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-4000, 0, -4000));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(180.0f), rotationAxis); //Reverse the missile
		war_pos = glm::vec3(0, 0, 0);
		war_dir = glm::vec3(1, 0, 0);
		war_speed = 35.0f;

		printf("\n First Missile Loaded");
	}
	else if(id == 7) {  //smart missle
		parent = 0;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(400, 400, 400));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(0.6f);//0.02
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(7000, 0, 7000));
		printf("\nSmart Missle Id:7 Created");
	}
	else
	{
		int random = rand();   // random num 0..MAXINT
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(50, 50, 50));  // make model larger
		rotationMatrix = glm::mat4();  // no initial orientation
		//set cube's  rotation axis and rotation radians
		switch (random % 3) {
		  case 0  : rotationAxis = glm::vec3(1,  0,  0); break;
		  case 1  : rotationAxis = glm::vec3(0,  1,  0); break;
		  case 2  : rotationAxis = glm::vec3(0,  0, -1); break;
		  default : printf("Shape:: randomRotate selection error\n"); 
					exit(-1); break;
		  }
		// rotate between 0.1 and 2.0 degrees
		radians = glm::radians(0.1f + (random % 20) / 10.0f );
		// random initial placement +/- 500 from origin in X, Y, Z
		translationMatrix = glm::translate(glm::mat4(), 
		  glm::vec3(-500 + rand() % 1000, -500 + rand() % 1000, -500 + rand() % 1000));
	}
  }
  glm::mat4 getModelMatrix() {

	   if (orbital) // orbital rotation
			return(rotationMatrix * translationMatrix * scaleMatrix); // center rotation
	   else
			return (translationMatrix * rotationMatrix * scaleMatrix);
  }

  glm::mat4 getModelMatrix_MissleTracking() {
		return (translationMatrix * scaleMatrix);
  }

  glm::mat4 getModelMatrix(glm::mat4 parent)
  {

		  if(!orbital)
			 return parent * translationMatrix * rotationMatrix * scaleMatrix;
		  else
			  return parent * rotationMatrix * translationMatrix * scaleMatrix;
	
  }
  glm::mat4 getPositioning()
  {
	  if(orbital)
		return rotationMatrix * translationMatrix;
	  else
		  return translationMatrix * rotationMatrix;
  }
  void setMatricies(glm::mat4 trans, glm::mat4 rot)
  {
	  translationMatrix = trans;
	  rotationMatrix = rot;
  }

  glm::mat4 getScaleMatrix()
  {
	  return scaleMatrix;
  }
  glm::mat4 getTranslationMatrix()
  {
	  return translationMatrix;
  }
  glm::mat4 getRotationMatrix()
  {
	  return rotationMatrix;
  }
  void update() {
	 rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
    }   

  void updateWarbirdPosition(char dir) {

	  double dArray[16] = {0.0};

	  const float *pSource = (const float*)glm::value_ptr(getModelMatrix());

	  for (int i = 0; i < 16; ++i)
		  dArray[i] = pSource[i];

	  
	  //printf("posssssss --------------- : %f, %f, %f \n", dArray[12], dArray[13], dArray[14]); 

	  glm::vec3 war_at = glm::vec3(dArray[8],dArray[9],dArray[10]);

	  //printf("atttttttt --------------- : %f, %f, %f \n", war_at.x, war_at.y, war_at.z); 

	  war_at = glm::normalize(war_at);

	  //printf("atttttttt normal--------------- : %f, %f, %f \n", war_at.x, war_at.y, war_at.z); 

	  if(dir == 'u')
		war_at = war_at * -50.0f;
	  if(dir == 'd')
		  war_at = war_at * 50.0f;

	  glm::vec3 war_pos = glm::vec3(dArray[12],dArray[13],dArray[14]);

	  war_pos = war_pos + war_at;

	  translationMatrix = glm::translate(glm::mat4(), war_pos);
	  
  }
  
  void updateWarbirdRotation(char dir) {
	  if(dir == 'l')
		rotationMatrix = glm::rotate(rotationMatrix, radiansWarLeft, glm::vec3(0,1,0));
	  if(dir == 'r')
		  rotationMatrix = glm::rotate(rotationMatrix, radiansWarRight, glm::vec3(0,1,0));
  }


  void updateSmartMisslePosition(glm::vec3 misslePosition) {
	  printf("SMART MISSLE::::::::::::::::::::: %f, %f, %f \n", misslePosition.x, misslePosition.y, misslePosition.z); 
	  
	  translationMatrix = glm::translate(glm::mat4(), misslePosition);
  }

  void updateSmartMissleRotation() {
	  
  }

  int getID()
  {
	  return id;
  }
  int getParentID()
  {
	  return parent;
  }
  bool isOrbital()
  {
	  return orbital;
  }
};  