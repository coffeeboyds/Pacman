#include "imageObject.h"  // To place icons (lights, ghosts) in each cell
#include "meshObject.h"   // For the ghost
#include "cell.h"
#include "ghost.h"        // To get the initial ghost positions

#ifndef GRID_H
#define GRID_H

class grid
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  grid();

  //=================================================================
  // Destructor
  //=================================================================
  ~grid();

  //=================================================================
  // Getters/Setters
  //=================================================================
  int   getRows();
  int   getCols();
  int   getStartingCell();
  float getCellSize3D();
  float getStartingX();
  float getStartingY();
  std::vector<glm::vec3> getLights();
  std::string getCellData(int row, int col);
  void collectModelMatrices();

  void getGhostPositions( std::vector<ghost*>& ghosts );
  void setRows( int _rows );
  void setCols( int _cols );

  //=================================================================
  // Place a wall/light/ghost/pacman icon in the appropriate cell
  //=================================================================
  void placeWall(float x, float y);
  void placeLight(float x, float y);
  void placeGhost(float x, float y);
  void placePacman(float x, float y);

  //=================================================================
  // Return true if there is a wall on that size at coordinate x,z
  //=================================================================
  bool isWall(SIDES side, float x, float z);

  //=================================================================
  // Prepare the grid for the editorScene
  //=================================================================
  void buildEditor();

  //=================================================================
  // Draw all the cells in 2D or 3D
  //=================================================================
  void draw2D();
  void draw3D();

  //=================================================================
  // Load the map on line "mapID" in mapNames.txt
  //=================================================================
  void loadMap(int mapID);

private:

  int rows, cols;
  float CELL_SIZE_2D;

  imageObject* lightIcon;
  imageObject* ghostIcon;
  imageObject* pacmanIcon;

  cubeObject* wall;
  std::vector<glm::mat4> wall_ModelMatrices;

  int startingCell;

};

#endif //GRID_H