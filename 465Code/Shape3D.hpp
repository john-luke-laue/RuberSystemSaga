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

public:

  Shape3D(int number) {
    id = number;  // for debugging

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
		radians = glm::radians(0.04f);//0.04
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
		radians = glm::radians(0.02f);//0.02
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-7000, 0, -7000));
		printf("\nDuo Created");
	}
	else if(id == 3) // Primus
	{
		parent = 2;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(0.04f);//0.04
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-500, 0, -500));
		printf("\nPrimus Created");
	}
	else if(id == 4)//Secundus
	{
		parent = 2;
		orbital = true;
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(150, 150, 150));
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(0.02f);//0.02f
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-700, 0, -700));
		printf("\nSecundus Created");
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
  void update() {
    rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
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