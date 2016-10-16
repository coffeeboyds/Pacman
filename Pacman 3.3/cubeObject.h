#include "worldObject.h"

#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

class cubeObject : public worldObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  cubeObject();

private:

  //=================================================================
  // Fill the buffers to form a cube
  //=================================================================
  void setVertices();
  void setUVs();
  void setNormals();

};

#endif //CUBEOBJECT_H