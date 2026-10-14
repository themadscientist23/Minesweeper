#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
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

int main(){
    cout << bold << "\nWelcome to Minesweeper!" << reset << endl << "-----------------------" << endl;
    cout << "(Type 100 100 to quit whenever you want)\n" << endl;
    Game g;
    g.play();
    string ans;
    while(true){
        cout << "Play Again (y or n): ";
        cin >> ans;
        if(ans == "y" || ans == "Y"){
            Game g;
            g.play();
        }
        else{
            break;
        }
    }
    cout << "Thanks for playing!";
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
    cout << "-----------------------" << endl << "     ";
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
    else cout << "Don't flag this. It's already open!" << endl;
}

void Board::processMove(int row, int col)
{
    if(checkMove(row, col)){
        if(m_grid[row][col].getFlag()) cout << "You can't open this. It is flagged!" << endl;
        else if(m_grid[row][col].getValue() == ' ') floodFill(row, col);
        else m_grid[row][col].openCell();
    }
    else{
        cout << "Invalid Move" << endl;
    }
}

bool Board::checkMove(int row, int col){
    return (row >= 0 && row < m_rows && col >= 0 && col < m_cols && !m_grid[row][col].isOpened());
}

void Board::handleFirstClick(int row, int col){
    int r = 0;
    int c = 0;
    srand(time(0));
    while(!checkMove(row, col)){
        cout << "Not the right format (row number, space, then column number)" << endl;
        cout << "Try again: ";
        cin >> row >> col;
    }

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
    cout << "Choose a level of difficulty" << endl;
    string inputchar;
    do{
        cout << "E, M, or H: ";
        cin >> inputchar;
    }while(inputchar != "E" && inputchar != "M" && inputchar != "H" && inputchar != "e" && inputchar != "m" && inputchar != "h");
    if(inputchar == "E" || inputchar == "e"){
        m_board = new Board(this, 10, 10, 10);
    }
    else if (inputchar == "M" || inputchar == "m"){
        m_board = new Board(this, 18, 18, 40);
    }
    else{
        m_board = new Board(this, 24, 24, 99);
    }
}

void Game::endGame(bool won)
{
    m_gameOver = true;
    if(won){
        cout << bold << colorTwo << "Congrats! You Won!" << reset << endl;
    }
    else{
        cout << bold << colorThree << "You Lost!" << reset << endl;
    }
}

void Game::quit()
{
    m_gameOver = true;
    cout << "You quit the game!" << endl;
}

bool Game::getGameVal()
{
    return m_gameOver;
}

void Game::play(){
    m_board->displayBoard();
    cout << "Make your move (row# col#): " ;
    int r, c;
    string a;
    cin >> r >> c;
    m_board->handleFirstClick(r, c);
    m_board->displayBoard();
    while(!m_gameOver){
        cout << "Make your move: row# col#: ";
        cin >> r >> c;
        if(r == 100){
            quit();
            break;
        }
        cout << "What action do you want to do? (O)pen/(F)lag: ";
        cin >> a;
        if(a == "F" || a == "f") m_board->flagCell(r, c);
        else if(a == "O" || a == "o") m_board->processMove(r,c);
        else cout << "Invalid action!" << endl;
        if(m_board->checkGameStatus()) endGame(true);
        m_board->displayBoard();
    }
}

//text color