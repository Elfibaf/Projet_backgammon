#include <stdio.h>
#include <stdlib.h>

#include "arbitrage.h"
#include "bot.h"


// ****************************************************************************************************
// Fonction d'initialisation du plateau
// Prend en parametre un SGameState
// ****************************************************************************************************
void InitPlateau(SGameState * gameState)
{
    // Initialisation du board
    unsigned int i;
    for (i = 0; i < 24; i++)
    {
        switch(i)
        {
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
            break;
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
    // Initialisation des scores des deux joueurs
    gameState->whiteScore = 0;
    gameState->blackScore = 0;
    // Initialisation du nombre de tours
    gameState->turn = 0;
    // Initialisation de la mise
    gameState->stake = 1;
}


// ****************************************************************************************************
// Fonction pour générer les deux dés
// ****************************************************************************************************
void GenerateDices(unsigned char dices[2])
{
    
    dices[0] = (char)(rand()%(6)+1);
    dices[1] = (char)(rand()%(6)+1);
    printf("Dé 1 : %d    Dé 2 : %d\n",(int)dices[0],(int)dices[1]);
    
}


// ****************************************************************************************************
// Fonction qui vérifie les mouvements demandés par le joueur
// et qui décremente le nombre d'essais restant si un mouvement n'est pas valide
//
// - Si le joueur propose un nombre de mouvement supérieur au nombre maximum (théorique) de mouvement possible
//		=> ce n'est pas valide
// - Si le joueur propose un nombre de mouvement égal au nombre maximum (théorique) de mouvement possible
//		=> on vérifie la validité de chacun de ses coups
// - Si le joueur propose un nombre de mouvement inférieur au nombre maximum (théorique) de mouvement possible
//		=> on vérifie que le joueur ne pouvait pas faire une plus grande suite de mouvements
//		=> puis on vérifie la validité de chacun de ses coups
//
// Retourne 0 si le tour n'est pas valide, 1 sinon
// ****************************************************************************************************
int IsValid(const SGameState * const gameState, const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves, const Player player)
{
	unsigned int i, nbMovesMax;
	nbMovesMax = (dices[0] == dices[1]) ? 4 : 2; // est égale à 4 si les deux dés sont égaux, 2 sinon	
	
	if (nbMoves > nbMovesMax)
	{
		// Si le joueur propose un nombre de mouvement supérieur au nombre maximum (théorique) de mouvement possible ( => non valide )
		return 0;
	}
	else
	{
		if ( !(IsInBoardBarOut(moves, nbMoves)) )
		{
			// Si au moins un des attributs du mouvement (src ou dest) n'est pas valide ( => non valide)
			return 0;
		}
		if ( !(IsValidDistance(dices, moves, nbMoves)) )
		{
			// Si au moins un des mouvements ne respecte pas le résultat d'un dé ( => non valide )
			return 0;
		}
		
		
		if (nbMoves == nbMovesMax) // Si le joueur propose un nombre de mouvement égal au nombre maximum (théorique) de mouvement possible
		{
		
			for (i = 0; i < nbMoves; i++)
			{
				if (gameState->bar[player] != 0) // Si le "bar" n'est pas vide, il faut jouer une dame du "bar"
				{
					if (moves[i].src_point != 0) // Si le joueur veut bouger un pion qui n'est pas dans le "bar" ( => non valide )
					{
						return 0;
					}
					else
					{
						if ((gameState->board[moves[i].dest_point-1].owner == ennemi) && (gameState->board[moves[i].dest_point-1].nbDames > 1))
						{
							// Le mouvement fait que la dame tombe sur une case ennemi avec plus de deux dames ( => non valide )
							return 0;
						}
					}	
				}
				else // Si le bar est vide
				{
					if (moves[i].src_point == 0) // Si la source du mouvement est le "bar" ( => non valide )
					{
						return 0;
					}
					else // Si la source du mouvement est dans le "board"
					{
						if ((gameState->board[moves[i].dest_point-1].owner == ennemi) && (gameState->board[moves[i].dest_point-1].nbDames > 1))
						{
							// Le mouvement fait que la dame tombe sur une case ennemi avec plus de deux dames ( => non valide )
							return 0;
						}
					}	
				}
			}
		}
		else // Si le joueur propose un nombre de mouvement inférieur au nombre maximum (théorique) de mouvement possible 
		{
			unsigned int i, nbMovesTheoretic;
			char *tabDices = NULL; // Tableau pour générer les dés qu'on pourra utiliser
			char *dicesUsed = NULL; // Permet de retenir les dés qui ont déjà été pris en compte (1 pris en compte, 0 sinon)
			
			if (dices[0] == dices[1])
			{
				nbMovesTheoretic = 4;
			}
			else
			{
				nbMovesTheoretic = 2;
			}
			
			// On réserve de la place pour seulement nbMovesTheoretic valeurs
			tabDices = (char*)malloc(nbMovesTheoretic * sizeof(char));
			dicesUsed = (char*)malloc(nbMovesTheoretic * sizeof(char));
			if ( (tabDices == NULL) || (dicesUsed == NULL) )
			{
				exit(0);
			}
	
			// On remplit tabDices avec les valeurs de chaque dés
			for (i = 0; i < nbMovesTheoretic; i++)
			{
				if (nbMovesTheoretic == 4)
				{
					tabDices[i] = dices[0]; // On remplit 4 fois avec la meme valeur (on double car les dés sont égaux)s
				}
				else
				{
					tabDices[i] = dices[i];
				}
				dicesUsed[i] = 0;
			}
			
			

			
			
		}
	}
}


// ****************************************************************************************************
// Fonction qui permet de vérifier que les sources des mouvements sont bien dans le "board" ou le "bar"
// et que les destinations des mouvements sont bien dans le "board" ou le "out"
//
// Retourne 0 si au moins un des mouvements ne respecte pas ces conditions, 1 sinon
// ****************************************************************************************************
int IsInBoardBarOut(const SMove moves[4], const unsigned int nbMoves)
{
	unsigned i;
	for (i = 0; i < nbMoves; i++)
	{
		if ( (moves[i].src_point < 0) || (moves[i].src_point > 24) || (moves[i].dest_point < 1) || (moves[i].dest_point > 25) )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


// ****************************************************************************************************
// Fonction qui permet de vérifier que la différence entre la source du mouvement et la destination
// correspond bien aux résultats fournis pas les deux dés
//
// Retourne 0 si au moins un des mouvements ne respecte pas le résultat d'un dé, 1 sinon
// ****************************************************************************************************
int IsValidDistance(const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves)
{
	unsigned int i, j;
	int *tabDices = NULL; // Tableau pour générer les dés qu'on pourra utiliser
	int *dicesUsed = NULL; // Permet de retenir les dés qui ont déjà été pris en compte (1 pris en compte, 0 sinon)
	
	if (dices[0] == dices[1])
	{
		// On réserve la place pour chaque mouvement
		tabDices = (int*)malloc(nbMoves*sizeof(int));
		dicesUsed = (int*)malloc(nbMoves*sizeof(int));
		if ( (tabDices == NULL) || (dicesUsed == NULL) )
		{
			exit(0);
		}
		
		// On remplit tabDices avec les memes valeurs (car les dés sont égaux)
		for (i = 0; i < nbMoves; i++)
		{
			tabDices[i] = dices[0];
			dicesUsed[i] = 0;
		}
	}
	else
	{
		// On réserve de la place pour seulement 2 valeurs (les valeurs des deux dés)
		tabDices = (int*)malloc(2*sizeof(int));
		dicesUsed = (int*)malloc(2*sizeof(int));
		if ( (tabDices == NULL) || (dicesUsed == NULL) )
		{
			exit(0);
		}
		
		// On remplit tabDices avec les valeurs de chaque dés (car les dés ne sont pas égaux)
		for (i = 0; i < 2; i++)
		{
			tabDices[i] = dices[i];
			dicesUsed[i] = 0;
		}
	}
	
	
	for (i = 0; i < nbMoves; i++)
	{
		for (j = 0; j < nbMoves; j++)
		{
			if ( ((moves[i].dest_point - moves[i].src_point) == tabDices[j]) && (dicesUsed[j] == 0) )
			{
				// On vérifie si la différence entre la destination et la source sont égales à une valeur de dé disponibles
				// Si c'est le cas, on signale qu'on a utilisé le dé :
				dicesUsed[j] = 1;
			}
			else
			{
				// Le mouvement demandé n'est pas correct, on libère la mémoire et on renvoit 0 (faux)
				free(tabDices);
				free(dicesUsed);
				return 0;
			}
		}
	}
	
	// On libère la mémoire et on renvoit 1 (vrai)
	free(tabDices);
	free(dicesUsed);
	return 1;
}


// ****************************************************************************************************
// Fonction pour qui compte le nombre maximum de mouvements qu'il est possible de faire
// selon les résultats des dés
//
// Renvoit le nombre maximum de mouvements qu'il est possible de faire
// ****************************************************************************************************
int getMaxNumberPossibleMoves(SGameState * gameState, const unsigned int nbMovesTheoretic, const unsigned char * tabDices, unsigned char * dicesUsed, const Player player)
{
	Player ennemi = (1 - player) // Pour connaitre la couleur de l'ennemi
	unsigned int numDice; // pour connaitre le numéro du dé en cours
	
	// Attributs pour la mise à jour de la copie du plateau
	SMoves moves[4];
	unsigned int nbMoves;
	
	if (gameState->bar[player] != 0) // Si le bar n'est pas vide
	{
		if (nbMovesTheoretic == 4)
		{
			// Il faut d'abord vérifier pour 1 dé (les 4 dés ayant la meme valeur)
			if ( (gameState->board[tabDices[0]-1].owner == ennemi) && (gameState->board[tabDices[0]-1].nbDames > 1) )
			{
				// Si aucune dame ne peut être sortie du bar
				return 0;
			}
			else
			{
				for (numDice = 0; numDice < nbMovesTheoretic; numDice++) // Pour chacun des dés
				{
					moves[0].src_point = 0;
					moves[0].dest_point = tabDices[numDice];
					ModifPlateau(gameState, moves, nbMoves, player);
					
					dicesUsed[numDice] = 1; // On retient lequel des dés a été joué
					return (1 + getMaxNumberPossibleMoves(gameState, nbMovesTheoretic, tabDices, dicesUsed, player));
				}
			}
		}
		else // Si on peut faire que 2 mouvements maximum (théorique)
		{
			for (numDice = 0; numDice < nbMovesTheoretic; numDice++) // Pour chacun des dés
			{
				if ( (gameState->board[tabDices[numDice]-1].owner == ennemi) && (gameState->board[tabDices[numDice]-1].nbDames > 1) )
				{
					// Si la dame ne peut être sortie du bar
					return 0;
				}
				else
				{
					moves[0].src_point = 0;
					moves[0].dest_point = tabDices[numDice];
					ModifPlateau(gameState, moves, nbMoves, player);
					
					dicesUsed[numDice] = 1; // On retient lequel des dés a été joué
					return (1 + getMaxNumberPossibleMoves(gameState, nbMovesTheoretic, tabDices, dicesUsed, player));
				}
			}
		}
	}
	else // Si le bar est vide
	{
		unsigned int numSquare; // pour connaitre le numéro de la case du plateau en cours
		for (numSquare = 0; numSquare < 24; numSquare++)
		{
			if (gameState->board[numSquare].owner == player)
			{
				for (numDice = 0; numDice < nbMovesTheoretic; numDice++) // Pour chacun des dés
				{
					if ( (gameState->board[tabDices[numDice]+numSquare].owner == ennemi)
					&& (gameState->board[tabDices[numDice]+numSquare].nbDames > 1) )
					{
						// Si la dame ne peut pas bouger, car la case est occupée par au moins 2 dames ennemies
						return 0;
					}
					else
					{
						moves[0].src_point = numSquare+1;
						moves[0].dest_point = numSquare+1+numDice;
						ModifPlateau(gameState, moves, nbMoves, player);
					
						dicesUsed[numDice] = 1; // On retient lequel des dés a été joué
						return (1 + getMaxNumberPossibleMoves(gameState, nbMovesTheoretic, tabDices, dicesUsed, player));
					}
				}
			}
		}
	}
}


// ****************************************************************************************************
// Fonction qui permet de mettre à jour le tableau
// ****************************************************************************************************
void ModifPlateau(SGameState * gameState, SMove moves[4], unsigned int *nbMoves, Player player)
{
    
    Player enemy = 1 - player;
    int i;
    int x,y;
    
    for (i = 0; i < *nbMoves; i++)
    {
        
        x = moves[i].src_point;
        y = moves[i].dest_point;
        
        if (x == 0) {
            
            gameState->bar[player] = gameState->bar[player] - 1 ;
        } 
        
        else {
            
            gameState->board[x-1].nbDames = gameState->board[x-1].nbDames - 1;
            if (gameState->board[x-1].nbDames == 0) 
            {
                
                gameState->board[x-1].owner = -1;
            }
        }
    
        
        if (y == 25) {
            gameState->out[player] = gameState->out[player] + 1;
        } 
        
        else if (gameState->board[y-1].owner == -1) {
            gameState->board[y-1].nbDames = gameState->board[y-1].nbDames + 1;
            gameState->board[y-1].owner = player;
        } 
        
        else if (gameState->board[y-1].owner == player)
        {
            gameState->board[y-1].nbDames = gameState->board[y-1].nbDames + 1;
        }
        
        else if (gameState->board[y-1].owner != player) {
            
            gameState->board[y-1].owner = player;
            gameState->bar[enemy] = gameState->bar[enemy] + 1;
        }
        printf("Le joueur %d bouge de la case %d à la case %d\n",(int)player,x,y);
    }
}


// ****************************************************************************************************
// Fonction qui permet de déterminer si un joueur a gagné la partie
// ****************************************************************************************************
int WinGame(const SGameState * const gameState, int player)
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
