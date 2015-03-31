//// Fichier pour la stratégie ////
#include "bot.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

//////////////////////////////////////////////////////////
// Dans la librairie
//

typedef struct {
	Player color;
	Player enemy;
	unsigned int targetScore;
	
} SBotInfo;

SBotInfo bot;


void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Ballz");
}

void StartMatch(const unsigned int target_score)
{
	bot.targetScore = target_score;
	printf("StartMatch\n");
}

void StartGame(Player p)
{
	
	bot.color = p;
	if(p == 1) bot.enemy = 0;
	else bot.enemy = 1;
	
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

int DoubleStack(const SGameState * const gameState)
{
	printf("DoubleStack\n");
	return(0);
}

int TakeDouble(const SGameState * const gameState)
{
	printf("TakeDouble\n");
	return(0);
}

void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
	int i,j = 0;
	int casesPionsBot[15];
	int dim;
	
	//Pour manipuler des entiers, c'est plus pratique que des caractères
	
	unsigned int dice[2]={(int)dices[0],(int)dices[1]};			
	unsigned int sizeDice = sizeof(dice)/sizeof(int);
	
	//Lorsque l'on est dans le bar (prison)
	
	if (gameState->bar[bot.color] =! 0)
	{
		/* Si on a au moins 2 pions dans le bar (si on a pas de double aux dés) on va devoir faire obligatoirement 2 moves
		   Soit : les 2 dés donnent des moves possible, on les fait et on return
		   Soit : un seul des 2 dés permet un bon déplacement, donc on n'en renvoiera qu'un seul et on return 
		   (pour pas envoyer un autre move qui ne serait pas correct puisque le bar ne sera pas vide)*/
		if (gameState->bar[bot.color] >= 2)
		{
			if ((IsMoveRight(0,dice[0])) && (IsMoveRight(0,dice[1])))
			{
				*nbMove = 2;
				moves[0].src_point = 0;
				moves[0].dest_point = move[0].src_point + dice[0]+1;
				
				moves[1].src_point = 0;
				moves[1].dest_point = move[1].src_point + dice[1]+1;
			}
			else if (IsMoveRight(0,dice[0]))
			{
				*nbMove = 1;
				moves[0].src_point = 0;
				moves[0].dest_point = move[0].src_point + dice[0]+1;
			}
			else if(IsMoveRight(0,dice[1]))
			{
				*nbMove = 1;
				moves[0].src_point = 0;
				moves[0].dest_point = move[0].src_point + dice[1]+1;
			}
			return;		
		}
		
		/* S'il n'y a qu'un seul pion dans le bar, on ne va effectuer que l'un des 2 moves possibles */
		else if (gameState->bar[bot.color] == 1)
		{
			if (IsMoveRight(0,dice[0]))
			{
				*nbMove = 1;
				moves[0].src_point = 0;
				moves[0].dest_point = move[0].src_point + dice[0]+1;
			}
			else if(IsMoveRight(0,dice[1]))
			{
				*nbMove = 1;
				moves[0].src_point = 0;
				moves[0].dest_point = move[0].src_point + dice[1]+1;
			}
			
			// Si nbMove == 0, le pion n'est pas sorti du bar, donc on ne peut pas jouer et on return
			if (*nbMove == 0)
			{
				return;
			}
		}
	}
	
	//Remplissage tableau contenant les indices des cases sur lesquelles sont présents les pions du bot
	for(i=0;i<24;i++)
	{
		if (gameState->board[i].owner == bot.color)
		{
			casesPionsBot[j] = i;
			j++;
		}
	}
	dim = j;
	
	
	/*** Permet, si on le peut, de faire bouger 2 pions sur la même case ***/
	
	/* Parcours du tableau contenant les indices des cases de nos pions
	   On va comparer tous les indices entre eux */
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
		{
			
			// Vérification : la dist entre les cases = la dist entre les dés + la case d'arrivée ne doit pas être a l'ennemi OU il ne doit y avoir qu'un seul pion dessus.
			if((fabs(casesPionsBot[i]-casesPionsBot[j]) == fabs(dice[0]-dice[1])) && (IsMoveRight(min(casesPionsBot[i],casesPionsBot[j]),max(dice[0],dice[1]))))
			{
				*nbMove = 2;
				moves[0].src_point = min(casesPionsBot[i],casesPionsBot[j])+1;
				moves[0].dest_point = moves[0].src_point + max(dice[0],dice[1])+1;
				
				moves[1].src_point = max(casesPionsBot[i],casesPionsBot[j])+1;
				moves[1].dest_point = moves[1].src_point + min(dice[0],dice[1])+1;
				return;
			}
		}
	}
	
	j=0;
	// Si on a pas fait le move d'avant
	
	
	for(i=0;i<dim;i++)
	{
		if(j == 2)
		{
			*nbMove = 2;
			return;
		}
		if(IsCaseOurs(casesPionsBot[i]+dice[0])
		{
			moves[j].src_point = casesPionsBot[i] + 1;
			moves[j].dest_point = casesPionsBot[i]+dice[0] + 1;
			j++;
		}
		else if(IsCaseOurs(casesPionsBot[i]+dice[1]))
		{
			moves[j].src_point = casesPionsBot[i] + 1;
			moves[j].dest_point = casesPionsBot[i]+dice[1] + 1;
			j++;
		}
	}

	

	
	/* Sinon si possible : déplacement d'un pion de la somme des dés sur une case à nous
	 * Déplacement de 2 pions sur 2 cases différentes à nous
	 */
	
	
	/** Maybe faire un enum dans la structure bot pour faire des etats (allié et ennemi, peut être utile pour savoir si on double ou non la mise)
	(debut/milieu de partie + fin de partie avec le ruch de l'ancre, plus besoin de se soucier de se faire prendre des pions) **/
	
	
	printf("max : %d\n",max(5,2));
	
	printf("bot : %d\n",(int)bot.color);
	
	printf("PlayTurn\n");
}

int min(int a, int b)
{
	if(a<b) return a;
	else return b;
	
}

int max(int a,int b)
{
	if(a>b) return a;
	else return b;
	
}



// Vérifie que la case sur laquelle le pion va se déplacer est libre, de sa couleur ou n'a qu'un pion dessus
int IsMoveRight(int numCaseDep, int dice, const SGameState * const gameState)
{
	if((numCaseDep + dice) <= 23)
	{
		if ((gameState->board[(numCaseDep + dice)].owner != bot.enemy) || (gameState->board[(numCaseDep + dice)].nbDames == 1))
		{
			return(1);
		}
		else return 0;
	}
	else return 0;
}


// Vérifie qu'une case est bien de la couleur de l'IA
int IsCaseOurs(int numCase, const SGameState * const gameState)
{
	if (gameState->board[numCase].owner == bot.color)
	{
		return 1;
	}
	else return 0;
}