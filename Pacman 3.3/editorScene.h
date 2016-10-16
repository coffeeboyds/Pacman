#include "scene.h"

#include "grid.h"
#include "toolbar.h"
#include "scroll.h"

#ifndef EDITORSCENE_H
#define EDITORSCENE_H

class editorScene : public scene
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  editorScene();

  //=================================================================
  // Destructor
  //=================================================================
  ~editorScene();

  //=================================================================
  // Build the lists for the scene
  //=================================================================
  void Build();

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
  // Prepare the grid to be viewed in 3D
  //=================================================================
  void buildArena();

  //=================================================================
  // Save the map name in mapNames.txt and its data in mapName.txt
  //=================================================================
  bool saveMap(char* mapName);

  //=================================================================
  // Validate input while entering the name of the map while saving
  //=================================================================
  bool validKey(int key);

  toolbar* Toolbar;
  grid* Grid;
  scroll* rowsScroll;
  scroll* colsScroll;
  textImageObject* rowsText;
  textImageObject* colsText;

  meshObject* ghostObject;
  pacman* Pacman;

  std::vector<ghost*> ghosts;  // Model matrix for each ghost

  // For the save state
  int nameLength;
  char* inputName;
  textImageObject* enterNameText;
  textImageObject* savedText;
  textImageObject* mapName;

};

#endif //EDITORSCENE_H