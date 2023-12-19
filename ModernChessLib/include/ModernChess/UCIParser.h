#pragma once

#include "BasicParser.h"
#include "Move.h"

#include <string_view>

namespace ModernChess
{
    class UCIParser : public BasicParser
    {
    public:
        struct UCIMove {
            explicit UCIMove(Square sourceSquare,
                             Square targetSquare,
                             bool legalPromotionCharacter) :
                             sourceSquare(sourceSquare),
                             targetSquare(targetSquare),
                             legalPromotionCharacter(legalPromotionCharacter)
            {};
            Square sourceSquare{};
            Square targetSquare{};
            bool legalPromotionCharacter{};
        };

        explicit UCIParser(std::string_view uiCommand);

        [[nodiscard]] bool uiRequestsUCIMode() const;

        [[nodiscard]] bool uiRequestsNewGame() const;

        [[nodiscard]] bool uiIsReady() const;

        [[nodiscard]] bool uiQuitGame() const;

        [[nodiscard]] bool uiHasSentPosition();

        [[nodiscard]] bool uiHasSentMoves();

        [[nodiscard]] bool uiHasSentFENPosition();

        [[nodiscard]] bool uiHasSentStartingPosition();

        [[nodiscard]] bool uiHasSentGoCommand();

        [[nodiscard]] bool uiHasSentSearchDepth();

        [[nodiscard]] bool uiHasSentStopCommand();

        [[nodiscard]] bool uiHasSentWhiteIncrement();

        [[nodiscard]] bool uiHasSentBlackIncrement();

        [[nodiscard]] bool uiHasSentTimeForWhite();

        [[nodiscard]] bool uiHasSentTimeForBlack();

        [[nodiscard]] bool uiHasSentMovesToGo();

        [[nodiscard]] bool uiHasSentMovesTime();

        [[nodiscard]] bool uiHasSentInfiniteTime();

        [[nodiscard]] UCIMove parseMove();

    private:
        [[nodiscard]] bool uiHasSentCommand(std::string_view command);
    };
}