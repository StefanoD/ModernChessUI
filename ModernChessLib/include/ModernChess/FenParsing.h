#pragma once

#include "BasicParser.h"
#include "Figure.h"
#include "GameState.h"

#include <array>
#include <string_view>
#include <string>

namespace ModernChess::FenParsing {
    // See https://de.wikipedia.org/wiki/Forsyth-Edwards-Notation

    // ASCII pieces
    constexpr auto asciiFigures = "PNBRQKpnbrqk";

    // unicode pieces
    constexpr std::array<std::string_view, 12> unicodeFigures {"♟︎", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"};

    constexpr Figure charToFigureType(char fenChar)
    {
        switch (fenChar)
        {
            case 'P': return Figure::WhitePawn;
            case 'N': return Figure::WhiteKnight;
            case 'B': return Figure::WhiteBishop;
            case 'R': return Figure::WhiteRook;
            case 'Q': return Figure::WhiteQueen;
            case 'K': return Figure::WhiteKing;
            case 'p': return Figure::BlackPawn;
            case 'n': return Figure::BlackKnight;
            case 'b': return Figure::BlackBishop;
            case 'r': return Figure::BlackRook;
            case 'q': return Figure::BlackQueen;
            case 'k': return Figure::BlackKing;
            default: throw std::range_error("Could not parse FEN character '" + std::string(1, fenChar) + "' to Figure");
        }
    }

    constexpr auto startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    class FenParser : public BasicParser
    {
    public:
        explicit FenParser(std::string_view fen);

        [[nodiscard]] GameState parse();

    private:
        [[nodiscard]] Color parseColor(char character);

        void parseCastlingRights(GameState &gameState);

        static void initOccupancyMaps(GameState &gameState);
    };


}