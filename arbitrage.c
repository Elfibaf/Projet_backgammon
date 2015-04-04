#include <stdio.h>
#include <stdlib.h>

#include "arbitrage.h"
#include "bot.h"


/*
Fonction d'initialisation du tableau
Prend en parametre un SGameState
*/
void InitPlateau(SGameState * gameState)
{
    // Initialisation du board
    int i;
    for( i = 0; i < 24; i++ )
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


/*
Simulation d'un lancé de deux dés
*/
void GenerateDices(unsigned char dices[2])
{
    
    dices[0] = (char)(rand()%(6)+1);
    dices[1] = (char)(rand()%(6)+1);
    printf("Dé 1 : %d    Dé 2 : %d\n",(int)dices[0],(int)dices[1]);
    
}


int VerifCoup(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMoves, unsigned int tries)
{
    // Je vais faire la fonction plus tard
    return 0;
}


int WinGame(const SGameState * const gameState, int player)
{
    if(gameState->out[player] == 15) // Si tous les pions sont sortis du plateau
    {
        return 1;
    }
    else {
        return 0;
    }
}

void ModifPlateau(SGameState * gameState, SMove moves[4], unsigned int *nbMoves, Player player) {
    
    Player enemy;
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
            if (gameState->board[x-1].nbDames == 0) {
                
                gameState->board[y-1].owner = -1;
                
        }
        
    
        
        if (y == 25) {
            gameState->out[player] = gameState->out[player] + 1;
        } 
        
        else if (gameState->board[y-1].owner == -1) {
            gameState->board[y-1].nbDames = gameState->board[y-1].nbDames + 1;
            gameState->board[y-1].owner = player;
        } 
        
        else if (gameState->board[y-1].owner != player) {
            enemy = 1 - gameState->board[y-1].owner;
            gameState->board[y-1].owner = enemy;
            gameState->bar[enemy] = gameState->bar[enemy] + 1;
        }
        printf("Le joueur %d bouge de la case %d à la case %d\n",(int)player,x,y);
    }
}