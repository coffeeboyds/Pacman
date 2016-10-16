#include "imageObject.h"
#include "lineObject.h"

#ifndef TOPCAM_H
#define TOPCAM_H

class topCam
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  topCam();

  //=================================================================
  // Destructor
  //=================================================================
  ~topCam();

  //=================================================================
  // 
  //=================================================================
  void activate();

  //=================================================================
  // 
  //=================================================================
  void deactivate();

  //=================================================================
  // Draw the camera
  //=================================================================
  void draw();

private:

  GLuint VAO;

  GLuint fboHandle;        // The handle to the FBO
  GLuint renderedTexture;  // The texture we're going to render to
  GLuint depthBuffer;

  imageObject* cam;
  int WIDTH, HEIGHT;
};

#endif //TOPCAM_H