#ifndef ARBITRAGE_H
	#define ARBITRAGE_H

#include "bot.h"
	
void InitPlateau(SGameState * gameState);
void GenerateDices(unsigned char dices[2]);
int IsValid(SGameState * gameState, const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves, const Player player);
int IsInBoardBarOut(const SMove moves[4], const unsigned int nbMoves);
int IsValidDistance(const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves);
int GetMaxNumberPossibleMoves(SGameState * gameState, const unsigned int nbMovesTheoretic, const unsigned char * tabDices, unsigned char * dicesUsed, const Player player);
void ModifPlateau(SGameState * gameState, SMove moves[4], unsigned int nbMoves, Player player);
int WinGame(const SGameState * const gameState, int player);

#endif
