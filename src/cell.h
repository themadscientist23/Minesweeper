#pragma once

class Cell {
  public:
    int getAdjacentBombs() const;
    void setAdjacentBombs(int count);
    bool getFlag() const;
    void setFlag(bool value);
    void setBomb(bool value);
    bool isBomb() const;
    bool isOpened() const;
    bool openCell();

  private:
    int m_adjacentBombs = 0;
    bool m_opened = false;
    bool m_flagged = false;
    bool m_bomb = false;
};