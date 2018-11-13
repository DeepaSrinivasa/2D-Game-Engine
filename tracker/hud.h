#include <string>
#include "gameData.h"
#include "ioMod.h"
#include <SDL.h>

class Hud{
public:
  static Hud& getInstance();
  void draw(SDL_Renderer * const);
  ~Hud();
private:
  const IoMod& io;
  const Gamedata& gdata;
  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};

