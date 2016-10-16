#include "gameScene.h"

//=================================================================
// Constructor
//=================================================================
gameScene::gameScene()
{
  Grid = new grid();
  TopCam = new topCam();

  ghostObject = new meshObject();
  ghostObject->loadTexture("textures/game/demon.tga");
  ghostObject->loadPLY("models/demon.ply");
  ghostSteps = Grid->getCellSize3D();  // Because it needs to figure out a new direction for them right away
  GHOST_SPEED = Grid->getCellSize3D()/30;
  
  /** BUILD THE ARENA **/

  Grid->loadMap(SceneMedium->mapID);

  // The amount of space within the center of a cell that pacman is allowed to turn
  lowTurnSpace = Grid->getCellSize3D()/2;
  highTurnSpace = Grid->getCellSize3D();

  // Get the position of the ghosts
  Grid->getGhostPositions(ghosts);

  // All ghosts begin stationary
  for( int i=0; i<ghosts.size(); i++ )
    ghostDirections.push_back( STOP );

  // Set pacman at the starting point
  Pacman = new pacman();
  Pacman->translate(Grid->getStartingX(), 0, Grid->getStartingY());
  Pacman->setSize(Grid->getCellSize3D()/4);
  PACMAN_SPEED = Grid->getCellSize3D()/20;    // Must be a dividend of Grid->getCellSize3D()
  dir = STOP;
  nextDir = STOP;
  pacmanSteps = 0;
  
  seeAllCamera();
  thirdPersonCamera = false;
}
//=================================================================
// Destructor
//=================================================================
gameScene::~gameScene()
{
  delete Grid;
  delete ghostObject;
  delete Pacman;

  for( int i=0; i<ghosts.size(); i++ )
    delete ghosts.at(i);
}
//=================================================================
void gameScene::Logic()
{
  pacmanMovement();
  ghostsMovement();
}
//=================================================================
int gameScene::Draw(float Milliseconds)
{
  deltaTime = Milliseconds;
  Logic();

  // Collect all model matrices of the ghosts to draw them later
  std::vector<glm::mat4> ghostModelMatrices;
  for( int i=0; i<ghosts.size(); i++)
	ghostModelMatrices.push_back( ghosts.at(i)->getModelMatrix() );

  if( thirdPersonCamera )
  {
	/** DRAW EVERYTHING FOR THE CAM **/
 
    TopCam->activate();

	seeAllCamera();  // Get a nice view for the camera

    Grid->draw3D();
    Pacman->draw(Milliseconds);

    // Draw the ghosts
    ghostObject->draw( ghostModelMatrices );

	updateThirdPersonCamera(); // Back to third person view
  }

  // DRAW EVERTHING FOR THE SCENE
  TopCam->deactivate();
  Grid->draw3D();
  Pacman->draw(Milliseconds);
  ghostObject->draw( ghostModelMatrices );

  if( thirdPersonCamera )
    TopCam->draw();  // Draw the camera!
  
  return changeScene;
}
//=================================================================
void gameScene::pacmanMovement()
{
  // If Pacman reached the center of the cell he is going to
  if( pacmanSteps >= Grid->getCellSize3D() )
  {
	pacmanSteps = 0;

	/** Place pacman in the exact center of the cell **/
	// Rounding to find the center of the cell...
	float xCenter = (Pacman->getX() < 0.0) ? ceil(Pacman->getX()*2 - 0.5)/2 : floor(Pacman->getX()*2 + 0.5)/2;
	float zCenter = (Pacman->getZ() < 0.0) ? ceil(Pacman->getZ() - 0.5) : floor(Pacman->getZ() + 0.5);
	// Translate pacman the difference to reach the center
	Pacman->translate(xCenter-Pacman->getX(), 0, zCenter-Pacman->getZ());
	
	// If there's no wall in your next direction
	if( !Grid->isWall( (SIDES)nextDir, Pacman->getX(), Pacman->getZ() ) )
	  dir = nextDir;
	else
      nextDir = dir;  // Cancel the direction you were going to next
	
    // If you're running into a wall
    if( Grid->isWall( (SIDES)dir, Pacman->getX(), Pacman->getZ() ) )
    {
	  dir = STOP;
	  nextDir = STOP;
    }
  }

  // Rotate and translate Pacman
  switch( dir )
  {
    case LEFT:  Pacman->setAngle( -90  );
		        Pacman->translate(PACMAN_SPEED, 0, 0);
				if( thirdPersonCamera ) updateThirdPersonCamera();
				break;

	case RIGHT: Pacman->setAngle(  90  );
		        Pacman->translate(-PACMAN_SPEED, 0, 0);
				if( thirdPersonCamera ) updateThirdPersonCamera();
				break;

	case UP:    Pacman->setAngle(  180 );
		        Pacman->translate(0, 0, PACMAN_SPEED);
				if( thirdPersonCamera ) updateThirdPersonCamera();
				break;

	case DOWN:  Pacman->setAngle(  0   );
		        Pacman->translate(0, 0, -PACMAN_SPEED);
				if( thirdPersonCamera ) updateThirdPersonCamera();
				break;
  }

  if( dir != STOP )
    pacmanSteps += PACMAN_SPEED;
}
//=================================================================
void gameScene::ghostsMovement()
{
  // If the ghosts reached the center of the cell they are going to
  if( ghostSteps >= Grid->getCellSize3D() )
  {
	ghostSteps = 0;

	// Place each ghost in the exact center of the cell and give a possibly new direction
	for( int i=0; i<ghosts.size(); i++ )
	{
	  /** Place in the exact center **/
	  // Rounding to find the center of the cell...
	  float xCenter = (ghosts.at(i)->getX() < 0.0) ? ceil(ghosts.at(i)->getX()*2 - 0.5)/2 : floor(ghosts.at(i)->getX()*2 + 0.5)/2;
	  float zCenter = (ghosts.at(i)->getZ() < 0.0) ? ceil(ghosts.at(i)->getZ() - 0.5) : floor(ghosts.at(i)->getZ() + 0.5);
	  // Translate the ghost the difference to reach the center
	  ghosts.at(i)->translate(xCenter-ghosts.at(i)->getX(), 0, zCenter-ghosts.at(i)->getZ());

	  /** Give a possibly new direction to move **/

	  DIRECTION newDirections[4] = {LEFT, RIGHT, UP, DOWN};

	  // Shuffle the directions
      std::random_shuffle ( &newDirections[0], &newDirections[4] );

	  // Put the "backwards" direction at the end so ghosts don't turn around unless they have to
	  for( int j=0; j<3; j++ )
		if( abs(ghostDirections.at(i) - newDirections[j]) == 1 )  // If it's the opposite direction
		{
		  // Swap it at the end
		  DIRECTION temp;
		  temp = newDirections[3];
		  newDirections[3] = newDirections[j];
		  newDirections[j] = temp;
		  break;
		}

	  // Choose a new direction to move
	  for( int j=0; j<4; j++ )
	  {
	    // If there's no wall in this direction
	    if( !Grid->isWall( (SIDES)newDirections[j], ghosts.at(i)->getX(), ghosts.at(i)->getZ() ) )
		{
		  ghostDirections.at(i) = newDirections[j];
		  break;
		}
	  }
	}
  }

  // Rotate and translate each ghost
  for( int i=0; i<ghosts.size(); i++ )
  {
	switch( ghostDirections.at(i) )
    {
      case LEFT:  ghosts.at(i)->setAngle( -90  );
		          ghosts.at(i)->translate(GHOST_SPEED, 0, 0);
			      break;

	  case RIGHT: ghosts.at(i)->setAngle(  90  );
		          ghosts.at(i)->translate(-GHOST_SPEED, 0, 0);
				  break;

	  case UP:    ghosts.at(i)->setAngle(  180 );
		          ghosts.at(i)->translate(0, 0, GHOST_SPEED);
				  break;

	  case DOWN:  ghosts.at(i)->setAngle(  0   );
		          ghosts.at(i)->translate(0, 0, -GHOST_SPEED);
				  break;
    }
  }

  ghostSteps += GHOST_SPEED;
}
//=================================================================
void gameScene::KeyInput(int key, int action)
{ 
  // Camera view
  if ( key == GLFW_KEY_RCTRL && action == GLFW_PRESS )
    if( thirdPersonCamera )
	{
	  thirdPersonCamera = false;
	  seeAllCamera();
	}
	else
	{
	  thirdPersonCamera = true;
	  updateThirdPersonCamera();
	}


  // Left key
  if ( key == GLFW_KEY_LEFT && action == GLFW_PRESS )
  {
	if( (dir == UP || dir == DOWN) && (pacmanSteps >= lowTurnSpace && pacmanSteps <= highTurnSpace) )
      nextDir = LEFT;
	else if (dir == RIGHT || dir == STOP)
	{
	  dir     = LEFT;
	  nextDir = LEFT;
      pacmanSteps = Grid->getCellSize3D() - pacmanSteps;
	}
  }

  // Right key
  if ( key == GLFW_KEY_RIGHT && action == GLFW_PRESS )
  {
	if( (dir == UP || dir == DOWN) && (pacmanSteps >= lowTurnSpace && pacmanSteps <= highTurnSpace) )
      nextDir = RIGHT;
	else if (dir == LEFT || dir == STOP)
	{
	  dir     = RIGHT;
	  nextDir = RIGHT;
      pacmanSteps = Grid->getCellSize3D() - pacmanSteps;
	}
  }

  // Up key
  if ( key == GLFW_KEY_UP && action == GLFW_PRESS )
  {
    if( (dir == LEFT || dir == RIGHT) && (pacmanSteps >= lowTurnSpace && pacmanSteps <= highTurnSpace) )
      nextDir = UP;
	else if (dir == DOWN || dir == STOP)
	{
	  dir     = UP;
	  nextDir = UP;
      pacmanSteps = Grid->getCellSize3D() - pacmanSteps;
	}
  }

  // Down key
  if ( key == GLFW_KEY_DOWN && action == GLFW_PRESS )
  {
    if( (dir == LEFT || dir == RIGHT) && (pacmanSteps >= lowTurnSpace && pacmanSteps <= highTurnSpace) )
      nextDir = DOWN;
	else if (dir == UP || dir == STOP)
	{
	  dir     = DOWN;
	  nextDir = DOWN;
      pacmanSteps = Grid->getCellSize3D() - pacmanSteps;
	}
  }

  // Escape key
  if ( key == GLFW_KEY_ESC && action == GLFW_PRESS )
	changeScene = TITLE;
}
//=================================================================
void gameScene::updateThirdPersonCamera()
{
  thirdPersonCamera = true;

  MatrixManager->lookAt( glm::vec3( Pacman->getX(), Pacman->getY()+Grid->getCellSize3D()*4, Pacman->getZ()-Grid->getCellSize3D() ), // Camera is here
	                     glm::vec3( Pacman->getX(), Pacman->getY(), Pacman->getZ() ),       // and looks here
						 glm::vec3( 0, 1, 0 )                                               // Head is up (set to 0,-1,0 to look upside-down)
					   );

  // Send the light's position in view-space (since the view just changed)
  MatrixManager->bindLights( Grid->getLights() );
}
//=================================================================
void gameScene::seeAllCamera()
{
  /** Get a nice view of the grid to see in 3D **/

  thirdPersonCamera = false;
  float cellSize = Grid->getCellSize3D();
  float length   = Grid->getRows()*cellSize;
  float width    = Grid->getCols()*cellSize;

  float size = (length > width)? length:width;  // How far back to go to see the entire grid (both length and width of it)
  float distanceFactor = 0.8; // Increase to make the camera further away from the grid

  MatrixManager->lookAt( glm::vec3( -width/2, size*distanceFactor, -size*distanceFactor ),  // Camera is here
	                     glm::vec3( -width/2, 0, length/2 ),                                // and looks here
						 glm::vec3( 0, 1, 0 )                                               // Head is up (set to 0,-1,0 to look upside-down)
					   );

  // Give the position of the lights (in view space) to the shader program to use
  // MAKES SURE YOU SET THE VIEW MATRIX FIRST BEFORE CALLING THIS METHOD!!!
  MatrixManager->bindLights( Grid->getLights() );
}
//=================================================================
void gameScene::MouseInput(int button, int action)
{
}