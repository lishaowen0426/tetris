//
//  tetrisImp.cpp
//  newTetris
//
//  Created by lishaowen on 18/06/2017.
//  Copyright © 2017 lishaowen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "tetrisHeader.h"
#include <SDL2_mixer/SDL_mixer.h>

bool init_window(Uint32 sys,const char*  title,const char* musicdir,vector<const char *>& musicName){
    
    //initialization flag
    bool flag=true;
    
    //initialize SDL
    if(SDL_Init(sys)<0){
        printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError());
        flag=false;
    }
    else{
        //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);
        sw=SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
        
        if(sw==nullptr) {
            printf("Window could not be created ! SDL_Error: %s\n",SDL_GetError());
            flag=false;
        }
        else {
            
            sr=SDL_CreateRenderer(sw, -1, SDL_RENDERER_ACCELERATED);
            if(sr==nullptr){
                printf("Could not get the renderer of the window! SDL_Error: %s\n",SDL_GetError());
                flag=false;
            }
            SDL_SetRenderDrawBlendMode(sr, SDL_BLENDMODE_BLEND);
            
            
        }
        
    }
    
    if(TTF_Init()==-1){
        printf("SDL_TTF could not initialize! TTF Error: %s\n",TTF_GetError());
        flag=false;
    }
    else{
        font=TTF_OpenFont("/Users/lishaowen/Desktop/newTetris/newTetris/Xcelsion.ttf", 30);
        if(font==nullptr){
            printf("Could not load font file: %s\n",TTF_GetError());
            flag=false;
        }
        else{
            textTexture=SDL_CreateTextureFromSurface(sr, TTF_RenderText_Solid(font, "Game Over!!", {255,255,255}));
            
        }
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)<0){
        printf("Could not initialize music! SDL_MIX ERROR: %s \n",Mix_GetError());
    }
    else{
        DIR* dir=opendir(musicdir);
        dirent* dirent=nullptr;
        
        while((dirent=readdir(dir))!=nullptr){
            musicName.push_back(dirent->d_name);
        }
    }

    
    return flag;
};

void drawGradle(Uint8 r=128, Uint8 g=128, Uint8 b=128, Uint8 a){
    
    SDL_SetRenderDrawColor(sr, r, g, b, a);
    
    for(int x=1;x<XSIZE;++x){
        SDL_RenderDrawLine(sr, x*INTERVAL, 0, x*INTERVAL, WIN_HEIGHT);
    }
    
    for(int y=1;y<YSIZE;++y){
        SDL_RenderDrawLine(sr, 0, y*INTERVAL, 240, y*INTERVAL);
    }
    
}

bool Game::isPossibleMovement (int kind,  int rot,int xboard, int yboard) const{
    if(xboard<boundary[kind][rot][0]||xboard>boundary[kind][rot][1]||yboard>boundary[kind][rot][2]) return false;
    
    for(int y=0;y<5;++y){
        for(int x=0;x<5;++x){
            if((xboard+x)>=0&&(yboard+y)>=0){
                if(board[yboard+y][xboard+x].state==FILLED&&pieces[kind][rot][y][x]!=0) return false;
            }
        }
    }
    return true;
}


void Game::drawBoard() const {
    SDL_Rect rect;
    int index;
    for(int y=0;y<22;++y){
        for(int x=0;x<10;++x){
            rect={x*INTERVAL,y*INTERVAL,INTERVAL,INTERVAL};
            index=board[y][x].color_index;
            //Uint8 alpha;
            //(board[y][x].state==PROJ)? alpha=0:alpha=255;
            SDL_SetRenderDrawColor(sr, colors[index].r, colors[index].g, colors[index].b, 255);
            SDL_RenderFillRect(sr, &rect);
        }
    }
    
    rect={240,0,60,528};
    SDL_SetRenderDrawColor(sr, 192, 192, 192, 255);
    SDL_RenderFillRect(sr, &rect);
    
    textTexture=SDL_CreateTextureFromSurface(sr, TTF_RenderText_Solid(font, std::to_string(score).c_str(), {34,139,34}));
    rect={250,280,30,20};
    SDL_RenderCopy(sr, textTexture, NULL, &rect);
    
}


void Game::drawPiece(int kind,int rot,int xboard,int yboard) const {
    SDL_Color color=colors[kind];
    SDL_Rect rect;
    for(int y=0;y<5;++y){
        for(int x=0;x<5;++x){
            if(pieces[kind][rot][y][x]!=0){
                rect={(xboard+x)*INTERVAL,(yboard+y)*INTERVAL,INTERVAL,INTERVAL};
                SDL_SetRenderDrawColor(sr, color.r, color.g, color.b, 255);
                SDL_RenderFillRect(sr, &rect);
            }
        }
    }
    
    int yproj=yboard;
    while(isPossibleMovement(kind, rot, xboard, ++yproj));
    --yproj;
    for(int y=0;y<5;++y){
        for(int x=0;x<5;++x){
            if(pieces[kind][rot][y][x]!=0&&(yproj+y-yboard)>=5){
                rect={(xboard+x)*INTERVAL,(yproj+y)*INTERVAL,INTERVAL,INTERVAL};
                SDL_SetRenderDrawColor(sr, color.r , color.g, color.b, 50);
                SDL_RenderFillRect(sr, &rect);
            }
        }
    }
    
}

bool Game::isLine(const int yboard) const{
    for(int x=0;x<XSIZE;++x){
        if(board[yboard][x].state!=FILLED) return false;
    }
    return true;
}


bool Game::isPossibleDeletion() const {
    bool flag=false;
    for(int y=0;y<YSIZE;++y){
        flag|=isLine(y);
    }
    return flag;
}

void Game::deleteLine() {
    block temp[22][10];
    int ytemp=21, yboard=21;
    for(;yboard>=0;--yboard){
        if(!isLine(yboard)){
            for(int x=0;x<10;++x){
                temp[ytemp][x]=board[yboard][x];
            }
            --ytemp;
        }
        else{
            score+=100;
        }
    }
    
    for(int y=0;y<YSIZE;++y){
        for(int x=0;x<XSIZE;++x){
            board[y][x]=temp[y][x];
        }
    }
    
}

void Game::setFilled(int kind, int rot, int xboard, int yboard){
    for(int y=0;y<5;++y){
        for(int x=0;x<5;++x){
            if(pieces[kind][rot][y][x]!=0){
                board[yboard+y][xboard+x].state=FILLED;
                board[yboard+y][xboard+x].color_index=kind;
            }
        }
    }
    
}

bool Game::next(int kind, int rot, int xboard, int yboard){
    if(isPossibleMovement(kind, rot, xboard, yboard)){
        drawBoard();
        drawPiece(kind, rot, xboard, yboard);
        drawGradle();
        return true;
    }
    else{
        
        setFilled(kind, rot, xboard, yboard-1);
        if(isPossibleDeletion()){
            deleteLine();
            drawBoard();
            drawGradle();
            return false;
        }
        else{
            drawBoard();
            drawGradle();
            return false;
        }
        
    }
}

bool Game::gameover(){
    textTexture=SDL_CreateTextureFromSurface(sr, TTF_RenderText_Solid(font, "Game Over!!", {255,255,255}));    SDL_Rect rect={20,100,200,300};
    SDL_RenderCopy(sr, textTexture, NULL, &rect);
    SDL_RenderPresent(sr);
    SDL_Delay(1500);
    
    textTexture=SDL_CreateTextureFromSurface(sr, TTF_RenderText_Solid(font, "Continue? \n Y/N", {255,255,255}));
    //drawBoard();
    //drawGradle();
    SDL_RenderCopy(sr, textTexture, NULL, &rect);
    SDL_RenderPresent(sr);
    
    bool flag=false;
    bool ret=false;
    while(!flag){
        while(SDL_PollEvent(&e)!=0){
            if(e.type==SDL_QUIT){
                flag=true;
            }
            if(e.type==SDL_KEYDOWN){
                SDL_Keycode keyPressed=e.key.keysym.sym;
                if(keyPressed==SDLK_y){
                    flag=true;
                    ret=true;
                }
                else if(keyPressed==SDLK_n){
                    flag=true;
                }
            }
        }
    }
    return ret;
}

void Quit(){
    SDL_DestroyRenderer(sr);
    sr=nullptr;
    SDL_DestroyWindow(sw);
    sw=nullptr;
    SDL_DestroyTexture(textTexture);
    textTexture=nullptr;
    TTF_CloseFont(font);
    font=nullptr;
    Mix_FreeMusic(music);
    music=nullptr;
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
