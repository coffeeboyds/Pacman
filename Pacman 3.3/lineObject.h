#include "screenObject.h"

#ifndef LINEOBJECT_H
#define LINEOBJECT_H

class lineObject : public screenObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  lineObject();

private:

  //=================================================================
  // Fill vertices buffer
  //=================================================================
  void setVertices();

};

#endif //LINEOBJECT_H