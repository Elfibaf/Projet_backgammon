#ifndef ARBITRAGE_H
	#define ARBITRAGE_H

#include "bot.h"

// ====================================================================================================
// Fonction d'initialisation du plateau
// Prend en paramètre un SGameState
// ====================================================================================================
void InitPlateau(SGameState * gameState);


// ==========;==============================================================================================================
// Fonction pour générer les deux dés
// ========================================================================================================================
void GenerateDices(unsigned char dices[2]);


// ====================================================================================================
// Fonction qui vérifie si le tour du joueur est valide
//
// Retourne 0 si le tour n'est pas valide, 1 sinon
// ====================================================================================================
int CheckTurn(SGameState * gameState, const unsigned char dices[2], const SMove moves[4], const int nbMoves, const Player player);


// ========================================================================================================================
// Fonction qui permet de vérifier qu'un déplacement est valide
//
// Retourne 0 le déplacement n'est pas valide, 1 sinon
// ========================================================================================================================
int CheckOneMove(const SGameState * const gameState, const SMove move, const unsigned int nbMovesTheoretic, const Player player, unsigned char * dicesTab, unsigned char * dicesUsed);


// ========================================================================================================================
// Fonction qui permet de vérifier que tous les déplacements sont valides
// et met à jour (à l'aide de la fonction "UpdateOneMove") le plateau
//
// Retourne 0 au moins un déplacement n'est pas valide, 1 sinon
// ========================================================================================================================
int CheckAllMove(SGameState * gameState, const SMove moves[4], const unsigned int nbMoves, const unsigned int nbMovesTheoretic, const Player player, unsigned char * dicesTab, unsigned char * dicesUsed);


// ========================================================================================================================
// Fonction pour qui compte le nombre maximum de mouvements qu'il est possible de faire
// selon les résultats des dés
//
// Renvoit le nombre maximum de mouvements qu'il est possible de faire
// ========================================================================================================================
int GetMaxNumberPossibleMoves(SGameState * gameState, const unsigned int nbMovesTheoretic, unsigned char * dicesTab, unsigned char * dicesUsed, const Player player, unsigned char nbDicesAvailable);


// ========================================================================================================================
// Fonction qui permet de mettre à jour le plateau avec 1 seul mouvement
// ========================================================================================================================
void UpdateOneMove(SGameState * gameState, const SMove move, const Player player);


// ========================================================================================================================
// Fonction qui permet de mettre à jour le plateau avec tous les mouvements
// ========================================================================================================================
void UpdateAllMove(SGameState * gameState, const SMove moves[4], const unsigned int nbMoves, const Player player);


// ========================================================================================================================
// Fonction qui permet de déterminer si un joueur a gagné la partie
// ========================================================================================================================
int WinGame(const SGameState * const gameState, Player player);


#endif
