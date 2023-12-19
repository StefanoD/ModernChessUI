#include "ModernChess/GameState.h"

namespace ModernChess
{
    const ZobristHasher GameState::m_hasher{};
    TranspositionTable GameState::transpositionTable{};

    GameState::GameState()
    {
        transpositionTable.clear();
    }
}

std::ostream& operator<<(std::ostream& os, const ModernChess::GameState &gameState)
{
    using namespace ModernChess;

    os << gameState.board;

    os << "Side to move: ";

    if (gameState.board.sideToMove == Color::White)
    {
        os << "white\n";
    }
    else
    {
        os << "black\n";
    }

    os << "En passant: ";
    if (gameState.board.enPassantTarget != Square::undefined)
    {
        os << squareToCoordinates[gameState.board.enPassantTarget] << '\n';
    }
    else
    {
        os << "no\n";
    }

    os << gameState.board.castlingRights << '\n';

    const std::ios::fmtflags os_flags (os.flags());
    os << "Hash key: " << std::hex << gameState.gameStateHash << '\n';
    os.flags(os_flags);
    return os;
}