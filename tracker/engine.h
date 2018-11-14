#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "smartSprite.h"

class CollisionStrategy;
class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World layer1;
  World layer2;
  World layer3;
  World layer4;
  World layer5;
  World layer6;
  World layer7;
  World layer8;
  World layer9;
  std::vector<SmartSprite*> bats;
  std::vector <Drawable*> sprites;
  SubjectSprite* player;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;
  Viewport& viewport;

  //Drawable* bird;
  //Drawable* fireman;
  int currentSprite;

  bool makeVideo;
  //Hud& hud;
  bool showhud;
  
  void draw() const;
  void drawhud() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
  
};

