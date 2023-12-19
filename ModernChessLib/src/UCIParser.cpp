#include "ModernChess/UCIParser.h"

namespace ModernChess
{
    UCIParser::UCIParser(std::string_view uiCommand) : BasicParser(uiCommand)
    {}

    bool UCIParser::uiRequestsUCIMode() const
    {
        return currentStringView().starts_with("uci");
    }

    bool UCIParser::uiRequestsNewGame() const
    {
        return currentStringView().starts_with("ucinewgame");
    }

    bool UCIParser::uiIsReady() const
    {
        return currentStringView().starts_with("isready");
    }

    bool UCIParser::uiQuitGame() const
    {
        return currentStringView().starts_with("quit");
    }

    bool UCIParser::uiHasSentPosition()
    {
        return uiHasSentCommand("position");
    }

    bool UCIParser::uiHasSentMoves()
    {
        return uiHasSentCommand("moves");
    }

    bool UCIParser::uiHasSentFENPosition()
    {
        return uiHasSentCommand("fen");
    }

    bool UCIParser::uiHasSentStartingPosition()
    {
        return uiHasSentCommand("startpos");
    }

    bool UCIParser::uiHasSentGoCommand()
    {
        return uiHasSentCommand("go");
    }

    bool UCIParser::uiHasSentSearchDepth()
    {
        return uiHasSentCommand("depth");
    }

    bool UCIParser::uiHasSentStopCommand()
    {
        return uiHasSentCommand("stop");
    }

    bool UCIParser::uiHasSentTimeForWhite()
    {
        return uiHasSentCommand("wtime");
    }

    bool UCIParser::uiHasSentTimeForBlack()
    {
        return uiHasSentCommand("btime");
    }

    bool UCIParser::uiHasSentWhiteIncrement()
    {
        return uiHasSentCommand("winc");
    }

    bool UCIParser::uiHasSentBlackIncrement()
    {
        return uiHasSentCommand("binc");
    }

    bool UCIParser::uiHasSentMovesToGo()
    {
        return uiHasSentCommand("movestogo");
    }

    bool UCIParser::uiHasSentMovesTime()
    {
        return uiHasSentCommand("movetime");
    }

    bool UCIParser::uiHasSentInfiniteTime()
    {
        return uiHasSentCommand("infinite");
    }

    bool UCIParser::uiHasSentCommand(std::string_view command)
    {
        if (currentStringView().starts_with(command))
        {
            m_currentPos += command.length();
            skipWhiteSpaces();
            return true;
        }
        return false;
    }

    UCIParser::UCIMove UCIParser::parseMove()
    {
        // Uses long algebraic notation, i.e.
        // - "e2e3"
        // - "e7e8q" (promoting to queen)
        const Square sourceSquare = parseSquare();
        const Square targetSquare = parseSquare();

        bool uiSentLegalPromotion = false;

        if (not isAtEndOfString() and
            currentCharacter() != ' ')
        {
            const char promotionCharacter = currentCharacter();

            uiSentLegalPromotion = promotionCharacter == 'q' or
                                   promotionCharacter == 'r' or
                                   promotionCharacter == 'b' or
                                   promotionCharacter == 'n';

            if (not uiSentLegalPromotion)
            {
                // Illegal Move --> Return Null Move
                return UCIMove{Square::undefined, Square::undefined, false};
            }
        }

        // move iterator to next position
        nextPosition();

        return UCIMove{sourceSquare, targetSquare, uiSentLegalPromotion};
    }
}