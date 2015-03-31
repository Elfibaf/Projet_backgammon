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
			}
		}
	}
	
	
	// Si on a pas fait le move d'avant
	if (*nbMove == 0)
	{
		for(i=0;i<dim;i++)
		{
			if((IsCaseOurs(casesPionsBot[i]+dice[0])) || IsCaseOurs(casesPionsBot[i]+dice[1]))
			{
				
			}
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

int IsCaseOurs(int numCase, const SGameState * const gameState)
{
	if (gameState->board[numCase].owner == bot.color)
	{
		return 1;
	}
	else return 0;
}