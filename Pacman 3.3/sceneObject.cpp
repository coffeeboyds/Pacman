#include "sceneObject.h"

//=================================================================
// Constructor
//=================================================================
sceneObject::sceneObject()
{
  MatrixManager = matrixManager::GetInstance();
  ShaderManager = shaderManager::GetInstance();

  // No shading program or texture to begin with
  programID = 0;
  texture = -1;

  // By default, since most objects are drawn with triangles
  drawMode = GL_TRIANGLES;

  vertex_count = 0;
  indices_count = 0;

  // Create 1 VAO, and VBOs to associate with it
  glGenVertexArrays(1, &VAO);
  glGenBuffers(num_buffers, VBO);

  // An element buffer is generated to make indicies to identical 
  // vertices that have the same position/color/uv/etc so the
  // same vertex is drawn only once (performance boost)
  glGenBuffers(1, &elementBuffer);
}
//=================================================================
// Destructor
//=================================================================
sceneObject::~sceneObject()
{
  glDeleteTextures(1, &texture);
  glDeleteBuffers(num_buffers, VBO);
  glDeleteBuffers(1, &elementBuffer);
  glDeleteVertexArrays(1, &VAO);
}
//=================================================================
int sceneObject::loadTexture(char* file_path)
{
  // Create one OpenGL texture
  glGenTextures(1, &texture);
	
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, texture);

  return loadImage(file_path);
}
//=================================================================
int sceneObject::loadImage(char* file_path)
{
  /** The image must be .tga or .bmp (24 or 32 bit) format **/

  printf("Reading image %s\n", file_path);

  // Check the file extension and load the image accordingly
  std::string file = file_path;
  if( file.substr(file.size()-4, 4) == ".bmp" )
    loadBMP(file_path);
  else
    glfwLoadTexture2D(file_path, 0);

  // Poor filtering, or ...
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

  // ... nice trilinear filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
  glGenerateMipmap(GL_TEXTURE_2D);

  return 1;
}
//=================================================================
int sceneObject::loadBMP(char* file_path)
{
  // Routine to read a bitmap file.
  // Works only for uncompressed bmp files of 24-bit or 32-bit color.
  // Both width and height must (or should) be powers of 2.
  unsigned int   size, width, height, offset, headerSize;
  unsigned char *data;  //The actual image itself
  unsigned short bits;  //24 or 32 (obtained in the header part of the .bmp file and is 2 bytes)

  // Read input file name.
  std::ifstream infile(file_path, std::ios::binary);

  // Get the starting point of the image data.
  infile.seekg(10);
  infile.read((char *) &offset, 4);

  // Get the header size of the bitmap.
  infile.read((char *) &headerSize,4);

  // Get width and height values in the bitmap header.
  infile.seekg(18);
  infile.read( (char *) &width, 4);
  infile.read( (char *) &height, 4);

  // Get the number of bits per pixel.
  infile.seekg(28);
  infile.read((char *) &bits, 2);

  // Allocate buffer for the image.
  size = width * height * 24;
  data = new unsigned char[size];

  // Read bitmap data.
  infile.seekg(offset);
  infile.read((char *) data , size);

  // Give the image to OpenGL
  if( bits == 32 )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  // OpenGL has now copied the data. Free our own version
  delete [] data;

  return 1;
}
//=================================================================
glm::vec3 sceneObject::getMainColor()
{
  return mainColor;
}
//=================================================================
void sceneObject::setMainColor(glm::vec3 _color)
{
  mainColor = _color;
  setColors();
}
//=================================================================
void sceneObject::setDrawMode(GLenum mode)
{
  drawMode = mode;
}
//=================================================================
GLenum sceneObject::getDrawMode()
{
  return drawMode;
}
//=================================================================
void sceneObject::setProgramID(GLuint ID)
{
  programID = ID;
}
//=================================================================
void sceneObject::enableDraw()
{
  drawEnabled = true;
}
//=================================================================
void sceneObject::disableDraw()
{
  drawEnabled = false;
}
//=================================================================
void sceneObject::setColors()
{
  buffers3D[color].clear();

  for( int i=0; i<vertex_count; i++ )
    buffers3D[color].push_back( getMainColor() );

  // Give the color data to the VBO
  bindBuffer(buffers3D[color], color);
}
//=================================================================
void sceneObject::bindBuffer(std::vector<glm::vec2> buffer, int bufferType)
{
  // Return if there is nothing to bind
  if( buffer.size() == 0 )
	  return;

  // Bind this object and the buffer
  glBindVertexArray( VAO );
  glBindBuffer(GL_ARRAY_BUFFER, VBO[bufferType]);
  
  // Give the data to the VBO
  glBufferData(GL_ARRAY_BUFFER,
               buffer.size() * sizeof(glm::vec2),
			   &buffer[0],
			   GL_STATIC_DRAW);

  // Set the attribute for GLSL
  glEnableVertexAttribArray(bufferType);
  glVertexAttribPointer(
    bufferType,         // attribute number, must match the "layout number" in the shader.
    2,                  // 2D or 3D
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Unbind/disable everything
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
//=================================================================
void sceneObject::bindBuffer(std::vector<glm::vec3> buffer, int bufferType)
{
  // Return if there is nothing to bind
  if( buffer.size() == 0 )
	  return;

  // Bind this object and the buffer
  glBindVertexArray( VAO );
  glBindBuffer(GL_ARRAY_BUFFER, VBO[bufferType]);
  
  // Give the data to the VBO
  glBufferData(GL_ARRAY_BUFFER,
               buffer.size() * sizeof(glm::vec3),
			   &buffer[0],
			   GL_STATIC_DRAW);

  // Set the attribute for GLSL
  glEnableVertexAttribArray(bufferType);
  glVertexAttribPointer(
    bufferType,         // attribute number, must match the "layout number" in the shader.
    3,                  // 2D or 3D
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Unbind/disable everything
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
//=================================================================
void sceneObject::makeIndicies()
{
  /** Incdices are made for idectical vertices that have the same
      position/color/uv/etc so the same vertex is
	  drawn only once (performance boost) **/

  std::vector<glm::vec2> indexed_buffers2D[num_buffers];
  std::vector<glm::vec3> indexed_buffers3D[num_buffers];
  std::vector<unsigned short> indices;
  indices_count = 0;

  if( vertex_count == 0 )
    return;

  // For each input vertex
  for ( unsigned int i=0; i<vertex_count; i++ )
  {
    // Try to find a similar vertex in indexed_buffers#D
    unsigned short index;
    bool found = false;
  
    for ( unsigned int j=0; j<indices_count; j++ )
    {
      found = true;
	  for ( unsigned int k=0; k<num_buffers; k++ )
	  {
	    if( indexed_buffers2D[k].size() > j && buffers2D[k].size() > i )
	      if(
		    !fabs( buffers2D[k].at(i).x - indexed_buffers2D[k].at(j).x ) < 0.1 ||
		    !fabs( buffers2D[k].at(i).y - indexed_buffers2D[k].at(j).y ) < 0.1 )
	      {
		    found = false;
		    break;
	      }

	    if( indexed_buffers3D[k].size() > j )
	      if(
		    !fabs( buffers3D[k].at(i).x - indexed_buffers3D[k].at(j).x ) < 0.01 ||
		    !fabs( buffers3D[k].at(i).y - indexed_buffers3D[k].at(j).y ) < 0.01 ||
		    !fabs( buffers3D[k].at(i).z - indexed_buffers3D[k].at(j).z ) < 0.01 )
	      {
		    found = false;
		    break;
	      }

	}

    if( found )
    {
      index = j;
      break;
    }
  }

    if ( found )
    {   // A similar vertex is already in the VBO, use it instead !
	    indices.push_back( index );
    }else
    {
      // If not, it needs to be added in the output data.
      for ( unsigned int j=0; j<num_buffers; j++ )
      {
        if( buffers2D[j].size() > i )
          indexed_buffers2D[j].push_back( buffers2D[j].at(i) );
	  
	    if( buffers3D[j].size() > i )
          indexed_buffers3D[j].push_back( buffers3D[j].at(i) );
      }

      indices.push_back( ++indices_count - 1 );
    }

  }

  // Give the buffer data to the VBO
  for( int i=0; i<num_buffers; i++ )
  {
    bindBuffer( indexed_buffers2D[i], i );
	bindBuffer( indexed_buffers3D[i], i );
  }

  indices_count = indices.size();

  // Bind the indicies to the element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//=================================================================
void sceneObject::translate(float x, float y, float z)
{
  modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z)); 
}
//=================================================================
void sceneObject::scale(float x, float y, float z)
{
  modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z)); 
}
//=================================================================
void sceneObject::rotate(float degrees, glm::vec3 axis)
{
  modelMatrix = glm::rotate(modelMatrix, degrees, axis); 
}
//=================================================================
void sceneObject::loadIdentity()
{
  modelMatrix = glm::mat4();
}
//=================================================================
void sceneObject::setModelMatrix( glm::mat4 _modelMatrix )
{
  modelMatrix = _modelMatrix;
}
//=================================================================
glm::mat4 sceneObject::getModelMatrix()
{
  return modelMatrix;
}