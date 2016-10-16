#include "imageObject.h"   // Each button is really just an image
#include "circleObject.h"  // To draw a circle around the selected button

#ifndef TOOLBAR_H
#define TOOLBAR_H

// List of buttons in this toolbar
static enum BUTTONS { WALL, LIGHT, GHOST, PACMAN, WORLD, SAVE, EXIT, NUM_BUTTONS };

class toolbar
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  toolbar();

  //=================================================================
  // Destructor
  //=================================================================
  ~toolbar();

  //=================================================================
  // Check the mouse coordinates for a button when mouse is clicked
  //=================================================================
  void checkClick(float x, float y);

  //=================================================================
  // Getter/setter for the current selection
  //=================================================================
  int getSelection();
  void setSelection(BUTTONS select);

  //=================================================================
  // Draw all the buttons and the circle around the selected button
  //=================================================================
  void draw();

private:

  circleObject* circle;     // Drawn around the selected button
  imageObject* buttonList[NUM_BUTTONS];

  int clicked;  // The button that is currently selected

  glm::vec2 origin; // Origin of the toolbar in clipspace

};

#endif //TOOLBAR_H