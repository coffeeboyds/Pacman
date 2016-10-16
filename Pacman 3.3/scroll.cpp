#include "scroll.h"

const float SIZE = 0.03;  // Size of each button (squares), the value (text) is half this size

//=================================================================
// Constructor
//=================================================================
scroll::scroll( float x, float y )
{
  // The parameters are the origin of the scroll

  value = 8;
  origin.x = x;
  origin.y = y;

  valueText = new textImageObject();
  valueText->loadTexture("textures/ascii.tga");
  valueText->setText("8");
  valueText->translate(x+SIZE/2, y);
  valueText->scale(SIZE, SIZE);
  
  increment = new triangleObject();
  increment->setMainColor(glm::vec3(0,0,1));
  increment->translate(x, y+SIZE);
  increment->scale(2*SIZE, SIZE);

  decrement = new triangleObject();
  decrement->setMainColor(glm::vec3(0,0,1));
  decrement->translate(x+SIZE*1.8, y-SIZE);
  decrement->rotate(180);
  decrement->scale(2*SIZE, SIZE);
}
//=================================================================
// Destructor
//=================================================================
scroll::~scroll()
{
  delete increment;
  delete decrement;
  delete valueText;
}
//=================================================================
void scroll::checkClick(float x, float y)
{
  /** Check the mouse coordinates for a button when mouse is clicked **/

  // If you clicked within the range of the scrollbar
  if( x >= origin.x && x <= origin.x+2*SIZE )
  {
	/** Check if a button was clicked **/

	// If the decrement button was clicked
	if( y < origin.y-SIZE/2 && y > origin.y-2*SIZE &&
		value > 5 )
	{
	  char numText[3];
	  value--;
	  valueText->setText(itoa(value,numText,10));
	}

    // If the increment button was clicked
	if( y > origin.y+SIZE/2 && y < origin.y+2*SIZE &&
		value < 16 )
	{
	  char numText[3];
	  value++;
	  valueText->setText(itoa(value,numText,10));
	}
  }
}
//=================================================================
int scroll::getValue()
{
  return value;
}
//=================================================================
void scroll::draw()
{
  increment->draw();
  decrement->draw();
  valueText->draw();
}