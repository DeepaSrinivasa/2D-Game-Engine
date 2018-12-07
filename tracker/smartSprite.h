#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "enemySprite.h"

class SmartSprite : public EnemySprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  virtual ~SmartSprite() { } 

  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  bool exploded() const { return checkExplode; }

private:
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;
  bool checkExplode;
 
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
