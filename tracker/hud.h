#include <string>
#include "gameData.h"
#include "ioMod.h"
#include "shootingSprite.h"
#include <SDL.h>

class Hud{
public:
  static Hud& getInstance();
  void draw(SDL_Renderer * const);
  void drawPool(SDL_Renderer * const, ShootingSprite* const);
  void drawwin(SDL_Renderer * const, const int );
  void drawlost(SDL_Renderer * const,const int);
  ~Hud();
private:
  const IoMod& io;
  const Gamedata& gdata;
  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};

