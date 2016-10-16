#include "titleScene.h"

const int NUM_MENU_ITEMS = 3;

//=================================================================
// Constructor
//=================================================================
titleScene::titleScene()
{
  mapSelection = false;  // First show the initial menu, not the saved maps
  select = 0;            // "START" is selected first

  /** LOAD ALL THE MAP NAMES **/

  std::ifstream inputFile;
  inputFile.open("maps/mapNames.txt");
  char map[23];

  while( inputFile.getline(map, 9) )
    maps.push_back(std::string(map));

  inputFile.close();

  maps.push_back("Return to main menu...");

  /** GET ALL THE IMAGES/TEXTS READY **/

  logoImage = new imageObject();
  logoImage->loadTexture("textures/title/logo.tga");
  logoImage->translate(0, 0.7);
  logoImage->disableDraw();

  pacmanImage = new imageObject();
  pacmanImage->loadTexture("textures/title/pacman.tga");
  pacmanImage->translate(-1.8, 0.0);
  pacmanImage->scale(1.5, 1.0);

  cherryImage = new imageObject();
  cherryImage->loadTexture("textures/title/cherry.tga");
  cherryImage->scale(0.10, 0.10);
  cherryImage->translate(-4, -5);
  cherryImage->disableDraw();

  startText = new textImageObject();
  startText->loadTexture("textures/ascii.tga");
  startText->setText("START");
  startText->translate(-0.3, -0.5);
  startText->scale(0.08, 0.08);
  startText->disableDraw();

  mapText = new textImageObject();
  mapText->loadTexture("textures/ascii.tga");
  mapText->setText("MAP EDITOR");
  mapText->translate(-0.3, -0.65);
  mapText->scale(0.08, 0.08);
  mapText->disableDraw();

  exitText = new textImageObject();
  exitText->loadTexture("textures/ascii.tga");
  exitText->setText("EXIT");
  exitText->translate(-0.3, -0.8);
  exitText->scale(0.08, 0.08);
  exitText->disableDraw();

  instructionsText = new textImageObject();
  instructionsText->loadTexture("textures/ascii.tga");
  instructionsText->setText("Press up/down to choose a map, and enter to load it.");
  instructionsText->translate(-0.8, -0.35);
  instructionsText->scale(0.03, 0.03);
  instructionsText->disableDraw();

  mapSelectText = new textImageObject();
  mapSelectText->loadTexture("textures/ascii.tga");
  mapSelectText->translate(-0.32, -0.5);
  mapSelectText->scale(0.04, 0.04);
  mapSelectText->disableDraw();

  // Set the text for the first map to select from
  strcpy( map, maps.at(0).c_str() );
  mapSelectText->setText(map);
}
//=================================================================
// Destructor
//=================================================================
titleScene::~titleScene()
{
  delete logoImage; 
  delete pacmanImage;
  delete cherryImage;
  delete startText;
  delete mapText;
  delete exitText;
  delete instructionsText;
}
//=================================================================
void titleScene::Logic()
{
  // Slowly slide the image until it reaches about the center of the screen
  if( pacmanImage->getX() < -0.25 )
  {
    pacmanImage->translate(deltaTime*0.0005, 0);
	return;
  }

  logoImage->enableDraw();
  cherryImage->enableDraw();

  if( mapSelection )  // If we are choosing a map to load
  {
	instructionsText->enableDraw();
	mapSelectText->enableDraw();

    startText->disableDraw();
    mapText->disableDraw();
    exitText->disableDraw();
  }
  else  // Display the initial menu
  {
	instructionsText->disableDraw();
	mapSelectText->disableDraw();

    startText->enableDraw();
    mapText->enableDraw();
    exitText->enableDraw();
  }
}
//=================================================================
int titleScene::Draw(float Milliseconds)
{
  deltaTime = Milliseconds;
  Logic();

  /** DRAW EVERYTHING **/

  // Draw the images
  logoImage->draw();
  pacmanImage->draw();
  cherryImage->draw();

  //Draw the texts
  startText->draw();
  mapText->draw();
  exitText->draw();
  instructionsText->draw();
  mapSelectText->draw();

  return changeScene;
}
//=================================================================
void titleScene::KeyInput(int key, int action)
{ 
  // Enter key
  if ( key == GLFW_KEY_ENTER && action == GLFW_PRESS )
  {
	// Push the image to its end point if it's still sliding
    if( pacmanImage->getX() < -0.25 )
	{
	  pacmanImage->loadIdentity();
	  pacmanImage->translate(-0.25, 0);
	  pacmanImage->scale(1.5, 1.0);
	}
	else
	{
	  // If you are selecting a map to load
	  if( mapSelection )
	  {
		// If you chose to return to the main menu
		if( select == maps.size()-1 )
		{
		  mapSelection = false;
		  select = 0;

		  // Set the text for the selected map back to the first one
		  // for the next time the user chooses to select a map
		  if( maps.size() != 1 )  // It's always at least 1 because of the return option
		  {
		    char map[9];
		    strcpy( map, maps.at(0).c_str() );
		    mapSelectText->setText(map);
		  }
		}
		else  // Set the map (that will load in gameScene) and end the current scene
		{
		  SceneMedium->mapID = select;
		  changeScene = GAME;
		}
	  }
	  else
	  {
        // Correspond to the selected menu item
	    switch( select )
	    {
	      case 0: mapSelection = true; break;
		  case 1: changeScene=EDITOR;  break;
		  case 2: exit(0);
	    }
	  }
	}
  }

  // Ignore Up key and Down Key until the image is done sliding
  if ( pacmanImage->getX() <= -0.8 )
    return;

  // Up key
  if ( key == GLFW_KEY_UP && action == GLFW_PRESS )
  {
	if( mapSelection )
	{
	  // Get the previous map name
	  if( select > 0 )
	  {
		char map[9];
		strcpy( map, maps.at(--select).c_str() );
		mapSelectText->setText(map);
	  }
	}
	else
	{
	  // Select the next menu item
	  if( select > 0 )
	  {
	    select--;
	    cherryImage->translate(0, 1.5);
      }
	}
 }

  // Down key
  if ( key == GLFW_KEY_DOWN && action == GLFW_PRESS )
  {
	if( mapSelection )
	{
	  // Get the next map name
	  if( select < maps.size()-1 )
	  {
		char map[23];
		strcpy( map, maps.at(++select).c_str() );
		mapSelectText->setText(map);
	  }
	}
	else
	{
	  // Select the next menu item
	  if( select < NUM_MENU_ITEMS-1 )
	  {
	    select++;
	    cherryImage->translate(0, -1.5);
      }
	}
  }

  // Escape key
  if ( key == GLFW_KEY_ESC && action == GLFW_PRESS )
	exit(0);
}
//=================================================================
void titleScene::MouseInput(int button, int action)
{
}