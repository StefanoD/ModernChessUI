#include "ModernChess/FenParsing.h"

using namespace ModernChess;

namespace ModernChess::FenParsing {

    FenParser::FenParser(std::string_view fen) : BasicParser(fen) {}

    Color FenParser::parseColor(char character)
    {
        if (character == 'w')
        {
            ++m_currentPos;
            return Color::White;
        }
        if (character == 'b')
        {
            ++m_currentPos;
            return Color::Black;
        }

        throw std::range_error("Expected 'w' or 'b' for color, but got '" + std::string(1, character) +
                               "' at position " + getCurrentPosition() + "!");
    }

    void FenParser::parseCastlingRights(GameState &gameState)
    {
        char character = getNextCharacter();

        // parse castling rights
        while (character != ' ')
        {
            switch (character)
            {
                case 'K': gameState.board.castlingRights = addWhiteKingSideCastlingRights(gameState.board.castlingRights); break;
                case 'Q': gameState.board.castlingRights = addWhiteQueenSideCastlingRights(gameState.board.castlingRights); break;
                case 'k': gameState.board.castlingRights = addBlackKingSideCastlingRights(gameState.board.castlingRights); break;
                case 'q': gameState.board.castlingRights = addBlackQueenSideCastlingRights(gameState.board.castlingRights); break;
                case '-': break;
                default: throw std::range_error("Could not parse castling rights character '" +
                                                std::string(1, character) + "' at position " +
                                                getCurrentPosition() + "!");
            }

            character = getNextCharacter();
        }

        ++m_currentPos;
    }

    GameState FenParser::parse()
    {
        GameState gameState;

        // FEN strings begin at the top left (a8) and continue to the right position
        for (int rank = 7; rank >= 0; --rank)
        {
            for (int file = 0; file < 8; )
            {
                const Square square = BitBoardOperations::getSquare(rank, file);

                char character = *m_currentPos;

                // match ascii pieces within FEN string
                if (isAlphabetic(character))
                {
                    // init figureType type
                    const Figure figureType = charToFigureType(character);

                    // set figureType on corresponding bitboard
                    gameState.board.bitboards[figureType] = BitBoardOperations::occupySquare(gameState.board.bitboards[figureType], square);

                    character = getNextCharacter();
                    ++file;
                }
                // match empty square numbers within FEN string
                else if (isRankNumber(character))
                {
                    // init offset (convert char 0 to int 0)
                    const int offset = character - '0';

                    // adjust file counter
                    file += offset;

                    character = getNextCharacter();
                }

                const bool characterIsRankSeparator = (character == '/');

                if (characterIsRankSeparator)
                {
                    nextPosition();
                }
            }
        }

        // parse side to move
        gameState.board.sideToMove = parseColor(getNextCharacter());

        // go to parsing castling rights
        parseCastlingRights(gameState);

        // parse en passant square
        gameState.board.enPassantTarget = parseSquare();

        // Skip space and parse half moves
        gameState.halfMoveClock = parseNumber<int32_t>();

        // Skip space and parse number of next move
        gameState.nextMoveClock = parseNumber<int32_t>();

        initOccupancyMaps(gameState);

        gameState.gameStateHash = ZobristHasher::generateHash(gameState.board);

        return gameState;
    }

    void FenParser::initOccupancyMaps(GameState &gameState)
    {
        // Init white occupancy map
        for (Figure figureType = Figure::WhitePawn; figureType <= Figure::WhiteKing; ++figureType)
        {
            gameState.board.occupancies[Color::White] |= gameState.board.bitboards[figureType];
        }

        // Init black occupancy map
        for (Figure figureType = Figure::BlackPawn; figureType <= Figure::BlackKing; ++figureType)
        {
            gameState.board.occupancies[Color::Black] |= gameState.board.bitboards[figureType];
        }

        // init all occupancies
        gameState.board.occupancies[Color::Both] |= gameState.board.occupancies[Color::White];
        gameState.board.occupancies[Color::Both] |= gameState.board.occupancies[Color::Black];
    }
}