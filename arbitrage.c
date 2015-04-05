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
    int i;
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
// Fonction pour générer toutes les suites de mouvement possibles
// et ne retenant que ceux qui offre la possibilité de faire le plus grand nombre de mouvements
// ****************************************************************************************************
int GeneratePossibleMoves(const SGameState * const gameState, const unsigned char dices[2], Player player)
{
	unsigned int i, nbMovesMax;
	int tabDices[4]; //Tableau qui va contenir les résultats des dés, 4 résultats identiques si les dés sont égaux
	int dicesUsed[4] = {0, 0, 0, 0}; // Tableau permettant de retenir les dés qui ont déjà été joués (0 pas encore joué, 1 joué)
	unsigned int ennemi;
	ennemi = (player == WHITE) ? BLACK : WHITE; // Pour connaitre la couleur de l'ennemi (BLACK si le joueur est WHITE, et inversement)

	if (dices[0] == dices[1]) // Si les deux dés ont le même chiffre
	{
		nbMovesMax = 4;
		for (i = 0; i <nbMovesMax; i++)
		{
			tabDices[i] = dices[0];
		}
	}
	else // Si les deux dés n'ont pas le même chiffre
	{
		nbMovesMax = 2;
		tabDices[0] = dices[0];
		tabDices[1] = dices[1];
	}
	
	
	// On cherche toutes les solutions
	int j, k;
	for( j = 0; j < nbMovesMax; j++ ) // On boucle tant que le nombre maximal théorique de mouvement n'est pas atteint
	{
		if (gameState->bar[player] != 0) // Si le bar n'est pas vide
		{
			for (k = 0; k < nbMovesMax; k++)
			{
				if (dicesUsed[k] == 0) // Si le dé n'a pas déjà été utilisé
				{
					// Si aucune dame ne peut être sortie du bar
					if ( (gameState->board[tabDices[k]-1].owner == ennemi) && (gameState->board[tabDices[k]-1].nbDames > 1) )
					{
						return
					}
					else
					{
					
					}
				}
			}
		}
		else // Si le bar est vide
		{
			for (k = 0; k < 24; k ++) // On parcout tout le plateau
			{
				if (gameState->board[k].owner == player)
				{
					
				}
			}
		}	
	}
    return 0;
}


// ****************************************************************************************************
// Fonction qui vérifie les mouvements demandés par le joueur
// et qui décremente le nombre d'essais restant si un mouvement n'est pas valide
//
// - Si le joueur propose un nombre de mouvement supérieur au nombre maximum (théorique) de mouvement possible
//		=> ce n'est pas valide
// - Si le joueur propose un nombre de mouvement égal au nombre maximum (théorique) de mouvement possible
//		=> on vérifie que chaque mouvement est valide
// - Si le joueur propose un nombre de mouvement inférieur au nombre maximum (théorique) de mouvement possible
//		=> on vérifie que le joueur ne pouvait pas faire une plus grande suite de mouvements
//		=> puis on vérifie la validité de chacun de ses coups
//
// Retourne 0 si le tour n'est pas valide, 1 sinon
// ****************************************************************************************************
int IsValid(const SGameState * const gameState, const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves, unsigned int *tries, const Player player)
{
	unsigned int i, nbMovesMax;
	nbMovesMax = (dices[0] == dices[1]) ? 4 : 2; // est égale à 4 si les deux dés sont égaux, 2 sinon	
	
	if (nbMoves > nbMovesMax) // Si le joueur propose un nombre de mouvement supérieur au nombre maximum (théorique) de mouvement possible ( => non valide )
	{
		*tries--;
		return 0;
	}
	else if (nbMoves == nbMovesMax) // Si le joueur propose un nombre de mouvement égal au nombre maximum (théorique) de mouvement possible
	{
		
		if ( !(IsValidDistance(dices, moves, nbMovesMax) )
		{
			// Si au moins un des mouvements ne respecte pas le résultat d'un dé ( => non valide )
			*tries--;
			return 0;
		}
		
		for (i = 0; i < nbMovesMax; i++)
		{
			if (gameState->bar[player] != 0) // Si le "bar" n'est pas vide, il faut jouer une dame du "bar"
			{
				if (moves[i].src_point != 0) // Si le joueur veut bouger un pion qui n'est pas dans le "bar" ( => non valide )
				{
					*tries--;
					return 0;
				}
				else
				{
					if ((gameState->board[moves[i].dest_point-1].owner == ennemi) && (gameState->board[moves[i].dest_point-1].nbDames > 1))
					{
						// Le mouvement fait que la dame tombe sur une case ennemi avec plus de deux dames ( => non valide )
						*tries--;
						return 0;
					}
				}	
			}
			else // Si le bar est vide
			{
				if (moves[i].src_point == 0) // Si la source du mouvement est le "bar" ( => non valide )
				{
					*tries--;
					return 0;
				}
				else if (moves[i].src_point == 25) // Si la source du mouvement est le "out" ( => non valide )
				{
					*tries--;
					return 0;
				}
				else // Si la source du mouvement est dans le "board"
				{
					if ((gameState->board[moves[i].dest_point-1].owner == ennemi) && (gameState->board[moves[i].dest_point-1].nbDames > 1))
					{
						// Le mouvement fait que la dame tombe sur une case ennemi avec plus de deux dames ( => non valide )
						*tries--;
						return 0;
					}
				}	
			}
		}	
	}
	else // Si le joueur propose un nombre de mouvement inférieur au nombre maximum (théorique) de mouvement possible 
	{
	
	}
}


// ****************************************************************************************************
// Fonction qui permet de vérifier que la différence entre la source du mouvement et la destination
// correspond bien aux résultats fournis pas les deux dés
//
// Retourne 0 si au moins un des mouvements ne respecte pas le résultat d'un dé, 1 sinon
// ****************************************************************************************************
int IsValidRange(const unsigned char dices[2], const SMove moves[4], const unsigned int nbMoves)
{
	if (nbMovesMax == 4) // Si les dés sont égaux, il suffit de vérifier que chaque dame fait un mouvement égale à un des deux dés
	{
		for (i = 0; i < nbMovesMax; i++)
		{
			if ( (moves[i].dest_point - moves[i].src_point) != dices[0] )
			{
				*tries--;
				return 0;
			}
		}
	}
	else // Si on a que deux mouvements possibles (les dés ont un résultat différent)
	{
		if ( ! ( ((moves[0].dest_point - moves[0].src_point) == dices[0]) && ((moves[1].dest_point - moves[1].src_point) == dices[1])
		|| ((moves[0].dest_point - moves[0].src_point) == dices[1]) && ((moves[1].dest_point - moves[1].src_point) == dices[0]) ) )
		{
			// On regarde si les mouvements proposés NE CORRESPONDENT PAS aux deux seuls cas possibles (d'où le "!")
			// - le mouvement 0 correspond au premier dé et le mouvement 1 correspond au deuxième dé
			// - le mouvement 1 correspond au premier dé et le mouvement 0 correspond au deuxième dé
			*tries--;
			return 0;
		}
	}
}
	

// ****************************************************************************************************
// Fonction qui permet de mettre à jour le tableau
// ****************************************************************************************************
void ModifPlateau(SGameState * gameState, SMove moves[4], unsigned int *nbMoves, Player player)
{
    
    Player enemy;
    int i;
    int x,y;
    
    for (i = 0; i < *nbMoves; i++)
    {
        
        x = moves[i].src_point;
        y = moves[i].dest_point;
        
        if (x == 0) s{
            
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
            enemy = 1 - gameState->board[y-1].owner;
            gameState->board[y-1].owner = enemy;
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
