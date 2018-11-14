#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "twoMultisprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "collisionStrategy.h"


Engine::~Engine() { 
  //delete bird;
  //delete fireman;
  for(auto sprite : bats){
    delete sprite;
  }
  for(auto sprite : sprites){
    delete sprite;
  }
  delete player;
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  layer1("layer1", Gamedata::getInstance().getXmlInt("layer1/factor")),
  layer2("layer2", Gamedata::getInstance().getXmlInt("layer2/factor")),
  layer3("layer3", Gamedata::getInstance().getXmlInt("layer3/factor")),
  layer4("layer4", Gamedata::getInstance().getXmlInt("layer4/factor")),
  layer5("layer5", Gamedata::getInstance().getXmlInt("layer5/factor")),
  layer6("layer6", Gamedata::getInstance().getXmlInt("layer6/factor")),
  layer7("layer7", Gamedata::getInstance().getXmlInt("layer7/factor")),
  layer8("layer8", Gamedata::getInstance().getXmlInt("layer8/factor")),
  layer9("layer9", Gamedata::getInstance().getXmlInt("layer9/factor")),
  bats(),
  sprites(),
  player(new SubjectSprite("fireman")),
  strategies(),
  currentStrategy(),
  collision(false),
  viewport( Viewport::getInstance() ),
  //bird(new TwoMultiSprite("bird")),
  //fireman(new TwoMultiSprite("fireman")),
  currentSprite(0),
  makeVideo( false ),
  //hud( Hud::getInstance() ),
  showhud( false )
{
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  bats.push_back(new SmartSprite("Bat1", pos, w, h));
  bats.push_back(new SmartSprite("Bat2", pos, w, h));
  bats.push_back(new SmartSprite("Bat3", pos, w, h));
  
  for(unsigned int i = 0; i < bats.size(); ++i){
    player->attach(bats[i]);
  }
  
  sprites.push_back(new TwoMultiSprite("bird"));
  sprites.push_back(new TwoMultiSprite("fireman"));
  
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  SDL_Color textColor;
  textColor.r = Gamedata::getInstance().getXmlInt("font/red");
  textColor.g = Gamedata::getInstance().getXmlInt("font/green");
  textColor.b = Gamedata::getInstance().getXmlInt("font/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("font/alpha");

  layer1.draw();
  layer2.draw();
  layer3.draw();
  layer4.draw();
  layer5.draw();
  layer6.draw();
  layer7.draw();
  layer8.draw();
  layer9.draw();

  for(auto sprite:sprites){
    sprite->draw();
  }

  for(auto sprite : bats){
    sprite->draw();
  }

  if(bats.size() > 0){
    std::stringstream strm;
    strm<<bats.size()<<"Bats remaining";
    IoMod::getInstance().writeText(strm.str(), 500, 120, textColor);
  }
  else{
    SDL_Color textColor1;
    textColor1.r = Gamedata::getInstance().getXmlInt("hud/border/Color/red");
    textColor1.g = Gamedata::getInstance().getXmlInt("hud/border/Color/blue");
    textColor1.b = Gamedata::getInstance().getXmlInt("hud/border/Color/green");
    textColor1.a = Gamedata::getInstance().getXmlInt("hud/border/Color/alpha");

    IoMod::getInstance().writeText(" All Bats collected", 500, 120,textColor1);
    IoMod::getInstance().writeText(" you cleared", 500, 150, textColor1);
  }
  strategies[currentStrategy]->draw();
  if ( collision ) {
    IoMod::getInstance().writeText("Oops: Collision", 500, 60);
  }  

  player->draw();
  drawhud();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::drawhud() const {
  if (showhud || clock.getSeconds() <= 3){
  	Hud::getInstance().draw(renderer);
  }
}

void Engine::checkForCollisions() {
  collision = false;
  for ( const Drawable* d : sprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
    }
  }

  auto it = bats.begin();
  while ( it != bats.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = bats.erase(it);
    }
    else ++it;
  }
  if ( collision ) {
    player->collided();
  }
  else {
    player->missed();
    collision = false;
  }
}


void Engine::update(Uint32 ticks) {
  checkForCollisions();
  for(auto sprite : bats){
  	sprite->update(ticks);
  }
  for(auto sprite : sprites){
    sprite->update(ticks);
  }
  
  layer1.update();
  layer2.update();
  layer3.update();
  layer4.update();
  layer5.update();
  layer6.update();
  layer7.update();
  layer8.update();
  layer9.update();
  player->update(ticks);
  viewport.update(); // always update viewport last
}


void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
 // int startTime = SDL_GetTicks();
  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
           currentStrategy = (1 + currentStrategy) % strategies.size();
        }
 	if( keystate[SDL_SCANCODE_F1]){
	  if(showhud) showhud = false;
	  else showhud = true;
        }
	if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
   
    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
