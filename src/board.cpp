#include <cstdlib>

#include <board.h>
#include <cell.h>

Board::Board(int rows, int cols, int bombs)
{
    m_bombs = bombs;
    m_rows = rows;
    m_cols = cols;
    m_grid = new Cell*[rows];
    for(int i = 0; i < rows; i++){
        m_grid[i] = new Cell[cols];
    }
    m_bombsFlagged = 0;
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
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            int nr = row + i;
            int nc = col + j;
            if ( (i == 0 && j == 0) || nr < 0 || nr >= m_rows || nc < 0 || nc >= m_cols || !m_grid[nr][nc].isBomb()){
                continue;
            }   
            valueCounter++;
        }
    }
    m_grid[row][col].setAdjacentBombs(valueCounter);
}

MoveResult Board::flagCell(int row, int col)
{
    if(row < 0 || row >= m_rows || col < 0 || col >= m_cols) return MoveResult::Invalid;
    if(m_grid[row][col].isOpened()) return MoveResult::AlreadyOpen;
    if(m_grid[row][col].getFlag()){
        m_grid[row][col].setFlag(false);
        m_bombsFlagged--;
    }
    else{
        m_grid[row][col].setFlag(true);
        m_bombsFlagged++;
    }
    return MoveResult::Valid;
}

MoveResult Board::processMove(int row, int col)
{
    if(!checkMove(row, col)) return MoveResult::Invalid;
    if(m_grid[row][col].getFlag()) return MoveResult::Flagged;
    if(!m_grid[row][col].isBomb() && m_grid[row][col].getAdjacentBombs() == 0) floodFill(row, col);
    else if(m_grid[row][col].openCell()) m_gameState = GameState::Lost;
    updateGameState();
    return MoveResult::Valid;
}

bool Board::checkMove(int row, int col){
    return (row >= 0 && row < m_rows && col >= 0 && col < m_cols && !m_grid[row][col].isOpened());
}

MoveResult Board::handleFirstClick(int row, int col){
    if(!checkMove(row, col)) return MoveResult::Invalid;
    int r = 0;
    int c = 0;

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
        }
    }
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            calculateValue(i, j);
        }
    }
    floodFill(row, col);
    updateGameState();

    for (int i = 0; i < m_rows; i++) {
        delete[] bombMatrix[i];
    }
    delete[] bombMatrix;
    return MoveResult::Valid;
}

void Board::floodFill(int row, int col)
{
    if(row < 0 || col < 0 || row >= m_rows || col >= m_cols || m_grid[row][col].isOpened() ||  m_grid[row][col].getFlag()) return;
    if(m_grid[row][col].openCell()) m_gameState = GameState::Lost;
    if(m_grid[row][col].isBomb() || m_grid[row][col].getAdjacentBombs() != 0) return;


    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if (i == 0 && j == 0){
                continue;
            }   
            floodFill(row + i, col + j);
        }
    }

}

GameState Board::getGameState() const
{
    return m_gameState;
}

Cell& Board::getCell(int row, int col) const
{
    return m_grid[row][col];
}

int Board::getFlagCount() const
{
    return m_bombsFlagged;
}

int Board::getBombCount() const
{
    return m_bombs;
}

void Board::updateGameState()
{
    if(m_gameState != GameState::Playing) return;
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if(!m_grid[i][j].isOpened() && !m_grid[i][j].isBomb()){
                return;
            }
        }
    }
    m_gameState = GameState::Won;
}

bool Board::isNeighbor(int r1, int c1, int r2, int c2)
{
    return (abs(r1 - r2) <= 1) && (abs(c1 - c2) <= 1);
}
