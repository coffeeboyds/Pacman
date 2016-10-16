#include "meshObject.h"

//=================================================================
// Constructor
//=================================================================
meshObject::meshObject()
{
  programID = ShaderManager->getMeshTextureProgramID();
}
//=================================================================
bool meshObject::loadPLY(const char* path)
{
  /** A .ply file can be exported in Blender **/

  printf("Loading PLY file %s...\n", path);

  // Number of vertices/faces in the file
  int num_vertices;
  int num_faces;

  // True if the file contains normal/uv/color data
  bool hasNormals = false;
  bool hasUVs = false;
  bool hasColors = false;

  // To hold all the vertex data in the file
  std::vector<glm::vec3> temp_vertices; 
  std::vector<glm::vec3> temp_normals;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_colors;

  // To hold the indices for the faces
  std::vector<unsigned int> indices;

  // The longest (meaningful) line in a .ply file won't exceed
  // 102 characters (2 extra for the newline and \0 character)
  const int BUFF_SIZE = 102+2;
  char buffer[BUFF_SIZE];

  FILE * file = fopen(path, "r");

  if( file == NULL )
  {
    printf("Impossible to open the file! Are you in the right path?\n");
    return false;
  }

  //** READ THROUGH THE HEADER **//
  // Data to collect in the header:
  // num_vertices, num_faces, and the indication of normals/uvs/colors

  while( strncmp(buffer, "end_header", strlen("end_header")) != 0 )
  {
    // Skip all the comments
    do
	{
      fgets(buffer, BUFF_SIZE, file);
	}
    while( strncmp(buffer, "comment", strlen("comment")) == 0 );

	// If the line contains the number of vertices
    if( strncmp(buffer, "element vertex ", strlen("element vertex ")) == 0 )
    {
	  // Get the number at the end of this line
      char *num = buffer + strlen("element vertex ");
      num_vertices = atoi( num );
      continue;
	};

	// If the line contains the number of faces
    if( strncmp(buffer, "element face ", strlen("element face ")) == 0 )
    {
	  // Get the number at the end of this line
      char *num = buffer + strlen("element face ");
      num_faces = atoi( num );
      continue;
	};

	// If the line indicates included normals
	if( strncmp(buffer, "property float nx", strlen("property float nx")) == 0 )
	{
      hasNormals = true;
	  continue;
	}

	// If the line indicates included uvs
	if( strncmp(buffer, "property float s", strlen("property float s")) == 0 )
	{
      hasUVs = true;
	  continue;
	}

	// If the line indicates included colors
	if( strncmp(buffer, "property uchar red", strlen("property uchar red")) == 0 )
	{
      hasColors = true;
	  continue;
	}
  }

  //** READ VERTEX DATA **//
  for( int i=0; i<num_vertices; i++ )
  {
    // Skip all the comments
    do
	{
      fgets(buffer, BUFF_SIZE, file);
	}
    while( strncmp(buffer, "comment", strlen("comment")) == 0 );

	// Break the line into tokens so they can be parsed
	char* numbers = strtok(buffer, " ");

	// Get the vertex coordinates
	float x = atof(numbers);
	numbers = strtok (NULL, " "); // Go to the next number

	float y = atof(numbers);
	numbers = strtok (NULL, " "); // Go to the next number

	float z = atof(numbers);
	numbers = strtok (NULL, " "); // Go to the next number

	temp_vertices.push_back( glm::vec3(x, y, z) );

	// Get the normal coordinate
	if( hasNormals )
	{
	  float nx = atof(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  float ny = atof(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  float nz = atof(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  temp_normals.push_back( glm::vec3( nx, ny, nz ) );
    }

	// Get the UV coordinate
	if( hasUVs )
	{
	  float u = atof(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  float v = atof(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  temp_uvs.push_back( glm::vec2( u, v ) );
    }

	// Get the color
	if( hasColors )
	{
	  int r = atoi(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  int g = atoi(numbers);
	  numbers = strtok (NULL, " "); // Go to the next number

	  int b = atoi(numbers);

	  // OpenGL likes its RGB values between 0 and 1, so divide by 255
	  temp_colors.push_back( glm::vec3( r/255.0, g/255.0, b/255.0 ) );
    }		
  }

  // READ ALL THE INDICES
  for( int i=0; i<num_faces; i++ )
  {
    // Skip all the comments or anything else that's not a face
    do
	{
      fgets(buffer, BUFF_SIZE, file);
	}
    while( buffer[0] != '3' && buffer[0] != '4' );

	// Break the line into tokens so they can be parsed
	char* numbers = strtok(buffer, " ");

	while( numbers != '\0' )
	{
	  indices.push_back( atoi(numbers) );
	  numbers = strtok (NULL, " "); // Go to the next number
	}
  }

  // Fill this meshObject's buffers
  for( int i=0; i<indices.size(); i+=indices[i]+1 )
  {
    buffers3D[position].push_back( temp_vertices.at( indices[i+1] ) );
	buffers3D[position].push_back( temp_vertices.at( indices[i+2] ) );
	buffers3D[position].push_back( temp_vertices.at( indices[i+3] ) );

	if( indices.at(i) == 4 )
	{
	  buffers3D[position].push_back( temp_vertices.at( indices[i+1] ) );
	  buffers3D[position].push_back( temp_vertices.at( indices[i+3] ) );
	  buffers3D[position].push_back( temp_vertices.at( indices[i+4] ) );
	}

	if( hasNormals )
	{
      buffers3D[normal].push_back( temp_normals.at( indices[i+1] ) );
      buffers3D[normal].push_back( temp_normals.at( indices[i+2] ) );
	  buffers3D[normal].push_back( temp_normals.at( indices[i+3] ) );

	  if( indices.at(i) == 4 )
	  {
	    buffers3D[normal].push_back( temp_normals.at( indices[i+1] ) );
        buffers3D[normal].push_back( temp_normals.at( indices[i+3] ) );
        buffers3D[normal].push_back( temp_normals.at( indices[i+4] ) );
	  }
	}

	if( hasUVs )
	{
      buffers2D[uv].push_back( temp_uvs.at( indices[i+1] ) );
      buffers2D[uv].push_back( temp_uvs.at( indices[i+2] ) );
	  buffers2D[uv].push_back( temp_uvs.at( indices[i+3] ) );

	  if( indices.at(i) == 4 )
	  {
		buffers2D[uv].push_back( temp_uvs.at( indices[i+1] ) );
        buffers2D[uv].push_back( temp_uvs.at( indices[i+3] ) );
        buffers2D[uv].push_back( temp_uvs.at( indices[i+4] ) );
	  }
	}

	if( hasColors )
	{
      buffers3D[color].push_back( temp_colors.at( indices[i+1] ) );
      buffers3D[color].push_back( temp_colors.at( indices[i+2] ) );
	  buffers3D[color].push_back( temp_colors.at( indices[i+3] ) );

	  if( indices.at(i) == 4 )
	  {
		buffers3D[color].push_back( temp_colors.at( indices[i+1] ) );
        buffers3D[color].push_back( temp_colors.at( indices[i+3] ) );
        buffers3D[color].push_back( temp_colors.at( indices[i+4] ) );
	  }
	}
  }


  // This gives proper lighting on normal maps, but I'm not using any and it's costly so I'll comment it out
  if( hasNormals && hasUVs )
    //computeTangentBasis();

  vertex_count = buffers3D[position].size();
  makeIndicies();

  return true;
}