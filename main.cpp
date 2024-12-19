#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

class Cell;
class Board;
class Game;

class Cell{
    public:
        Cell();
        Cell(bool bomb, Board* board);
        int getValue() const;
        void setValue(int value);
        bool getFlag() const;
        bool isOpened() const;
        bool isBomb();
        char getDisplayValue();
        void setDisplayValue(char val);
    private: 
        Board* m_board;
        int m_value;
        char m_display_value;
        bool m_opened;
        bool m_flagged;
        bool m_bomb;
};


class Board{
    public:
        Board(Game* game, int rows, int cols);
        ~Board();
        void calculateValue(int row, int col);
        int getRows();
        int getCols();
        void bombOpened();
        void displayBoard();
        void openCell(int row, int col);
    private: 
        Game* m_game;
        Cell** m_grid;
        int m_rows;
        int m_cols;
        int bombsFlagged;
};

class Game{
   public:
        Game();
        void endGame(bool won);
        bool checkMove(int row, int col);
        void play();
    private: 
        Board* m_board;
        bool m_gameOver;//might not need this
};


int main(){
    cout << "Welcome to Minesweeper!" << endl;
    cout << "-----------------------" << endl;
    Game g;
    g.play();
}

Cell::Cell()
{
    m_display_value = 'O';
}

Cell::Cell(bool bomb, Board *board)
{
    m_value = 0;
    m_bomb = bomb;
    m_opened = false;
    m_flagged = false;
    m_board = board;
}

int Cell::getValue() const
{
    return m_value;
}

void Cell::setValue(int value)
{
    m_value = value;
}

bool Cell::getFlag() const
{
    return m_flagged;
}

bool Cell::isOpened() const
{
    return m_opened;
}

bool Cell::isBomb()
{
    return m_bomb;
}

char Cell::getDisplayValue()
{
    return m_display_value;
}

void Cell::setDisplayValue(char val)
{
    m_display_value = val;
}

Board::Board(Game* game, int rows, int cols)
{
    m_game = game;
    m_rows = rows;
    m_cols = cols;
    m_grid = new Cell*[rows];
    for(int i = 0; i < rows; i++){
        m_grid[i] = new Cell[cols]{Cell(false, this)};
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            int bombRatio = 100 * (sqrt(rows*cols)/rows*cols);
            bool isBomb = (rand() % 100 < bombRatio);
            m_grid[i][j] = Cell(isBomb, this);
        }
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            calculateValue(i, j);
        }
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
    if(col > 0 && row + 1 < m_cols && m_grid[row+1][col-1].isBomb()) valueCounter++;
    m_grid[row][col].setValue(valueCounter);
}

int Board::getRows()
{
    return m_rows;
}

int Board::getCols()
{
    return m_cols;
}

void Board::bombOpened()
{
    m_game->endGame(false);
}

void Board::displayBoard()
{
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            cout << m_grid[i][j].getDisplayValue() << " ";
        }
        cout << "\n";
    }
}

void Board::openCell(int row, int col)
{
    Cell& cellToOpen = m_grid[row][col];
    if(cellToOpen.isBomb()){
        bombOpened();
    }
    else{
        cellToOpen.setDisplayValue(cellToOpen.getValue());
    }
}

Game::Game()
{
    cout << "Choose a level of difficulty" << endl;
    string inputchar;
    do{
        cout << "E, M, or H" << endl;
        cin >> inputchar;
    }while(inputchar != "E" && inputchar != "M" && inputchar != "H");
    if(inputchar == "E"){
        m_board = new Board(this, 5, 5);
        cout << "easy board" << endl;
    }
    else if (inputchar == "M"){
        m_board = new Board(this, 20, 20);
        cout << "medium board" << endl;
    }
    else{
        m_board = new Board(this, 100, 100);
        cout << "hard board" << endl;
    }
}

void Game::endGame(bool won)
{
    if(won){
        cout << "Congrats! You Won!";
    }
    else{
        cout << "You Lost!";
    }
}

bool Game::checkMove(int row, int col)
{
    if(row >= 0 && row < m_board->getRows() && col >= 0 && col < m_board->getCols()){
        return true;
    }
    return false;
}

void Game::play(){
    //implement this
    cout << "here";
}
