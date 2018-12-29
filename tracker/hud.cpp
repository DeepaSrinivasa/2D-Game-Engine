#include <iostream>
#include <string>
#include <sstream>
#include <SDL.h>
#include "hud.h"
#include "clock.h"
#include "bulletPool.h"


Hud& Hud::getInstance() {
  static Hud instance;
  return instance;
}
Hud::Hud() :
  io( IoMod::getInstance() ),
  gdata( Gamedata::getInstance() ) {
}

Hud::~Hud(){}

void Hud::draw(SDL_Renderer * const renderer){
  SDL_Rect box;
  box.x = gdata.getXmlInt("hud/posX");
  box.y = gdata.getXmlInt("hud/posY");
  box.w = gdata.getXmlInt("hud/width");
  box.h = gdata.getXmlInt("hud/height");
  
  SDL_Color background;
  background.r = gdata.getXmlInt("hud/hudColor/red");
  background.g = gdata.getXmlInt("hud/hudColor/green");
  background.b = gdata.getXmlInt("hud/hudColor/blue");
  background.a = gdata.getXmlInt("hud/hudColor/alpha");
  SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
  SDL_RenderFillRect(renderer, &box);
  SDL_Color textColor;
  textColor.r = gdata.getXmlInt("hud/textColor/red");
  textColor.g = gdata.getXmlInt("hud/textColor/green");
  textColor.b = gdata.getXmlInt("hud/textColor/blue");
  textColor.a = gdata.getXmlInt("hud/textColor/alpha");
  std::ostringstream strm ;
  strm << "Fps:"<< Clock::getInstance().getFps()<<std::endl;
  IoMod::getInstance().writeText(strm.str(),box.x+=15,box.y+=15,textColor);
  io.writeText(gdata.getXmlStr("hud/text").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/up").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/left").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/right").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/down").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/collision").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/hudshow").c_str(), box.x, box.y+=30, textColor);
  io.writeText(gdata.getXmlStr("hud/info").c_str(), box.x, box.y+=30, textColor);
}


void Hud::drawPool(SDL_Renderer * const renderer, ShootingSprite* const player){
  SDL_Rect box;
  box.x = gdata.getXmlInt("hud/Pool/posX");
  box.y = gdata.getXmlInt("hud/Pool/posY");
  box.w = gdata.getXmlInt("hud/Pool/width");
  box.h = gdata.getXmlInt("hud/Pool/height");
  
  SDL_Rect box1;
  box1.x = gdata.getXmlInt("hud/Pool1/posX");
  box1.y = gdata.getXmlInt("hud/Pool1/posY");
  box1.w = gdata.getXmlInt("hud/Pool1/width");
  box1.h = gdata.getXmlInt("hud/Pool1/height");
 
  SDL_Color background;
  background.r = gdata.getXmlInt("hud/hudColor/red");
  background.g = gdata.getXmlInt("hud/hudColor/green");
  background.b = gdata.getXmlInt("hud/hudColor/blue");
  background.a = gdata.getXmlInt("hud/hudColor/alpha");
  SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
  SDL_RenderFillRect(renderer, &box);
   SDL_RenderFillRect(renderer, &box1);
  
  std::stringstream stream;
  stream << "Active bullets: " << player->bulletCount();
  io.writeText(stream.str(), box.x+=5, box.y+=5, {0,250,0,120});
  stream.clear();
  stream.str("");
  stream << "Bullet pool: " << player->freeCount();
  io.writeText(stream.str(),box1.x+=5,box1.y+=5, {0,250,0,120});
  
}

void Hud::drawwin(SDL_Renderer * const renderer,const int score){
    SDL_Rect box;
    box.x = gdata.getXmlInt("score/posX");
    box.y = gdata.getXmlInt("score/posY");
    box.w = gdata.getXmlInt("score/width");
    box.h = gdata.getXmlInt("score/height");

   SDL_Color background;
   background.r = gdata.getXmlInt("score/scorecolor/red");
   background.g = gdata.getXmlInt("score/scorecolor/green");
   background.b = gdata.getXmlInt("score/scorecolor/blue");
   background.a = gdata.getXmlInt("score/scorecolor/alpha");
 

    SDL_Color textColor;
    textColor.r = gdata.getXmlInt("score/scoretextColor/red");
    textColor.g = gdata.getXmlInt("score/scoretextColor/green");
    textColor.b = gdata.getXmlInt("score/scoretextColor/blue");
    textColor.a = gdata.getXmlInt("score/scoretextColor/alpha");

    std::stringstream str1;
    str1 << "You Won!";
    std::stringstream str2;
    str2 << "Game Over";
    std::stringstream str3;
    str3 << "Press R to Restart";
    std::stringstream str4;
    str4 << "Score:"<<score<<std::endl;
    
   SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
   SDL_RenderFillRect(renderer, &box);
   
    io.writeText(str1.str(),box.x+=15, box.y+=5,textColor);
    io.writeText(str4.str(),box.x,box.y+=30, textColor);
    io.writeText(str2.str(),box.x, box.y+=30,textColor);
    io.writeText(str3.str(),box.x, box.y+=30,textColor);
}


void Hud::drawlost(SDL_Renderer * const renderer, const int score){
    SDL_Rect box;
    box.x = gdata.getXmlInt("score/posX");
    box.y = gdata.getXmlInt("score/posY");
    box.w = gdata.getXmlInt("score/width");
    box.h = gdata.getXmlInt("score/height");

   SDL_Color background;
   background.r = gdata.getXmlInt("score/scorecolor/red");
   background.g = gdata.getXmlInt("score/scorecolor/green");
   background.b = gdata.getXmlInt("score/scorecolor/blue");
   background.a = gdata.getXmlInt("score/scorecolor/alpha");
 

    SDL_Color textColor;
    textColor.r = gdata.getXmlInt("score/scoretextColor/red");
    textColor.g = gdata.getXmlInt("score/scoretextColor/green");
    textColor.b = gdata.getXmlInt("score/scoretextColor/blue");
    textColor.a = gdata.getXmlInt("score/scoretextColor/alpha");
    
    
    std::stringstream str1;
    str1 << "You lost!";
    std::stringstream str2;
    str2 << "Game Over";
    std::stringstream str3;
    str3 << "Press R to Restart";
    std::stringstream str4;
    str4 << "Score:"<<score<<std::endl;
    
   
    SDL_SetRenderDrawColor( renderer, background.r, background.g, background.b, background.a );
    SDL_RenderFillRect( renderer, &box );
    
    
    io.writeText(str1.str(),box.x+=15, box.y+=15,textColor);
    io.writeText(str4.str(),box.x,box.y+=30, textColor);
    io.writeText(str2.str(),box.x, box.y+=30,textColor);
    io.writeText(str3.str(),box.x, box.y+=30,textColor);
    

}

