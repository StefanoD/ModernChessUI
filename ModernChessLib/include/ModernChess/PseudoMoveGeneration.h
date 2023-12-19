#pragma once

#include "AttackQueries.h"
#include "GameState.h"
#include "BitBoardOperations.h"
#include "Move.h"

#include <vector>
#include <functional>

namespace ModernChess
{
    /**
     * @brief Leaves king sometimes in check. There it's only a pseudo-move-generation
     */
    class PseudoMoveGeneration
    {
    public:
        PseudoMoveGeneration() = delete;

        [[nodiscard]] static std::vector<Move> generateMoves(const GameState &gameState)
        {
            std::vector<Move> movesToBeGenerated;
            movesToBeGenerated.reserve(256);

            if (gameState.board.sideToMove == Color::White)
            {
                PseudoMoveGeneration::generateWhiteFigureMoves(gameState, movesToBeGenerated);
            }
            else
            {
                PseudoMoveGeneration::generateBlackFigureMoves(gameState, movesToBeGenerated);
            }

            return movesToBeGenerated;
        }

        static void generateBlackFigureMoves(const GameState &gameState, std::vector<Move> &movesToBeGenerated)
        {
            generateBlackPawnMoves(gameState, movesToBeGenerated);
            generateBlackKingMoves(gameState, movesToBeGenerated);

            {
                // Black Knight Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::knightAttackTable[sourceSquare] &
                           (~gameState.board.occupancies[Color::Black]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::White, Figure::BlackKnight,
                                   std::move(getAttacks));
            }
            {
                // Black Bishop Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::bishopAttacks.getAttacks(sourceSquare,
                                                                   gameState.board.occupancies[Color::Both]) &
                           (~gameState.board.occupancies[Color::Black]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::White, Figure::BlackBishop,
                                   std::move(getAttacks));
            }
            {
                // Black Rook Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::rookAttacks.getAttacks(sourceSquare,
                                                                 gameState.board.occupancies[Color::Both]) &
                           (~gameState.board.occupancies[Color::Black]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::White, Figure::BlackRook,
                                   std::move(getAttacks));
            }
            {
                // Black Queen Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::queenAttacks.getAttacks(sourceSquare,
                                                                  gameState.board.occupancies[Color::Both]) &
                           (~gameState.board.occupancies[Color::Black]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::White, Figure::BlackQueen,
                                   std::move(getAttacks));
            }
        }

        static void generateWhiteFigureMoves(const GameState &gameState, std::vector<Move> &movesToBeGenerated)
        {
            generateWhitePawnMoves(gameState, movesToBeGenerated);
            generateWhiteKingMoves(gameState, movesToBeGenerated);

            {
                // White Knight Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::knightAttackTable[sourceSquare] &
                           (~gameState.board.occupancies[Color::White]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::Black, Figure::WhiteKnight,
                                   std::move(getAttacks));
            }
            {
                // White Bishop Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::bishopAttacks.getAttacks(sourceSquare,
                                                                   gameState.board.occupancies[Color::Both]) &
                           (~gameState.board.occupancies[Color::White]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::Black, Figure::WhiteBishop,
                                   std::move(getAttacks));
            }
            {
                // White Rook Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::rookAttacks.getAttacks(sourceSquare,
                                                                 gameState.board.occupancies[Color::Both]) &
                           (~gameState.board.occupancies[Color::White]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::Black, Figure::WhiteRook,
                                   std::move(getAttacks));
            }
            {
                // White Queen Moves
                std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
                {
                    return AttackQueries::queenAttacks.getAttacks(sourceSquare,
                                                                  gameState.board.occupancies[Color::Both]) &
                           (~gameState.board.occupancies[Color::White]);
                };

                generatePieceMoves(gameState, movesToBeGenerated, Color::Black, Figure::WhiteQueen,
                                   std::move(getAttacks));
            }
        }

    private:
        static void generateWhitePawnMoves(const GameState &gameState, std::vector<Move> &movesToBeGenerated)
        {
            BitBoardState whitePawnBitboard = gameState.board.bitboards[Figure::WhitePawn];

            // loop over white pawns within white pawn bitboard
            while (whitePawnBitboard != BoardState::empty)
            {
                const Square sourceSquare = BitBoardOperations::bitScanForward(whitePawnBitboard);

                {
                    // White pawns are moving towards north direction
                    // generate quite pawn moves
                    const Square targetSquare = BitBoardOperations::getNorthSquareFromGivenSquare(sourceSquare);

                    if (const bool targetSquareIsOnBoard = (targetSquare <= Square::h8);
                            targetSquareIsOnBoard &&
                            !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], targetSquare))
                    {
                        // pawn promotion
                        if (sourceSquare >= a7 && sourceSquare <= h7)
                        {
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                            Figure::WhiteQueen,
                                                            false, false, false, false);
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                            Figure::WhiteRook,
                                                            false, false, false, false);
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                            Figure::WhiteBishop,
                                                            false, false, false, false);
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                            Figure::WhiteKnight,
                                                            false, false, false, false);
                        }
                        else
                        {
                            // single pawn push
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn, Figure::None,
                                                            false, false, false, false);

                            // double pawn push
                            if ((sourceSquare >= a2 && sourceSquare <= h2) &&
                                !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both],
                                                                BitBoardOperations::getNorthSquareFromGivenSquare(
                                                                        targetSquare)))
                            {
                                movesToBeGenerated.emplace_back(sourceSquare,
                                                                BitBoardOperations::getNorthSquareFromGivenSquare(
                                                                        targetSquare), Figure::WhitePawn, Figure::None,
                                                                false, true, false, false);
                            }
                        }
                    }
                }

                // init pawn attacks of whitePawnBitboard and generate pawn captures
                for (BitBoardState attacks = AttackQueries::pawnAttackTable[Color::White][sourceSquare] &
                                             gameState.board.occupancies[Color::Black];
                     attacks != BoardState::empty;
                        )
                {
                    // init target square
                    const Square targetSquare = BitBoardOperations::bitScanForward(attacks);

                    // pawn promotion
                    if (sourceSquare >= a7 && sourceSquare <= h7)
                    {
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                        Figure::WhiteQueen,
                                                        true, false, false, false);
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                        Figure::WhiteRook,
                                                        true, false, false, false);
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                        Figure::WhiteBishop,
                                                        true, false, false, false);
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn,
                                                        Figure::WhiteKnight,
                                                        true, false, false, false);
                    }
                    else
                    {
                        // one square ahead pawn move
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::WhitePawn, Figure::None,
                                                        true,
                                                        false, false, false);
                    }

                    attacks = BitBoardOperations::eraseSquare(attacks, targetSquare);
                }

                // generate en passant captures
                if (gameState.board.enPassantTarget != Square::undefined)
                {
                    // lookup pawn attacks and bitwise AND with en passant square (bit)
                    const BitBoardState enPassantAttacks =
                            AttackQueries::pawnAttackTable[Color::White][sourceSquare] &
                            BitBoardOperations::occupySquare(BoardState::empty, gameState.board.enPassantTarget);

                    // make sure en passant capture possible
                    if (enPassantAttacks != BoardState::empty)
                    {
                        // init en passant capture target square
                        const Square targetEnPassant = BitBoardOperations::bitScanForward(enPassantAttacks);
                        movesToBeGenerated.emplace_back(sourceSquare, targetEnPassant, Figure::WhitePawn, Figure::None,
                                                        true, false, true, false);
                    }
                }

                // pop ls1b from figure whitePawnBitboard copy
                whitePawnBitboard = BitBoardOperations::eraseSquare(whitePawnBitboard, sourceSquare);
            }
        }

        static void generateWhiteKingMoves(const GameState &gameState, std::vector<Move> &movesToBeGenerated)
        {
            // king side castling is available
            if (whiteCanCastleKingSide(gameState.board.castlingRights))
            {
                // make sure square between king and king's rook are empty
                if (!BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], f1) &&
                    !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], g1))
                {
                    // make sure king and the f1 squares are not under attacks.
                    // The g1 square will be checked in the executeMove() function due to performance reasons
                    if (!AttackQueries::squareIsAttackedByBlack(gameState.board, Square::e1) &&
                        !AttackQueries::squareIsAttackedByBlack(gameState.board, Square::f1))
                    {
                        movesToBeGenerated.emplace_back(Square::e1, Square::g1, Figure::WhiteKing, Figure::None, false,
                                                        false, false, true);
                        // The rook move to f1 is generated in the rook move generation
                    }
                }
            }

            // queen side castling is available
            if (whiteCanCastleQueenSide(gameState.board.castlingRights))
            {
                // make sure square between king and queen's rook are empty
                if (!BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], d1) &&
                    !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], c1) &&
                    !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], b1))
                {
                    // make sure king and the d1 squares are not under attacks
                    // The c1 square will be checked in the executeMove() function due to performance reasons
                    if (!AttackQueries::squareIsAttackedByBlack(gameState.board, e1) &&
                        !AttackQueries::squareIsAttackedByBlack(gameState.board, d1))
                    {
                        movesToBeGenerated.emplace_back(Square::e1, Square::c1, Figure::WhiteKing, Figure::None, false,
                                                        false, false, true);
                        // The rook move to d1 is generated in the rook move generation
                    }
                }
            }

            std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
            {
                return AttackQueries::kingAttackTable[sourceSquare] & (~gameState.board.occupancies[Color::White]);
            };

            generatePieceMoves(gameState, movesToBeGenerated, Color::Black, Figure::WhiteKing, std::move(getAttacks));
        }

        static void generatePieceMoves(const GameState &gameState,
                                       std::vector<Move> &movesToBeGenerated,
                                       Color opponentsColor,
                                       Figure piece,
                                       std::function<BitBoardState(Square)> getAttacks)
        {
            BitBoardState pieceBitboard = gameState.board.bitboards[piece];

            // loop over source squares of piece pieceBitboard copy
            while (pieceBitboard != BoardState::empty)
            {
                const Square sourceSquare = BitBoardOperations::bitScanForward(pieceBitboard);

                // init piece attacks in order to get set of target squares
                BitBoardState attacks = getAttacks(sourceSquare);

                // loop over target squares available from generated attacks
                while (attacks != BoardState::empty)
                {
                    // init target square
                    const Square targetSquare = BitBoardOperations::bitScanForward(attacks);

                    if (BitBoardOperations::isOccupied(gameState.board.occupancies[opponentsColor], targetSquare))
                    {
                        // capture move
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, piece, Figure::None, true, false,
                                                        false, false);
                    }
                    else
                    {
                        // quiet move
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, piece, Figure::None, false, false,
                                                        false, false);
                    }

                    // pop ls1b in current attacks set
                    attacks = BitBoardOperations::eraseSquare(attacks, targetSquare);
                }

                // pop ls1b of the current piece pieceBitboard copy
                pieceBitboard = BitBoardOperations::eraseSquare(pieceBitboard, sourceSquare);
            }
        }

        static void generateBlackPawnMoves(const GameState &gameState, std::vector<Move> &movesToBeGenerated)
        {
            BitBoardState blackPawnBitboard = gameState.board.bitboards[Figure::BlackPawn];

            // loop over white pawns within black pawn bitboard
            while (blackPawnBitboard != BoardState::empty)
            {
                const Square sourceSquare = BitBoardOperations::bitScanForward(blackPawnBitboard);

                {
                    // Black pawns are moving towards south direction
                    // generate quite pawn moves
                    const Square targetSquare = BitBoardOperations::getSouthSquareFromGivenSquare(sourceSquare);

                    if (const bool targetSquareIsOnBoard = (targetSquare >= Square::a1);
                            targetSquareIsOnBoard &&
                            !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], targetSquare))
                    {
                        // pawn promotion
                        if (sourceSquare >= a2 && sourceSquare <= h2)
                        {
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                            Figure::BlackQueen,
                                                            false, false, false, false);
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                            Figure::BlackRook,
                                                            false, false, false, false);
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                            Figure::BlackBishop,
                                                            false, false, false, false);
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                            Figure::BlackKnight,
                                                            false, false, false, false);
                        }
                        else
                        {
                            // single pawn push
                            movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn, Figure::None,
                                                            false, false, false, false);

                            // double pawn push
                            if ((sourceSquare >= a7 && sourceSquare <= h7) &&
                                !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both],
                                                                BitBoardOperations::getSouthSquareFromGivenSquare(
                                                                        targetSquare)))
                            {
                                movesToBeGenerated.emplace_back(sourceSquare,
                                                                BitBoardOperations::getSouthSquareFromGivenSquare(
                                                                        targetSquare), Figure::BlackPawn, Figure::None,
                                                                false, true, false, false);
                            }
                        }
                    }
                }

                // init pawn attacks of blackPawnBitboard and generate pawn captures
                for (BitBoardState attacks = AttackQueries::pawnAttackTable[Color::Black][sourceSquare] &
                                             gameState.board.occupancies[Color::White];
                     attacks != BoardState::empty;
                        )
                {
                    // init target square
                    const Square targetSquare = BitBoardOperations::bitScanForward(attacks);

                    // pawn promotion
                    if (sourceSquare >= a2 && sourceSquare <= h2)
                    {
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                        Figure::BlackQueen,
                                                        true, false, false, false);
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                        Figure::BlackRook,
                                                        true, false, false, false);
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                        Figure::BlackBishop,
                                                        true, false, false, false);
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn,
                                                        Figure::BlackKnight,
                                                        true, false, false, false);
                    }
                    else
                    {
                        // one square ahead pawn move
                        movesToBeGenerated.emplace_back(sourceSquare, targetSquare, Figure::BlackPawn, Figure::None,
                                                        true,
                                                        false, false, false);
                    }

                    attacks = BitBoardOperations::eraseSquare(attacks, targetSquare);
                }

                // generate en passant captures
                if (gameState.board.enPassantTarget != Square::undefined)
                {
                    // lookup pawn attacks and bitwise AND with en passant square (bit)
                    const BitBoardState enPassantAttacks =
                            AttackQueries::pawnAttackTable[Color::Black][sourceSquare] &
                            BitBoardOperations::occupySquare(BoardState::empty, gameState.board.enPassantTarget);

                    // make sure en passant capture possible
                    if (enPassantAttacks != BoardState::empty)
                    {
                        // init en passant capture target square
                        const Square targetEnPassant = BitBoardOperations::bitScanForward(enPassantAttacks);
                        movesToBeGenerated.emplace_back(sourceSquare, targetEnPassant, Figure::BlackPawn, Figure::None,
                                                        true, false, true, false);
                    }
                }

                // pop ls1b from figure blackPawnBitboard copy
                blackPawnBitboard = BitBoardOperations::eraseSquare(blackPawnBitboard, sourceSquare);
            }
        }

        static void generateBlackKingMoves(const GameState &gameState, std::vector<Move> &movesToBeGenerated)
        {
            // king side castling is available
            if (blackCanCastleKingSide(gameState.board.castlingRights))
            {
                // make sure square between king and king's rook are empty
                if (!BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], f8) &&
                    !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], g8))
                {
                    // make sure king and the f8 squares are not under attacks.
                    // The g1 square will be checked in the executeMove() function due to performance reasons
                    if (!AttackQueries::squareIsAttackedByWhite(gameState.board, Square::e8) &&
                        !AttackQueries::squareIsAttackedByWhite(gameState.board, Square::f8))
                    {
                        movesToBeGenerated.emplace_back(Square::e8, Square::g8, Figure::BlackKing, Figure::None, false,
                                                        false, false, true);
                        // The rook move to f1 is generated in the rook move generation
                    }
                }
            }

            // queen side castling is available
            if (blackCanCastleQueenSide(gameState.board.castlingRights))
            {
                // make sure square between king and queen's rook are empty
                if (!BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], d8) &&
                    !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], c8) &&
                    !BitBoardOperations::isOccupied(gameState.board.occupancies[Color::Both], b8))
                {
                    // make sure king and the d8 squares are not under attacks
                    // The c8 square will be checked in the executeMove() function due to performance reasons
                    if (!AttackQueries::squareIsAttackedByWhite(gameState.board, e8) &&
                        !AttackQueries::squareIsAttackedByWhite(gameState.board, d8))
                    {
                        movesToBeGenerated.emplace_back(Square::e8, Square::c8, Figure::BlackKing, Figure::None, false,
                                                        false, false, true);
                        // The rook move to d1 is generated in the rook move generation
                    }
                }
            }

            std::function<BitBoardState(Square)> getAttacks = [&gameState](Square sourceSquare)
            {
                return AttackQueries::kingAttackTable[sourceSquare] & (~gameState.board.occupancies[Color::Black]);
            };

            generatePieceMoves(gameState, movesToBeGenerated, Color::White, Figure::BlackKing, std::move(getAttacks));
        }
    };
}