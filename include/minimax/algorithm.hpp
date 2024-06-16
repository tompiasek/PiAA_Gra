#pragma once
#ifndef MINIMAX_ALGORITHM_HPP_
#define MINIMAX_ALGORITHM_HPP_

#include "game.hpp"

std::pair<int, Board> minimax(std::pair<int,int> pos, int depth, bool max_player, Game game);


//if (max_player) {
//		int maxEval = INT_MIN;
//		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves = board.allAvailableMoves(1);
//		Board bestMove = board;
//		for (auto move : moves) {
//			Board newBoard = board;
//			newBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second);
//			Game newGame = game;
//			newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
//			int eval = minimax(std::pair<int,int>(move.second.first, move.second.second), depth - 1, false, newGame).first;
//			if (eval > maxEval) {
//				maxEval = eval;
//				bestMove = newBoard;
//			}
//		}
//		return std::pair<int, Board>(maxEval, bestMove);
//	}
//	else {
//		int minEval = INT_MAX;
//		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves = board.allAvailableMoves(0);
//		Board bestMove = board;
//		for (auto move : moves) {
//			Board newBoard = board;
//			newBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second);
//			Game newGame = game;
//			newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
//			int eval = minimax(std::pair<int,int>(move.second.first, move.second.second), depth - 1, true, newGame).first;
//			if (eval < minEval) {
//				minEval = eval;
//				bestMove = newBoard;
//			}
//		}
//		return std::pair<int, Board>(minEval, bestMove);
//	}

#endif // !MINIMAX_ALGORITHM_HPP_