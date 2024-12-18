#include <iostream>
#include <string>
using namespace std;

class Cell{
    public:
        Cell(int value, bool bomb, bool opened);
        int getValue() const;
        bool placeFlag() const;
        bool getFlag() const;
        bool isOpened() const;
        bool isBomb();
        void openCell();
    private: 
        int m_value;
        bool m_opened;
        bool m_flagged;
        bool m_bomb;
};



class Board{
    public:
        Board(int rows, int cols);
        ~Board();
        void calculateValue(int row, int col);
        int getRows();
        int getCols();
        void displayBoard();
    private: 
        Game* m_game;
        Cell** grid;
        int m_rows;
        int m_cols;
        int bombsFlagged;
};

class Game{
   public:
        Game();
        void createBoard(int rows, int cols);
        void introToGame();
        bool isGameOver();
        void startNewGame();
        void processMove(int rows, int col);
    private: 
        Board* m_board;
        int levelOfDiff;
        bool m_gameOver;//might not need this
};


int main(){
    

}