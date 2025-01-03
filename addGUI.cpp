#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <SDL2/SDL.h>
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
class Game;

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
        Board(Game* game, int rows, int cols, int bombs);
        ~Board();
        void calculateValue(int row, int col);
        void bombOpened();
        void displayBoard();
        void flagCell(int row, int col);
        void processMove(int row, int col);
        bool checkMove(int row, int col);
        void handleFirstClick(int row, int col);
        void floodFill(int row, int col);
        bool checkGameStatus();
        bool isNeighbor(int r1, int c1, int r2, int c2);
        void printValue(char value);
    private: 
        Game* m_game;
        Cell** m_grid;
        int m_bombs;
        int m_rows;
        int m_cols;
        int bombsFlagged;
};

class Game{
   public:
        Game();
        void endGame(bool won);
        bool getGameVal();
        void play();
        void quit();
    private: 
        Board* m_board;
        bool m_gameOver;
};

int main( int argc, char *argv[] ){
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window *window = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 670, 750, SDL_WINDOW_ALLOW_HIGHDPI );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if ( NULL == window ){
        cout << "Could not create window: " << SDL_GetError( ) << endl;
        return 1;}

    
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
            SDL_RenderDrawRect(renderer, &box);
        }
    }

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


    while(true){
        Game g;
        g.play();
        break;
    }

    //Thanks for playing!
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window );
    SDL_Quit( );
    return EXIT_SUCCESS;
}

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

Board::Board(Game* game, int rows, int cols, int bombs)
{
    m_bombs = bombs;
    m_game = game;
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
    m_game->endGame(false);
}

void Board::displayBoard()
{
    for(int i = -1; i < m_rows; i++){
        if(i > 9) cout << i << " | ";
        else if(i != -1) cout << i << "  | ";
        for(int j = 0; j < m_cols; j++){
            if(i == -1){
                if(j > 9) cout << j << " ";
                else cout << j << "  ";
            }
            else if(!m_game->getGameVal()){
                if(m_grid[i][j].getFlag()) cout << colorThree << "F" << reset << "  ";
                else printValue(m_grid[i][j].getDisplayValue()); 
            }
            else printValue(m_grid[i][j].getValue());
        }
        cout << "\n";
    }
    cout << "\n" << "Bombs Flagged: " << bombsFlagged << " / " << m_bombs << endl;
    cout << "-----------------------" << endl;
}

void Board::printValue(char value){
    //change text color
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

bool Board::checkGameStatus()
{
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if(!m_grid[i][j].isOpened() && !m_grid[i][j].isBomb()){
                return false;
            }
        }
    }
    return true;
}

bool Board::isNeighbor(int r1, int c1, int r2, int c2)
{
    return (abs(r1 - r2) <= 1) && (abs(c1 - c2) <= 1);
}

Game::Game()
{
    m_gameOver = false;
    string inputchar;
    m_board = new Board(this, 10, 10, 10);
}

void Game::endGame(bool won)
{
    m_gameOver = true;
    if(won){
        //You Won!
    }
    else{
        //You Lost
    }
}



bool Game::getGameVal()
{
    return m_gameOver;
}

void Game::play(){
    SDL_Event windowEvent;
    bool running = true;
    while(running){
        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_MOUSEBUTTONDOWN: { // A mouse button was pressed
                    if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                        printf("Left button clicked at (%d, %d)\n", windowEvent.button.x, windowEvent.button.y);
                        // Handle left-click logic here
                    } else if (windowEvent.button.button == SDL_BUTTON_RIGHT) {
                        printf("Right button clicked at (%d, %d)\n", windowEvent.button.x, windowEvent.button.y);
                        // Handle right-click logic here
                    }
                    break;
                }

                case SDL_MOUSEBUTTONUP: { // A mouse button was released
                    if (windowEvent.button.button == SDL_BUTTON_LEFT) {
                        printf("Left button released\n");
                    } else if (windowEvent.button.button == SDL_BUTTON_RIGHT) {
                        printf("Right button released\n");
                    }
                    break;
                }
                case SDL_QUIT: {
                    m_gameOver = true;
                    running = false;
                }
            }
        }
    }
}