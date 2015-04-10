#ifndef ARBITRAGE_H
	#define ARBITRAGE_H

#include "api.h"


/**
 * Fonction d'initialisation du plateau
 *
 * @param SGameState * gameState 		: Le plateau à initialiser
 *
 * @return void
 */
void InitPlateau(SGameState * gameState);



/**
 * Fonction pour générer les deux dés
 *
 * @param unsigned char dices[2] 		: Les dés dont on doit générer les valeurs
 *
 * @return void
 */
void GenerateDices(unsigned char dices[2]);



/**
 * Fonction qui vérifie si le tour du joueur est valide
 * La plateau sera mis à jour à l'aide d'une des fonctions Update de l'interface
 *
 * @param SGameState * gameState		: Le plateau
 * @param const unsigned char dices[2]	: Les dés qui ont été générés pour le tour du joueur
 * @param const SMove moves[4]			: Les mouvements que le joueur a effectué et qu'on doit vérifier
 * @param const int nbMoves				: Le nombre de mouvement que le joueur a effectué
 * @param const Player player			: Le joueur qui a ordonné les mouvements
 * 
 * @return int							: retourne 0 si le tour n'est pas valide, 1 si le tour est valide
 */
int CheckTurn(SGameState * gameState, const unsigned char dices[2], const SMove moves[4], const int nbMoves, const Player player);



/**
 * Fonction qui permet de vérifier qu'un déplacement est valide
 *
 * @param const SGameState * const gameState	: Le plateau
 * @param const SMove move						: Le mouvement à traiter
 * @param const unsigned int nbMovesTheoretic	: Le nombre théorique de mouvement qu'il est possible de faire (4 si double, 2 sinon)
 * @param const Player player					: Le joueur qui a ordonné le mouvement
 * @param unsigned char * dicesTab				: Tableau qui sert à connaitre les dés qui sont utilisables
 * @param unsigned char * dicesUsed				: Tableau qui sert à retenir les dés qui ont été utilisés
 * 
 * @return int									: retourne 0 si le mouvement n'est pas valide, 1 si le tour est valide
 */
int CheckOneMove(const SGameState * const gameState, const SMove move, const unsigned int nbMovesTheoretic, const Player player, unsigned char * dicesTab, unsigned char * dicesUsed);



/**
 * Fonction qui permet de vérifier que tous les déplacements sont valides
 *
 * @param SGameState * gameState				: Le plateau
 * @param const SMove moves[4]					: Les mouvements à traiter
 * @param const unsigned int nbMovesTheoretic	: Le nombre théorique de mouvement qu'il est possible de faire (4 si double, 2 sinon)
 * @param const Player player					: Le joueur qui a ordonné le mouvement
 * @param unsigned char * dicesTab				: Tableau qui sert à connaitre les dés qui sont utilisables
 * @param unsigned char * dicesUsed				: Tableau qui sert à retenir les dés qui ont été utilisés
 * 
 * @return int									: retourne 0 si au moins un mouvement n'est pas valide, 1 sinon
 */
int CheckAllMove(SGameState * gameState, const SMove moves[4], const unsigned int nbMoves, const unsigned int nbMovesTheoretic, const Player player, unsigned char * dicesTab, unsigned char * dicesUsed);



/**
 * Fonction qui compte le nombre maximum de mouvements qu'il est possible de faire selon les valeurs fournies par les dés
 *
 * @param SGameState * gameState				: Le plateau
 * @param const unsigned int nbMovesTheoretic	: Le nombre théorique de mouvement qu'il est possible de faire (4 si double, 2 sinon)
 * @param unsigned char * dicesTab				: Tableau qui sert à connaitre les dés qui sont utilisables
 * @param unsigned char * dicesUsed				: Tableau qui sert à retenir les dés qui ont été utilisés 
 * @param const Player player					: Le joueur qui a ordonné le mouvement
 * @param unsigned char nbDicesAvailable		: Le nombre de dés disponibles
 *
 * @return int									: retourne le nombre maximum de mouvements qu'il est possible de faire
 */
int GetMaxNumberPossibleMoves(SGameState * gameState, const unsigned int nbMovesTheoretic, unsigned char * dicesTab, unsigned char * dicesUsed, const Player player, unsigned char nbDicesAvailable);



/**
 * Fonction qui permet de mettre à jour le plateau avec un seul mouvement
 *
 * @param SGameState * gameState				: Le plateau
 * @param const SMove move						: Le mouvement à mettre à jour
 * @param const Player player					: Le joueur dont c'est le tour
 * 
 * @return void
 */
void UpdateOneMove(SGameState * gameState, const SMove move, const Player player);



/**
 * Fonction qui permet de mettre à jour le plateau avec tous les mouvements
 *
 * @param SGameState * gameState				: Le plateau
 * @param const SMove moves[4]					: Les mouvements à mettre à jour
 * @param const unsigned nbMoves				: Le nombre de mouvements effectués
 * @param const Player player					: Le joueur dont c'est le tour
 * 
 * @return void
 */
void UpdateAllMove(SGameState * gameState, const SMove moves[4], const unsigned int nbMoves, const Player player);



/**
 * Fonction qui permet de déterminer si un joueur a gagné la partie
 *
 * @param const SGameState * const gameState	: Le plateau
 * @param const Player player					: Le joueur dont c'est le tour
 * 
 * @return int									: Retourne 1 si le joueur a gagné, 0 sinon
 */
int WinGame(const SGameState * const gameState, Player player);


#endif
