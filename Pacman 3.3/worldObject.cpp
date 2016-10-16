#include "worldObject.h"

//=================================================================
// Constructor
//=================================================================
worldObject::worldObject()
{
  // No normal map texture to begin with
  normalMapTexture = -1;
}
//=================================================================
// Destructor
//=================================================================
worldObject::~worldObject()
{
  glDeleteTextures(1, &normalMapTexture);
}
//=================================================================
void worldObject::draw()
{ 
  if( !drawEnabled )
    return;

  // Use the shader program and bind the object's VAO
  glUseProgram(programID);
  glBindVertexArray( VAO );
  
  if( texture != -1 )  // If a texture was loaded already...
  {
    // Bind the texture and set the "textureSampler" (in the GLSL code) to Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  if( normalMapTexture != -1 )
  {
    // Bind the normal mapping texture and set the "normalTextureSampler" (in the GLSL code) to Texture Unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	glUniform1i(glGetUniformLocation(programID, "normalTextureSampler"), 1);
  }

  MatrixManager->bindModel( modelMatrix, programID );

  //glDrawArrays( getDrawMode(), 0, vertex_count );  //Draw
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glDrawElements(
                  getDrawMode(),     // mode
			      indices_count,     // count
			      GL_UNSIGNED_SHORT, // type
			      (void*)0          // element array buffer offset
		);
  
  glBindVertexArray(0);    // Unbind
  glUseProgram(0);         // Turn the shader off
}
//=================================================================
void worldObject::draw( std::vector<glm::mat4> ModelMatrices )
{ 
  if( !drawEnabled || ModelMatrices.empty() )
    return;

  // Use the shader program and bind the object's VAO
  glUseProgram(programID);
  glBindVertexArray( VAO );
  
  if( texture != -1 )  // If a texture was loaded already...
  {
    // Bind the texture and set the "textureSampler" (in the GLSL code) to Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  if( normalMapTexture != -1 )
  {
    // Bind the normal mapping texture and set the "normalTextureSampler" (in the GLSL code) to Texture Unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	glUniform1i(glGetUniformLocation(programID, "normalTextureSampler"), 1);
  }

  MatrixManager->bindModel( ModelMatrices, programID, VBO[modelsView] );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glDrawElementsInstanced(
                  getDrawMode(),     // mode
			      indices_count,     // count
			      GL_UNSIGNED_SHORT, // type
			      (void*)0,          // element array buffer offset
				  ModelMatrices.size()
		);
  
  glBindVertexArray(0);    // Unbind
  glUseProgram(0);         // Turn the shader off
}
//=================================================================
void worldObject::computeTangentBasis()
{
  /** Tangents and bitangents give proper lighting on normal maps **/

  for ( int i=0; i<buffers3D[position].size(); i+=3 )
  {
    // Shortcuts for vertices
    glm::vec3 & v0 = buffers3D[position][i+0];
    glm::vec3 & v1 = buffers3D[position][i+1];
    glm::vec3 & v2 = buffers3D[position][i+2];
 
    // Shortcuts for UVs
    glm::vec2 & uv0 = buffers2D[uv][i+0];
    glm::vec2 & uv1 = buffers2D[uv][i+1];
    glm::vec2 & uv2 = buffers2D[uv][i+2];
 
    // Edges of the triangle : postion delta
    glm::vec3 deltaPos1 = v1-v0;
    glm::vec3 deltaPos2 = v2-v0;
 
    // UV delta
    glm::vec2 deltaUV1 = uv1-uv0;
    glm::vec2 deltaUV2 = uv2-uv0;

    // Now use this formula to compute the tangent and the bitangent:
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 _tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
    glm::vec3 _bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

    // Set the same tangent for all three vertices of the triangle.
    buffers3D[tangent].push_back(_tangent);
    buffers3D[tangent].push_back(_tangent);
    buffers3D[tangent].push_back(_tangent);
 
    // Same thing for binormals
    buffers3D[bitangent].push_back(_bitangent);
    buffers3D[bitangent].push_back(_bitangent);
    buffers3D[bitangent].push_back(_bitangent);
  }
}
//=================================================================
int worldObject::loadNormalMap(char* file_path)
{
  // Change shading program to one that handles normal mapping
  if( texture != -1 )  // If a texture was loaded already...
    programID = ShaderManager->getMeshTextureNormalMappingProgramID();
  else
	programID = ShaderManager->getMeshColorNormalMappingProgramID();

  // Create one OpenGL texture
  glGenTextures(1, &normalMapTexture);
	
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, normalMapTexture);

  computeTangentBasis();  // For proper lighting on normal maps

  return loadImage(file_path);
}
