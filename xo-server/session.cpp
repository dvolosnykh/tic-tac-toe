#include "gameengine.h"
#include "session.h"
#include <QtNetwork/QTcpSocket>


namespace
{
    int idCounter = 0;
}


Session::Session(QTcpSocket* const connection, QObject* const parent)
    : QObject(parent)
    , _id(++idCounter)
    , _connection(connection)
    , _game(new GameEngine)
{
    Q_ASSERT(_connection != nullptr);

    connect(_connection, &QTcpSocket::readyRead, this, &Session::handleRequest);
    connect(_connection, &QTcpSocket::disconnected, this, &Session::socketDisconnected);
}

Session::~Session()
{}

int Session::id() const
{
    return _id;
}

void Session::handleRequest()
{
    switch (_game->state()) {
    case GameEngine::State::Negotiation:
        startGame();
        break;

    case GameEngine::State::OpponentTurn:
        if (playerTurn() && !isGameEnded()) {
            computerTurn();
            (void)isGameEnded();
        }
        break;

    default:
        break;
    }
}

void Session::startGame()
{
    const Command command = pendingCommand();

    switch (command) {
    case Command::ComputerFirst:
        _game->start(true);
        emit message(_id, tr("Начинает компьютер"));
        computerTurn();
        break;

    case Command::PlayerFirst:
        _game->start(false);
        emit message(_id, tr("Начинает игрок"));
        break;

    default:
        emit message(_id, tr("Некорректная команда: %1 - игнорируется").arg(static_cast<int>(command)));
    }
}

bool Session::playerTurn()
{
    const int index = static_cast<int>(pendingCommand());
    const Cell cell(index / GameEngine::TilesCount, index % GameEngine::TilesCount);

    if (!cell.isValid()) {
        emit message(_id, tr("Недопустимый ход игрока: (%1, %2) - игнорируется").arg(cell.row).arg(cell.column));
        return false;
    }

    emit message(_id, tr("Игрок сделал ход: (%1, %2)").arg(cell.row).arg(cell.column));
    _game->recordOpponentTurn(cell);
    return true;
}

void Session::computerTurn()
{
    const Cell cell = _game->makeMyTurn();
    emit message(_id, tr("Ход компьютера: (%1, %2)").arg(cell.row).arg(cell.column));
    sendCommand(static_cast<Command>(cell.row * GameEngine::TilesCount + cell.column));
}

bool Session::isGameEnded()
{
    bool ended = false;

    switch (_game->state()) {
    case GameEngine::State::Victory:
        emit message(_id, tr("Победил компьютер"));
        sendCommand(Command::ComputerWon);
        ended = true;
        break;

    case GameEngine::State::Loss:
        emit message(_id, tr("Победил игрок"));
        sendCommand(Command::PlayerWon);
        ended = true;
        break;

    case GameEngine::State::Draw:
        emit message(_id, tr("Ничья"));
        sendCommand(Command::DrawGame);
        ended = true;
        break;

    default:
        break;
    }

    if (ended) {
        _game->reset();
    }

    return ended;
}

Command Session::pendingCommand()
{
    Command command;
    _connection->read(reinterpret_cast<char*>(&command), sizeof(command));
    return command;
}

void Session::sendCommand(const Command command)
{
    _connection->write(reinterpret_cast<const char*>(&command), sizeof(command));
    _connection->flush();
}

void Session::socketDisconnected()
{
    emit message(_id, tr("Сеанс окончен"));
    sender()->deleteLater();
    deleteLater();
}
