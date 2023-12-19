#include "ModernChess/ZobristHasher.h"
#include "ModernChess/Figure.h"
#include "ModernChess/Square.h"
#include "ModernChess/PseudoRandomGenerator.h"
#include "ModernChess/BitBoardOperations.h"
#include "ModernChess/Board.h"

namespace ModernChess {

    std::array<std::array<uint64_t, NumberOfSquares>, NumberOfFigureTypes> ZobristHasher::pieceKeys = {};
    std::array<uint64_t, NumberOfSquares> ZobristHasher::enpassantKeys = {};
    std::array<uint64_t, 16> ZobristHasher::castleKeys = {};
    uint64_t ZobristHasher::sideKey = {};

    ZobristHasher::ZobristHasher()
    {
        PseudoRandomGenerator randomGenerator;
        // loop over piece codes
        for (Figure figure = Figure::WhitePawn; figure <= Figure::BlackKing; ++figure)
        {
            // loop over board squares
            for (Square square = Square::a1; square <= Square::h8; ++square)
            {    // init random figure keys
                pieceKeys[figure][square] = randomGenerator.getRandomU64Number();
            }
        }

        // loop over board squares
        for (Square square = Square::a1; square <= Square::h8; ++square)
        {    // init random en passant keys
            enpassantKeys[square] = randomGenerator.getRandomU64Number();
        }

        // loop over castling keys (see CastlingRights.h)
        for (uint8_t index = 0; index < 16; ++index)
        {
            // init castling keys
            castleKeys[index] = randomGenerator.getRandomU64Number();
        }

        // init random side key
        sideKey = randomGenerator.getRandomU64Number();
    }

    uint64_t ZobristHasher::generateHash(const Board &board)
    {
        uint64_t finalKey = 0;

        for (Figure figure = Figure::WhitePawn; figure <= Figure::BlackKing; ++figure)
        {
            for (BitBoardState bitboard = board.bitboards[figure]; bitboard != BoardState::empty; )
            {
                const Square square = BitBoardOperations::bitScanForward(bitboard);
                finalKey ^= pieceKeys[figure][square];

                // pop LS1B
                bitboard = BitBoardOperations::eraseSquare(bitboard, square);
            }
        }

        if (board.enPassantTarget != Square::undefined)
        {
            finalKey ^= enpassantKeys[board.enPassantTarget];
        }

        finalKey ^= castleKeys[board.castlingRights];

        // hash the side only if black is to move
        if (board.sideToMove == Color::Black)
        {
            finalKey ^= sideKey;
        }

        // return generated hash key
        return finalKey;
    }
}