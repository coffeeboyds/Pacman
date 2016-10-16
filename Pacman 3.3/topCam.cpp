#include "topCam.h"

//=================================================================
// Constructor
//=================================================================
topCam::topCam()
{
  WIDTH = 1024; HEIGHT = 768;
  cam = new imageObject();

  // Generate and bind the framebuffer
  glGenFramebuffers(1, &fboHandle);
  glBindFramebuffer(GL_FRAMEBUFFER, fboHandle); 

  // Create the texture object 
  glGenTextures(1, &renderedTexture);
  glBindTexture(GL_TEXTURE_2D, renderedTexture);  // // "Bind" the newly created texture : all future texture functions will modify this texture

  // Give an empty image to OpenGL ( the last "0" )
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

  // Poor filtering. Needed !
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Create the depth buffer
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
 
  // Set the list of draw buffers.
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    exit(0);

  cam->setTexture(renderedTexture);  // Give the texture to the camera
  cam->translate(-0.7, 0.75);
  cam->scale(0.5,0.5);
}
//=================================================================
// Destructor
//=================================================================
topCam::~topCam()
{
}
//=================================================================
void topCam::activate()
{
  // Render to our framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

  // Clear the previous colors on the rendered texture
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//=================================================================
void topCam::deactivate()
{
  // Unbind texture's FBO (back to default FB)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//=================================================================
void topCam::draw()
{
  cam->draw();
}
