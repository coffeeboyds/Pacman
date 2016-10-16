#include "shaderManager.h"

shaderManager* shaderManager::shaderManager_Instance = nullptr;

//=================================================================
// Constructor
//=================================================================
shaderManager::shaderManager()
{
  // Prepare all the shader programs to use

  imageTextureProgramID = loadShaders( "GLSL/imageTexture.vertexshader", "GLSL/imageTexture.fragmentshader" );
  imageColorProgramID   = loadShaders( "GLSL/imageColor.vertexshader", "GLSL/imageColor.fragmentshader" );
  meshColorProgramID    = loadShaders( "GLSL/meshColor.vertexshader", "GLSL/meshColor.fragmentshader" );
  meshTextureProgramID  = loadShaders( "GLSL/meshTexture.vertexshader", "GLSL/meshTexture.fragmentshader" );
  meshColorNormalMappingProgramID    = loadShaders( "GLSL/meshColorNormalMapping.vertexshader", "GLSL/meshColorNormalMapping.fragmentshader" );
  meshTextureNormalMappingProgramID  = loadShaders( "GLSL/meshTextureNormalMapping.vertexshader", "GLSL/meshTextureNormalMapping.fragmentshader" );
}
//=================================================================
// Destructor
//=================================================================
shaderManager::~shaderManager()
{
  glDeleteProgram(imageTextureProgramID);
  glDeleteProgram(imageColorProgramID);
  glDeleteProgram(meshColorProgramID);
  glDeleteProgram(meshTextureProgramID);
  glDeleteProgram(meshTextureNormalMappingProgramID);
}
//=================================================================
GLuint shaderManager::loadShaders(char* vertex_file_path, char* fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return -1;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return programID;
}
//=================================================================
GLuint shaderManager::getImageTextureProgramID()
{
  return imageTextureProgramID;
}
//=================================================================
GLuint shaderManager::getImageColorProgramID()
{
  return imageColorProgramID;
}
//=================================================================
GLuint shaderManager::getMeshColorProgramID()
{
  return meshColorProgramID;
}
//=================================================================
GLuint shaderManager::getMeshTextureProgramID()
{
  return meshTextureProgramID;
}
//=================================================================
GLuint shaderManager::getMeshColorNormalMappingProgramID()
{
  return meshColorNormalMappingProgramID;
}
//=================================================================
GLuint shaderManager::getMeshTextureNormalMappingProgramID()
{
  return meshTextureNormalMappingProgramID;
}