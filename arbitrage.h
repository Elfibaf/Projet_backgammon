#ifndef ARBITRAGE_H
	#define ARBITRAGE_H

#include "bot.h"
	
void initPlateau(SGameState);
void generateDices(unsigned char[2]);
int verifCoup(const SGameState * const, const unsigned char[2], SMove[4], unsigned int*, unsigned int);
int winGame(const SGameState * const, int);
void modifPlateau(const SGameState * const , SMove[4], unsigned int*, int) {

#endif