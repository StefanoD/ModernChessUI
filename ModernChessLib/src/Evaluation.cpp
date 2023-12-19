#include "ModernChess/Evaluation.h"

#include "ModernChess/PseudoMoveGeneration.h"
#include "ModernChess/MoveExecution.h"

#include <algorithm>

std::ostream &operator<<(std::ostream &os, const ModernChess::EvaluationResult &evalResult)
{
    os << "info score cp " << evalResult.score << " depth " << evalResult.depth << " nodes " <<
    evalResult.numberOfNodes << " pv ";

    for (const ModernChess::Move move : *evalResult.pvTable)
    {
        os << move << " ";
    }

    os << "\n";

    return os;
}

namespace ModernChess
{
    EvaluationResult Evaluation::getBestMove(uint8_t depth)
    {
        m_followPv = true;
        // find best move within a given position
        const int32_t score = negamax(-Infinity, Infinity, depth);

        return EvaluationResult{score, m_numberOfNodes, depth, pvTable};
    }

    bool Evaluation::kingIsInCheck() const
    {
        return kingIsInCheck(m_gameState.board.sideToMove);
    }

    bool Evaluation::kingIsInCheck(Color sideToMove) const
    {
        if (sideToMove == Color::White)
        {
            const Square kingsSquare = BitBoardOperations::bitScanForward(m_gameState.board.bitboards[Figure::WhiteKing]);
            return AttackQueries::squareIsAttackedByBlack(m_gameState.board, kingsSquare);
        }

        const Square kingsSquare = BitBoardOperations::bitScanForward(m_gameState.board.bitboards[Figure::BlackKing]);
        return AttackQueries::squareIsAttackedByWhite(m_gameState.board, kingsSquare);
    }

    std::vector<Move> Evaluation::generateSortedMoves()
    {
        std::vector<Move> moves =  PseudoMoveGeneration::generateMoves(m_gameState);

        if (m_followPv)
        {
            // Has current ply a PV?
            m_scorePv = std::any_of(moves.begin(), moves.end(), [this](const Move move){
                return pvTable->pvTable[m_halfMoveClockRootSearch][m_gameState.halfMoveClock] == move;
            });
            m_followPv = m_scorePv;
        }

        // Sort moves, such that captures with higher scores are evaluated first and makes an early pruning more probable
        // Use also stable_sort, because on capture moves, we insert first the most valuable pieces!
        std::stable_sort(moves.begin(), moves.end(), [this](const Move leftOrderedMove, const Move rightOrderedMove){
            return scoreMove(leftOrderedMove) > scoreMove(rightOrderedMove);
        });

        m_scorePv = false;

        return moves;
    }

    int32_t Evaluation::negamax(int32_t alpha, int32_t beta, uint8_t depth)
    {
        if (const int32_t score = GameState::transpositionTable.getScore(m_gameState.gameStateHash, alpha, beta, depth);
            score != TranspositionTable::NoHashEntryFound &&
            // In the first iteration/move/ply, there is no PV node to be returned, therefore don't return a score for the first ply.
            m_gameState.halfMoveClock > m_halfMoveClockRootSearch
            )
        {
            // Position has already been scored with at least the same depth
            return score;
        }

        // Init PV length
        pvTable->pvLength[m_gameState.halfMoveClock] = m_gameState.halfMoveClock;

        const bool kingInCheck = kingIsInCheck();

        // increase search depth if the king has been exposed into a check
        if (kingInCheck)
        {
            ++depth;
        }

        // recursion escape condition
        if (depth == 0)
        {
            // return evaluation
            return quiescenceSearch(alpha, beta);
        }

        // we are too deep, hence there's an overflow of arrays relying on max half-moves constant
        if (m_gameState.halfMoveClock >= MaxHalfMoves)
        {
            return evaluatePosition();
        }

        // Assume alpha score does not increase
        HashFlag hashFlag = HashFlag::Alpha;

        // increment nodes count
        ++m_numberOfNodes;

        // Null Move Pruning
        // see also https://web.archive.org/web/20071031095933/http://www.brucemo.com/compchess/programming/nullmove.htm
        if (m_allowNullMove && depth >= 3 &&
            not kingInCheck &&
            m_gameState.halfMoveClock > m_halfMoveClockRootSearch &&
            not isEndGame() // Null Move Pruning does not work for end games
            )
        {
            m_allowNullMove = false; // Don't allow consecutive null moves
            // preserve board state
            const GameState gameStateCopy = m_gameState;

            // remove en passant square from hash key if available, because the new move invalidates it
            if (m_gameState.board.enPassantTarget != Square::undefined)
            {
                m_gameState.gameStateHash ^= ZobristHasher::enpassantKeys[m_gameState.board.enPassantTarget];
            }

            // reset en-passant capture square, because opponent missed the chance in the last move
            m_gameState.board.enPassantTarget = Square::undefined;

            // switch the side, literally giving opponent an extra move to make
            m_gameState.board.sideToMove = Color(!bool(m_gameState.board.sideToMove));
            m_gameState.gameStateHash ^= ZobristHasher::sideKey;

            // search moves with reduced depth to find beta cutoffs (depth - 1 - R) where R is a depth reduction
            const int32_t score = -negamax(-beta, -beta + 1, depth - 1 - NullMovePruningDepthReduction);

            // restore board state
            m_gameState = gameStateCopy;

            // fail-hard beta cutoff
            if (score >= beta)
            {
                // node (move) fails high
                GameState::transpositionTable.addEntry(m_gameState.gameStateHash, HashFlag::Beta, score, depth);
                return beta;
            }
        }

        // create move list instance
        const std::vector<Move> moves = generateSortedMoves();

        uint32_t movesSearched = 0;

        // legal moves counter
        uint32_t legalMoves = 0;

        // loop over moves within a move list
        for (const Move move : moves)
        {
            m_allowNullMove = true;

            // preserve board state
            const GameState gameStateCopy = m_gameState;

            // make sure to make only legal moves
            if (not MoveExecution::executeMove(m_gameState, move, MoveType::AllMoves))
            {
                // skip to next move
                continue;
            }

            ++legalMoves;

            int32_t score;

            // full depth search in order to get a PV node
            if (movesSearched == 0)
            {
                // do normal alpha beta search
                score = -negamax(-beta, -alpha, depth - 1);
            }
            else
            {
                // late move reduction (LMR) & Principal Variation Search
                // @see https://www.chessprogramming.org/Late_Move_Reductions
                // @see https://web.archive.org/web/20150212051846/http://www.glaurungchess.com/lmr.html
                // condition to consider LMR
                if (movesSearched > NumberOfMovesForFullDepthSearch &&
                    depth > MinimumDepthForFullDepthSearch &&
                    not kingInCheck &&
                    not move.isCapture() &&
                    move.getPromotedPiece() == Figure::None &&
                    not kingIsInCheck(Color(!bool(m_gameState.board.sideToMove)))) // Also opponent must not be in check
                {
                    // search current move with reduced depth:
                    score = -negamax(-(alpha + 1), -alpha, depth - 2);
                }
                else
                {
                    // hack to ensure that full-depth search is done
                    score = alpha + 1;
                }

                // principle variation search (PVS)
                if (score > alpha)
                {
                    // Algorithm is from here:
                    // https://web.archive.org/web/20071030220825/http://www.brucemo.com/compchess/programming/pvs.htm

                    // Once you've found a move with a score that is between alpha and beta,
                    // the rest of the moves are searched with the goal of proving that they are all bad.
                    // It's possible to do this a bit faster than a search that worries that one
                    // of the remaining moves might be good.
                    score = -negamax(-(alpha + 1), -alpha , depth - 1);

                    // If the algorithm finds out that it was wrong, and that one of the
                    // subsequent moves was better than the first PV move, it has to search again,
                    // in the normal alpha-beta manner. This happens sometimes, and it's a waste of time,
                    // but generally not often enough to counteract the savings gained from doing the
                    // "bad move proof" search referred to earlier.
                    const bool foundBetterPV = (score > alpha) && (score < beta);
                    if (foundBetterPV) // Check for failure.
                    {
                        // re-search the move that has failed to be proved to be bad
                        // with normal alpha beta score bounds
                        score = -negamax(-beta, -alpha, depth - 1);
                    }
                }
            }

            m_allowNullMove = true;
            // take move back
            m_gameState = gameStateCopy;

            ++movesSearched;

            // fail-hard beta cutoff
            if (score >= beta)
            {
                if (not move.isCapture())
                {
                    // store killer moves for later reuse
                    m_killerMoves[1][m_gameState.halfMoveClock] = m_killerMoves[0][m_gameState.halfMoveClock]; // old killer move
                    m_killerMoves[0][m_gameState.halfMoveClock] = move; // new and better killer move
                }

                GameState::transpositionTable.addEntry(m_gameState.gameStateHash, HashFlag::Beta, score, depth);

                // node (move) fails high
                return beta;
            }

            // found a better move
            if (score > alpha)
            {
                if (not move.isCapture())
                {
                    // store history moves.
                    // The added value is typically depth * depth or 2 ^ depth,
                    // based on the assumption that otherwise moves from the plies near the leaves would have too
                    // much impact on the result.
                    m_historyMoves[move.getMovedFigure()][move.getTo()] += (depth * depth);
                }

                // PV node (move)
                alpha = score;
                hashFlag = HashFlag::Exact; // Store PV node
                pvTable->addPrincipalVariation(move, m_gameState.halfMoveClock);
            }

            if (m_stopSearching())
            {
                break;
            }
        }

        // we don't have any legal moves to make in the current position
        if (legalMoves == 0)
        {
            // king is in check
            if (kingInCheck)
            {
                // return mating score (assuming closest distance to mating position)
                alpha = CheckMateScore + m_gameState.halfMoveClock;
            }
            else
            {
                // king is not in check
                // return stalemate score
                alpha = StaleMateScore;
            }
        }

        GameState::transpositionTable.addEntry(m_gameState.gameStateHash, hashFlag, alpha, depth);

        // node (move) fails low
        return alpha;
    }

    int32_t Evaluation::quiescenceSearch(int32_t alpha, int32_t beta)
    {
        ++m_numberOfNodes;

        // evaluate position
        const int32_t evaluation = evaluatePosition();

        // fail-hard beta cutoff
        if (evaluation >= beta)
        {
            // node (move) fails high
            return beta;
        }

        // we are too deep, hence there's an overflow of arrays relying on max half-moves constant
        if (m_gameState.halfMoveClock >= MaxHalfMoves)
        {
            return evaluation;
        }

        // Assume alpha score does not increase
        //HashFlag hashFlag = HashFlag::Alpha;

        // found a better move
        if (evaluation > alpha)
        {
            //hashFlag = HashFlag::Exact; // Store PV node
            // PV node (move)
            alpha = evaluation;
        }

        const std::vector<Move> moves = generateSortedMoves();

        // loop over moves within a move list
        for (const Move move : moves)
        {
            // preserve board state
            const GameState gameStateCopy = m_gameState;

            // make sure to make only legal moves
            if (not MoveExecution::executeMove(m_gameState, move, MoveType::CapturesOnly))
            {
                // skip to next move
                continue;
            }

            // score current move
            const int32_t score = -quiescenceSearch(-beta, -alpha);

            // take move back
            m_gameState = gameStateCopy;

            // fail-hard beta cutoff
            if (score >= beta)
            {
                //GameState::transpositionTable.addEntry(m_gameState.gameStateHash, HashFlag::Beta, score, 0);
                // node (move) fails high
                return beta;
            }

            // found a better move
            if (score > alpha)
            {
                //hashFlag = HashFlag::Exact; // Store PV node
                // PV node (move)
                alpha = score;
            }

            if (m_stopSearching())
            {
                break;
            }
        }

        //GameState::transpositionTable.addEntry(m_gameState.gameStateHash, hashFlag, alpha, 0);

        // node (move) fails low
        return alpha;
    }

    int32_t Evaluation::evaluatePosition() const
    {
        // static evaluation score
        int32_t score = 0;

        // loop over piece bitboards
        for (Figure figure = Figure::WhitePawn; figure <= Figure::BlackKing; ++figure)
        {
            // loop over figure within a bitboard

            for (BitBoardState bitboard = m_gameState.board.bitboards[figure]; bitboard != BoardState::empty; )
            {
                const Square square = BitBoardOperations::bitScanForward(bitboard);

                // score material weights
                score += materialScore[figure];

                // score positional piece scores
                switch (figure)
                {
                    // evaluate white pieces
                    case Figure::WhitePawn: score += pawnScore[square]; break;
                    case Figure::WhiteKnight: score += knightScore[square]; break;
                    case Figure::WhiteBishop: score += bishopScore[square]; break;
                    case Figure::WhiteRook: score += rookScore[square]; break;
                    case Figure::WhiteKing: score += kingScore[square]; break;

                        // evaluate black pieces
                    case Figure::BlackPawn: score -= pawnScore[mirrorScore[square]]; break;
                    case Figure::BlackKnight: score -= knightScore[mirrorScore[square]]; break;
                    case Figure::BlackBishop: score -= bishopScore[mirrorScore[square]]; break;
                    case Figure::BlackRook: score -= rookScore[mirrorScore[square]]; break;
                    case Figure::BlackKing: score -= kingScore[mirrorScore[square]]; break;
                    default:
                        // Ignore queen
                        break;
                }

                // pop ls1b
                bitboard = BitBoardOperations::eraseSquare(bitboard, square);
            }
        }

        // return final evaluation based on side
        return (m_gameState.board.sideToMove == Color::White) ? score : -score;
    }

    int32_t Evaluation::scoreMove(Move move)
    {
        // make sure we are dealing with PV move
        if (m_scorePv && pvTable->pvTable[m_halfMoveClockRootSearch][m_gameState.halfMoveClock] == move)
        {
            // give PV move the highest score to search it first
            return PvScore;
        }

        // score capture move
        if (move.isCapture())
        {
            // init target figure with white pawn or black pawn (doesn't matter which one,
            // because scoring with the same color yields the same valid score), in case of en-passant captures
            Figure targetFigure = Figure::WhitePawn;

            // pick up bitboard figure index ranges depending on side
            Figure opponentsFigureStart;
            Figure opponentsFigureEnd;

            if (m_gameState.board.sideToMove == Color::White)
            {
                opponentsFigureStart = Figure::BlackPawn;
                opponentsFigureEnd = Figure::BlackKing;
            }
            else
            {
                opponentsFigureStart = Figure::WhitePawn;
                opponentsFigureEnd = Figure::BlackKing;
            }

            for (Figure figure = opponentsFigureStart; figure <= opponentsFigureEnd; ++figure)
            {
                if (BitBoardOperations::isOccupied(m_gameState.board.bitboards[figure], move.getTo()))
                {
                    targetFigure = figure;
                    break;
                }
            }

            // score move by MVV LVA lookup [source piece][target piece]
            return mvvLva[move.getMovedFigure()][targetFigure] + CaptureScoreOffset;
        }

        // score 1st killer move
        if (m_killerMoves[0][m_gameState.halfMoveClock] == move)
        {
            return BestKillerMoveScore;
        }

        // score 2nd killer move
        if (m_killerMoves[1][m_gameState.halfMoveClock] == move)
        {
            return SecondBestKillerMoveScore;
        }

        return m_historyMoves[move.getMovedFigure()][move.getTo()];
    }

    bool Evaluation::isEndGame() const
    {
        return BitBoardOperations::countBits(m_gameState.board.occupancies[Color::White]) <= NumberOfFiguresForEndGameDefinition or
               BitBoardOperations::countBits(m_gameState.board.occupancies[Color::Black]) <= NumberOfFiguresForEndGameDefinition;
    }
}