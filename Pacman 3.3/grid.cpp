#include "grid.h"

const int MAX_ROWS = 16;
const int MAX_COLS = 16;
const float CELL_SIZE_3D = 5.0;
const glm::vec2 origin2D = glm::vec2(-0.7, -0.7);

cell* cellList[MAX_ROWS*MAX_COLS];  // How do I declare this within the class itself?

//=================================================================
// Constructor
//=================================================================
grid::grid()
{
  wall = new cubeObject();
  wall->setMainColor( glm::vec3(0, 0, 1) );
  //wall->loadNormalMap("textures/normalMaps/normal.tga");  // Normal maps don't work well on a cube...
}
//=================================================================
// Destructor
//=================================================================
grid::~grid()
{
  delete lightIcon;
  delete ghostIcon;
  delete pacmanIcon;
}
//=================================================================
void grid::buildEditor()
{
  rows = 8;
  cols = 8;
  CELL_SIZE_2D = (1.0-origin2D.y-0.1)/rows;

  // Make 16x16 cells - the user will choose how many to use/display
  for(int i=0; i<MAX_ROWS; i++)
    for(int j=0; j<MAX_COLS; j++)
    {
      cellList[i*MAX_COLS+j] = new cell();
	  cellList[i*MAX_COLS+j]->setPosition2D(i, j, CELL_SIZE_2D, origin2D );
	  cellList[i*MAX_COLS+j]->setPosition3D(i, j, CELL_SIZE_3D );
    }

  // Default starting cell
  startingCell = 0;
  cellList[startingCell]->togglePacman();

  // Prepare the icons to draw
  lightIcon = new imageObject();
  lightIcon->loadTexture("textures/editor/light.tga");

  ghostIcon = new imageObject();
  ghostIcon->loadTexture("textures/editor/ghost.tga");

  pacmanIcon = new imageObject();
  pacmanIcon->loadTexture("textures/editor/pacman.tga");
}
//=================================================================
int grid::getRows()
{
  return rows;
}
//=================================================================
int grid::getCols()
{
  return cols;
}
//=================================================================
std::string grid::getCellData(int row, int col)
{
  std::string data;

  /** 6 binary digits indicating walls (left, right, up, down), light and ghost. 1 means yes, 0 means no **/

  // Has walls? (left, right, up, down) - 1 means yes, 0 means no
  for( int i=0; i<NUM_SIDES; i++ )
    if( cellList[row*MAX_COLS + col]->hasWall( (SIDES)i ) )
      data.append("1");
    else
	  data.append("0");

  // Has ghost? 1 means yes, 0 means no
  if( cellList[row*MAX_COLS + col]->hasLight() )
    data.append("1");
  else
	data.append("0");

  // Has light? 1 means yes, 0 means no
  if( cellList[row*MAX_COLS + col]->hasGhost() )
    data.append("1");
  else
	data.append("0");

  return data;
}
//=================================================================
float grid::getCellSize3D()
{
  return CELL_SIZE_3D;
}
//=================================================================
float grid::getStartingX()
{
  return -CELL_SIZE_3D*(startingCell%MAX_COLS) - CELL_SIZE_3D/2;
}
//=================================================================
float grid::getStartingY()
{
  return CELL_SIZE_3D*(startingCell/MAX_COLS);
}
//=================================================================
int grid::getStartingCell()
{
  return startingCell;
}
//=================================================================
void grid::getGhostPositions(std::vector<ghost*>& ghosts)
{
  /** Fill the parameter vector with the initial model matrix of each ghost (their starting positions) **/

  ghosts.clear();

  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
	  if( cellList[i*MAX_COLS+j]->hasGhost() )
	  {
		ghosts.push_back( new ghost );
		ghosts.at( ghosts.size()-1 )->translate(-CELL_SIZE_3D*j - CELL_SIZE_3D/2, 0, CELL_SIZE_3D*i);
		ghosts.at( ghosts.size()-1 )->setSize( CELL_SIZE_3D/5 );
	  }
}
//=================================================================
std::vector<glm::vec3> grid::getLights()
{
  /** Fill the parameter vector with the position of each light **/

  std::vector<glm::vec3> lights;

  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
	  if( cellList[i*MAX_COLS+j]->hasLight() )
	    lights.push_back( glm::vec3( -j*CELL_SIZE_3D - CELL_SIZE_3D/2.0, CELL_SIZE_3D/2, i*CELL_SIZE_3D ) );

  return lights;
}
//=================================================================
void grid::placeWall(float x, float y)
{
  /** Place a wall in the appropriate cell and side **/

  float TOLERANCE = 0.02;  // How close the mouse should be to the wall when clicked

  // If you clicked outside the grid
  if( x < origin2D.x || x > origin2D.x + cols*CELL_SIZE_2D ||
	  y < origin2D.y || y > origin2D.y + rows*CELL_SIZE_2D )
    return;

  int row = (y-origin2D.y) / CELL_SIZE_2D;
  int col = (x-origin2D.x) / CELL_SIZE_2D;

  // LEFT WALL
  if( x-origin2D.x-col*CELL_SIZE_2D < TOLERANCE )
  {
    cellList[row*MAX_COLS + col]->toggleWall(LEFT);

	// If there is a cell on the left, toggle its right wall as well
	if( col != 0 )
      cellList[row*MAX_COLS + col - 1]->toggleWall(RIGHT);

	return;
  }

  // RIGHT WALL
  if( x-origin2D.x-col*CELL_SIZE_2D - CELL_SIZE_2D > -TOLERANCE )
  {
    cellList[row*MAX_COLS + col]->toggleWall(RIGHT);

	// If there is a cell on the right, toggle its left wall as well
	if( col != cols-1 )
      cellList[row*MAX_COLS + col + 1]->toggleWall(LEFT);

	return;
  }

  // TOP WALL
  if( y-origin2D.y-row*CELL_SIZE_2D - CELL_SIZE_2D > -TOLERANCE )
  {
    cellList[row*MAX_COLS + col]->toggleWall(UP);

	// If there is a cell above, toggle its bottom wall as well
	if( row != rows-1 )
      cellList[(row+1)*MAX_COLS + col]->toggleWall(DOWN);

	return;
  }

  // BOTTOM WALL
  if( y-origin2D.y-row*CELL_SIZE_2D < TOLERANCE )
  {
    cellList[row*MAX_COLS + col]->toggleWall(DOWN);

	// If there is a cell below, toggle its top wall as well
	if( row != 0 )
      cellList[(row-1)*MAX_COLS + col]->toggleWall(UP);

	return;
  }
}
//=================================================================
void grid::placeLight(float x, float y)
{
  /** Place a light icon in the appropriate cell **/

  // If you clicked outside the grid
  if( x < origin2D.x || x > origin2D.x + cols*CELL_SIZE_2D ||
	  y < origin2D.y || y > origin2D.y + rows*CELL_SIZE_2D )
    return;

  int row = (y-origin2D.y) / CELL_SIZE_2D;
  int col = (x-origin2D.x) / CELL_SIZE_2D;

  cellList[row*MAX_COLS + col]->toggleLight();
}
//=================================================================
void grid::placeGhost(float x, float y)
{
  /** Place a ghost icon in the appropriate cell **/

  // If you clicked outside the grid
  if( x < origin2D.x || x > origin2D.x + cols*CELL_SIZE_2D ||
	  y < origin2D.y || y > origin2D.y + rows*CELL_SIZE_2D )
    return;

  int row = (y-origin2D.y) / CELL_SIZE_2D;
  int col = (x-origin2D.x) / CELL_SIZE_2D;

  // If pacman hasn't already been placed there
  if( !cellList[row*MAX_COLS + col]->hasPacman() )
    cellList[row*MAX_COLS + col]->toggleGhost();
}
//=================================================================
void grid::placePacman(float x, float y)
{
  /** Place a pacman icon in the appropriate cell **/

  // If you clicked outside the grid
  if( x < origin2D.x || x > origin2D.x + cols*CELL_SIZE_2D ||
	  y < origin2D.y || y > origin2D.y + rows*CELL_SIZE_2D )
    return;

  int row = (y-origin2D.y) / CELL_SIZE_2D;
  int col = (x-origin2D.x) / CELL_SIZE_2D;

  // If a ghost hasn't been placed there already 
  if( !cellList[row*MAX_COLS + col]->hasGhost() )
  {
    cellList[startingCell]->togglePacman();
    cellList[row*MAX_COLS + col]->togglePacman();
    startingCell = row*MAX_COLS + col;
  }
}
//=================================================================
bool grid::isWall(SIDES side, float x, float z)
{
  int cell = -(int)x/CELL_SIZE_3D + (int)(z/CELL_SIZE_3D)*MAX_COLS;

  // Make sure we are accessing a valid cell
  if( cell >= 0 && cell < MAX_ROWS*MAX_COLS && cellList[cell] != 0 )
    return cellList[cell]->hasWall(side);
  else
	return false;  // The cell doesn't even exist, there are no walls
}
//=================================================================
void grid::draw2D()
{
  /** Draw all the cells in 2D with lines **/

  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
	{
	  // Draw the walls
	  cellList[i*MAX_COLS+j]->draw2D();

	  // Draw the light icon if it has one
	  if( cellList[i*MAX_COLS+j]->hasLight() )
	  {
		lightIcon->loadIdentity();
		lightIcon->translate(origin2D.x + j*CELL_SIZE_2D + CELL_SIZE_2D/2.0 + CELL_SIZE_2D/4, origin2D.y + i*CELL_SIZE_2D + CELL_SIZE_2D/2.0 + CELL_SIZE_2D/4);
		lightIcon->scale(CELL_SIZE_2D/4, CELL_SIZE_2D/4);
		lightIcon->draw();
	  }

	  // Draw the ghost icon if it has one
	  if( cellList[i*MAX_COLS+j]->hasGhost() )
	  {
		ghostIcon->loadIdentity();
		ghostIcon->translate(origin2D.x + j*CELL_SIZE_2D + CELL_SIZE_2D/2.0, origin2D.y + i*CELL_SIZE_2D + CELL_SIZE_2D/2.0);
		ghostIcon->scale(CELL_SIZE_2D/4, CELL_SIZE_2D/4);
		ghostIcon->draw();
	  }

	  // Draw the pacman icon if it has one
	  if( cellList[i*MAX_COLS+j]->hasPacman() )
	  {
		pacmanIcon->loadIdentity();
		pacmanIcon->translate(origin2D.x + j*CELL_SIZE_2D + CELL_SIZE_2D/2.0, origin2D.y + i*CELL_SIZE_2D + CELL_SIZE_2D/2.0);
		pacmanIcon->scale(CELL_SIZE_2D/4, CELL_SIZE_2D/4);
		pacmanIcon->draw();
	  }
	}
}
//=================================================================
void grid::draw3D()
{
  // Draw all the cells in 3D with scaled cubes
  wall->draw( wall_ModelMatrices );
}
//=================================================================
void grid::setRows(int _rows)
{
  /** Set the number of rows and re-adjust the size/position of the walls **/

  // If you are removing the row that contains the starting position,
  // place the starting position at the first cell
  if( _rows < rows && startingCell/MAX_COLS == rows-1 )
  {
	cellList[startingCell]->togglePacman();
	cellList[0]->togglePacman();
    startingCell = 0;
  }

  rows = _rows;

  // Set the cell size so that it fits the screen in 2D
  if( rows > cols )
	CELL_SIZE_2D = (1.0-origin2D.y-0.1)/rows;
  else
	CELL_SIZE_2D = (1.0-origin2D.x-0.1)/cols;

  // Re-position the cells so they all fit on the screen in 2D
  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
	  cellList[i*MAX_COLS+j]->setPosition2D(i, j, CELL_SIZE_2D, origin2D );
}
//=================================================================
void grid::setCols(int _cols)
{
  /** Set the number of columns and re-adjust the size/position of the walls **/

  // If you are removing the column that contains the starting position,
  // place the starting position at the first cell
  if( _cols < cols && startingCell-(startingCell/MAX_COLS)*MAX_COLS == cols-1 )
  {
	cellList[startingCell]->togglePacman();
	cellList[0]->togglePacman();
    startingCell = 0;
  }
    
  cols = _cols;

  // Set the cell size so that it fits the screen
  CELL_SIZE_2D = (rows > cols)? (1.0-origin2D.y-0.1)/rows :  // if (rows > cols)
	                            (1.0-origin2D.x-0.1)/cols;   // else

  // Re-position the cells so they all fit on the screen in 2D
  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
	  cellList[i*MAX_COLS+j]->setPosition2D(i, j, CELL_SIZE_2D, origin2D );
}
//=================================================================
void grid::loadMap(int mapID)
{
  /** LOAD THE MAP NAME **/

  std::ifstream inputFile;
  inputFile.open("maps/mapNames.txt");
  char map[9];

  // Skip to line "mapID"
  for( int i=0; i<mapID; i++ )
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

  inputFile.getline(map, 9);  // Read the map name now
  inputFile.close();

  /** LOAD THE MAP DATA **/
  char mapNameTxt[18] = "maps/";
  strcat(mapNameTxt, map);
  strcat(mapNameTxt, ".txt");
  inputFile.open(mapNameTxt);

  // Get the rows and columns
  inputFile.getline(map, 3);
  rows = atoi(map);
  inputFile.getline(map, 3);
  cols = atoi(map);

  // Get the starting cell
  inputFile.getline(map, 4);
  startingCell = atoi(map);

  // Get the cell data
  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
    {
      cellList[i*MAX_COLS+j] = new cell();
	  cellList[i*MAX_COLS+j]->setPosition3D(i, j, CELL_SIZE_3D );

	  /** 6 binary digits indicating walls (left, right, up, down), light and ghost. 1 means yes, 0 means no **/
	  inputFile.getline(map, 7);

	  if( map[0] == '1' )
	    cellList[i*MAX_COLS+j]->toggleWall(LEFT);

	  if( map[1] == '1' )
	    cellList[i*MAX_COLS+j]->toggleWall(RIGHT);

	  if( map[2] == '1' )
	    cellList[i*MAX_COLS+j]->toggleWall(UP);

	  if( map[3] == '1' )
	    cellList[i*MAX_COLS+j]->toggleWall(DOWN);

	  if( map[4] == '1' )
		cellList[i*MAX_COLS+j]->toggleLight();

	  if( map[5] == '1' )
	    cellList[i*MAX_COLS+j]->toggleGhost();
    }

  cellList[startingCell]->togglePacman();

  collectModelMatrices();

  inputFile.close();
}
//=================================================================
void grid::collectModelMatrices()
{
  // Collect all the model matrices for the walls to draw later
  std::vector<glm::mat4> cell_ModelMatrices;
  for(int i=0; i<rows; i++)
    for(int j=0; j<cols; j++)
    {
      cellList[i*MAX_COLS+j]->getModelMatrices( cell_ModelMatrices );
	  
	  for( int k=0; k<cell_ModelMatrices.size(); k++ )
	    wall_ModelMatrices.push_back( cell_ModelMatrices.at(k) );

	  cell_ModelMatrices.clear();
	}

  // Performance can be improved by removing duplicates from wall_ModelMatrices !
}