#include "twoMultisprite.h"
#include "gameData.h"
#include "renderContext.h"
#include "explodingSprite.h"

TwoMultiSprite::~TwoMultiSprite( ) { if (explosion) delete explosion; }

void TwoMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwoMultiSprite::TwoMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight( RenderContext::getInstance()->getImages(name)),
  imagesLeft( RenderContext::getInstance()->getImages(name+"Left")),
  images( imagesRight),
  
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  deletesprite(false)
 { }

TwoMultiSprite::TwoMultiSprite(const TwoMultiSprite& s) :
  Drawable(s), 
  imagesRight(s.imagesRight),
  imagesLeft(s.imagesLeft),
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  deletesprite(s.deletesprite)
  { }

TwoMultiSprite& TwoMultiSprite::operator=(const TwoMultiSprite& s) {
  Drawable::operator=(s);
  imagesRight = (s.imagesRight);
  imagesLeft= (s.imagesLeft);
  images = (s.images);
  explosion = (s.explosion);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  deletesprite = ( s.deletesprite );
  return *this;
}

void TwoMultiSprite::explode() {
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void TwoMultiSprite::draw() const { 
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwoMultiSprite::update(Uint32 ticks) { 
  if ( getX() < 0) {
    images = imagesRight;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    images = imagesLeft;
  }  
 
  if ( explosion && explosion->chunkCount() > 0) {
    explosion->update(ticks);
    return;
  }
    else if ( explosion && explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
      deletesprite = true;
      return;
    }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
   
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
   
  }  

}
