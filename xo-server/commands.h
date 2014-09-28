#pragma once

enum class Command : int
{
    PlayerFirst = -128,
    ComputerFirst,
    PlayerWon,
    ComputerWon,
    DrawGame
};
