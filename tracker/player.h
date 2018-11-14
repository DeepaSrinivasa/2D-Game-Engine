#ifndef PLAYER__H
#define PLAYER__H

#include "twoMultisprite.h"

// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public TwoMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();
private:
  bool collision;
  Vector2f initialVelocity;
};
#endif
