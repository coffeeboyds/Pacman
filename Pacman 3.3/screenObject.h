#include "sceneObject.h"

#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H

class screenObject : public sceneObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  screenObject();

  //=================================================================
  // Getters for the coordinates (in clip space)
  //=================================================================
  float getX();
  float getY();

  //=================================================================
  // Draw the image on the screen
  //=================================================================
  void draw();

  //=================================================================
  // Transformations
  //=================================================================
  void translate(float x, float y) {sceneObject::translate(x, y, 0);};
  void scale    (float x, float y) {sceneObject::scale(x, y, 0);};
  void rotate   (float degrees)    {sceneObject::rotate(degrees, glm::vec3(0,0,1));};

};

#endif //SCREENOBJECT_H