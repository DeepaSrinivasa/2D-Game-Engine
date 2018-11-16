#include "player.h"

Player::Player( const std::string& name) :
  TwoMultiSprite(name),
  collision(false),
  initialVelocity(getVelocity())
{ }

Player::Player(const Player& s) :
  TwoMultiSprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity())
  { }

Player& Player::operator=(const Player& s) {
  TwoMultiSprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
  //setVelocityX( 0.93*getVelocityX() );
 // setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth() + 20) {
    setVelocityX(initialVelocity[0]);
    TwoMultiSprite::images = TwoMultiSprite::imagesRight;
  }
} 
void Player::left()  { 
  if ( getX() > -30) {
    setVelocityX(-initialVelocity[0]);
    TwoMultiSprite::images = TwoMultiSprite::imagesLeft;
  }
} 
void Player::up()    { 
  if ( getY() > -25) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight() + 5) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  //if ( !collision ) TwoMultiSprite::update(ticks);
 // stop();
    advanceFrame(ticks);
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    stop();
}

