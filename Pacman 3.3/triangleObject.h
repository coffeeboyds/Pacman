#include "screenObject.h"

#ifndef TRIANGLEOBJECT_H
#define TRIANGLEOBJECT_H

class triangleObject : public screenObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  triangleObject();

private:

  //=================================================================
  // Fill vertices buffer and bind it to VAO/shader program
  //=================================================================
  void setVertices();

};

#endif //TRIANGLEOBJECT_H