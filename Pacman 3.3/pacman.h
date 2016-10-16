#include "hemisphereObject.h"

#ifndef PACMANOBJECT_H
#define PACMANOBJECT_H

class pacman
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  pacman();

  //=================================================================
  // Getters
  //=================================================================
  float getX();
  float getY();
  float getZ();

  //=================================================================
  // Transformations
  //=================================================================
  void loadIdentity();
  void translate(float x, float y, float z);
  void setAngle(float degrees);
  void setSize(float factor);

  //=================================================================
  // Draw both hemispheres
  // Milliseconds: How far to rotate the hemispheres
  //=================================================================
  void draw(float Milliseconds);

private:

  bool opening;
  float mouthRotation;
  float size;
  float xPos, yPos, zPos;
  float angle;

  hemisphereObject* top;
  hemisphereObject* bottom;

};

#endif //PACMANOBJECT_H