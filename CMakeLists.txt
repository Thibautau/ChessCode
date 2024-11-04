cmake_minimum_required(VERSION 3.29)
project(ChessCode)

set(CMAKE_CXX_STANDARD 20)


include_directories(.)  # Inclure le répertoire courant

# Inclure GoogleTest via FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Créer l'exécutable principal
add_executable(ChessCode
        main.cpp
        MainChessGame.h
        Board.h
        Piece.h
        Type.h
        PlayerHuman.h
        PlayerHuman.cpp
        Piece.cpp
        Board.cpp
        Board.cpp
        MainChessGame.cpp
        Bot.h
        Bot.cpp
        Player.h
        Player.cpp
        Zobrist.cpp
        Zobrist.h
        UCI/UCI.cpp
        UCI/UCI.h
)

# Créer l'exécutable pour les tests
add_executable(runTests Test/Test.cpp
        Test/Test.h
        Test/Test.h
        Test/Test.cpp
        Test/Test.cpp
        Test/Test.cpp
        Test/TestBoard.cpp
        Test/TestBoard.h
        Test/TestBoard.cpp
        Test/TestPiece.cpp
        Test/TestPlayer.cpp
        Test/TestUCI.cpp
)

# Créer l'exécutable pour les perf
add_executable(runPerf
        Test/TestPerf.cpp
        Test/TestPerf.h
)

# Lier GoogleTest à l'exécutable de tests
target_link_libraries(runTests gtest gtest_main)