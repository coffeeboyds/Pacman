#include "sceneObject.h"

#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

class worldObject : public sceneObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  worldObject();

  //=================================================================
  // Destructor
  //=================================================================
  ~worldObject();

  //=================================================================
  // Load a normal mapping texture
  //=================================================================
  int loadNormalMap(char* file_path);

  //=================================================================
  // Draw the object
  // Use overload to draw in multiple locations/sizes/orientations
  //=================================================================
  void draw();
  void draw( std::vector<glm::mat4> ModelMatrices );

protected:

  //=================================================================
  // Compute the tangents and bitangents for the whole mesh
  //=================================================================
  void computeTangentBasis();

private:

  GLuint normalMapTexture;  // A handler for the normal mapping texture

};

#endif //WORLDOBJECT_H