#include "player.h"
#include "explodingSprite.h"
#include "twoMultisprite.h"

Player::~Player( ) { if (explosion) delete explosion; }

Player::Player( const std::string& name) :
  TwoMultiSprite(name),
  observers(),
  collision(false),
  initialVelocity(getVelocity()),
  direction(RIGHT),
  explosion(nullptr)
{ }

Player::Player(const Player& s) :
  TwoMultiSprite(s), 
  observers(),
  collision(false),
  initialVelocity(s.getVelocity()),
  direction(RIGHT),
  explosion(nullptr)
  { }

Player& Player::operator=(const Player& s) {
  TwoMultiSprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  direction = s.direction;
  return *this;
}

void Player::explode(){
   // std::cout<<"helloo explode"<<std::endl;
        if(!explosion){
            explosion = new ExplodingSprite(Sprite(getName(), getPosition(), getVelocity(), images[currentFrame]));
		
    }
}

void Player::draw() const{
  if(explosion && explosion->chunkCount() != 0){
      explosion->draw();
  }
  else{
      TwoMultiSprite::draw();
  }
}


void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
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
   // direction = LEFT;
  }
} 
void Player::left()  { 
  if ( getX() > -25) {
    setVelocityX(-initialVelocity[0]);
    TwoMultiSprite::images = TwoMultiSprite::imagesLeft;
    //direction = RIGHT;
  }
} 
void Player::up()    { 
  if ( getY() > -0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight() + 5) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
    if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
   }
    //stop();
    if(!collision)
    advanceFrame(ticks);
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
    stop();
    std::list<SmartSprite*>::iterator ptr = observers.begin();
    while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
}
}

