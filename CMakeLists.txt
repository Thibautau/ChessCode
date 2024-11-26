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

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.12.0  # Vous pouvez choisir une autre version ici
)
FetchContent_MakeAvailable(spdlog)

# Assurez-vous que les répertoires d'inclusion de spdlog sont bien ajoutés
include_directories(${spdlog_SOURCE_DIR}/include)

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
        LogFile.cpp
        LogFile.h
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
        Test/TestBot.cpp
        LogFile.cpp
        LogFile.h
)

# Créer l'exécutable pour les perf
add_executable(runPerf
        Test/TestPerf.cpp
        Test/TestPerf.h
        LogFile.cpp
        LogFile.h
)

# Lier GoogleTest à l'exécutable de tests
target_link_libraries(runTests gtest gtest_main)
target_link_libraries(ChessCode PRIVATE spdlog::spdlog)