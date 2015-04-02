#ifndef ARBITRAGE_H
	#define ARBITRAGE_H

#include "bot.h"
	
void InitPlateau(SGameState);
void GenerateDices(unsigned char[2]);
int VerifCoup(const SGameState * const, const unsigned char[2], SMove[4], unsigned int*, unsigned int);
int WinGame(const SGameState * const, int);
void ModifPlateau(const SGameState * const , SMove[4], unsigned int*, int) {

#endif