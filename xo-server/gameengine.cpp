#include "gameengine.h"
#include <QtCore/QtGlobal>
#include <algorithm>


namespace
{

int rowBit(const int index)
{
    return (index & 0x1);
}

int columnBit(const int index)
{
    return ((index >> 1) & 0x1);
}

}


GameEngine::GameEngine()
{
    reset();
}

void GameEngine::reset()
{
    _state = State::Negotiation;
    std::fill(*_board, *_board + TilesCount * TilesCount, Player::None);
    std::fill(_counters[Player::Me].begin(), _counters[Player::Me].end(), 0);
    std::fill(_counters[Player::Opponent].begin(), _counters[Player::Opponent].end(), 0);
    _emptyCellsCount = TilesCount * TilesCount;
}

void GameEngine::start(const bool meFirst)
{
    if (_state != State::Negotiation) return;

    _state = (meFirst ? State::MyTurn : State::OpponentTurn);
}

GameEngine::State GameEngine::state() const
{
    return _state;
}

void GameEngine::recordOpponentTurn(const Cell cell)
{
    Q_ASSERT(cell.isValid());

    if (_state != State::OpponentTurn) return;

    Q_ASSERT(_emptyCellsCount > 0);

    writeTurn(cell, Player::Opponent);
    if (!maybeEnded(cell)) {
        _state = State::MyTurn;
    }
}

Cell GameEngine::makeMyTurn()
{
    if (_state != State::MyTurn) return Cell();

    Q_ASSERT(_emptyCellsCount > 0);

    const Cell cell = evaluateMyTurn();
    Q_ASSERT(cell.isValid());
    writeTurn(cell, Player::Me);
    if (!maybeEnded(cell)) {
        _state = State::OpponentTurn;
    }

    return cell;
}

/**
 * See http://en.wikipedia.org/wiki/Tic-tac-toe#Strategy
 * http://programmers.stackexchange.com/questions/213559/algorithm-to-create-an-tictactoe-game-ai
 */
Cell GameEngine::evaluateMyTurn() const
{
    Cell cell = tryWin(Player::Me);
    if (!cell.isValid()) {
        cell = tryWin(Player::Opponent);
    }
    if (!cell.isValid()) {
        cell = tryFork(Player::Me);
    }
    if (!cell.isValid()) {
        cell = tryFork(Player::Opponent);
    }
    if (!cell.isValid()) {
        cell = tryCenter();
    }
    if (!cell.isValid()) {
        cell = tryOppositeCorner(Player::Opponent);
    }
    if (!cell.isValid()) {
        cell = tryEmptyCorner();
    }
    if (!cell.isValid()) {
        cell = tryEmptySide();
    }
    Q_ASSERT(cell.isValid());
    return cell;
}

Cell GameEngine::tryWin(const Player player) const
{
    const PlayerCounters& playerCounters = _counters[player];
    const PlayerCounters& opponentCounters = _counters[otherPlayer(player)];

    int index = 0;
    while (index < CountersPerPlayer && !(playerCounters[index] == TilesCount - 1 && opponentCounters[index] == 0)) {
        ++index;
    }
    if (index == CountersPerPlayer) return Cell();

    Cell cell;
    if (index < Offset::Column) {
        cell = Cell(index, 0);
        while (cell.isValid() && _board[cell.row][cell.column] != Player::None) {
            ++cell.column;
        }
    } else if (index < Offset::MainDiagonal) {
        cell = Cell(0, index - Offset::Column);
        while (cell.isValid() && _board[cell.row][cell.column] != Player::None) {
            ++cell.row;
        }
    } else if (index == Offset::MainDiagonal) {
        cell = Cell(0, 0);
        while (cell.isValid() && _board[cell.row][cell.column] != Player::None) {
            ++cell.row, ++cell.column;
        }
    } else if (index == Offset::SecondaryDiagonal) {
        cell = Cell(0, TilesCount - 1);
        while (cell.isValid() && _board[cell.row][cell.column] != Player::None) {
            ++cell.row, --cell.column;
        }
    } else {
        Q_UNREACHABLE();
    }

    Q_ASSERT(cell.isValid());
    return cell;
}

Cell GameEngine::tryFork(const Player player) const
{
    const PlayerCounters& playerCounters = _counters[player];
    const PlayerCounters& opponentCounters = _counters[otherPlayer(player)];

    for (Cell cell(0, 0); cell.row < TilesCount; ++cell.row) {
        if (playerCounters[Offset::Row + cell.row] + opponentCounters[Offset::Row + cell.row] == TilesCount) continue;

        for (cell.column = 0; cell.column < TilesCount; ++cell.column) {
            if (_board[cell.row][cell.column] != Player::None) continue;

            const_cast<GameEngine*>(this)->writeTurn(cell, player);
            const bool fork = madeFork(player);
            const_cast<GameEngine*>(this)->eraseTurn(cell);

            if (fork) return cell;
        }
    }

    return Cell();
}

bool GameEngine::madeFork(GameEngine::Player player) const
{
    const PlayerCounters& playerCounters = _counters[player];
    const PlayerCounters& opponentCounters = _counters[otherPlayer(player)];

    int linesCounter = 0;

    for (int row = 0; row < TilesCount; ++row) {
        if (playerCounters[Offset::Row + row] == TilesCount - 1 && opponentCounters[Offset::Row + row] == 0) {
            ++linesCounter;
        }
    }

    for (int column = 0; column < TilesCount; ++column) {
        if (playerCounters[Offset::Column + column] == TilesCount - 1 && opponentCounters[Offset::Column + column] == 0) {
            ++linesCounter;
        }
    }

    if (playerCounters[Offset::MainDiagonal] == TilesCount - 1 && opponentCounters[Offset::MainDiagonal] == 0) {
        ++linesCounter;
    }
    if (playerCounters[Offset::SecondaryDiagonal] == TilesCount - 1 && opponentCounters[Offset::SecondaryDiagonal] == 0) {
        ++linesCounter;
    }

    return (linesCounter >= 2);
}

Cell GameEngine::tryCenter() const
{
    if (TilesCount % 2 == 0) return Cell();

    const int center = TilesCount / 2;
    return (_board[center][center] == Player::None ? Cell(center, center) : Cell());
}

Cell GameEngine::tryOppositeCorner(const Player player) const
{
    const Player opponent = otherPlayer(player);

    for (int i = 0; i < 4; ++i) {
        const int r = (TilesCount - 1) * rowBit(i);
        const int c = (TilesCount - 1) * columnBit(i);
        if (_board[r][c] == opponent && _board[TilesCount - 1 - r][TilesCount - 1 - c] == Player::None) {
            return Cell(r, c);
        }
    }

    return Cell();
}

Cell GameEngine::tryEmptyCorner() const
{
    for (int i = 0; i < 4; ++i) {
        const int r = (TilesCount - 1) * rowBit(i);
        const int c = (TilesCount - 1) * columnBit(i);
        if (_board[r][c] == Player::None) {
            return Cell(r, c);
        }
    }

    return Cell();
}

Cell GameEngine::tryEmptySide() const
{
    if (TilesCount % 2 == 0) return Cell();

    const int center = TilesCount / 2;

    if (_board[center][0] == Player::None) {
        return Cell(center, 0);
    }
    if (_board[center][TilesCount - 1] == Player::None) {
        return Cell(center, TilesCount - 1);
    }
    if (_board[0][center] == Player::None) {
        return Cell(0, center);
    }
    if (_board[TilesCount - 1][center] == Player::None) {
        return Cell(TilesCount - 1, center);
    }
    return Cell();
}

void GameEngine::writeTurn(const Cell cell, const Player player)
{
    Q_ASSERT(_board[cell.row][cell.column] == Player::None);
    Q_ASSERT(player != Player::None);

    _board[cell.row][cell.column] = player;
    PlayerCounters& counters = _counters[player];

    ++counters[Offset::Row + cell.row];
    ++counters[Offset::Column + cell.column];
    if (cell.row == cell.column) {
        ++counters[Offset::MainDiagonal];
    }
    if (cell.row + cell.column == TilesCount - 1) {
        ++counters[Offset::SecondaryDiagonal];
    }

    --_emptyCellsCount;
}

void GameEngine::eraseTurn(const Cell cell)
{
    Player& player = _board[cell.row][cell.column];
    Q_ASSERT(player != Player::None);

    PlayerCounters& counters = _counters[player];
    player = Player::None;

    --counters[Offset::Row + cell.row];
    --counters[Offset::Column + cell.column];
    if (cell.row == cell.column) {
        --counters[Offset::MainDiagonal];
    }
    if (cell.row + cell.column == TilesCount - 1) {
        --counters[Offset::SecondaryDiagonal];
    }

    ++_emptyCellsCount;
}

bool GameEngine::maybeEnded(const Cell cell)
{
    const Player player = _board[cell.row][cell.column];
    const PlayerCounters& counters = _counters[player];
    const auto& iter = std::find(counters.begin(), counters.end(), TilesCount);
    if (iter != counters.end()) {
        switch (player) {
        case Player::Me:
            _state = State::Victory;
            return true;

        case Player::Opponent:
            _state = State::Loss;
            return true;

        default:
            Q_UNREACHABLE();
        }
    } else if (_emptyCellsCount == 0) {
        _state = State::Draw;
        return true;
    }

    return false;
}

GameEngine::Player GameEngine::otherPlayer(const Player player)
{
    return static_cast<Player>(Player::Me + Player::Opponent - player);
}
