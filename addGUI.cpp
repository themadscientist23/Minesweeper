#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

#define bold "\033[1m"
#define reset "\033[0m" 
#define colorOne "\033[34m" 
#define colorTwo "\033[32m"   
#define colorThree "\033[31m"               
#define colorFour "\033[104m"      
#define colorFive "\033[101m"     
#define colorSix "\033[46m"     
#define colorSeven "\033[30m"      
#define colorEight "\033[90m"      

class Cell;
class Board;

class Cell{
    public:
        Cell();
        char getValue() const;
        void setValue(char value);
        bool getFlag() const;
        void setFlag(bool value);
        char getDisplayValue();
        void setDisplayValue(char value);
        void setBomb(bool value);
        void setBoard(Board* board);
        bool isBomb() const;
        bool isOpened() const;
        void openCell();
    private: 
        Board* m_board;
        char m_value;
        char m_display_value;
        bool m_opened;
        bool m_flagged;
        bool m_bomb;
};

class Board{
    public:
        Board(int rows, int cols, int bombs);
        ~Board();
        void calculateValue(int row, int col);
        void bombOpened();
        void displayBoard(SDL_Renderer*);
        void flagCell(int row, int col);
        void processMove(int row, int col);
        bool checkMove(int row, int col);
        void handleFirstClick(int row, int col);
        void floodFill(int row, int col);
        bool stillPlaying();
        bool getWin();
        bool isNeighbor(int r1, int c1, int r2, int c2);
        void printValue(char value);
    private: 
        Cell** m_grid;
        int m_bombs;
        int m_rows;
        int m_cols;
        int bombsFlagged;
        bool activeBoard;
        bool won;
};

Cell::Cell()
{
    m_display_value = 'U';
    m_bomb = false;
    if(m_bomb) m_value = 'X';
    else m_value = ' ';
    m_opened = false;
    m_flagged = false;
    m_board = nullptr;
}

char Cell::getValue() const
{
    return m_value;
}

void Cell::setValue(char value)
{
    m_value = value;
}

bool Cell::getFlag() const
{
    return m_flagged;
}

void Cell::setFlag(bool value)
{
    m_flagged = value;
}

bool Cell::isBomb() const
{
    return m_bomb;
}

void Cell::setBoard(Board *board)
{
    m_board = board;
}

bool Cell::isOpened() const
{
    return m_opened;
}

char Cell::getDisplayValue()
{
    return m_display_value;
}

void Cell::setDisplayValue(char value)
{
    m_display_value = value;
}

void Cell::setBomb(bool value)
{
    m_bomb = value;
}

void Cell::openCell()
{
    if(m_flagged) return;
    m_opened = true;
    if(isBomb()){
        m_board->bombOpened();
    }
    else{
        setDisplayValue(m_value);
    }
}

Board::Board(int rows, int cols, int bombs)
{
    activeBoard = true;
    won = false;
    m_bombs = bombs;
    m_rows = rows;
    m_cols = cols;
    m_grid = new Cell*[rows];
    for(int i = 0; i < rows; i++){
        m_grid[i] = new Cell[cols];
    }
    bombsFlagged = 0;
}

Board::~Board()
{
    for(int i = 0; i < m_rows; i++){
        delete[] m_grid[i];
    }
    delete[] m_grid;
}

void Board::calculateValue(int row, int col)
{
    int valueCounter = 0;
    if(col + 1 < m_cols && m_grid[row][col+1].isBomb()) valueCounter++;
    if(row + 1 < m_rows && m_grid[row+1][col].isBomb()) valueCounter++;
    if(row + 1 < m_rows && col + 1 < m_cols && m_grid[row+1][col+1].isBomb()) valueCounter++;
    if(row > 0 && m_grid[row-1][col].isBomb()) valueCounter++;
    if(col > 0 && m_grid[row][col-1].isBomb()) valueCounter++;
    if(row > 0 && col > 0 && m_grid[row-1][col-1].isBomb()) valueCounter++;
    if(row > 0 && col + 1 < m_cols && m_grid[row-1][col+1].isBomb()) valueCounter++;
    if(col > 0 && row + 1 < m_rows && m_grid[row+1][col-1].isBomb()) valueCounter++;
    if (m_grid[row][col].isBomb()) m_grid[row][col].setValue('X');
    else if(valueCounter > 0) m_grid[row][col].setValue(valueCounter + '0');
    else m_grid[row][col].setValue(' ');
}

void Board::bombOpened()
{
    activeBoard = false;
    won = false;
}


void Board::printValue(char value){
    if(value == 'U') cout << colorEight << value << reset << "  ";
    else if(value == 'X') cout << colorFive << value << reset << "  ";
    else if(value == 'F') cout << colorThree << value << reset <<  "  ";
    else if(value == '1') cout << colorOne << value << reset << "  "; 
    else if(value == '2') cout << colorTwo << value << reset << "  ";
    else if(value == '3') cout << colorThree << value << reset << "  ";    
    else if(value == '4') cout << colorFour << value << reset << "  ";
    else if(value == '5') cout << colorFive << value << reset << "  ";
    else if(value == '6') cout << colorSix << value << reset << "  ";
    else if(value == '7') cout << colorSeven << value << reset << "  ";
    else if(value == '8') cout << colorEight << value << reset << "  ";
    else cout << value << "  ";
}

void Board::flagCell(int row, int col)
{
    if(!m_grid[row][col].isOpened()) {
        if(m_grid[row][col].getFlag()){
            m_grid[row][col].setFlag(false);
            bombsFlagged--;
        }
        else{
            m_grid[row][col].setFlag(true);
            bombsFlagged++;
        }
    }
}

void Board::processMove(int row, int col)
{
    if(checkMove(row, col)){
        if(m_grid[row][col].getValue() == ' ') floodFill(row, col);
        else m_grid[row][col].openCell();
    }
}

bool Board::checkMove(int row, int col){
    return (row >= 0 && row < m_rows && col >= 0 && col < m_cols && !m_grid[row][col].isOpened());
}

void Board::handleFirstClick(int row, int col){
    int r = 0;
    int c = 0;
    srand(time(0));
    bool** bombMatrix;
    bombMatrix = new bool*[m_rows];
    for(int i = 0; i < m_rows; i++){
        bombMatrix[i] = new bool[m_cols]();
    }
    for(int i = 0; i < m_bombs; i++){
        r = rand() % m_rows;
        c = rand() % m_cols;
        while(isNeighbor(r,c,row,col) || bombMatrix[r][c]){
            r = rand() % m_rows;
            c = rand() % m_cols;
        }
        bombMatrix[r][c] = true;
    }

    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            m_grid[i][j].setBomb(bombMatrix[i][j]);
            m_grid[i][j].setBoard(this);
        }
    }
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            calculateValue(i, j);
        }
    }
    floodFill(row, col);

    for (int i = 0; i < m_rows; i++) {
        delete[] bombMatrix[i];
    }
    delete[] bombMatrix;
}

void Board::floodFill(int row, int col)
{
    if(row < 0 || col < 0 || row >= m_rows || col >= m_cols || m_grid[row][col].isOpened() ||  m_grid[row][col].getFlag()) return;
    m_grid[row][col].openCell();
    if(m_grid[row][col].getValue() != ' ') return;
    floodFill(row+1,col);
    floodFill(row,col+1);
    floodFill(row+1,col+1);
    floodFill(row-1,col-1);   
    floodFill(row,col-1); 
    floodFill(row-1,col); 
    floodFill(row-1,col+1);
    floodFill(row+1,col-1); 
}

void drawBoxWithBorder(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color fillColor, SDL_Color borderColor) {
    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &rect);
}

bool Board::stillPlaying()
{
    if(!activeBoard){
        return false;
    }
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if(!m_grid[i][j].isOpened() && !m_grid[i][j].isBomb()){
                return true;
            }
        }
    }
    won = true;
    return false;
}

void Board::displayBoard(SDL_Renderer* rend){
    SDL_Color b = {255,255,255,0};
    SDL_Color f = {0,0,0,0};
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            SDL_Rect box;
            box.x = 70 + j*53;
            box.y = 150 + i*53;
            box.w = 53;
            box.h = 53;
            if(m_grid[i][j].getFlag()){
                f = {255, 255, 255, 0}; 
            }
            else if(m_grid[i][j].isOpened()){
                f = {255, 0, 255, 0};
            }
            else{
                f = {0, 0, 255, 0};   
            }
            drawBoxWithBorder(rend, box, f, b);
        }
    }
}

bool Board::getWin()
{
    return won;
}

bool Board::isNeighbor(int r1, int c1, int r2, int c2)
{
    return (abs(r1 - r2) <= 1) && (abs(c1 - c2) <= 1);
}

bool translateMove(int x, int y, int &r, int &c){
    r = (y - 150) / 53;
    c = (x - 70)/53;
    if(r > -1 && r < 10 && c > -1 && c < 10) return true;
    else return false;
}



int main( int argc, char *argv[] ){
   if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL initialization failed: " << SDL_GetError() << endl;
        return 1;
    }

    if (TTF_Init() < 0) {
        cout << "TTF_Init failed: " << TTF_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        670, 750, SDL_WINDOW_ALLOW_HIGHDPI);
    
    if (window == NULL) {
        cout << "Could not create window: " << SDL_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == NULL) {
        cout << "Could not create renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("include/SDL2/Roboto-Medium.ttf", 24);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color color = {255,255,255,0};
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, "Hello", color);
    if (!textSurface) {
        cout << "Failed to create text surface: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        cout << "Failed to create texture: " << SDL_GetError() << endl;
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_Rect renderRect = {100,100,textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer,textTexture,NULL,&renderRect);
    
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
    gameplayBox.x = 70;
    gameplayBox.y = 150;
    gameplayBox.w = 530;
    gameplayBox.h = 530;
    SDL_RenderDrawRect(renderer, &gameplayBox);

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            SDL_Rect box;
            box.x = 70 + i*53;
            box.y = 150 + j*53;
            box.w = 53;
            box.h = 53;
            SDL_Color f = {0, 0, 255, 0};   
            SDL_Color b = {0, 255, 0, 0};  
            drawBoxWithBorder(renderer, box, f, b);
        }
    }

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

    SDL_RenderPresent(renderer);
    Board* board = new Board(10, 10, 10);

    SDL_Event windowEvent;
    bool firstClick = true;
    bool running = true;
    int r,c;
    while(running && board->stillPlaying()){
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
                            board->processMove(r,c);
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
        board->displayBoard(renderer);
        SDL_RenderPresent(renderer);
    }

    if(board->getWin()){
        cout << "You won";
    }
    else cout << "You lost";

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window );
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit( );
    SDL_Quit( );
    return EXIT_SUCCESS;
}