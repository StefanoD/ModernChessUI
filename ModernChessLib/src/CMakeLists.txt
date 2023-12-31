add_library(${target}
        ../include/ModernChess/AttackQueries.h
        ../include/ModernChess/BasicParser.h
        ../include/ModernChess/Board.h
        ../include/ModernChess/BitBoardConstants.h
        ../include/ModernChess/BitBoardOperations.h
        ../include/ModernChess/BishopAttacks.h
        ../include/ModernChess/CastlingRights.h
        ../include/ModernChess/Color.h
        ../include/ModernChess/GameState.h
        ../include/ModernChess/GlobalConstants.h
        ../include/ModernChess/FenParsing.h
        ../include/ModernChess/Figure.h
        ../include/ModernChess/FlatMap.h
        ../include/ModernChess/Evaluation.h
        ../include/ModernChess/KingAttacks.h
        ../include/ModernChess/KnightAttacks.h
        ../include/ModernChess/MemoryAllocator.h
        ../include/ModernChess/Move.h
        ../include/ModernChess/MoveExecution.h
        ../include/ModernChess/PseudoMoveGeneration.h
        ../include/ModernChess/PseudoRandomGenerator.h
        ../include/ModernChess/PawnPushes.h
        ../include/ModernChess/OneStepMoves.h
        ../include/ModernChess/PawnAttacks.h
        ../include/ModernChess/PawnQueries.h
        ../include/ModernChess/PeriodicTask.h
        ../include/ModernChess/Player.h
        ../include/ModernChess/PrincipalVariationTable.h
        ../include/ModernChess/QueenAttacks.h
        ../include/ModernChess/RookAttacks.h
        ../include/ModernChess/Square.h
        ../include/ModernChess/TranspositionTable.h
        ../include/ModernChess/Timer.h
        ../include/ModernChess/TUI.h
        ../include/ModernChess/UCIParser.h
        ../include/ModernChess/UCICommunication.h
        ../include/ModernChess/Utilities.h
        ../include/ModernChess/WaitCondition.h
        ../include/ModernChess/ZobristHasher.h

        AttackQueries.cpp
        BasicParser.cpp
        Board.cpp
        Evaluation.cpp
        FenParsing.cpp
        Utilities.cpp
        Player.cpp
        GameState.cpp
        MemoryAllocator.cpp
        Move.cpp
        PawnAttacks.cpp
        RookAttacks.cpp
        BishopAttacks.cpp
        CastlingRights.cpp
        TranspositionTable.cpp
        TUI.cpp
        UCIParser.cpp
        UCICommunication.cpp
        ZobristHasher.cpp
        MemoryAllocator.cpp
        )

target_include_directories(${target} PUBLIC ../include)

