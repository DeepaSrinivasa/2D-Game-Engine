#ifndef SHOOTINGSPRITE_H
#define SHOOTINGSPRITE_H

#include <string>
#include <iostream>
#include <list>
#include "sprite.h"
#include "bullet.h"
#include "bulletPool.h"
#include "subjectSprite.h"

class ShootingSprite : public Player {
public:
  ShootingSprite(const std::string& n);
  ShootingSprite(const ShootingSprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  void shoot();
  void shootLeft();
  virtual bool collidedWith(const Drawable*) const;

private:
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  ShootingSprite& operator=(const ShootingSprite&)=delete;
};
#endif
