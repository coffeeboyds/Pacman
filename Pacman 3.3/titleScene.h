#include "scene.h"

#ifndef TITLESCENE_H
#define TITLESCENE_H

class titleScene : public scene
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  titleScene();

  //=================================================================
  // Destructor
  //=================================================================
  ~titleScene();

  //=================================================================
  // Do any logical computation before drawing the scene
  //=================================================================
  void Logic();

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

  int  select;        // Selected menu option
  bool mapSelection;  // True when displaying saved maps to choose from

  std::vector<std::string> maps;

  imageObject* logoImage; 
  imageObject* pacmanImage;
  imageObject* cherryImage;

  textImageObject* startText;
  textImageObject* mapText;
  textImageObject* exitText;
  textImageObject* instructionsText;
  textImageObject* mapSelectText;

};

#endif //TITLESCENE_H