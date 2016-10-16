#include "scene.h"
#include "ghost.h"
#include "grid.h"
#include "topCam.h"

// For ghost movement
#include <algorithm> // std::random_shuffle

#ifndef GAMESCENE_H
#define GAMESCENE_H

class gameScene : public scene
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  gameScene();

  //=================================================================
  // Destructor
  //=================================================================
  ~gameScene();

  //=================================================================
  // Milliseconds: Time for animation
  // return value: An int representing a scene to transition to
  //               Return -1 to keep the current scene
  //=================================================================
  int Draw(float Milliseconds);

  //=================================================================
  // Handle input
  //=================================================================
  void KeyInput(int key, int action);
  void MouseInput(int button, int action);

private:

  //=================================================================
  // Do any logical computation before drawing the scene
  // Milliseconds: How long it's been since the last frame
  //=================================================================
  void Logic();

  //=================================================================
  // Do logic for pacman movement
  //=================================================================
  void pacmanMovement();

  //=================================================================
  // Do logic for the ghosts movement
  //=================================================================
  void ghostsMovement(); 

  //=================================================================
  // Update the camera's position to look at Pacman in third-person
  //=================================================================
  void updateThirdPersonCamera(); 

  //=================================================================
  // Change the camera's position to look at everything in the area
  //=================================================================
  void seeAllCamera(); 

  // The direction pacman is facing (right is in the negative x direction, up is positive z direction)
  enum DIRECTION { LEFT, RIGHT, UP, DOWN, STOP };

  grid* Grid;

  meshObject* ghostObject;                 // Object to be drawn for each ghost
  std::vector<ghost*> ghosts;              // List of ghosts
  std::vector<DIRECTION> ghostDirections;  // List of ghosts
  float ghostSteps;                        // How far each ghost has moved already from 1 cell to another (same for all ghosts)
  float GHOST_SPEED;                       // Must be a dividend of Grid->getCellSize3D()

  pacman* Pacman;
  DIRECTION dir;       // The direction pacman is facing (right is in the negative x direction, up is positive z direction)
  DIRECTION nextDir; 
  float pacmanSteps;   // How far pacman has moved already from 1 cell to another (always less than grid::CELL_SIZE_3D)
  float PACMAN_SPEED;  // Must be a dividend of Grid->getCellSize3D()

  // The amount of space within the center of a cell that pacman is allowed to turn
  float lowTurnSpace;
  float highTurnSpace;

  bool thirdPersonCamera; // Change between third-person view, or a nice overall view of the arena

  topCam* TopCam;

};

#endif //GAMESCENE_H