#include <glm.hpp>
#include <gtc/matrix_transform.hpp> // For matrix transformations

#ifndef GHOSTOBJECT_H
#define GHOSTOBJECT_H

class ghost
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  ghost();

  //=================================================================
  // Getters
  //=================================================================
  float getX();
  float getY();
  float getZ();
  glm::mat4 getModelMatrix();

  //=================================================================
  // Transformations
  //=================================================================
  void loadIdentity();
  void translate(float x, float y, float z);
  void setAngle(float degrees);
  void setSize(float factor);

  //=================================================================
  // Draw both hemispheres
  //=================================================================
  void draw();

private:

  float size;
  float xPos, yPos, zPos;
  float angle;

};

#endif //GHOSTOBJECT_H