#ifndef ARBITRAGE_H
	#define ARBITRAGE_H

#include "bot.h"
	
void InitPlateau(SGameState * gameState);
void GenerateDices(unsigned char dices[2]);
int GeneratePossibleMoves(const SGameState * const gameState, const unsigned char dices[2], Player player);
int IsValid(const SGameState * const gameState, const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves, unsigned int *tries, const Player player);
int IsValidRange(const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves);
void ModifPlateau(SGameState * gameState, SMove moves[4], unsigned int *nbMoves, Player player);
int WinGame(const SGameState * const gameState, int player);

#endif
