#include "screenObject.h"

#ifndef CIRCLEOBJECT_H
#define CIRCLEOBJECT_H

class circleObject : public screenObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  circleObject();

private:

  //=================================================================
  // Fill vertices buffer
  //=================================================================
  void setVertices();

  float radius;

};

#endif //CIRCLEOBJECT_H