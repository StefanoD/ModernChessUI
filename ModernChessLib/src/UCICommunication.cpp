#include "ModernChess/UCICommunication.h"
#include "ModernChess/MoveExecution.h"
#include "ModernChess/PseudoMoveGeneration.h"
#include "ModernChess/UCIParser.h"
#include "ModernChess/FenParsing.h"
#include "ModernChess/Evaluation.h"

#include <string>

using ModernChess::FenParsing::FenParser;
using namespace std::chrono_literals;

namespace ModernChess
{
    UCICommunication::UCICommunication(std::istream &inputStream, std::ostream &outputStream, std::ostream &errorStream) :
            m_inputStream(inputStream),
            m_outputStream(outputStream),
            m_errorStream(errorStream),
            m_searchThread(&UCICommunication::searchBestMove, this)
    {}

    UCICommunication::~UCICommunication()
    {
        quitGame();

        if (m_searchThread.joinable())
        {
            m_searchThread.join();
        }
    }

    void UCICommunication::startCommunication()
    {
        registerToUI();

        std::string uiCommand;

        for (getInput(uiCommand); ;getInput(uiCommand))
        {
            if (uiCommand.empty())
            {
                continue;
            }
            // make sure uiCommand is available
            UCIParser parser(uiCommand);

            if (parser.uiQuitGame())
            {
                quitGame();
                break;
            }

            if (parser.uiIsReady())
            {
                sendAcknowledgeToUI();
                continue;
            }

            if (parser.uiHasSentPosition())
            {
                parsePosition(parser);
                continue;
            }

            if (parser.uiRequestsNewGame())
            {
                createNewGame();
            }
            else if (parser.uiHasSentGoCommand())
            {
                executeGoCommand(parser);
            }
            else if (parser.uiHasSentStopCommand())
            {
                stopSearch();
            }
            else if (parser.uiRequestsUCIMode())
            {
                registerToUI();
            }
            else
            {
                m_errorStream << "Unknown command: " << parser.currentStringView() << std::endl << std::flush;
            }
        }

        if (m_searchThread.joinable())
        {
            m_searchThread.join();
        }
    }

    void UCICommunication::registerToUI()
    {
        m_outputStream << "id name Modern Chess\n"
                       << "id author Stefano Di Martino\n"
                       << "uciok\n" << std::flush;
    }

    void UCICommunication::sendAcknowledgeToUI()
    {
        m_outputStream << "readyok\n" << std::flush;
    }

    void UCICommunication::getInput(std::string &uiCommand)
    {
        m_inputStream.clear(); // Clear errors
        std::getline(m_inputStream, uiCommand);
    }

    void UCICommunication::parsePosition(UCIParser &parser)
    {
        if (parser.uiHasSentStartingPosition())
        {
            createNewGame();
        }
        else if (parser.uiHasSentFENPosition())
        {
            FenParser fenParser(parser.currentStringView());
            setGameState(fenParser.parse());

            // There might be consecutive move commands. So continue with parsing...
            parser = UCIParser(fenParser.currentStringView());
        }
        else
        {
            throw std::runtime_error("Missing position after position command: " +
                                      std::string(parser.completeStringView()));
        }

        if (parser.uiHasSentMoves())
        {
            const std::string_view moveCommand = parser.currentStringView();
            const std::lock_guard lock(m_mutex); // Add mutex for game state change

            while (parser.hasNextCharacter())
            {
                const Move move = executeMoves(parser);

                if (not move.isNullMove())
                {
                    MoveExecution::executeMove(m_searchRequest.gameState, move, MoveType::AllMoves);
                }
                else
                {
                    m_errorStream << "Illegal move detected: " << moveCommand << std::endl;
                    break;
                }
            }
        }
    }

    void UCICommunication::createNewGame()
    {
        setGameState(FenParser(FenParsing::startPosition).parse());
    }

    Move UCICommunication::executeMoves(UCIParser &parser) const
    {
        const UCIParser::UCIMove uciMove = parser.parseMove();
        const std::vector<Move> possibleMovesFromCurrentSate = PseudoMoveGeneration::generateMoves(m_searchRequest.gameState);

        for (const Move move : possibleMovesFromCurrentSate)
        {
            if (move.getFrom() == uciMove.sourceSquare and
                move.getTo() == uciMove.targetSquare)
            {
                if (const Figure promotedPiece = move.getPromotedPiece();
                        promotedPiece != Figure::None)
                {
                    if (uciMove.legalPromotionCharacter)
                    {
                        return move;
                    }
                    // Illegal Move --> Return Null Move
                    return {};
                }
                return move;
            }
        }

        // Illegal Move --> Return Null Move
        return {};
    }

    void UCICommunication::executeGoCommand(UCIParser &parser)
    {
        std::chrono::milliseconds timeToSearch = -1ms;
        std::chrono::milliseconds timeIncrement = 0ms;
        int64_t movesToGo = 1;

        while (parser.hasNextCharacter())
        {
            if (parser.uiHasSentSearchDepth())
            {
                const std::lock_guard lock(m_mutex);
                m_searchRequest.depth = parser.parseNumber<uint8_t>();
            }
            if (parser.uiHasSentMovesTime())
            {
                timeToSearch = std::chrono::milliseconds(parser.parseNumber<int64_t>());
            }
            if (parser.uiHasSentTimeForWhite())
            {
                // Even is side to move is not white, the value has to be read by the parser, so the
                // string_view-pointer can continue to the next characters
                if (auto timeValue = std::chrono::milliseconds(parser.parseNumber<int64_t>());
                        m_searchRequest.gameState.board.sideToMove == Color::White)
                {
                    // Only set, if side to move is white
                    timeToSearch = timeValue;
                }
            }
            if (parser.uiHasSentTimeForBlack())
            {
                // Even is side to move is not black, the value has to be read by the parser, so the
                // string_view-pointer can continue to the next characters
                if (auto timeValue = std::chrono::milliseconds(parser.parseNumber<int64_t>());
                        m_searchRequest.gameState.board.sideToMove == Color::Black)
                {
                    // Only set, if side to move is black
                    timeToSearch = timeValue;
                }
            }
            if (parser.uiHasSentWhiteIncrement())
            {
                // Even is side to move is not white, the value has to be read by the parser, so the
                // string_view-pointer can continue to the next characters
                if (auto timeValue = std::chrono::milliseconds(parser.parseNumber<int64_t>());
                        m_searchRequest.gameState.board.sideToMove == Color::White)
                {
                    // Only set, if side to move is white
                    timeIncrement = timeValue;
                }
            }
            if (parser.uiHasSentBlackIncrement())
            {
                // Even is side to move is not black, the value has to be read by the parser, so the
                // string_view-pointer can continue to the next characters
                if (auto timeValue = std::chrono::milliseconds(parser.parseNumber<int64_t>());
                        m_searchRequest.gameState.board.sideToMove == Color::Black)
                {
                    // Only set, if side to move is black
                    timeIncrement = timeValue;
                }
            }
            if (parser.uiHasSentMovesToGo())
            {
                movesToGo = parser.parseNumber<int64_t>();
            }
            if (parser.uiHasSentInfiniteTime())
            {
                timeToSearch = InfiniteTime;
            }

            parser.skipWhiteSpaces();
        }

        if (timeToSearch == -1ms)
        {
            // Time has not been set. Set infinite time
            timeToSearch = InfiniteTime;
        }

        // If we have infinite time, these calculations don't really have a practical affect
        timeToSearch /= movesToGo;
        timeToSearch += (timeIncrement - TimeSecurityMargin);

        {
            const std::lock_guard lock(m_mutex);
            m_stopped = false;
            m_searchRequest.timePointToStopSearch = std::chrono::steady_clock::now() + timeToSearch;
        }

        m_waitForSearchRequest.notifyOne();
    }

    void UCICommunication::searchBestMove()
    {
        auto stopCondition = [this] { return searchHasBeenStopped(); };

        while (not gameHasBeenQuit())
        {
            {
                std::unique_lock lock(m_mutex);
                m_waitForSearchRequest.wait(lock, [this]{
                    return (not m_stopped) or m_quit;
                });
            }

            Evaluation evaluation(getGameState(), stopCondition);
            EvaluationResult evalResult;

            uint8_t depth;

            {
                const std::lock_guard lock(m_mutex);
                depth = m_searchRequest.depth;
            }

            for (uint8_t currentDepth = 1; currentDepth <= depth && (not searchHasBeenStopped()); ++currentDepth)
            {
                evalResult = evaluation.getBestMove(currentDepth);
                m_outputStream << evalResult << std::flush;
            }

            if (evalResult.pvTable != nullptr)
            {
                m_outputStream << "bestmove " << evalResult.bestMove() << "\n" << std::flush;
            }

            stopSearch();
        }
    }

    void UCICommunication::stopSearch()
    {
        const std::lock_guard lock(m_mutex);
        m_stopped = true;
    }

    void UCICommunication::quitGame()
    {
        {
            const std::lock_guard lock(m_mutex);
            m_stopped = true;
            m_quit = true;
        }
        m_waitForSearchRequest.notifyOne();
    }

    bool UCICommunication::searchHasBeenStopped() const
    {
        const std::lock_guard lock(m_mutex);
        return m_stopped or (m_searchRequest.timePointToStopSearch < std::chrono::steady_clock::now());
    }

    bool UCICommunication::gameHasBeenQuit() const
    {
        const std::lock_guard lock(m_mutex);
        return m_quit;
    }

    void UCICommunication::setGameState(GameState gameState)
    {
        const std::lock_guard lock(m_mutex);
        m_searchRequest = SearchRequest(gameState);
    }
}