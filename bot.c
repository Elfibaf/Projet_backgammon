//// Fichier pour la stratégie ////
#include "api.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "fonction.h"

//////////////////////////////////////////////////////////
// Dans la librairie
//




SBotInfo bot; // Structure qui contient les infos dont l'IA a besoin entre les tours


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
	//On sauvegarde la couleur de l'IA et de l'ennemi
	bot.color = p;
	if(p == 1) bot.enemy = BLACK;
	else bot.enemy = WHITE;
	
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
	// On demande à doubler la mise si on a au moins 5 pions d'avance dans le out par rapport à l'ennemi
	if(gameState->out[bot.color] > (gameState->out[bot.enemy] + 4))
	{
		return(1);
	}
	else
	{
		return(0);
	}
	
}

int TakeDouble(const SGameState * const gameState)
{
	// On accepte le double si on a au moins 5 pions d'avance dans le out OU si le score cible sera atteint à la fin de la game
	if((gameState->out[bot.color] > (gameState->out[bot.enemy] + 4)) || ((max(gameState->whiteScore,gameState->blackScore) + gameState->stake) >= bot.targetScore))
	{
		return(1);
	}
	else return 0;
}


// Retourne le minimum entre 2 nombres
int min(int a, int b)
{
	if(a<b) return a;
	else return b;
	
}

// Retourne le maximum entre 2 nombres
int max(int a,int b)
{
	if(a>b) return a;
	else return b;
	
}



// Vérifie que la case sur laquelle le pion va se déplacer est libre, de sa couleur ou n'a qu'un pion dessus
int IsMoveRight(int numCaseDep, int dice, const SGameState * const gameState)
{
	// Verification que la case d'arrivée est bien dans le plateau et que le dé n'est pas utilisé
	if(((numCaseDep + dice) <= 23) && (numCaseDep + dice >=0) && (!IsDiceUsed(dice)) && (bot.color == WHITE))
	{
		// Vérification que la case d'arrivée n'est pas possédée par l'ennemi ou qu'il n'y a qu'un pion dessus
		if ((gameState->board[(numCaseDep + dice)].owner != bot.enemy) || (gameState->board[(numCaseDep + dice)].nbDames == 1))
		{
	
			return(1);
		}
		else return 0;
	}
	// On fait les même vérifications pour le joueur noir
	else if ((bot.color == BLACK) && ((numCaseDep - dice) <= 23) && (numCaseDep - dice >=0) && (!IsDiceUsed(dice)))
	{
		if ((gameState->board[(numCaseDep - dice)].owner != bot.enemy) || (gameState->board[(numCaseDep - dice)].nbDames == 1))
		{
			return 1;
		}
		else return 0;
	}	
		
	
	return 0;
}


// Vérifie qu'une case est bien de la couleur de l'IA
int IsCaseOurs(int numCase, int dice, const SGameState * const gameState)
{
	if (bot.color == WHITE)
	{	
		//On vérifie que la case d'arrivée est bien de la couleur du bot et que la case est dans le plateau
		if ((gameState->board[numCase + dice].owner == bot.color) && ((numCase +dice) < 24) && ((numCase + dice) >= 0))
		{
			return 1;
		}
		else return 0;
	}
	
	else if (bot.color == BLACK)
	{
		//Idem pour le black
		if ((gameState->board[numCase - dice].owner == bot.color) && ((numCase - dice) < 24) && ((numCase - dice) >= 0))
		{
			return 1;
		}
		else return 0;
	}
	return 0;
}




//Indique si le dé a été utilisé ou non (1 s'il a déjà été utilisé, 0 sinon)
int IsDiceUsed(int dice)
{
	if (dice < 0)
	{
		return 1;
	}
	else return 0;
}

//Indique si les dés forment un double
int IsDiceDouble(const unsigned char dice[2])
{
	if (dice[0] == dice[1])
	{
		return 1;
	}
	else return 0;
}


//Indique si la case a été vidée de ses pions lors du tour courant en comptant le nombre de déplacements effectués à partir de cette case
int IsCaseEmpty(int caseDep, int nbMove, SMove moves[4], const SGameState * const gameState)
{
	int i,j=0;
	for(i=0;i<nbMove;i++)
	{
		if(moves[i].src_point -1 == caseDep)
		{
			j++;
		}	
	}
	if(j == gameState->board[caseDep].nbDames)
	{
		return(1);
	}
	else return(0);
	
}


// Effectue la somme des dés non encore utilisés
int SumDice(int *dice,int sizeDice)
{
	int i;
	int sum = 0;
	for(i=0;i<sizeDice;i++)
	{
		if(!IsDiceUsed(dice[i]))
		{
			sum += dice[i];
		}
	}
	return(sum);
}

// Retourne le nombre de dés non encore utilisés
int NbDiceLeft(int *dice, int sizeDice)
{
	int i;
	int nb=0;
	for(i=0;i<sizeDice;i++)
	{
		if(!IsDiceUsed(dice[i]))
		{
			nb++;
		}
	}
	return(nb);
}

// Définit les différents états dans lesquels peut entrer le bot
Etat DefEtat(const SGameState * const gameState)
{
	int i,j;
	int sum1 = 0, sum2 = gameState->out[bot.color];
	
	// Si il y a des pions dans le bar on entre dans l'etat NORMAL
	if(gameState->bar[bot.color] != 0)
	{
		return(NORMAL);
	}
	

	if(bot.color == WHITE)
	{
		
		for(j=18;j<24;j++)
		{
			
			if(gameState->board[j].owner == bot.color)
			{
				// On compte le nombre de pions dans le jan intérieur du bot
				sum2 = sum2 + gameState->board[j].nbDames;
				
			}
		}
		
		//Si ils y sont tous, il passe dans l'etat de fin
		if(sum2 == 15)
		{
			return FIN;	
		
		}
		else if(sum2 >= 11)
		{
			return TRANSITION;
		}
		
		for(i=0;i<6;i++)
		{
			if(((gameState->board[i].owner == bot.enemy) && (gameState->board[i].nbDames >= 2)))
			{
				sum1 = sum1 +1;
				if(sum1 > 2)
				{
					return TRANSITION;
				}
			}
		}
		
		return NORMAL;
	}
	
	//Meme manip pour le BLACK
	else
	{
		
		for(j=0;j<6;j++)
		{
			if(gameState->board[j].owner == bot.color)
			{
				
				sum2 = sum2 + gameState->board[j].nbDames;
				
			}
		}
		
		if(sum2 == 15)
		{
			return FIN;	
		
		}
		else if(sum2 >= 11)
		{
			return TRANSITION;
		}
		
		for(i=18;i<24;i++)
		{
			if(((gameState->board[i].owner == bot.enemy) && (gameState->board[i].nbDames >= 2)))
			{
				sum1 = sum1 +1;
				if(sum1 > 2)
				{
					return TRANSITION;
				}
			}
		}
		
		
		
		
		return NORMAL;
	}
}


//Fonction qui determine les sources et dest des mouvements que veut faire le bot
void ApplyMove(unsigned int *nbMove, SMove moves[4], int caseSrc, int *dice, int j)
{
	if(bot.color == WHITE)
	{
		moves[*nbMove].src_point = caseSrc + 1;
		moves[*nbMove].dest_point = caseSrc + dice[j] + 1;
	}
	else
	{
		moves[*nbMove].src_point = caseSrc + 1;
		moves[*nbMove].dest_point = caseSrc - dice[j] + 1;
	}
	//On incrémente le nombre de moves et on indique que le dé a été utilisé
	*nbMove = *nbMove + 1;
	dice[j] = -1;
}



void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries)
{
	int sortie = 1; //Permet de sortir de la boucle principale

	int i,j = 0; 
	int casesPionsBot[15];		// Tableau contenant les indices des cases appartenant au bot
	int dim; 	// Dimension du tableau contenant les indices des cases appartenant au bot
	unsigned int sizeDice; //Nombre de dés
	int nbMoveInter, numCaseInter, sumInter;	// Variables utiles lorsque le bot doit faire des mouvements combinés (ex : de 2 à 5 puis de 5 à 10...)
	Etat etatJeu; //Etat du bot (NORMAL, TRANSITION ou FIN)
	
	*nbMove = 0;
	
	/*** Transformation du dé en int (prise en compte des doubles) ***/
	
	//Determination du nombre de dés en fonction de si ce sont des doubles
	if (IsDiceDouble(dices))
	{
		sizeDice = 4;
	}
	else 
	{
		sizeDice = 2;
	}
	
	//Création du tableau dynamique contenant les dés
	int *dice = (int *)malloc(sizeDice*sizeof(int));
	if(sizeDice == 4)
	{
		dice[0] = (int)dices[0];
		dice[1] = (int)dices[0];
		dice[2] = (int)dices[0];
		dice[3] = (int)dices[0];
		
	}
	else
	{
		dice[0]=(int)dices[0];
		dice[1]=(int)dices[1];
		
	}
	
	

	// Recuperation de l'etat du bot+
	etatJeu = DefEtat(gameState);
	
	
	
	
	while(sortie)
	{
		
		switch(etatJeu)
		{
			//Etat dans lequel on ramene les pions les plus éloignés en priorité
			case TRANSITION :
			
				
				//Remplissage tableau contenant les indices des cases sur lesquelles sont présents les pions du bot (en fonction de la couleur du bot)
				if(bot.color == BLACK)
				{
					for(i=23;i>=0;i--)
					{
						if (gameState->board[i].owner == bot.color)
						{
							casesPionsBot[j] = i;
							j++;
						}
					}
				}
				else
				{
					for(i=0;i<24;i++)
					{
						if (gameState->board[i].owner == bot.color)
						{
							casesPionsBot[j] = i;
							j++;
						}
					}
				}
				dim = j;
				
				
				
				//On bouge l'ancre en priorité de plusieurs cases si possible
				
				nbMoveInter = 0;
				numCaseInter = 0;
				sumInter = 0;
				
				
				if((IsMoveRight(casesPionsBot[0],SumDice(dice,sizeDice),gameState)) && (!IsCaseEmpty(casesPionsBot[0],*nbMove,moves,gameState)))
				{
					for(j=0;j<sizeDice;j++)
					{
						if(!IsDiceUsed(dice[j]))
						{
							sumInter += dice[j];
						}
						
						if(IsMoveRight(casesPionsBot[0],sumInter,gameState))
						{
							nbMoveInter++;
						}
					}
					// Si le nombre de moves intermédiaires est égal au nombre de dés alors on effectue la suite de mouvements
					if(nbMoveInter == NbDiceLeft(dice,sizeDice))
					{
						for(j=0;j<sizeDice;j++)
						{
							if((!IsDiceUsed(dice[j])) && (numCaseInter == 0))
							{
								ApplyMove(nbMove,moves,casesPionsBot[0],dice,j);
								numCaseInter = 1;
								
								
							}
							else if((!IsDiceUsed(dice[j])) && (numCaseInter != 0))
							{
								ApplyMove(nbMove,moves,moves[*nbMove-1].dest_point-1, dice, j);
								
							}
							if (*nbMove == sizeDice)
							{
								free(dice);
								return;
							}
						}
					}
				}
				
				//Si on peut pas faire de long move on fait un move simple
				for(j=0;j<sizeDice;j++)
				{
					if((IsMoveRight(casesPionsBot[0],dice[j],gameState)) && (!IsCaseEmpty(casesPionsBot[0],*nbMove,moves,gameState)))
					{
						ApplyMove(nbMove,moves,casesPionsBot[0], dice, j);
						
					}
				}
				
				if(*nbMove == sizeDice)
				{
					free(dice);
					return;
				}
				etatJeu = NORMAL;
				break;
				
				
				
				
			case NORMAL :
					
			
				
			
				
				/***Lorsque l'on a des pions dans le bar  ***/
				
				if (gameState->bar[bot.color] != 0)
				{
					/* On parcourt le bar*/
					   
					for(i=0;i<gameState->bar[bot.color];i++)
					{
						//Pour chaque dé on vérifie si le move potentiel est bon en fonction de la couleur, et on fait le move
						for(j=0;j<sizeDice;j++)
						{
							
							if(IsMoveRight(-1,dice[j],gameState) && (bot.color == WHITE))
							{	
								moves[*nbMove].src_point = 0;
								moves[*nbMove].dest_point = 0 + dice[j];
								*nbMove = *nbMove + 1;
								dice[j] = -1;
								break;  //On a utilisé le dé courant et le pion courant donc on sort de la boucle pour passer au pion suivant
							}
							else if(IsMoveRight(24,dice[j],gameState) && (bot.color == BLACK))
							{
								moves[*nbMove].src_point = 0;
								moves[*nbMove].dest_point = 25 - dice[j];
								*nbMove = *nbMove + 1;
								dice[j] = -1;
								break; 
							}
						}
					}
					
					//Si on a fait moins de moves que de pions dans le bar on n'a pas le droit de jouer donc on return, idem si on a fait autant de moves qu'il y a de dés
					if((*nbMove < gameState->bar[bot.color]) || (*nbMove == sizeDice))
					{
						free(dice);
						return;
					}
				}
				
				
				
			
				//Remplissage tableau contenant les indices des cases sur lesquelles sont présents les pions du bot 
				//(On ne le remplit pas dans le même sens en fonction de la couleur du bot, cela facilite l'implémentation de l'IA pour les 2 couleurs)
				if(bot.color == BLACK)
				{
					for(i=23;i>=0;i--)
					{
						if (gameState->board[i].owner == bot.color)
						{
							casesPionsBot[j] = i;
							j++;
						}
					}
				}
				else
				{
					for(i=0;i<24;i++)
					{
						if (gameState->board[i].owner == bot.color)
						{
							casesPionsBot[j] = i;
							j++;
						}
					}
				}

				dim = j; 
				
				
				
				
				/*** Permet, si on le peut, de faire bouger 2 pions sur la même case ***/
		
				/* Parcours du tableau contenant les indices des cases de nos pions
				   On va comparer tous les indices entre eux */
				   
				if((*nbMove == 0) && (!IsDiceDouble(dices)))
				{
			
					for(i=1;i<dim;i++)
					{
						for(j=1;j<dim;j++)
						{
			
							
							// Vérification : la dist entre les cases = la dist entre les dés + la case d'arrivée ne doit pas être a l'ennemi OU il ne doit y avoir qu'un seul pion dessus.
							if(((int)fabs(casesPionsBot[i]-casesPionsBot[j]) == (int)fabs(dice[0]-dice[1])) && (IsMoveRight(min(casesPionsBot[i],casesPionsBot[j]),max(dice[0],dice[1]),gameState)))
							{
								if((bot.color == WHITE) && (IsMoveRight(min(casesPionsBot[i],casesPionsBot[j]),max(dice[0],dice[1]),gameState)))
								{
									*nbMove = 2;
									moves[0].src_point = min(casesPionsBot[i],casesPionsBot[j])+1;
									moves[0].dest_point = moves[0].src_point + max(dice[0],dice[1]);
									
									moves[1].src_point = max(casesPionsBot[i],casesPionsBot[j])+1;
									moves[1].dest_point = moves[1].src_point + min(dice[0],dice[1]);
									
									free(dice);
									return;
								}
								else if((bot.color == BLACK) && (IsMoveRight(min(casesPionsBot[i],casesPionsBot[j]),min(dice[0],dice[1]),gameState)))
								{
									*nbMove = 2;
									moves[0].src_point = min(casesPionsBot[i],casesPionsBot[j])+1;
									moves[0].dest_point = moves[0].src_point - min(dice[0],dice[1]);
									
									moves[1].src_point = max(casesPionsBot[i],casesPionsBot[j])+1;
									moves[1].dest_point = moves[1].src_point - max(dice[0],dice[1]);
									
									free(dice);
									return;
								}
								
							}
						}
					}
				}
				
				
				
				/*** QUAND ON A UN DOUBLE + 2 ou 4 dés dispos : on bouge 2 pions de la même case sur une autre case ***/
				
				if((IsDiceDouble(dices)) && (NbDiceLeft(dice,sizeDice)%2 == 0)) 
				{
					
					for(i=1;i<dim;i++)
					{
						//On vérifie que les mouvements intermédiaires sont bons et que la case d'arrivée est libre (4 dés)
						numCaseInter = 0;
						if(((gameState->board[casesPionsBot[i]].nbDames == 2) || (gameState->board[casesPionsBot[i]].nbDames > 3)) && (NbDiceLeft(dice,sizeDice) == 4) && (IsMoveRight(casesPionsBot[i],((int)dices[0])*2,gameState)) && (IsMoveRight(casesPionsBot[i],(int)dices[0],gameState)) && (!IsCaseEmpty(casesPionsBot[i],*nbMove,moves,gameState)))
						{
			
							ApplyMove(nbMove,moves,casesPionsBot[i],dice,0);
							ApplyMove(nbMove,moves,moves[*nbMove-1].dest_point-1,dice,1);
							ApplyMove(nbMove,moves,casesPionsBot[i],dice,2);
							ApplyMove(nbMove,moves,moves[*nbMove-1].dest_point-1,dice,3);
							
							free(dice);
							return;
							
						
						}
						// (2 dés)
						else if(((gameState->board[casesPionsBot[i]].nbDames == 2) || (gameState->board[casesPionsBot[i]].nbDames > 3)) && (NbDiceLeft(dice,sizeDice) == 2) && (IsMoveRight(casesPionsBot[i],(int)dices[0],gameState)) && (!IsCaseEmpty(casesPionsBot[i],*nbMove,moves,gameState)))
						{
							if(bot.color == WHITE)
							{
								moves[*nbMove].src_point = casesPionsBot[i] + 1;
								moves[*nbMove].dest_point = casesPionsBot[i] + (int)dices[0] + 1;
								*nbMove = *nbMove+1;
								
								moves[*nbMove].src_point = casesPionsBot[i] + 1;
								moves[*nbMove].dest_point = casesPionsBot[i] + (int)dices[0] + 1;
								*nbMove = *nbMove+1;
								
								free(dice);
								return;
							}
							else //BLACK
							{
								moves[*nbMove].src_point = casesPionsBot[i] + 1;
								moves[*nbMove].dest_point = casesPionsBot[i] - (int)dices[0] + 1;
								*nbMove = *nbMove+1;
								
								moves[*nbMove].src_point = casesPionsBot[i] + 1;
								moves[*nbMove].dest_point = casesPionsBot[i] - (int)dices[0] + 1;
								*nbMove = *nbMove+1;
								
								free(dice);
								return;
							}
						}
					}
				}
				
				
				
				// On fait bouger un pion de la somme des dés
				for(i=1;i<dim;i++)
				{
					nbMoveInter = 0;
					numCaseInter = 0;
					sumInter = 0;
					if((IsCaseOurs(casesPionsBot[i],SumDice(dice,sizeDice),gameState)) && (!IsCaseEmpty(casesPionsBot[i],*nbMove,moves,gameState)))
					{
						for(j=0;j<sizeDice;j++)
						{
							if(!IsDiceUsed(dice[j]))
							{
								sumInter += dice[j];
							}
							
							if(IsMoveRight(casesPionsBot[i],sumInter,gameState))
							{
								nbMoveInter++;
							}
						}
						if(nbMoveInter == NbDiceLeft(dice,sizeDice))
						{
							for(j=0;j<sizeDice;j++)
							{
								if((!IsDiceUsed(dice[j])) && (numCaseInter == 0))
								{
									ApplyMove(nbMove, moves, casesPionsBot[i], dice, j);
									numCaseInter = 1;
									
									
								}
								else if((!IsDiceUsed(dice[j])) && (numCaseInter != 0))
								{
									ApplyMove(nbMove,moves,moves[*nbMove-1].dest_point-1,dice,j);
								}
								if (*nbMove == sizeDice)
								{
									free(dice);
									return;
								}
							}
						}
					}
				}
				
				
				
				/*** Mouvement de pions sur une case alliée si on a pas pu faire le coup d'avant ***/
				
				for(i=1;i<dim;i++)
				{
					for(j=0;j<sizeDice;j++)
					{
						
						if((IsCaseOurs(casesPionsBot[i],dice[j],gameState)) && (!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove,moves,gameState)))
						{
							ApplyMove(nbMove,moves,casesPionsBot[i],dice,j);
							printf("Move safe 1 pion\n");
						}
						
						if(*nbMove == sizeDice)
						{
							free(dice);
							return;
						}
					}
				}
					
				
				
					/*** Si on a réussi à faire aucun des moves d'avant ou s'il reste des moves à faire, on va bouger le ou les pions les plus éloignés 
					(en partant de l'avant derniere case de notre couleur pour garder une ancre) sur une case dispo où une case où il n'y a qu'un pion ennemi ***/
					
					
					
					
				for(i=1;i<dim;i++)
				{
					for(j=0;j<sizeDice;j++)
					{

						if((IsMoveRight(casesPionsBot[i],dice[j],gameState)) && (!IsCaseEmpty(casesPionsBot[i],*nbMove,moves,gameState)))
						{
							ApplyMove(nbMove,moves,casesPionsBot[i],dice,j);
							printf("Normal\n");
						}
						if(*nbMove == sizeDice)
						{
							free(dice);
							return;
						}
						
					}
				}
				
				
				//Mouvement de l'ancre si vraiment on ne peut rien faire d'autre
				for(j=0;j<sizeDice;j++)
				{
					if((IsMoveRight(casesPionsBot[0],dice[j],gameState)) && (!IsCaseEmpty(casesPionsBot[0],*nbMove,moves,gameState)))
					{
						ApplyMove(nbMove,moves,casesPionsBot[0],dice,j);
					}
				}
				
				sortie = 0;
				break;
				
				
				
			case FIN :
			
				printf("\n Etat FIN\n");
				
				//Remplissage tableau contenant les indices des cases sur lesquelles sont présents les pions du bot
				if(bot.color == BLACK)
				{
					for(i=23;i>=0;i--)
					{
						if (gameState->board[i].owner == bot.color)
						{
							casesPionsBot[j] = i;
							j++;
						}
					}
				}
				else
				{
					for(i=0;i<24;i++)
					{
						if (gameState->board[i].owner == bot.color)
						{
							casesPionsBot[j] = i;
							j++;
						}
					}
				}
				dim = j;
				
				
				// Ici on regarde si on peut bouger un pion directement dans le out
				for(i=0;i<dim;i++)
				{
					printf("Case %d : %d \n",casesPionsBot[i]+1,gameState->board[casesPionsBot[i]].nbDames );
					for(j=0;j<sizeDice;j++)
					{
						if((!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove, moves,gameState)) && ((casesPionsBot[i]-dice[j]) == (-1)) && (bot.color == BLACK))
						{
							moves[*nbMove].src_point = casesPionsBot[i] + 1;
							moves[*nbMove].dest_point = 25;
							*nbMove = *nbMove + 1;
							dice[j] = -1;
						}
						else if((!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove, moves,gameState)) && ((casesPionsBot[i]+dice[j]) == 24) && (bot.color == WHITE))
						{
							moves[*nbMove].src_point = casesPionsBot[i] + 1;
							moves[*nbMove].dest_point = 25;
							*nbMove = *nbMove + 1;
							dice[j] = -1;
						}
					}
				}
				
				
				// Ici on regarde si on peut bouger le pion le plus éloigné sur une case plus proche du out
				for(i=dim-1;i>=0;i--)
				{
					for(j=0;j<sizeDice;j++)
					{
						if((!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove, moves,gameState)) && (casesPionsBot[i]-dice[j] >= (-1)) && (IsMoveRight(casesPionsBot[i],dice[j],gameState)) && (bot.color == BLACK))
						{
							ApplyMove(nbMove,moves,casesPionsBot[i],dice,j);

						}
						else if((!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove, moves,gameState)) && (casesPionsBot[i]+dice[j] <= 24) && (IsMoveRight(casesPionsBot[i],dice[j],gameState)) && (bot.color == WHITE))
						{
							ApplyMove(nbMove,moves,casesPionsBot[i],dice,j);
						}
					}
				}
				
				
				//Ici on regarde (si les mouvements d'avant n'ont pas été faits -> on a le droit) si on peut bouger un pion dans le out alors que la valeur du dé est plus élevée 
				for(i=0;i<dim;i++)
				{
					for(j=0;j<sizeDice;j++)
					{
						if((!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove, moves,gameState) && (casesPionsBot[i]-dice[j] <= (-1)) && (bot.color == BLACK)))
						{
							moves[*nbMove].src_point = casesPionsBot[i] + 1;
							moves[*nbMove].dest_point = 25;
							*nbMove = *nbMove + 1;
							dice[j] = -1;
						}
						else if((!IsDiceUsed(dice[j])) && (!IsCaseEmpty(casesPionsBot[i],*nbMove, moves,gameState) && (casesPionsBot[i]+dice[j] >= 24) && (bot.color == WHITE)))
						{
							moves[*nbMove].src_point = casesPionsBot[i] + 1;
							moves[*nbMove].dest_point = 25;
							*nbMove = *nbMove + 1;
							dice[j] = -1;
						}
					}
				}
				
				sortie = 0;
				break;
			
			
			
					
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	

	
		
		

	free(dice);
	

	
	
	
	
	printf("PlayTurn\n");
	return;
}






