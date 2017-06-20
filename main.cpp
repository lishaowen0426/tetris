//
//  main.cpp
//  newTetris
//
//  Created by lishaowen on 18/06/2017.
//  Copyright © 2017 lishaowen. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "tetrisHeader.h"
#include <climits>
#include <SDL2_mixer/SDL_mixer.h>
#include <dirent.h>

using std::string;
using std::vector;
const unsigned WIN_WIDTH=300;
const unsigned WIN_HEIGHT=528;
const int INTERVAL=24;
const int XSIZE=10; //XSIZE=10
const int YSIZE=22;//YSIZE=22
SDL_Window* sw=nullptr;
SDL_Renderer* sr=nullptr;
TTF_Font* font=nullptr;
SDL_Texture* textTexture=nullptr;
SDL_Event e;
Mix_Music* music=nullptr;


int main(int argc, const char * argv[]) {
    
    vector<const char*> names;
    
    init_window(SDL_INIT_VIDEO|SDL_INIT_AUDIO, "Tetris","/Users/lishaowen/Music/网易云音乐",names);
    int time_stub=INT_MIN;
    Game game;
    bool quit=false;
    int kind=getRand(0, 7);
    int rot=getRand(0, 4);
    int xboard=init[kind][rot][0];
    int yboard=init[kind][rot][1]-1;
    bool pauseflag=false;
    auto ite=names.cbegin();
    ++ite;
    ++ite;
    ++ite;
    string s="/Users/lishaowen/Music/网易云音乐/"+string(*ite);
    music=Mix_LoadMUS(s.c_str());
    if(music==nullptr) std::cout<<Mix_GetError();
    
    Mix_PlayMusic(music, -1);
    //game.start(kind, rot, xboard, yboard);
    //SDL_RenderPresent(sr);
    
    
    while(!quit){
        while(SDL_PollEvent(&e)!=0){
            if(e.type==SDL_QUIT) quit=true;
            
            if(e.type==SDL_KEYDOWN){
                SDL_Keycode keyPressed=e.key.keysym.sym;
                
                if(keyPressed==SDLK_LEFT){
                    if(game.isPossibleMovement(kind, rot, xboard-1, yboard)) {
                        time_stub=INT_MIN;
                        --xboard;
                    }
                }
                else if(keyPressed==SDLK_RIGHT){
                    if(game.isPossibleMovement(kind, rot, xboard+1, yboard)){
                        time_stub=INT_MIN;
                        ++xboard;
                    }
                }
                else if (keyPressed==SDLK_UP){
                    if(game.isPossibleMovement(kind, (rot+1)%4, xboard, yboard)){
                        time_stub=INT_MIN;
                        rot=(rot+1)%4;
                    }
                }
                else if(keyPressed==SDLK_DOWN){
                    while(game.isPossibleMovement(kind, rot, xboard, ++yboard));
                    --yboard;
                    time_stub=INT_MIN;
                }
                else if(keyPressed==SDLK_n){
                    
                    ++ite;
                    
                    if(ite==names.end()){
                        ite=names.begin();
                        ++ite;++ite;++ite;
                    }
                    
                    string s="/Users/lishaowen/Music/网易云音乐/"+string(*ite);
                    music=Mix_LoadMUS(s.c_str());
                    if(music==nullptr) std::cout<<Mix_GetError();
                    
                    Mix_PlayMusic(music, -1);
                }
                else if(keyPressed==SDLK_m){
                    if(!pauseflag){
                        pauseflag=true;
                        Mix_PauseMusic();
                    }
                    else{
                        pauseflag=false;
                        Mix_ResumeMusic();
                    }
                }
            }
            
        }
        
        if(SDL_GetTicks()-time_stub>=500){
            time_stub=SDL_GetTicks();
            
            if(game.next(kind, rot, xboard, yboard+1)){
                ++yboard;
            }
            else{
                kind=getRand(0, 7);
                rot=getRand(0, 4);
                xboard=init[kind][rot][0];
                yboard=init[kind][rot][1]-1;
                time_stub=INT_MIN;
                if(!game.isPossibleMovement(kind, rot, xboard, yboard)){
                    if(game.gameover()){
                        game=Game();
                        kind=getRand(0, 7);
                        rot=getRand(0, 4);
                        xboard=init[kind][rot][0];
                        yboard=init[kind][rot][1]-1;
                        
                    }
                    
                    else{
                        quit=true;
                    }
                }
               
            }
            SDL_RenderPresent(sr);
        }
        
        
    }
    
    Quit();
    return 0;
}
