#include "imageObject.h"
#include "lineObject.h"
#include "cubeObject.h"

#ifndef CELL_H
#define CELL_H

static enum SIDES { LEFT, RIGHT, UP, DOWN, NUM_SIDES };

class cell
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  cell();

  //=================================================================
  // Destructor
  //=================================================================
  ~cell();

  //=================================================================
  // Getter for the attributes
  //=================================================================
  bool hasWall(SIDES side);
  bool hasLight();
  bool hasGhost();
  bool hasPacman();
  void getModelMatrices(std::vector<glm::mat4>& cell_ModelMatrices);

  //=================================================================
  // Set the position of the cell in the grid
  //=================================================================
  void setPosition2D(int row, int col, float size, glm::vec2 origin);
  void setPosition3D(int row, int col, float size);

  //=================================================================
  // Toggle the attributes on/off
  //=================================================================
  void toggleWall(SIDES wall);
  void toggleLight();
  void toggleGhost();
  void togglePacman();

  //=================================================================
  // Draw the walls
  //=================================================================
  void draw2D();

private:

  bool isWall[NUM_SIDES];   // Indicates if there is a wall on all 4 sides
  bool light;               // Indicates if there is a light in this cell
  bool pacmanStartingCell;  // Indicates if this is a starting cell
  bool ghostStartingCell;   // Indicates if a ghost starts in this cell

  lineObject* walls2D[NUM_SIDES];
  cubeObject* walls3D[NUM_SIDES];

};

#endif //CELL_H