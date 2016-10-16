#include "triangleObject.h"
#include "textImageObject.h"

#ifndef SCROLL_H
#define SCROLL_H

class scroll
{

public:

  //=================================================================
  // Constructor - The parameters are the origin of the scroll
  //=================================================================
  scroll( float x, float y );

  //=================================================================
  // Destructor - Use default so it deletes everything itself
  //=================================================================
  ~scroll();

  //=================================================================
  // Check the mouse coordinates for a button when mouse is clicked
  //=================================================================
  void checkClick(float x, float y);

  //=================================================================
  // Getter for the current value
  //=================================================================
  int getValue();

  //=================================================================
  // Draw the 2 buttons and the current value
  //=================================================================
  void draw();

private:

  triangleObject*  increment;
  triangleObject*  decrement;
  textImageObject* valueText;

  int value;

  glm::vec2 origin;

};

#endif //SCROLL_H