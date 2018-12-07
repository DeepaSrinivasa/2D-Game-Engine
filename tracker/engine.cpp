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
#include "shootingSprite.h"
#include "enemySprite.h"


Engine::~Engine() { 
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
  player(new ShootingSprite("fireman")),
  strategies(),
  currentStrategy(),
  collision(false),
  viewport( Viewport::getInstance() ),
  currentSprite(0),
  makeVideo( false ),
  flag( false ),
  shootingLeft(false)
{
  int n = Gamedata::getInstance().getXmlInt("numberOfBats");
  bats.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for(int i = 0; i<n; ++i){
    bats.push_back(new SmartSprite("Bat1", pos, w, h));
    player->attach(bats[i]);
  }
  
  sprites.push_back(new TwoMultiSprite("bird"));
   
  strategies.push_back( new PerPixelCollisionStrategy );
  
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  SDL_Color displayColor;
  displayColor.r = Gamedata::getInstance().getXmlInt("hud/textColor/red");
  displayColor.g = Gamedata::getInstance().getXmlInt("hud/textColor/green");
  displayColor.b = Gamedata::getInstance().getXmlInt("hud/textColor/blue");
  displayColor.a = Gamedata::getInstance().getXmlInt("hud/textColor/alpha");

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
    strm<<bats.size()<<" Bats remaining";
    IoMod::getInstance().writeText(strm.str(), 600, 30,displayColor);
  }
  else{
    IoMod::getInstance().writeText("You have killed all bats", 500, 50,displayColor);
    IoMod::getInstance().writeText(" You are clear to go", 500, 100, displayColor);
  }
  strategies[currentStrategy]->draw();
  if ( collision ) {
    IoMod::getInstance().writeText("Oops: Collision, Player is dead", 200, 50,displayColor);
  }  

  player->draw();
  drawhud();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::drawhud() const {
   if (flag || clock.getSeconds() <= 3){
    Hud::getInstance().draw(renderer);
    Hud::getInstance().drawPool(renderer, player);

  }
}


void Engine::checkForCollisions() {
  collision = false;
  for (  EnemySprite* d : bats ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
    }
  }
  for ( const Drawable* d : sprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
    }
  }
  if ( collision ) {
    player->collided();
  }
  else {
    player->missed();
    collision = false;
  }
  if(strategies[currentStrategy]->execute(*sprites[0], *player)){
    collision = true;
    player->explode();
  }
  auto it = bats.begin();
   while ( it != bats.end() ) {
     if ( strategies[currentStrategy]->execute(*player, **it)) {
	(*it)->explode();
	  (*it)->setPosition(Vector2f((*it)->getPosition()[0], (*it)->getPosition()[1]));	
		++it; 
      }
      else if((player)->collidedWith((*it))){
	(*it)->explode();
	(*it)->setPosition(Vector2f((*it)->getPosition()[0], (*it)->getPosition()[1]));
        	 ++it;
       }
       else ++it;
    }

  it = bats.begin();
  while ( it != bats.end() ) {
    if ( (*it)->deleteenemy() ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = bats.erase(it);
   }
    else ++it;
  }
	
  auto it1 = sprites.begin();
  while ( it1 != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it1) ) {
      it1 = sprites.erase(it1);
    }
    else ++it1;
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
	if ( keystate[SDL_SCANCODE_SPACE] ) {
	  if(shootingLeft==true){
	  	player->shootLeft();
	  }
	  else		
            player->shoot();
	}
        if ( keystate[SDL_SCANCODE_M] ) {
           currentStrategy = (1 + currentStrategy) % strategies.size();
        }
 	if( keystate[SDL_SCANCODE_F1]){
	 if(flag) flag = false;
	  else flag = true;
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
 	shootingLeft = true;
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
	shootingLeft = false;
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
