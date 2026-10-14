#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_ttf.h>

#include <cell.h>
#include <board.h>

using namespace std;

const int ROW_COUNT = 9;
const int COL_COUNT = 9;

const int BOARD_X = 70;
const int BOARD_Y = 150;
const int BOARD_SIZE = 530;
const int CELL_SIZE = BOARD_SIZE / ROW_COUNT;

void renderText(SDL_Renderer* renderer, TTF_Font* font, string& text, SDL_Color color, int x, int y){
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderRect = {x,y,textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer,textTexture,NULL,&renderRect);
    SDL_DestroyTexture(textTexture);
}

void drawBoxWithBorder(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color fillColor, SDL_Color borderColor) {
    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &rect);
}

void drawBackground(SDL_Renderer* renderer){
   SDL_RenderClear(renderer);
   SDL_Rect outerBox;
   outerBox.x = 50;
   outerBox.y = 50;
   outerBox.w = 570;
   outerBox.h = 650;
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   SDL_RenderDrawRect(renderer, &outerBox);
   
   SDL_Rect innerBox;
   innerBox.x = 70;
   innerBox.y = 70;
   innerBox.w = 530;
   innerBox.h = 60;
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   SDL_RenderDrawRect(renderer, &innerBox);

    SDL_Rect gameplayBox;
    gameplayBox.x = BOARD_X;
    gameplayBox.y = BOARD_Y;
    gameplayBox.w = CELL_SIZE * COL_COUNT;
    gameplayBox.h = CELL_SIZE * ROW_COUNT;
    SDL_RenderDrawRect(renderer, &gameplayBox);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect smileBox;
    smileBox.x = 315;
    smileBox.y = 80;
    smileBox.w = 40;
    smileBox.h = 40;
    SDL_RenderDrawRect(renderer, &smileBox);

    SDL_Rect eyeBox;
    eyeBox.x = 323;
    eyeBox.y = 88;
    eyeBox.w = 8;
    eyeBox.h = 8;
    SDL_RenderFillRect(renderer, &eyeBox);

    SDL_Rect eyeBox2;
    eyeBox2.x = 338;
    eyeBox2.y = 88;
    eyeBox2.w = 8;
    eyeBox2.h = 8;
    SDL_RenderFillRect(renderer, &eyeBox2);

    SDL_RenderDrawLine(renderer, 323, 105, 335, 115);
    SDL_RenderDrawLine(renderer, 335, 115, 347, 105);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect flagBox;
    flagBox.x = 100;
    flagBox.y = 80;
    flagBox.w = 120;
    flagBox.h = 40;
    SDL_RenderDrawRect(renderer, &flagBox);

    SDL_Rect timerBox;
    timerBox.x = 450;
    timerBox.y = 80;
    timerBox.w = 120;
    timerBox.h = 40;
    SDL_RenderDrawRect(renderer, &timerBox);



}

string cellText(const Cell& cell){
    if(!cell.isOpened()) return " ";
    if(cell.isBomb()) return "X";
    if(cell.getAdjacentBombs() == 0) return " ";
    return to_string(cell.getAdjacentBombs());
}

void displayBoard(Board* board, SDL_Renderer* rend, TTF_Font* font){
    SDL_Color b = {255,255,255,0};
    SDL_Color f = {0,0,0,0};
    SDL_Color color = {255,255,255,0};

    SDL_RenderClear(rend);
    drawBackground(rend);
    string text = to_string(board->getFlagCount());
    int buffer = 0;
    if(board->getFlagCount() > 9) buffer = 8;
    renderText(rend, font, text, color, 155 - buffer, 88);
    color = {255,255,255,255};

    for(int i = 0; i < ROW_COUNT; i++){
        for(int j = 0; j < COL_COUNT; j++){
            const Cell& cell = board->getCell(i, j);
            SDL_Rect box;
            box.x = BOARD_X + j*CELL_SIZE;
            box.y = BOARD_Y + i*CELL_SIZE;
            box.w = CELL_SIZE;
            box.h = CELL_SIZE;
            if(cell.getFlag()){
                f = {255, 255, 255, 0};
            }
            else if(cell.isOpened()){
                f = {255, 0, 255, 0};
            }
            else{
                f = {0, 0, 255, 0};
            }
            text = cellText(cell);
            drawBoxWithBorder(rend, box, f, b);
            int textW, textH;
            TTF_SizeText(font, text.c_str(), &textW, &textH);
            renderText(rend, font, text, color, box.x + (CELL_SIZE - textW) / 2, box.y + (CELL_SIZE - textH) / 2);
        }
    }
}

bool translateMove(int x, int y, int &r, int &c){
    if(x < BOARD_X || y < BOARD_Y) return false;
    r = (y - BOARD_Y) / CELL_SIZE;
    c = (x - BOARD_X) / CELL_SIZE;
    return r < ROW_COUNT && c < COL_COUNT;
}



int main( int argc, char *argv[] ){
   srand(time(0));
   SDL_Init(SDL_INIT_EVERYTHING);
   if (TTF_Init() < 0) {
        cerr << "TTF initialization failed: " << TTF_GetError() << endl;
        return EXIT_FAILURE;
    }

   SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 670, 750, 0);
   SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   TTF_Font* font = TTF_OpenFont("/Users/ciaranturner/Downloads/Roboto-Medium.ttf", 20);
   drawBackground(renderer);
   Board* board = new Board(ROW_COUNT, COL_COUNT, 10);
    SDL_Event windowEvent;
    bool firstClick = true;
    bool running = true;
    int r,c;
    Uint32 startTime = SDL_GetTicks();
    while(running && board->getGameState() == GameState::Playing){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_MOUSEBUTTONDOWN: {
                    if(translateMove(windowEvent.button.x, windowEvent.button.y, r, c)){
                        cout << "r: " << r << " c: " << c << endl;
                        if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                            if(firstClick){
                                board->handleFirstClick(r,c);
                                firstClick = false;
                            }
                            else board->processMove(r,c);
                        } else if (windowEvent.button.button == SDL_BUTTON_RIGHT) {
                            board->flagCell(r,c);
                        }
                    }
                    break;
                }
                case SDL_QUIT: {
                    running = false;
                    break;
                }
            }
        }
        if(board->getGameState() != GameState::Playing){
            if(board->getGameState() == GameState::Won) cout << "You won";
            else cout << "You lost";
            running = false;
        }

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        string timerTxt = to_string(elapsedTime/1000);
    

        displayBoard(board, renderer, font);
        SDL_Color timeColor = {255,255,255,255};
        renderText(renderer,font, timerTxt,timeColor, 505,88);
        SDL_RenderPresent(renderer);
    }


    delete board;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window );
    TTF_CloseFont(font);
    TTF_Quit( );
    SDL_Quit( );
    return EXIT_SUCCESS;
}
