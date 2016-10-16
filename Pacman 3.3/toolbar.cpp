#include "toolbar.h"

const float SPACING = 0.25;      // Spacing between each button
const float SIZE    = 0.12;      // Size of the buttons (square)

//=================================================================
// Constructor
//=================================================================
toolbar::toolbar()
{
  clicked = WALL;  // Default selection

  origin = glm::vec2(-0.85, 0.85);

  for(int i=0; i<NUM_BUTTONS; i++)
    buttonList[i] = new imageObject();

  buttonList[WALL]->  loadTexture("textures/editor/wall.tga");
  buttonList[LIGHT]-> loadTexture("textures/editor/light.tga");
  buttonList[GHOST]-> loadTexture("textures/editor/ghost.tga");
  buttonList[PACMAN]->loadTexture("textures/editor/pacman.tga");
  buttonList[WORLD]-> loadTexture("textures/editor/world.tga");
  buttonList[SAVE]->  loadTexture("textures/editor/save.tga");
  buttonList[EXIT]->  loadTexture("textures/editor/exit.tga");

  // Line them up
  for(int i=0; i<NUM_BUTTONS; i++)
  {
	buttonList[i]->translate(origin.x, origin.y-(float)i*SPACING);
	buttonList[i]->scale(SIZE, SIZE);
  }

  // Circle the selected button
  circle = new circleObject();
  circle->translate(origin.x, origin.y-(float)clicked*SPACING);
  circle->setMainColor(glm::vec3(0,1,0));
}
//=================================================================
// Destructor
//=================================================================
toolbar::~toolbar()
{
  delete circle;

  for(int i=0; i<NUM_BUTTONS; i++)
    delete buttonList[NUM_BUTTONS];
}
//=================================================================
void toolbar::checkClick(float x, float y)
{
  /** Check the mouse coordinates for a button when mouse is clicked **/

  // If you clicked within the range of the toolbar
  if( x >= origin.x-SIZE &&
	  x <= origin.x+SIZE )
  {
	// Search for which button was clicked
    for(int i=0; i<NUM_BUTTONS; i++)
	  if( y > buttonList[i]->getY()-SIZE &&
		  y < buttonList[i]->getY()+SIZE
		)
	  {
		setSelection( (BUTTONS)i );
		break;
	  }
  }
}
//=================================================================
int toolbar::getSelection()
{
  return clicked;
}
//=================================================================
void toolbar::setSelection(BUTTONS select)
{
  /** Set the selected icon and circle it **/

  clicked = select;
  circle->loadIdentity();
  circle->translate(origin.x, origin.y-(float)clicked*SPACING);
}
//=================================================================
void toolbar::draw()
{
  /** Draw all the buttons and the circle around the selected button **/

  for(int i=0; i<NUM_BUTTONS; i++)
    buttonList[i]->draw();

  circle->draw();
}
