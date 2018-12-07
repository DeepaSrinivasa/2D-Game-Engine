#ifndef PLAYER__H
#define PLAYER__H

#include "twoMultisprite.h"
#include "smartSprite.h"
#include <list>

// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public TwoMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  ~Player();
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();

  enum DIRECTION { LEFT, RIGHT };
  DIRECTION getPlayerDirection() const { return direction;}
  void attach(SmartSprite* o) { observers.push_back(o); } 
  void detach(SmartSprite* o);
  virtual void explode();
  virtual void draw() const;
protected:
  std::list<SmartSprite*> observers;
private:
  bool collision;
  Vector2f initialVelocity;
  DIRECTION direction;
  ExplodingSprite* explosion;
};
#endif
