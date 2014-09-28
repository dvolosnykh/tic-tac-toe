#pragma once

#include <array>

struct Cell;


class GameEngine
{
public:
    enum { TilesCount = 3, CountersPerPlayer = 2 * (TilesCount + 1) };
    enum class State { Negotiation, MyTurn, OpponentTurn, Victory, Loss, Draw };

private:
    enum Player { Me, Opponent, None };
    enum Offset { Row = 0, Column = TilesCount, MainDiagonal = 2 * TilesCount, SecondaryDiagonal};

    typedef std::array<int, CountersPerPlayer> PlayerCounters;

public:
    GameEngine();

    void reset();
    void start(bool meFirst);

    State state() const;

    void recordOpponentTurn(Cell);
    Cell makeMyTurn();

private:
    Cell evaluateMyTurn() const;
    Cell tryWin(Player player) const;
    Cell tryFork(Player player) const;
    bool madeFork(Player player) const;
    Cell tryCenter() const;
    Cell tryOppositeCorner(Player player) const;
    Cell tryEmptyCorner() const;
    Cell tryEmptySide() const;
    void writeTurn(Cell cell, Player player);
    void eraseTurn(Cell cell);
    bool maybeEnded(Cell cell);

    static Player otherPlayer(Player player);

private:
    State _state;
    Player _board[TilesCount][TilesCount];
    PlayerCounters _counters[2];
    int _emptyCellsCount;
};


struct Cell
{
    int row, column;

    Cell(int row = -1, int column = -1)
        : row(row)
        , column(column)
    {}

    bool isValid() const {
        return (0 <= row && row < GameEngine::TilesCount && 0 <= column && column < GameEngine::TilesCount);
    }
};

