#include <glew.h>

#include <fstream> // To read an image in loadImage() method
#include <string>  // For substr() in loadImage() method
#include <vector>

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

class shaderManager
{

public:

  //=================================================================
  // Destructor
  //=================================================================
  ~shaderManager();

  //=================================================================
  // Singleton Getter
  //=================================================================
  static shaderManager* GetInstance()
  {
    if(shaderManager_Instance == nullptr)
    {
      shaderManager_Instance = new shaderManager();
    }

    return shaderManager_Instance;
  }

  //=================================================================
  // Getters for the shader program IDs
  //=================================================================
  GLuint getImageTextureProgramID();
  GLuint getImageColorProgramID();
  GLuint getMeshColorProgramID();
  GLuint getMeshTextureProgramID();
  GLuint getMeshColorNormalMappingProgramID();
  GLuint getMeshTextureNormalMappingProgramID();

  // Handlers for the shader programs
  GLuint imageTextureProgramID;  // Shader program for all the images with a texture to use
  GLuint imageColorProgramID;    // Shader program for all the images with a color to use (blue line, green circle, etc)
  GLuint meshColorProgramID;     // Shader program for the 3D objects with colored vertices to use
  GLuint meshTextureProgramID;   // Shader program for the 3D objects with texture to use
  GLuint meshColorNormalMappingProgramID;     // Shader program for the 3D objects with normal mapping, but no texture to use
  GLuint meshTextureNormalMappingProgramID;   // Shader program for the 3D objects with texture and normal mapping to use

private:

  //=================================================================
  // Create and compile a GLSL program from the shaders
  //=================================================================
  GLuint loadShaders(char* vertex_file_path, char* fragment_file_path);

  //=================================================================
  // Constructor
  //=================================================================
  shaderManager();

  // Don't forget to declare these two. You want to make sure they
  // are unaccessible otherwise you may accidentally get copies of
  // this singleton appearing.
  shaderManager(shaderManager const&);  // Don't Implement
  void operator=(shaderManager const&); // Don't implement

  //singleton
  static shaderManager* shaderManager_Instance;

};

#endif //SHADERMANAGER_H