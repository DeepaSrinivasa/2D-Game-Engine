#include <vector>
#include <SDL.h>
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "smartSprite.h"
#include "sound.h"
#include "lights.h"



class ShootingSprite;
class CollisionStrategy;
//class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  // void switchSprite();

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
  ShootingSprite* player;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;
  Viewport& viewport;

  int currentSprite;

  bool makeVideo;
  bool flag;
  
  bool shootingLeft;
  SDLSound sound;
  int godmodeFlag;
  bool godmode;
  bool restart;
  Lights lights;
  int score;
  
  void draw() const;
  void drawhud() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
  
};

