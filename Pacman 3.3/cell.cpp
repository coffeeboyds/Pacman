#include "cell.h"

//=================================================================
// Constructor
//=================================================================
cell::cell()
{
  // Prepare 4 walls in both 2D (line) and 3D (cube)
  for(int i=0; i<NUM_SIDES; i++)
  {
    walls2D[i] = new lineObject();
	walls3D[i] = new cubeObject();
	walls3D[i]->setMainColor( glm::vec3(0, 0, 1) );
	isWall[i] = false;
  }

  light = false;
  ghostStartingCell = false;
  pacmanStartingCell = false;
}
//=================================================================
// Destructor
//=================================================================
cell::~cell()
{
  for(int i=0; i<NUM_SIDES; i++)
  {
    delete walls2D[i];
    delete walls3D[i];
  }
}
//=================================================================
void cell::setPosition2D(int row, int col, float size, glm::vec2 origin)
{
  /** Set the position of the cell in the grid **/

  for(int i=0; i<NUM_SIDES; i++)
  {
	walls2D[i]->loadIdentity();
    walls2D[i]->translate(origin.x, origin.y);
  }

  walls2D[LEFT]->translate(col*size, row*size);
  walls2D[LEFT]->rotate(90);

  walls2D[RIGHT]->translate(col*size + size, row*size);
  walls2D[RIGHT]->rotate(90);

  walls2D[UP]->translate(col*size, row*size + size);

  walls2D[DOWN]->translate(col*size, row*size);

  // Scale it appropriately
  for(int i=0; i<NUM_SIDES; i++)
    walls2D[i]->scale(size,1.0);
}
//=================================================================
void cell::setPosition3D(int row, int col, float size)
{
  /** Set the position of the cell in the grid **/

  for(int i=0; i<NUM_SIDES; i++)
    walls3D[i]->loadIdentity();

  walls3D[LEFT]->translate(-col*size, 0, row*size);
  walls3D[LEFT]->rotate(90, glm::vec3(0,1,0) );

  walls3D[RIGHT]->translate(-col*size - size, 0, row*size);
  walls3D[RIGHT]->rotate(90, glm::vec3(0,1,0) );

  walls3D[UP]->translate(-col*size - size/2, 0, row*size + size/2);

  walls3D[DOWN]->translate(-col*size - size/2, 0, row*size - size/2);

  // Scale it appropriately
  for(int i=0; i<NUM_SIDES; i++)
    walls3D[i]->scale( size, size/10, size/10 );
}
//=================================================================
void cell::toggleWall(SIDES wall)
{
  // Toggle the wall on/off
  if ( isWall[wall] == true )
  {
    isWall[wall] = false;
    walls2D[wall]->setMainColor( glm::vec3(0.25, 0.25, 0.25) );
  }
  else
  {
    isWall[wall] = true;
    walls2D[wall]->setMainColor( glm::vec3(0, 0, 1) );
  }
}
//=================================================================
void cell::toggleLight()
{
  // Toggle light on/off
  light = light? false:true;
}
//=================================================================
void cell::toggleGhost()
{
  // Toggle ghost on/off
  ghostStartingCell = ghostStartingCell? false:true;
}
//=================================================================
void cell::togglePacman()
{
  // Toggle starting cell on/off
  pacmanStartingCell = pacmanStartingCell? false:true;
}
//=================================================================
bool cell::hasWall(SIDES side)
{
  return isWall[side];
}
//=================================================================
bool cell::hasLight()
{
  return light;
}
//=================================================================
bool cell::hasGhost()
{
  return ghostStartingCell;
}
//=================================================================
bool cell::hasPacman()
{
  return pacmanStartingCell;
}
//=================================================================
void cell::draw2D()
{
  // Draw the walls in 2D (lines)
  for(int i=0; i<NUM_SIDES; i++)
    walls2D[i]->draw();
}
//=================================================================
void cell::getModelMatrices(std::vector<glm::mat4>& cell_ModelMatrices)
{
  for(int i=0; i<NUM_SIDES; i++)
	if( isWall[i] )
	  cell_ModelMatrices.push_back( walls3D[i]->getModelMatrix() );
}