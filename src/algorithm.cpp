#include "algorithm.hpp"


std::pair<int, Board*> minimax(std::pair<int, int> pos, int depth, bool max_player, Game game) {
    Board board = game.getCurrentState();

    if (depth == 0 || game.isGameOver()) {
        return std::pair<int, Board*>(game.getCurrentState().evaluate(), &board);
    }

    int maxEval = INT_MIN, minEval = INT_MAX;

    if (max_player) {
        for (auto& move : board.allAvailableMoves(1)) {
            Game newGame = game;
            if (newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second) == nullptr) {
                std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }
            int eval = minimax(std::pair<int, int>(move.second.first, move.second.second), depth - 1, false, &newGame).first;
            if (eval > maxEval) {
                maxEval = eval;
            }
        }
    } else {
        for (auto& move : board.allAvailableMoves(0)) {
            Board newBoard = board;
            newBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second);
            Game newGame = game;
            newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
            int eval = minimax(std::pair<int, int>(move.second.first, move.second.second), depth - 1, true, &newGame).first;
            if (eval < minEval) {
                minEval = eval;
            }
        }
    }

    if (max_player) {
        return std::pair<int, Board*>(maxEval, &board);
    } else {
        return std::pair<int, Board*>(minEval, &board);
    }
}