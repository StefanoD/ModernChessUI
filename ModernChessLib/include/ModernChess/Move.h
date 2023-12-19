#pragma once

#include "Square.h"
#include "Figure.h"

#include <ostream>

namespace ModernChess {

    /**
     * Binary Move Bits                                     Hexadecimal Constants
     *
     * 0000 0000 0000 0000 0011 1111    source square       0x3F
     * 0000 0000 0000 1111 1100 0000    target square       0xFC0
     * 0000 0000 1111 0000 0000 0000    piece               0xF000
     * 0000 1111 0000 0000 0000 0000    promoted piece      0xF0000
     * 0001 0000 0000 0000 0000 0000    capture flag        0x100000
     * 0010 0000 0000 0000 0000 0000    double push flag    0x200000
     * 0100 0000 0000 0000 0000 0000    en passant flag     0x400000
     * 1000 0000 0000 0000 0000 0000    castling flag       0x800000
    */

    /**
     * @see https://www.chessprogramming.org/Encoding_Moves
     */
    struct Move
    {
        /**
         * @brief NULL move
         */
        Move() = default;

        explicit Move(Square from,
                      Square to,
                      Figure movedFigure,
                      Figure promotedPiece,
                      bool isCapture,
                      bool isDoublePawnPush,
                      bool isEnPassantCapture,
                      bool isCastlingMove) :
                m_move{from |
                       (to << 6) |
                       (movedFigure << 12) |
                       (promotedPiece << 16) |
                       (uint8_t(isCapture) << 20) |
                       (uint8_t(isDoublePawnPush) << 21) |
                       (uint8_t(isEnPassantCapture) << 22) |
                       (uint8_t(isCastlingMove) << 23)
                }
        {}

        [[nodiscard]] Square getFrom() const
        {
            return Square(m_move & 0x3F);
        }

        [[nodiscard]] Square getTo() const
        {
            return Square((m_move & 0xFC0) >> 6);
        }

        [[nodiscard]] Figure getMovedFigure() const
        {
            return Figure((m_move & 0xF000) >> 12);
        }

        [[nodiscard]] Figure getPromotedPiece() const
        {
            return Figure((m_move & 0xF0000) >> 16);
        }

        [[nodiscard]] bool isCapture() const
        {
            return (m_move & SpecialMove::Capture) != 0;
        }

        [[nodiscard]] bool isDoublePawnPush() const
        {
            return (m_move & SpecialMove::DoublePawnPush) != 0;
        }

        [[nodiscard]] bool isEnPassantCapture() const
        {
            return (m_move & SpecialMove::EnPassantCapture) != 0;
        }

        [[nodiscard]] bool isCastlingMove() const
        {
            return (m_move & SpecialMove::Castling) != 0;
        }

        [[nodiscard]] bool isNullMove() const
        {
            return m_move == 0;
        }

        bool operator==(const Move &other) const = default;

    private:
        int32_t m_move = 0;

        enum SpecialMove : uint32_t
        {
            Capture = 0x100000,
            DoublePawnPush = 0x200000,
            EnPassantCapture = 0x400000,
            Castling = 0x800000,
        };
    };
}

/**
 * @return UCI compliant move encoding
 */
std::ostream &operator<<(std::ostream &os, ModernChess::Move move);