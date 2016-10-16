#include <glew.h>
#include <glfw.h>  // For glfwLoadTexture2D() in loadImage() method

#include <fstream> // To read an image in loadImage() method
#include <string>  // For substr() in loadImage() method
#include <vector>

#include "matrixManager.h"
#include "shaderManager.h"

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

// DATA TO BE SHARED WITH ALL SUB-CLASSES

static const float PI = 3.14159265359;
static const float DEG_RAD = PI/180;

class sceneObject
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  sceneObject();

  //=================================================================
  // Destructor
  //=================================================================
  ~sceneObject();

  //=================================================================
  // Load a texture
  //=================================================================
  int loadTexture(char* file_path);

  //=================================================================
  // Getter/Setter for the main color to fill the color buffer with
  //=================================================================
  glm::vec3 getMainColor();
  void      setMainColor(glm::vec3 _color);

  //=================================================================
  // Getter/Setter for the drawing mode for glDrawElements()
  //=================================================================
  GLenum getDrawMode();
  void   setDrawMode(GLenum mode);

  //=================================================================
  // Set GLSL program to use
  //=================================================================
  void setProgramID(GLuint ID);

  //=================================================================
  // Enable/disable drawing this object
  //=================================================================
  void enableDraw();
  void disableDraw();

  //=================================================================
  // Transformations
  //=================================================================
  virtual void translate(float x, float y, float z);
  virtual void scale(float x, float y, float z);
  virtual void rotate(float degrees, glm::vec3 axis);
  void loadIdentity();
  void setModelMatrix( glm::mat4 _modelMatrix );
  glm::mat4 getModelMatrix();

private:

  GLenum drawMode;          // The drawing mode for glDrawElements()
  glm::vec3 mainColor;  // Color all the vertices with this if no texture is loaded

  //=================================================================
  // Load a .bmp image (24 or 32 bit). Called within loadTexture()
  //=================================================================
  int loadBMP(char* file_path);

  //=================================================================
  // Fill the color buffer with mainColor
  //=================================================================
  void setColors();

protected:

  //=================================================================
  // Make a list of indicies for the buffers and then bind them
  //=================================================================
  void makeIndicies();

  //=================================================================
  // Bind the buffer to the VAO and shader program
  //=================================================================
  void bindBuffer(std::vector<glm::vec2> buffer, int bufferType);
  void bindBuffer(std::vector<glm::vec3> buffer, int bufferType);

  //=================================================================
  // Called within loadTexture()
  //=================================================================
  int loadImage(char* file_path);

  shaderManager* ShaderManager;

  GLuint VAO;
  GLuint VBO[num_buffers];
  GLuint elementBuffer;
  GLuint texture;             // A handler for the actual texture itself
  GLuint programID;           // A handler for the shader program to be used

  int vertex_count;           
  int indices_count;          // Incdices are made for idectical vertices that have the same position/color/uv/etc so the same vertex is drawn only once (performance boost)
  bool drawEnabled;           // Only draw this object when this is true

  glm::mat4 modelMatrix;
  matrixManager* MatrixManager;

  // Buffers
  std::vector<glm::vec2> buffers2D[num_buffers]; // position for images (x and y), and uv
  std::vector<glm::vec3> buffers3D[num_buffers]; // position for 3D objects, color, normal, etc

};

#endif //SCENEOBJECT_H