#include "shootingSprite.h"
#include "gameData.h"
#include "imageFactory.h"


ShootingSprite::ShootingSprite(const std::string& name) :
  Player(name),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(bulletName),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0)
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  Player(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame)
{ }

/*unsigned int ShootingSprite::getBulletList(){
  return bullets.size();
}

unsigned int ShootingSprite::getFreeList(){
  return freelist.size();
}*/

void ShootingSprite::shoot() { 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth() - 50;
  float deltaY = getScaledHeight()/4;
  // I need to add some minSpeed to velocity:
  Bullet bullet(bulletName);
  bullets.shoot( getPosition() +Vector2f(deltaX, deltaY) , getVelocity() + Vector2f(minSpeed, 0));
  timeSinceLastFrame = 0;
}

void ShootingSprite::shootLeft() { 
 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth()-150;
  float deltaY = getScaledHeight()/2;
  // I need to add some minSpeed to velocity:
  
  Bullet bullet(bulletName);
  bullets.shoot( getPosition() +Vector2f(deltaX, deltaY) , getVelocity() - Vector2f(minSpeed, 0));
  timeSinceLastFrame = 0;
}

bool ShootingSprite::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void ShootingSprite::draw() const { 
  Player::draw();
  bullets.draw();
 // for ( const Bullet& bullet : bullets ) {
    //bullet.draw();
  //}
}

void ShootingSprite::update(Uint32 ticks) { 
  timeSinceLastFrame += ticks;
  Player::update(ticks);
  bullets.update(ticks);
  
}

