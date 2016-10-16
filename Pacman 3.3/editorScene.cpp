#include "editorScene.h"

const int MAX_MAP_NAME_LENGTH = 8;

//=================================================================
// Constructor
//=================================================================
editorScene::editorScene()
{
  Toolbar = new toolbar();
  Grid = new grid();
  rowsScroll = new scroll(0.65, -0.9);
  colsScroll = new scroll(0.80, -0.9);

  // Prepare the grid for editing
  Grid->buildEditor();

  rowsText = new textImageObject();
  rowsText->loadTexture("textures/ascii.tga");
  rowsText->setText("rows");
  rowsText->translate(0.64, -0.80);
  rowsText->scale(0.03, 0.03);

  colsText = new textImageObject();
  colsText->loadTexture("textures/ascii.tga");
  colsText->setText("cols");
  colsText->translate(0.79, -0.80);
  colsText->scale(0.03, 0.03);

  ghostObject = new meshObject();
  ghostObject->loadTexture("textures/game/demon.tga");
  ghostObject->loadPLY("models/demon.ply");

  inputName = new char[5];
  nameLength = 0;

  enterNameText = new textImageObject();
  enterNameText->loadTexture("textures/ascii.tga");
  enterNameText->setText("Enter map name, then press enter to save.");
  enterNameText->translate(-0.5, 0.5);
  enterNameText->scale(0.03, 0.03);

  savedText = new textImageObject();
  savedText->loadTexture("textures/ascii.tga");
  savedText->setText("Save successful!");
  savedText->translate(-0.5, -0.5);
  savedText->scale(0.03, 0.03);

  mapName = new textImageObject();
  mapName->loadTexture("textures/ascii.tga");
  mapName->translate(-0.4, 0.1);
  mapName->scale(0.08, 0.08);
}
//=================================================================
// Destructor
//=================================================================
editorScene::~editorScene()
{
  delete Toolbar;
  delete Grid;
  delete rowsScroll;
  delete colsScroll;
  delete rowsText;
  delete colsText;
  delete ghostObject;
  delete Pacman;
  delete inputName;
  delete enterNameText;
  delete savedText;
  delete mapName;

  for( int i=0; i<ghosts.size(); i++ )
    delete ghosts.at(i);
}
//=================================================================
int editorScene::Draw(float Milliseconds)
{
  deltaTime = Milliseconds;

  /** DRAW EVERYTHING **/

  // If you are viewing the map in 3D...
  if( Toolbar->getSelection() == WORLD )
  {
    Grid->draw3D();
	Pacman->draw(Milliseconds);

    // Collect their model matrices of the ghosts...
    std::vector<glm::mat4> ghostModelMatrices;
    for( int i=0; i<ghosts.size(); i++)
	  ghostModelMatrices.push_back( ghosts.at(i)->getModelMatrix() );

    // ...then draw them
    ghostObject->draw( ghostModelMatrices );
  }
  // or if you are in the save state...
  else if( Toolbar->getSelection() == SAVE )
  {
    Toolbar->draw();
	enterNameText->draw();
    mapName->draw();
	savedText->draw();
  }
  // or if you are editing the map
  else
  {
    Toolbar->draw();
    Grid->draw2D();
	rowsText->draw();
	rowsScroll->draw();
	colsText->draw();
	colsScroll->draw();
  }

  return changeScene;
}
//=================================================================
void editorScene::KeyInput(int key, int action)
{
  // If you are viewing the map in the 3D world and you press enter, go back to map editor
  if( Toolbar->getSelection() == WORLD && glfwGetKey( GLFW_KEY_ENTER ) == GLFW_PRESS )
  {
    Toolbar->setSelection(WALL);
	return;
  }

  // If you are in the save state and you pressed a key...
  if( Toolbar->getSelection() == SAVE && action == GLFW_PRESS )
  {
	// If it's a backspace, erase a character...
	if( glfwGetKey( GLFW_KEY_BACKSPACE ) == GLFW_PRESS && nameLength > 0 )
	{
	  inputName[nameLength-1] = '\0';
	  nameLength--;
	  mapName->setText(inputName);
	  savedText->disableDraw();
	}
	// or if you hit the enter key, save the map...
	else if( glfwGetKey( GLFW_KEY_ENTER ) == GLFW_PRESS && nameLength > 0 )
	{
	  if( saveMap(inputName) )
	    savedText->enableDraw();
	}
	// or if it's another valid key, print the character
	else if( nameLength < MAX_MAP_NAME_LENGTH && validKey(key) )
	{
      inputName[nameLength++] = key;
	  inputName[nameLength] = '\0';
	  mapName->setText(inputName);
	  savedText->disableDraw();
	}
  }

  // Escape key
  if ( key == GLFW_KEY_ESC && action == GLFW_PRESS )
    changeScene = TITLE;
}
//=================================================================
void editorScene::MouseInput(int button, int action)
{
  if( Toolbar->getSelection() != WORLD && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
  {
    int xScreenSpace, yScreenSpace;
	float xClipSpace, yClipSpace;

    glfwGetMousePos(&xScreenSpace, &yScreenSpace);  // Get the mouse coordinates

	/** Map [0...1024][0...768] to [-1...1][-1...1] **/

	xScreenSpace -= (float)WIDTH/2;  // [0...1024] -> [-512...512]
	yScreenSpace -= (float)HEIGHT/2;

	xClipSpace = (float)xScreenSpace / (float)(WIDTH/2);  // [-512...512] -> [-1...1]
	yClipSpace = (float)yScreenSpace / (float)(HEIGHT/2);

	// Check if the scroll or the toolbar was clicked
	rowsScroll->checkClick(xClipSpace, -yClipSpace);
	colsScroll->checkClick(xClipSpace, -yClipSpace);
	Toolbar->checkClick(xClipSpace, -yClipSpace);

	// Update the number of rows/cols in case the scrolls were clicked
	Grid->setRows(rowsScroll->getValue());
	Grid->setCols(colsScroll->getValue());

	// Perform the action according to the selected button
	switch( Toolbar->getSelection() )
	{
	  case WALL:   Grid->placeWall(xClipSpace, -yClipSpace);  break;
	  case LIGHT:  Grid->placeLight(xClipSpace, -yClipSpace); break;
	  case GHOST:  Grid->placeGhost(xClipSpace, -yClipSpace); break;
	  case PACMAN: Grid->placePacman(xClipSpace, -yClipSpace); break;
	  case WORLD:  buildArena(); break;
	  case SAVE:   savedText->disableDraw(); break;
      case EXIT:   changeScene=TITLE; break;
	}
  } 
}
//=================================================================
void editorScene::buildArena()
{
  // Get the model matrices ready for drawing
  Grid->collectModelMatrices();

  // Get the position of the ghosts
  Grid->getGhostPositions(ghosts);
  
  // Set pacman at the starting point
  Pacman = new pacman();
  Pacman->translate(Grid->getStartingX(), 0, Grid->getStartingY());
  Pacman->setSize(Grid->getCellSize3D()/4);

  /** Get a nice view of the grid to see in 3D **/

  float cellSize = Grid->getCellSize3D();
  float length   = Grid->getRows()*cellSize;
  float width    = Grid->getCols()*cellSize;

  float size = (length > width)? length:width;  // How far back to go to see the entire grid (both length and width of it)
  float distanceFactor = 0.8; // Increase to make the camera further away from the grid

  MatrixManager->lookAt( glm::vec3( -width/2, size*distanceFactor, -size*distanceFactor ),  // Camera is here
	                     glm::vec3( -width/2, 0, length/2 ),                                // and looks here
						 glm::vec3( 0, 1, 0 )                                               // Head is up (set to 0,-1,0 to look upside-down)
					   );

  // Give the position of the lights to the shader program to use
  // MAKES SURE YOU SET THE VIEW MATRIX FIRST BEFORE CALLING THIS METHOD!!!
  MatrixManager->bindLights( Grid->getLights() );
}
//=================================================================
bool editorScene::validKey(int key)
{
  /** Check if 'key' is an acceptable character in a file name to save the map as **/

  if( (key >= 48 && key <= 57)  ||  // If it's a number...
	  (key >= 65 && key <= 90)  ||  // or a upper-case latter...
	  (key >= 97 && key <= 122) )   // or a lower-case letter
	    return true;
  else
	    return false;
}
//=================================================================
bool editorScene::saveMap(char* mapName)
{
  char buffer[18];
  std::ifstream inputFile;
  std::ofstream outputFile;

  /** WRITE THE NAME OF THE MAP TO THE LIST OF MAP NAMES **/

  // Check if the map name already exists
  bool found = false;
  inputFile.open("maps/mapNames.txt");
  while( inputFile.getline(buffer, 9) )
    if( strcmp(buffer, mapName) == 0 )
	{
	  found = true;
	  break;
	}

  inputFile.close();

  // Re-open the file and append the map name
  if( !found )
  {
    outputFile.open("maps/mapNames.txt", std::ios::app);
    outputFile << mapName << '\n';
    outputFile.close();
  }

  /** WRITE THE MAP DATA **/

  // Prepare the file for writing
  strcpy(buffer, "maps/");
  strcat(buffer, mapName);
  strcat(buffer, ".txt");
  outputFile.open(buffer);

  // Write the number of rows and columns
  int rows = Grid->getRows();
  int cols = Grid->getCols();
  outputFile << rows << '\n';
  outputFile << cols << '\n';

  // Write the starting cell
  outputFile << Grid->getStartingCell() << '\n';

  // Write the data of each cell
  for(int i=0; i<rows; i++)
	for(int j=0; j<cols; j++)
	  outputFile << Grid->getCellData(i, j) << '\n';
  
  outputFile.close();

  return true;
}