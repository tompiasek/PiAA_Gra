#include "algorithm.hpp"
#include <climits>
#include <iostream>

std::pair<std::pair<int, Board>, std::pair<std::pair<int, int>, bool>> minimax(std::pair<int, int> pos, int depth, bool max_player, Game game, int alpha, int beta) {
    Board board = Board(*game.getCurrentState());

    bool bestJump = false;

    if (depth == 0 || game.isGameOver()) {
        return std::make_pair(std::make_pair((*game.getCurrentState()).evaluate(), board), std::make_pair(std::make_pair(-1, -1), bestJump));
    }

    int maxEval = INT_MIN, minEval = INT_MAX;
    Board bestBoard;
    std::pair<int, int> bestMove = std::make_pair(-1, -1);

    if (max_player) {
        for (auto& move : board.allAvailableMoves(1)) {
            Game newGame = game; // Make a new copy of the game state
            std::pair<Board*, bool> result = newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
            if (result.first == nullptr) {
                std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }
            int eval = minimax(std::make_pair(move.second.first, move.second.second), depth - 1, false, newGame, alpha, beta).first.first;
            if (eval > maxEval) {
                maxEval = eval;
                bestBoard = Board(*newGame.getCurrentState());
                bestMove = move.second;
                bestJump = result.second;
            }
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return std::make_pair(std::make_pair(maxEval, bestBoard), std::make_pair(bestMove, bestJump));
    } else {
        for (auto& move : board.allAvailableMoves(2)) {
            Game newGame = game; // Make a new copy of the game state
            std::pair<Board*, bool> result = newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
            if (result.first == nullptr) {
                std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }
            int eval = minimax(std::make_pair(move.second.first, move.second.second), depth - 1, true, newGame, alpha, beta).first.first;
            if (eval < minEval) {
                minEval = eval;
                bestBoard = Board(*newGame.getCurrentState());
                bestMove = move.second;
                bestJump = result.second;
            }
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return std::make_pair(std::make_pair(minEval, bestBoard), std::make_pair(bestMove, bestJump));
    }
}