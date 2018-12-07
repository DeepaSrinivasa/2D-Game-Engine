#ifndef ENEMYSPRITE_H
#define ENEMYSPRITE_H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class EnemySprite : public Drawable {
public:
  EnemySprite(const std::string&);
  EnemySprite(const EnemySprite&);
  ~EnemySprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  virtual void explode();
  bool deleteenemy() const { return deletesprite; }
  
protected:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  bool deletesprite;
  void advanceFrame(Uint32 ticks);
  EnemySprite& operator=(const EnemySprite&);
  Vector2f makeVelocity(int, int) const;
};
#endif
