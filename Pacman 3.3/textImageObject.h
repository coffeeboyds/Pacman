#include "screenObject.h"

#ifndef TEXTIMAGEOBJECT_H
#define TEXTIMAGEOBJECT_H

class textImageObject : public screenObject
{

public:
  //=================================================================
  // Constructor
  //=================================================================
  textImageObject();

  //=================================================================
  // Set the text to be drawn
  //=================================================================
  void setText(char* text);

private:

  char* text;

  //=================================================================
  // Fill vertices buffer with vertices for each letter
  //=================================================================
  void setVertices();

  //=================================================================
  // Set the UV coordinates around the letters in ascii.tga
  //=================================================================
  void setUVs();

};

#endif //TEXTIMAGEOBJECT_H