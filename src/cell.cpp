#include <cell.h>

int Cell::getAdjacentBombs() const { return m_adjacentBombs; }

void Cell::setAdjacentBombs(int count) { m_adjacentBombs = count; }

bool Cell::getFlag() const { return m_flagged; }

void Cell::setFlag(bool value) { m_flagged = value; }

bool Cell::isBomb() const { return m_bomb; }

bool Cell::isOpened() const { return m_opened; }

void Cell::setBomb(bool value) { m_bomb = value; }

bool Cell::openCell() {
    if (m_flagged)
        return false;
    m_opened = true;
    return m_bomb;
}