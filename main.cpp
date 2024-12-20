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
        char getValue() const;
        void setValue(char value);
        bool getFlag() const;
        void flag();
        bool isBomb() const;
        bool isOpened() const;
        void openCell();
        char getDisplayValue();
        void setDisplayValue(char val);
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
        Board(Game* game, int rows, int cols);
        ~Board();
        void calculateValue(int row, int col);
        int getRows();
        int getCols();
        void bombOpened();
        void displayBoard();
        void flagCell(int row, int col);
        void processMove(int row, int col);
        bool unopened(int row, int col) const;
        void handleFirstClick(int row, int col);
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
        bool getGameVal();
        void move(int row, int col);
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
    m_display_value = 'O';
    m_bomb = bomb;
    if(m_bomb) m_value = 'X';
    else m_value = '0';
    m_opened = false;
    m_flagged = false;
    m_board = board;
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

void Cell::flag()
{
    m_flagged = !m_flagged;
}

bool Cell::isBomb() const
{
    return m_bomb;
}

bool Cell::isOpened() const
{
    return m_opened;
}

char Cell::getDisplayValue()
{
    return m_display_value;
}

void Cell::setDisplayValue(char val)
{
    m_display_value = val;
}


void Cell::openCell()
{
    if(isBomb()){
        m_board->bombOpened();
    }
    else{
        setDisplayValue(m_value);
    }
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
            double bombRatio = (sqrt(rows*cols)/(rows*cols)) * 100;
            bool bomb = (rand() % 100 < bombRatio);
            m_grid[i][j] = Cell(bomb, this);
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
    if (m_grid[row][col].isBomb()) m_grid[row][col].setValue('X');
    else if(valueCounter > 0) m_grid[row][col].setValue(valueCounter + '0');
    else m_grid[row][col].setValue(' ');

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
    cout << "-----------------------" << endl;
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if(!m_game->getGameVal()){
                if(m_grid[i][j].getFlag()) cout << "F" << " ";
                else cout << m_grid[i][j].getDisplayValue() << " ";

            }
            else cout << m_grid[i][j].getValue() << " ";
        }
        cout << "\n";
    }
    cout << "-----------------------" << endl;
}

void Board::flagCell(int row, int col)
{
    m_grid[row][col].flag();
}

void Board::processMove(int row, int col)
{
    m_grid[row][col].openCell();

}

bool Board::unopened(int row, int col) const
{
    return !m_grid[row][col].isOpened();
}


void Board::handleFirstClick(int row, int col){
    while(!m_game->checkMove(row, col)){
        cout << "Not the right format (row number, space, then column number)" << endl;
        cout << "Try again: ";
        int r, c;
        cin >> r >> c;
    }
    cout << "first click" << endl;
}

Game::Game()
{
    m_gameOver = false;
    cout << "Choose a level of difficulty" << endl;
    string inputchar;
    do{
        cout << "E, M, or H: ";
        cin >> inputchar;
    }while(inputchar != "E" && inputchar != "M" && inputchar != "H");
    if(inputchar == "E"){
        m_board = new Board(this, 5, 5);
    }
    else if (inputchar == "M"){
        m_board = new Board(this, 20, 20);
    }
    else{
        m_board = new Board(this, 100, 100);
    }
}

void Game::endGame(bool won)
{
    m_gameOver = true;
    if(won){
        cout << "Congrats! You Won!" << endl;
    }
    else{
        cout << "You Lost!" << endl;

    }
}

bool Game::getGameVal()
{
    return m_gameOver;
}

bool Game::checkMove(int row, int col){
    return (row >= 0 && row < m_board->getRows() && col >= 0 && col < m_board->getCols() && m_board->unopened(row, col));
}

void Game::move(int row, int col)
{
    if(checkMove(row, col)){
        m_board->processMove(row, col);
    }
    else{
        cout << "Invalid Move" << endl;
    }
}


void Game::play(){
    m_board->displayBoard();
    cout << "Make your move (row# col#): " ;
    int r, c;
    string f;
    cin >> r >> c;
    m_board->handleFirstClick(r, c);
    m_board->displayBoard();
    while(!m_gameOver){
        cout << "Make your move: row# col# f - flag or n - no flag: ";
        cin >> r >> c >> f;
        if(f == "F" || f == "f") m_board->flagCell(r, c);
        else move(r,c);
        m_board->displayBoard();
    }
}

//change it so the board is made after the first click: maybe reintroduce level of diff
//make it so first click does "flood fill"
//trouble shoot what is wrong with bottom row
//add clear cells function
