#pragma once

#include "QueenAttacks.h"
#include "Color.h"
#include "Figure.h"
#include "Board.h"

namespace ModernChess {
    class AttackQueries {
    public:
        static inline bool squareIsAttackedByWhite(const Board &board, Square square)
        {
            // attacked by pawns
            if ((pawnAttackTable[Color::Black][square] & board.bitboards[Figure::WhitePawn]) != BoardState::empty)
            {
                return true;
            }

            // attacked by knights
            if ((knightAttackTable[square] & board.bitboards[Figure::WhiteKnight]) != BoardState::empty)
            {
                return true;
            }

            // attacked by bishops
            if ((bishopAttacks.getAttacks(square, board.occupancies[Color::Both]) & board.bitboards[Figure::WhiteBishop]) != BoardState::empty)
            {
                return true;
            }

            // attacked by rooks
            if ((rookAttacks.getAttacks(square, board.occupancies[Color::Both]) &  board.bitboards[Figure::WhiteRook]) != BoardState::empty)
            {
                return true;
            }

            // attacked by queens
            if ((queenAttacks.getAttacks(square, board.occupancies[Color::Both]) & board.bitboards[Figure::WhiteQueen]) != BoardState::empty)
            {
                return true;
            }

            // attacked by king
            if ((kingAttackTable[square] & board.bitboards[Figure::WhiteKing]) != BoardState::empty)
            {
                return true;
            }

            return false;
        }

        static inline bool squareIsAttackedByBlack(const Board &board, Square square)
        {
            // attacked by pawns
            if ((pawnAttackTable[Color::White][square] & board.bitboards[Figure::BlackPawn]) != BoardState::empty)
            {
                return true;
            }

            // attacked by knights
            if ((knightAttackTable[square] & board.bitboards[Figure::BlackKnight]) != BoardState::empty)
            {
                return true;
            }

            // attacked by bishops
            if ((bishopAttacks.getAttacks(square, board.occupancies[Color::Both]) & board.bitboards[Figure::BlackBishop]) != BoardState::empty)
            {
                return true;
            }

            // attacked by rooks
            if ((rookAttacks.getAttacks(square, board.occupancies[Color::Both]) &  board.bitboards[Figure::BlackRook]) != BoardState::empty)
            {
                return true;
            }

            // attacked by queens
            if ((queenAttacks.getAttacks(square, board.occupancies[Color::Both]) & board.bitboards[Figure::BlackQueen]) != BoardState::empty)
            {
                return true;
            }

            // attacked by king
            if ((kingAttackTable[square] & board.bitboards[Figure::BlackKing]) != BoardState::empty)
            {
                return true;
            }

            return false;
        }

        static const std::array<std::array<BitBoardState, 64>, 2> pawnAttackTable;
        static const std::array<BitBoardState, 64> knightAttackTable;
        static const std::array<BitBoardState, 64> kingAttackTable;
        static const BishopAttacks bishopAttacks;
        static const RookAttacks rookAttacks;
        static const QueenAttacks queenAttacks;
    };
}