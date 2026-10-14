#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include <board.h>
#include <cell.h>

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

class Game {
  public:
    Game();
    ~Game();
    void endGame(bool won);
    void play();
    void quit();

  private:
    Board *m_board;
    int m_rows;
    int m_cols;
    bool m_gameOver;
    void displayBoard();
    void printValue(char value);
    void checkGameState();
};

int main() {
    srand(time(0));
    cout << bold << "\nWelcome to Minesweeper!" << reset << endl
         << "-----------------------" << endl;
    cout << "(Type 100 as the row number to quit whenever you want)\n" << endl;
    Game g;
    g.play();
    string ans;
    while (true) {
        cout << "Play Again (y or n): ";
        cin >> ans;
        if (ans == "y" || ans == "Y") {
            Game g;
            g.play();
        } else {
            break;
        }
    }
    cout << "Thanks for playing!";
}

char cellSymbol(const Cell &cell, bool reveal) {
    if (!reveal) {
        if (cell.getFlag())
            return 'F';
        if (!cell.isOpened())
            return 'U';
    }
    if (cell.isBomb())
        return 'X';
    if (cell.getAdjacentBombs() == 0)
        return ' ';
    return '0' + cell.getAdjacentBombs();
}

void Game::displayBoard() {
    cout << "     ";
    for (int i = -2; i < m_rows; i++) {
        if (i > 9)
            cout << i << " | ";
        else if (i > -1)
            cout << i << "  | ";
        else if (i == -1)
            cout << "     ";
        for (int j = 0; j < m_cols; j++) {
            if (i == -1) {
                cout << "---";
            } else if (i == -2) {
                if (j > 9)
                    cout << j << " ";
                else
                    cout << j << "  ";
            } else
                printValue(cellSymbol(m_board->getCell(i, j), m_gameOver));
        }
        cout << "\n";
    }
    cout << "\n"
         << "Bombs Flagged: " << m_board->getFlagCount() << " / " << m_board->getBombCount()
         << endl;
    cout << "-----------------------" << endl;
}

void Game::printValue(char value) {
    if (value == 'U')
        cout << colorEight << value << reset << "  ";
    else if (value == 'X')
        cout << colorFive << value << reset << "  ";
    else if (value == 'F')
        cout << colorThree << value << reset << "  ";
    else if (value == '1')
        cout << colorOne << value << reset << "  ";
    else if (value == '2')
        cout << colorTwo << value << reset << "  ";
    else if (value == '3')
        cout << colorThree << value << reset << "  ";
    else if (value == '4')
        cout << colorFour << value << reset << "  ";
    else if (value == '5')
        cout << colorFive << value << reset << "  ";
    else if (value == '6')
        cout << colorSix << value << reset << "  ";
    else if (value == '7')
        cout << colorSeven << value << reset << "  ";
    else if (value == '8')
        cout << colorEight << value << reset << "  ";
    else
        cout << value << "  ";
}

Game::Game() {
    m_gameOver = false;
    cout << "Choose a level of difficulty" << endl;
    string inputchar;
    do {
        cout << "E, M, or H: ";
        cin >> inputchar;
    } while (inputchar != "E" && inputchar != "M" && inputchar != "H" && inputchar != "e" &&
             inputchar != "m" && inputchar != "h");
    int bombs;
    if (inputchar == "E" || inputchar == "e") {
        m_rows = 10;
        m_cols = 10;
        bombs = 10;
    } else if (inputchar == "M" || inputchar == "m") {
        m_rows = 18;
        m_cols = 18;
        bombs = 40;
    } else {
        m_rows = 24;
        m_cols = 24;
        bombs = 99;
    }
    m_board = new Board(m_rows, m_cols, bombs);
}

Game::~Game() { delete m_board; }

void Game::endGame(bool won) {
    m_gameOver = true;
    if (won) {
        cout << bold << colorTwo << "Congrats! You Won!" << reset << endl;
    } else {
        cout << bold << colorThree << "You Lost!" << reset << endl;
    }
}

void Game::quit() {
    m_gameOver = true;
    cout << "You quit the game!" << endl;
}

void Game::checkGameState() {
    GameState state = m_board->getGameState();
    if (state == GameState::Lost)
        endGame(false);
    else if (state == GameState::Won)
        endGame(true);
}

void Game::play() {
    displayBoard();
    cout << "Make your move (row# col#): ";
    int r, c;
    string a;
    cin >> r >> c;
    while (m_board->handleFirstClick(r, c) == MoveResult::Invalid) {
        cout << "Not the right format (row #, space, then column #)" << endl;
        cout << "Try again: ";
        cin >> r >> c;
    }
    checkGameState();
    displayBoard();
    while (!m_gameOver) {
        cout << "Make your move: row# col#: ";
        cin >> r >> c;
        if (r == 100) {
            quit();
            break;
        }
        cout << "What action do you want to do? (O)pen/(F)lag: ";
        cin >> a;
        MoveResult result;
        if (a == "F" || a == "f")
            result = m_board->flagCell(r, c);
        else if (a == "O" || a == "o")
            result = m_board->processMove(r, c);
        else {
            cout << "Invalid action!" << endl;
            continue;
        }

        if (result == MoveResult::Invalid)
            cout << "Invalid Move" << endl;
        else if (result == MoveResult::Flagged)
            cout << "You can't open this. It's flagged!" << endl;
        else if (result == MoveResult::AlreadyOpen)
            cout << "Don't flag this. It's already open!" << endl;
        checkGameState();
        displayBoard();
    }
}
