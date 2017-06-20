//
//  tetrisHeader.h
//  newTetris
//
//  Created by lishaowen on 18/06/2017.
//  Copyright © 2017 lishaowen. All rights reserved.
//

#ifndef tetrisHeader_h
#define tetrisHeader_h

#include <stdio.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <dirent.h>
#include <vector>

using std::vector;
using std::string;
extern const char pieces[7][4][5][5];
extern const int init[7][4][2];
extern const SDL_Color colors[8];
extern const int boundary[7][4][3];
extern  const unsigned WIN_WIDTH;
extern  const unsigned WIN_HEIGHT;
extern const int INTERVAL;
extern const int XSIZE;
extern const int YSIZE;
extern SDL_Window* sw;
extern SDL_Renderer* sr;
extern TTF_Font* font;
extern SDL_Texture* textTexture;
extern SDL_Event e;
extern Mix_Music* music;


bool init_window(Uint32 sys,const char*  title,const char*,vector<const char*>&);

void drawGradle(Uint8 r, Uint8 g, Uint8 b, Uint8 a=0xFF);


class Game;

class block{
    friend class Game;
    block(int s=0,int color=7):state(s),color_index(color){};
    block& operator=(const block& b){state=b.state;color_index=b.color_index;return *this;};
    int state;
    int color_index;
};


class Game {
    
public:
    Game()=default;
    bool isPossibleMovement (int kind,int rot,int xboard,int yboard) const;
    //if return false, need to generate a new piece
    bool next(int kind,int rot,int xboard,int yboard);
    bool gameover();
   
private:
    enum {EMPTY=0, FILLED=1,PROJ=2};
    block board[22][10];
    int score=0;
    void drawBoard() const;
    void drawPiece(int kind,int rot,int xboard,int yboard) const;
    bool isLine(const int yboard) const;
    bool isPossibleDeletion() const;
    void deleteLine();
    void setFilled(int kind,int rot,int xboard,int yboard);
};


//A  inclusive,B exclusive
inline int getRand(int A,int B){
    return std::rand()%(B-A)+A;
};


void Quit();
#endif /* tetrisHeader_h */

