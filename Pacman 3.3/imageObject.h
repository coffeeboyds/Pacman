#include "screenObject.h"

#ifndef IMAGEOBJECT_H
#define IMAGEOBJECT_H

class imageObject : public screenObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  imageObject();

  //=================================================================
  // Load a texture and fill the vertex buffer using the image ratio
  //=================================================================
  int loadTexture(char* file_path);

  //=================================================================
  // Set texture to use
  //=================================================================
  void setTexture(GLuint _texture);

private:

  //=================================================================
  // Fill vertices buffer and bind it to VAO/shader program
  //=================================================================
  void setVertices();

  //=================================================================
  // Set the UV coordinates from image and bind to VAO/shader program
  //=================================================================
  void setUVs();

};

#endif //IMAGEOBJECT_H