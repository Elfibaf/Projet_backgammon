#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "arbitrage.h"
#include "bot.h"


// ====================================================================================================
// Fonction d'initialisation du plateau
// Prend en paramètre un SGameState
// ====================================================================================================
void InitPlateau(SGameState * gameState)
{
    // Initialisation du board
    unsigned int i;
    for (i = 0; i < 24; i++)
    {
        switch(i)
        {/*
        case 0 :
            gameState->board[i].owner = WHITE;
            gameState->board[i].nbDames = 2;
            break;
        case 5 :
            gameState->board[i].owner = BLACK;
            gameState->board[i].nbDames = 5;
            break;
        case 7 :
            gameState->board[i].owner = BLACK;
            gameState->board[i].nbDames = 3;
            break;
        case 11 :
            gameState->board[i].owner = WHITE;
            gameState->board[i].nbDames = 5;
            break;
        case 12 :
            gameState->board[i].owner = BLACK;
            gameState->board[i].nbDames = 5;
            break;
        case 16 :
            gameState->board[i].owner = WHITE;
            gameState->board[i].nbDames = 3;
            break;
        case 18 :
            gameState->board[i].owner = WHITE;
            gameState->board[i].nbDames = 5;
            break;
        case 23 :
            gameState->board[i].owner = BLACK;
            gameState->board[i].nbDames = 2;
            break;*/
        default :
            gameState->board[i].owner = NOBODY;
            gameState->board[i].nbDames = 0;
            break;
        }
    }
    // Initialisation du bar
    gameState->bar[WHITE] = 0;
    gameState->bar[BLACK] = 0;
    // Initialisation du out
    gameState->out[WHITE] = 0;
    gameState->out[BLACK] = 0;
    // Initialisation du nombre de tours
    gameState->turn = 0;
    // Initialisation de la mise
    gameState->stake = 1;
}


// ========================================================================================================================
// Fonction pour générer les deux dés
// ========================================================================================================================
void GenerateDices(unsigned char dices[2])
{
	dices[0] = (char)(rand()%(6)+1);
	dices[1] = (char)(rand()%(6)+1);
	printf("Dé 1 : %d    Dé 2 : %d\n",(int)dices[0],(int)dices[1]);
}


// ====================================================================================================
// Fonction qui vérifie si le tour du joueur est valide
// Le plateau sera mis à jour à l'aide des fonctions Update de l'interface
//
// Retourne 0 si le tour n'est pas valide, 1 sinon
// ====================================================================================================
int CheckTurn(SGameState * gameState, const unsigned char dices[2], const SMove moves[4], const int nbMoves, const Player player)
{
	// Le nombre théorique de mouvement qu'il est possible de faire
	unsigned int nbMovesTheoretic = (dices[0] == dices[1]) ? 4 : 2;
	
	if ( (nbMoves > nbMovesTheoretic) || (nbMoves < 1) )
	{
		// Si le joueur propose un nombre de mouvement supérieur au nombre maximum (théorique) de mouvement possible
		// ou s'il ne propose pas de mouvements
		return 0;
	}
	else
	{
		// dicesTab = Tableau remplit selon les valeurs fournises par les dés
		//            Si les valeurs des deux dés sont égales, le tableau contient 4 valeurs identiques
		//            Sinon le tableau contient les 2 valeurs fournises par les deux déss
		// dicesUsed = Tableau permettant de retenir les dés qui ont déjà été pris en compte
		//             La case "i" contient 1 si la "ieme" valeur de tabDices à déjà été prise en compte, 0 sinon
		unsigned char *dicesTab = (unsigned char*)malloc(nbMovesTheoretic * sizeof(unsigned char));
		unsigned char *dicesUsed = (unsigned char*)malloc(nbMovesTheoretic * sizeof(unsigned char));
	    if ( (dicesTab == NULL) || (dicesUsed == NULL) )
	    {
	        exit(0);
	    }
	    
		unsigned int numDice; // Numéro du dé
	
		// On remplit tabDices avec les valeurs de chaque dés
		for (numDice = 0; numDice < nbMovesTheoretic; numDice++)
		{
		    if (nbMovesTheoretic == 2)
		    {
		        dicesTab[numDice] = dices[numDice];
			    dicesUsed[numDice] = 0;
		    }
		    else
		    {
		        dicesTab[numDice] = dices[0];
			    dicesUsed[numDice] = 0;
		    }
		}
		
		if (nbMoves == nbMovesTheoretic) 
		{
			if (CheckAllMove(gameState, moves, nbMoves, nbMovesTheoretic, player, dicesTab, dicesUsed) == 0)
		    {
		        free(dicesTab);
	            free(dicesUsed);
		        return 0;
		    }
		}
		else 
		{
			// ************************************************************************
			// Si le joueur propose un nombre de mouvement inférieur au nombre maximum
			// (théorique) de mouvement possible :
			//
			// => on cherche le nombre maximum de mouvement qu'il est possible de faire
			// ************************************************************************
			
			// nombre de mouvements maximal
			unsigned int nbMovesMax = GetMaxNumberPossibleMoves(gameState, nbMovesTheoretic, dicesTab, dicesUsed, player, 4);
			
			// On réinitialise dicesTab et dicesUsed pour la suite
			for (numDice = 0; numDice < nbMovesTheoretic; numDice++)
		    {
		        if (nbMovesTheoretic == 2)
		        {
		            dicesTab[numDice] = dices[numDice];
			        dicesUsed[numDice] = 0;
		        }
		        else
		        {
		            dicesTab[numDice] = dices[0];
			        dicesUsed[numDice] = 0;
		        }
		    }
			
			// ************************************************************************
			// Puis on vérifit s'il a fait le maximum de mouvement qu'il était possible de faire
			// ************************************************************************
			if (nbMoves == nbMovesMax) // Le joueur a proposé une solution ayant un nombre de mouvements maximal
			{
			    // On vérifie chaque mouvement
			    if (CheckAllMove(gameState, moves, nbMoves, nbMovesTheoretic, player, dicesTab, dicesUsed) == 0)
			    {
			        free(dicesTab);
	                free(dicesUsed);
			        return 0;
			    }
			}
			else // Le joueur aurait pu faire un tour avec plus de coup
			{
			    free(dicesTab);
	            free(dicesUsed);
				return 0;
			}	
		}
		// On est passé dans aucun cas invalide
		free(dicesTab);
	    free(dicesUsed);
	    return 1;
	}
}


// ========================================================================================================================
// Fonction qui permet de vérifier qu'un déplacement est valide
//
// Retourne 0 le déplacement n'est pas valide, 1 sinon
// ========================================================================================================================
int CheckOneMove(const SGameState * const gameState, const SMove move, const unsigned int nbMovesTheoretic, const Player player, unsigned char * dicesTab, unsigned char * dicesUsed)
{
	unsigned char numDice; // Numéro du mouvement, numéro du dé
	Player ennemi = 1 - player; // Pour connaitre le numéro de l'adversaire
	
	// Variables pour rendre le code plus lisible
	int source = move.src_point;
	int destination = move.dest_point;
	
	// Utiliser dans l'algorithme pour gérer les cas selon la couleur du joueur
	unsigned int diff;
	unsigned char start;
    unsigned char end;
	
	
    // *************************************************************************
	// CONTROLE LA VALIDITE DES CHIFFRES
	// *************************************************************************
	if ( (source < 0) || (source > 24) || (destination < 1) || (destination > 25) )
	{
	    // On vérifie si la valeur de la source du mouvement n'est pas dans le "bar" ou dans le "board"
	    // et si la valeur de la destination du mouvement n'est pas dans le "board" ou dans le "out"
		return 0;
	}
	
	
	// *************************************************************************
	// CONTROLE DU SENS DU MOUVEMENT (WHITE VS BLACK)
	// *************************************************************************
	if (player == WHITE)
	{
		if (source > destination)
	    {
	        return 0;
	    }
	}
	else
	{
	    // On enleve les cas particuliers du "bar" et du "out" pour le joueur BLACK
	    if ( (destination != 25) || (source == 0) )
	    {
	        if (source < destination)
	        {
	            return 0;
	        }
	    }
	}
	

	// *************************************************************************
	// CONTROLE DU "BAR"
	// *************************************************************************
	if (source == 0) // Le joueur veut sortir une dame du "bar"
	{
	    if (gameState->bar[player] == 0)
	    {
	        // Si le "bar" est vide
	        return 0;
	    }
	    
	    for (numDice = 0; numDice < nbMovesTheoretic; numDice++)
	    {
	        // Il faut d'abord calculer la distance entre le "bar" et la destination selon la couleur du joueur
	        
	        if (player == WHITE)
	        {
	            diff = destination;
	        }
	        else
	        {
	            diff = 25 - destination;
	        }
	        // Si la dame est exactement à la distance nécessaire (selon le dé) pour pouvoir etre sorti
	        // et que le dé n'a pas encore été utilisé
		    if ((diff == dicesTab[numDice]) && (dicesUsed[numDice] == 0) )
		    {
			    // Si c'est le cas, on signale qu'on a utilisé le dé
			    dicesUsed[numDice] = 1;
			    return 1;
		    }
		    else // Sinon, le joueur a utilisé une valeur qui ne correspond pas à ce dé ou que le dé n'est pas disponible
		    {
		        // On continue pour voir si le mouvement ne concernait pas un autre dé
			    continue;
		    }
        }
	}
	else // La source n'est le "bar" du joueur (donc le "board")
	{
	    if (gameState->bar[player] != 0)
	    {
	        // Si le "bar" n'est pas vide
	        return 0;
	    }
	    
	    if (gameState->board[source-1].owner != player)
	    {
	        // Si la source du mouvement concerne le "board" : si le joueur ne possède pas la dame, il n'a pas le droit de la déplacer
	        return 0;
	    }
	}

    // *************************************************************************
	// CONTROLE DU "OUT"
	// *************************************************************************
	if (destination == 25) // Le joueur veut sortir une dame du jeu
	{
		if (player == WHITE)
		{
		    start = 0;
		    end = 18;
		}
		else
		{
		    start = 6;
		    end = 24;		
		}
		// il faut d'abord qu'il n'y a aucune case en dehors du out et des 6 derniers cases du plateau
		// on a pas besoin de vérif le "bar" car on serait pas pensé dans le "if (destination == 25)"
		unsigned char numSquare; // numéro de la case du plateau
		for (numSquare = start; numSquare < end; numSquare++)
		{
			// Si le joueur a une dame qui n'est pas dans son jan intérieur
			if (gameState->board[numSquare].owner == player)
			{
				return 0;
			}
		}
		
		// On parcours la liste des valeurs fournie par les dés
	    for (numDice = 0; numDice < nbMovesTheoretic; numDice++)
	    {
	        if (player == WHITE)
	        {
	            diff = destination - source;
	        }
	        else
	        {
	            // La différence est égale à la case (le joueur noir sort une dame de la case 6 avec un dé ayant une valeur de 6)
	            diff = source;
	        }
	        // Si la dame est exactement à la distance nécessaire (selon le dé) pour pouvoir etre sorti
	        // et que le dé n'a pas encore été utilisé
		    if ( (diff == dicesTab[numDice]) && (dicesUsed[numDice] == 0) )
		    {
			    // Si c'est le cas, on signale qu'on a utilisé le dé
			    dicesUsed[numDice] = 1;
			    return 1;
		    }
		    else if ( (diff < dicesTab[numDice]) && (dicesUsed[numDice] == 0) )
		    {
			    // Si le joueur fait un mouvement plus petit que le résultat du dé
			    // il faut parcourir toutes les cases du jan intérieur qui précède la dame que le joueur souhaite déplacer
			    // => si on trouve une autre dame qui lui appartient, le mouvement n'est pas bon, sinon il est valide
			    
			    // Il faut également adapter selon la couleur du joueur
			    if (player == WHITE)
	            {
	                start = 18;
	                end = source - 1; // car il y a un décalage entre les chiffres d'un "move" et celui du "board"
	            }
	            else
	            {
	                start = source;
	                end = 7;
	            }
			    for (numSquare = start; numSquare < end; numSquare++)
			    {
				    if (gameState->board[numSquare].owner == player)
				    {
				        // Si une dame est plus éloignée du "out", il aurait fallu la jouer
				        return 0;
				    }		    
			    }
			    
			    // Bien que le dé soit supérieur à la différence entre destination et source,
			    // etant donné que le joueur ne possède pas de dame avant ce case concernée par la source du mouvement
			    // le mouvement est valide
			    dicesUsed[numDice] = 1;
			    return 1;
		    }
		    else // Sinon, le joueur a utilisé une valeur qui ne correspond pas à ce dé ou que le dé n'est pas disponible
		    {
		        // On continue pour voir si le mouvement ne concernait pas un autre dé
			    continue;
		    }
        }
	}
	else // Si la destination n'est pas le "out" (destination != 25)
	{
	    // Le joueur demande à déplacer une dame sur une case qui est occupée par au moins 2 dames du joueur adverse
        if ( (gameState->board[destination-1].owner == ennemi) && (gameState->board[destination-1].nbDames > 1) )
        {
	        return 0;
        }
        
	    // On parcours la liste des valeurs fournie par les dés
	    for (numDice = 0; numDice < nbMovesTheoretic; numDice++)
	    {
		    // Si on utilise bien un dé disponible
		    // Cela permet de vérifier en même temps que le mouvement va dans le bon sens (sens positif)
		    if ( (abs(destination - source) == dicesTab[numDice]) && (dicesUsed[numDice] == 0) )
		    {
			    // Si c'est le cas, on signale qu'on a utilisé le dé
			    dicesUsed[numDice] = 1;
			    return 1;
		    }
		    else // Sinon, le joueur a utilisé une valeur qui ne correspond pas à ce dé ou que le dé n'est pas dispo
		    {
			    // On continue pour voir si le mouvement ne concernait pas un autre dé
			    continue;
		    }
        }
    }
    return 0; // Aucun des dés ne permet de jouer le mouvement
}


// ========================================================================================================================
// Fonction qui permet de vérifier que tous les déplacements sont valides
// et met à jour (à l'aide de la fonction "UpdateOneMove") le plateau
//
// Retourne 0 au moins un déplacement n'est pas valide, 1 sinon
// ========================================================================================================================
int CheckAllMove(SGameState * gameState, const SMove moves[4], const unsigned int nbMoves, const unsigned int nbMovesTheoretic, const Player player, unsigned char * dicesTab, unsigned char * dicesUsed)
{
    unsigned char numMove;
    unsigned char nbDicesUsed = 0; // Nombre de dés utilisés
    
    for (numMove = 0; numMove < nbMoves; numMove++)
    {
        if (nbDicesUsed < nbMoves)
        {
            if (CheckOneMove(gameState, moves[numMove], nbMovesTheoretic, player, dicesTab, dicesUsed) == 0)
            {
                free(dicesTab);
                free(dicesUsed);
                return 0;
            }
            else
            {
                nbDicesUsed++;
                // Mise à jour de l'état du jeu pour vérifier le coup suivant
                UpdateOneMove(gameState, moves[numMove], player);
            }
        }
    }
    return 1;
}


// ========================================================================================================================
// Fonction pour qui compte le nombre maximum de mouvements qu'il est possible de faire
// selon les résultats des dés
//
// Renvoit le nombre maximum de mouvements qu'il est possible de faire
// ========================================================================================================================
int GetMaxNumberPossibleMoves(SGameState * gameState, const unsigned int nbMovesTheoretic, unsigned char * dicesTab, unsigned char * dicesUsed, const Player player, unsigned char nbDicesAvailable)
{
    unsigned char numCase=0, numDice=0;
    SMove move;
    
    unsigned int nbMovesMax = 0;
        
	if (nbDicesAvailable != 0) // Il reste des dés disponibles
	{
	    for (numCase = 0; numCase < 25; numCase++)
        {
            move.src_point = numCase;
            move.dest_point = move.src_point + dicesTab[numDice];
            if (CheckOneMove(gameState, move, nbMovesTheoretic, player, dicesTab, dicesUsed) == 0)
            {
                dicesUsed[numDice] = 1;
                nbDicesAvailable--;
                printf("%d : \n", nbDicesAvailable);
                UpdateOneMove(gameState, move, player);
                // On calcule récursivement le nombre maximal de mouvements possibles
                nbMovesMax = 1 + GetMaxNumberPossibleMoves(gameState, nbMovesTheoretic, dicesTab, dicesUsed, player, nbDicesAvailable);
            }
        }
	}
	else
	{
	    return 0;
	}
	return nbMovesMax;
}


// ========================================================================================================================
// Fonction qui permet de mettre à jour le plateau avec 1 seul mouvement
// ========================================================================================================================
void UpdateOneMove(SGameState * gameState, const SMove move, const Player player)
{
    Player enemy = 1 - player;
	unsigned int source, destination;
	
	source = move.src_point;
	destination = move.dest_point;
	
	// Mis à jour de la case source du mouvement
	if (source == 0)
	{
		gameState->bar[player]--;
	}
	else
	{
		gameState->board[source-1].nbDames--;
		if (gameState->board[source-1].nbDames == 0)
		{
			gameState->board[source-1].owner = NOBODY;
		}
	}
	
	// Mis à jour de la case destination du mouvement
	if (destination == 25)
	{
		gameState->out[player]++;
	}
	else if (gameState->board[destination-1].owner == NOBODY)
	{
		gameState->board[destination-1].nbDames++;
		gameState->board[destination-1].owner = player; // La case a maintenant un propriétaire
	}
	else if (gameState->board[destination-1].owner == player)
	{
		gameState->board[destination-1].nbDames++;
	}
	else
	{
		gameState->bar[enemy]++;
		gameState->board[destination-1].owner = player;
	}
	printf("Le joueur %d bouge de la case %d à la case %d\n",(int)player, source, destination);
}


// ========================================================================================================================
// Fonction qui permet de mettre à jour le plateau avec tous les mouvements
// ========================================================================================================================
void UpdateAllMove(SGameState * gameState, const SMove moves[4], const unsigned int nbMoves, const Player player)
{
	unsigned int numMove;
	
	// Pour chacun des mouvements
	for (numMove = 0; numMove < nbMoves; numMove++)
	{
		UpdateOneMove(gameState, moves[numMove], player);
	}
}


// ========================================================================================================================
// Fonction qui permet de déterminer si un joueur a gagné la partie
// ========================================================================================================================
int WinGame(const SGameState * const gameState, Player player)
{
    if (gameState->out[player] == 15) // Si tous les pions sont sortis du plateau
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

