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


Engine::~Engine() { 
  delete bird;
  delete fireman;
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
  viewport( Viewport::getInstance() ),
  bird(new TwoMultiSprite("bird")),
  fireman(new TwoMultiSprite("fireman")),
  currentSprite(0),
  makeVideo( false ),
  //hud( Hud::getInstance() ),
  showhud( false )
{
  Viewport::getInstance().setObjectToTrack(fireman);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  layer1.draw();
  layer2.draw();
  layer3.draw();
  layer4.draw();
  layer5.draw();
  layer6.draw();
  layer7.draw();
  layer8.draw();
  layer9.draw();
  bird->draw();
  fireman->draw();
  drawhud();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::drawhud() const {
  if (showhud || clock.getSeconds() <= 3){
  	Hud::getInstance().draw(renderer);
  }
}

void Engine::update(Uint32 ticks) {
  bird->update(ticks);
  fireman->update(ticks);
  layer1.update();
  layer2.update();
  layer3.update();
  layer4.update();
  layer5.update();
  layer6.update();
  layer7.update();
  layer8.update();
  layer9.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(bird);
    //std::cout<<"deleting all field with cout"<<std::endl;
  }
  else {
    Viewport::getInstance().setObjectToTrack(fireman);
   // std::cout<<"star field removed"<<std::endl;
  }
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
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
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
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
