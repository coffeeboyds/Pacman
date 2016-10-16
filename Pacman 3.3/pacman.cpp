#include "pacman.h"

//=================================================================
// Constructor
//=================================================================
pacman::pacman()
{
  opening = true;
  mouthRotation = 0;
  size = 1;
  xPos = 0;
  yPos = 0;
  zPos = 0;
  angle = 0;

  top = new hemisphereObject();
  top->setProgramID( shaderManager::GetInstance()->getMeshColorProgramID() );

  bottom = new hemisphereObject();
  bottom->setProgramID( shaderManager::GetInstance()->getMeshTextureProgramID() );
  bottom->loadTexture("textures/game/mouth.bmp");
}
//=================================================================
float pacman::getX()
{
  return xPos;
}
//=================================================================
float pacman::getY()
{
  return yPos;
}
//=================================================================
float pacman::getZ()
{
  return zPos;
}
//=================================================================
void pacman::loadIdentity()
{
  top->   loadIdentity();
  bottom->loadIdentity();

  xPos = 0;
  yPos = 0;
  zPos = 0;
}
//=================================================================
void pacman::translate(float x, float y, float z)
{
  xPos += x;
  yPos += y;
  zPos += z;
}
//=================================================================
void pacman::setAngle(float degrees)
{
  angle = degrees;
}
//=================================================================
void pacman::setSize(float factor)
{
  size = factor;
}
//=================================================================
void pacman::draw(float Milliseconds)
{
  if(opening)
  {
    mouthRotation += (Milliseconds/1000) * ( 360 / (2*PI) * 4.0f );
    if(mouthRotation > 45.0f)
      opening = false;
  }
  else
  {
    mouthRotation -= (Milliseconds/1000) * ( 360 / (2*PI) * 4.0f );
    if(mouthRotation < 0.0f)
    {
      mouthRotation = 0;
      opening = true;
    }
  }

  top->loadIdentity();
  top->translate(xPos, yPos, zPos);
  top->rotate( angle, glm::vec3(0,1,0) );
  top->rotate( mouthRotation, glm::vec3(1,0,0) );
  top->scale(size, size, size);
  top->draw();

  bottom->loadIdentity();
  bottom->translate(xPos, yPos, zPos);
  bottom->rotate( angle, glm::vec3(0,1,0) );
  bottom->rotate( mouthRotation, glm::vec3(1,0,0) );
  bottom->scale(size, -size, size);
  bottom->draw();
}