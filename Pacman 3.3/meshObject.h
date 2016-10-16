#include "worldObject.h"

#ifndef MESHOBJECT_H
#define MESHOBJECT_H

class meshObject : public worldObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  meshObject();

  //=================================================================
  // Extract vertices/uvs/colors/normals from a .ply file
  //=================================================================
  bool loadPLY(const char* file);

};

#endif //MESHOBJECT_H