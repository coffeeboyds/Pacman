#include "worldObject.h"

#ifndef HEMISPHEREOBJECT_H
#define HEMISPHEREOBJECT_H

class hemisphereObject : public worldObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  hemisphereObject();

private:

  //=================================================================
  // Fill vertices buffer with vertices that form a hemisphere
  //=================================================================
  void setVertices();

};

#endif //HEMISPHEREOBJECT_H