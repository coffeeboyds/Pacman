#include <vector>

#ifndef SCENEMEDIUM_H
#define SCENEMEDIUM_H

/** This class serves as a medium between scenes so they can share data **/

class sceneMedium
{

public:

  //=================================================================
  // Singleton Getter
  //=================================================================
  static sceneMedium* GetInstance()
  {
    if(sceneMedium_Instance == nullptr)
    {
      sceneMedium_Instance = new sceneMedium();
    }

    return sceneMedium_Instance;
  }
  
  int mapID; // titleScene lets user select a map to load in gameScene

private:

  //=================================================================
  // Constructor
  //=================================================================
  sceneMedium();

  // Don't forget to declare these two. You want to make sure they
  // are unaccessible otherwise you may accidentally get copies of
  // this singleton appearing.
  sceneMedium(sceneMedium const&);  // Don't Implement
  void operator=(sceneMedium const&); // Don't implement

  //singleton
  static sceneMedium* sceneMedium_Instance;

};

#endif //SCENEMEDIUM_H