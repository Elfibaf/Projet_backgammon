#include "gui.h"
#include "../api.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

 
void setBoardTokens(const SGameState * const state, SDL_Rect noirs[15] , SDL_Rect rouges[15], SDL_Rect *rectPlateau)
{
    printf("Calcul des coordonnees des jetons ... \n");
    //Tableau d'equivalence entre les cases et leurs positions en pixels
    int equivalence_x[12] = {1087, 1014, 944, 872, 802, 730, 581, 509, 438, 367, 296, 224};
    
    //Tableau d'equivalence entre la position sur une case et la position en pixel correspondante
    int equivalence_y[6] = {662, 625, 588, 551, 514, 477};
    int equivalence_y2[6] = {50, 87, 124, 161, 198, 235};
    
    //Tableau d'equivalence entre les coordonnees de out et le nombre de jetons noirs dans out
    int equivalence_outN[15] = {604, 593, 582, 571, 560, 549, 538, 527, 516, 505, 494, 483, 472, 461, 451};
    
    //Tableau d'equivalence entre les coordonnees de out et le nombre de jetons blancs dans out
    int equivalence_outB[15] = {107, 118, 129, 140, 151, 162, 173, 184, 195, 206, 217, 228, 239, 250, 261};
    
    //Tableau d'equivalence entre les coordonnees de bar et le nombre de jetons noirs dans bar
    int equivalence_barN[15] = {500, 492, 484, 475, 467, 459, 450, 442, 434, 425, 417, 409, 400, 392, 384};
    
    //Tableau d'equivalence entre les coordonees de bar et le nombre de jetons blancs dans bar
    int equivalence_barB[15] = {210, 218, 226, 235, 243, 251, 260, 268, 276, 285, 293, 301, 310, 318, 326};
    
    rectPlateau->x = 0;
    rectPlateau->y = 0;
    
    int i = 0, j = 0, cptN = 0, cptR = 0;
    
    //Parcours du SGameState
    for (i = 0; i<24; i++)
    {
        if (state->board[i].owner == 0)
        {
            for (j = 0; j < state->board[i].nbDames; j++)
            {
		if (i<12) noirs[cptN].x = equivalence_x[i];
	        else noirs[cptN].x = equivalence_x[24-(i+1)];
                
                if(i<12) noirs[cptN].y = equivalence_y[j];
                else noirs[cptN].y = equivalence_y2[j];

                cptN++;
            }
        }
        else
        {
            for (j = 0; j<state->board[i].nbDames; j++)
            {
		if(i<12) rouges[cptR].x = equivalence_x[i];
		else rouges[cptR].x = equivalence_x[24-(i+1)];
                
                if(i<12) rouges[cptR].y = equivalence_y[j];
		else rouges[cptR].y = equivalence_y2[j];
		
                cptR ++;
            }
        }
    }
    
    //Affichage de la sortie des jetons noirs
    if ( state->out[0] != 0 )
    {
      for(i = 0; i < state->out[0]; i++)
      {
	noirs[cptN].x = 1167;
	noirs[cptN].y = equivalence_outN[i];
	cptN++;
      }
    }
    
    
    //Affichage de la sortie des jetons blancs
    if ( state->out[1] != 0 )
    {
      for(i = 0; i < state->out[1]; i++)
      {
	rouges[cptR].x = 1167;
	rouges[cptR].y = equivalence_outB[i];
	cptR++;
      }
    }
    
    //Affichage du bar de jetons noirs
    if ( state->bar[0] != 0 )
    {
      for (i = 0; i < state->bar[0]; i++)
      {
	noirs[cptN].x = 655;
	noirs[cptN].y = equivalence_barN[i];
	cptN++;
      }
    }
    
    //Affichage du bar de jetons noirs
    if ( state->bar[1] != 0 )
    {
      for (i = 0; i < state->bar[1]; i++)
      {
	rouges[cptR].x = 655;
	rouges[cptR].y = equivalence_barB[i];
	cptR++;
      }
    }
    
}

 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void afficher(SDL_Surface *surfPlateau, SDL_Surface *surfJetonNoir, SDL_Surface *surfJetonBlanc, SDL_Rect noirs[15] , SDL_Rect blancs[15], SDL_Rect *rectPlateau, SDL_Rect *rectDes, SDL_Surface *screen)
{
	int i = 0, j = 0;
	printf("Affichage ... \n");
	//Afficher un ecran noir
	SDL_FillRect(screen, NULL, SDL_MapRGB(surfPlateau->format, 0,0,0));

	//Affichage du plateau 
	SDL_BlitSurface(surfPlateau, 0, screen, rectPlateau);

	//Parcours du tableau de jetons noirs et affichage
	for (i=0; i<15;i++)
	{
	    //printf("BlitSurface du jeton i = %d \n",i); 
	    SDL_BlitSurface(surfJetonNoir, 0, screen, &noirs[i]);
	}

	//Parcours du tableau de jetons blancs et affichage
	for (j=0; j<15;j++)
	{
	    //printf("BlitSurface du jeton j = %d \n",j);
	    SDL_BlitSurface(surfJetonBlanc, 0, screen, &blancs[j]);
	}
}


 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void afficherDes(SDL_Surface *des, SDL_Rect *rectDes, unsigned char dices[2], char stringDes[10], SDL_Color colorFont, TTF_Font *font, SDL_Surface *screen)
{
      sprintf(stringDes, " %d : %d ", dices[0], dices[1]);
      
      des = TTF_RenderText_Blended(font, stringDes, colorFont);
      
      SDL_BlitSurface(des, 0, screen, rectDes);
}


 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void afficherScore(SDL_Surface *titleBlack, SDL_Surface *titleWhite, SDL_Surface *scoreBlack, SDL_Surface *scoreWhite, SDL_Rect *rectScoreBlack, SDL_Rect *rectScoreWhite, SDL_Rect *rectTitleBlack, SDL_Rect *rectTitleWhite, char stringScoreBlack[20], char stringScoreWhite[20], SDL_Color colorFont, TTF_Font *font, SDL_Surface *screen, unsigned int blackScore, unsigned int whiteScore )
{
      sprintf(stringScoreBlack, "%d", blackScore);
      sprintf(stringScoreWhite, "%d", whiteScore);
      
      titleWhite = TTF_RenderText_Blended(font, "Black : ", colorFont);
      titleBlack = TTF_RenderText_Blended(font, "White : ", colorFont);
      
      scoreBlack = TTF_RenderText_Blended(font, stringScoreBlack, colorFont);
      scoreWhite = TTF_RenderText_Blended(font, stringScoreWhite, colorFont);
      
      SDL_BlitSurface(titleBlack, 0, screen, rectTitleBlack);
      SDL_BlitSurface(titleWhite, 0, screen, rectTitleWhite);
      SDL_BlitSurface(scoreBlack, 0, screen, rectScoreBlack);
      SDL_BlitSurface(scoreWhite, 0, screen, rectScoreWhite);
}

void afficherMise(SDL_Surface *mise, SDL_Rect *rectMise, char stringMise[20], SDL_Color colorFont, TTF_Font *font, SDL_Surface *screen, unsigned int stake)
{
      sprintf(stringMise, "%d", stake);
      
      mise = TTF_RenderText_Blended(font, stringMise, colorFont);
      
      SDL_BlitSurface(mise, 0, screen, rectMise);
}
 
 
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
 void initHitBoxesTab(Hitbox *hitboxesTab, SDL_Surface* screen)
{

    int i,y1 = 477, y2= 50, width1 = 48, height = 233; // Coordonnées qui resteront inchangées pour les hitboxs



    // Pour chaque hitbox, on met les caractéristiques nécessaires pour les modéliser
    for(i = 0; i < 28; i++){

        hitboxesTab[i].rectHB = (SDL_Rect*) malloc (1*sizeof(SDL_Rect)); 

        if(i == 0)
        {
            hitboxesTab[i].rectHB->x = 1087;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 1)
        {
            hitboxesTab[i].rectHB->x = 1014;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 2)
        {
            hitboxesTab[i].rectHB->x = 944;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 3)
        {
            hitboxesTab[i].rectHB->x = 872;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 4)
        {
            hitboxesTab[i].rectHB->x = 802;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 5)
        {
            hitboxesTab[i].rectHB->x = 730;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 6)
        {
            hitboxesTab[i].rectHB->x = 581;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 7)
        {
            hitboxesTab[i].rectHB->x = 509;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 8)
        {
            hitboxesTab[i].rectHB->x = 438;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 9)
        {
            hitboxesTab[i].rectHB->x = 367;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 10)
        {
            hitboxesTab[i].rectHB->x = 296;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 11)
        {
            hitboxesTab[i].rectHB->x = 224;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 12)
        {
            hitboxesTab[i].rectHB->x = 224;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 13)
        {
            hitboxesTab[i].rectHB->x = 296;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 14)
        {
            hitboxesTab[i].rectHB->x = 367;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 15)
        {
            hitboxesTab[i].rectHB->x = 438;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 16)
        {
            hitboxesTab[i].rectHB->x = 509;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 17)
        {
            hitboxesTab[i].rectHB->x = 581;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 18)
        {
            hitboxesTab[i].rectHB->x = 730;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 19)
        {
            hitboxesTab[i].rectHB->x = 802;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 20)
        {
            hitboxesTab[i].rectHB->x = 872;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 21)
        {
            hitboxesTab[i].rectHB->x = 944;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 22)
        {
            hitboxesTab[i].rectHB->x = 1014;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }

        else if(i == 23)
        {
            hitboxesTab[i].rectHB->x = 1087;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
        }
	//bar BLACK
	else if (i == 24)                               
	{
	    hitboxesTab[i].rectHB->x = 655;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	//bar WHITE
	else if (i == 25)
	{
            hitboxesTab[i].rectHB->x = 655;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	//out WHITE
	else if (i == 26)
	{
            hitboxesTab[i].rectHB->x = 1167;
            hitboxesTab[i].rectHB->y = y2;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	//out BLACK
	else if (i == 27)
	{
            hitboxesTab[i].rectHB->x = 1167;
            hitboxesTab[i].rectHB->y = y1;
            hitboxesTab[i].rectHB->w = width1;
            hitboxesTab[i].rectHB->h = height;
	}
	
       //SDL_FillRect(screen, hitboxesTab[i].rectHB, SDL_MapRGB(screen->format, 255, 0, 0)); TEST, qui permettait d'afficher les hitbox pour voir si elles sont au bon endroit

    }

}


 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int detectClickIntoHitbox(Hitbox *hitboxesTab, int x, int y)
{
    int i;


    // Pour chaque hitbox
    for(i = 0; i < 28; i++)
    {
        
        
        // On vérifie si l'abscisse de la hitbox récupérée est comprise entre les coordonnées de la hitbox courante
        if((x >= hitboxesTab[i].rectHB->x) && (x <= hitboxesTab[i].rectHB->x + hitboxesTab[i].rectHB->w))
        {
            // On vérifie si l'ordonnée de la hitbox récupérée est comprise entre les coordonnées de la hitbox courante
            if((y >= hitboxesTab[i].rectHB->y) && (y <= hitboxesTab[i].rectHB->y + hitboxesTab[i].rectHB->h))
            {
                //printf("Hitbox numéro : %d atteinte\n", i); TEST
                return i;
            }
        }
    }
    printf("Aucune hitbox atteinte\n");
    return -1; // On retourne -1 si aucune hitbox n'a été atteinte

}

 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void clickToSMoves(int* indiceHBTab, SMove* moves,unsigned int *nbMoves, Player curPlayer, int cpt)
{
    
    // Si le joueur peut encore faire un move
    if(*nbMoves <= cpt)
    {
        
        
        // On parcourt le tableau contenant les hitbox sources et destination
        for (i = 0; i < 2; i++)
        {
            if(indiceHBTab[i]>=0 && indiceHBTab[i] <= 23) indiceHBTab[i] += 1; // Si la hitbox touchée est comprise entre 0 et 23, alors cela signifie que la case du Board correspondant est le numéro de cette hitbox -1
            else if(indiceHBTab[i] == 24 || indiceHBTab[i] == 25) indiceHBTab[i] = 0; // Si la hitbox touchée est égale à 24 ou 25 , alors cela signifie que la case du Board correspondant est le bar black ou white
            else if(indiceHBTab[i] == 26 || indiceHBTab[i] == 27) indiceHBTab[i] = 25; // Si la hitbox touchée est égale à 26 ou 27 , alors cela signifie que la case du Board correspondant est le out black ou white

        }

        //printf("indiceHBTab[0] %d | indiceHBTab[1] %d | nbMoves %d\n",indiceHBTab[0],indiceHBTab[1], *nbMoves); TEST
        
        // Si le joueur est WHITE, alors on a pas besoin de faire de modifications supplémentaires et on ajoute les cases du board correspondant dans la source et la destination du nouveau move
        if(curPlayer == WHITE)
        {

            moves[*nbMoves].src_point = indiceHBTab[0];
            moves[*nbMoves].dest_point = indiceHBTab[1];
        }

        // Si le joueur est BLACK, alors on a besoin de changer encore la case correspondante puisque le joueur BLACK parcourt le plateau dans l'autre sens et on ajoute les cases du board correspondant dans la source et la destination du nouveau move
        else if(curPlayer == BLACK)
        {

            if(indiceHBTab[0]!=0 && indiceHBTab[0]!=25)
            {
                moves[*nbMoves].src_point = 24-indiceHBTab[0]+1;
            }
            else
            {
                moves[*nbMoves].src_point = indiceHBTab[0];
            }

            if(indiceHBTab[1]!=0 && indiceHBTab[1]!=25)
            {
                moves[*nbMoves].dest_point = 24-indiceHBTab[1]+1;
            }
            else
            {
                moves[*nbMoves].dest_point = indiceHBTab[1];
            }

        }

        printf("moves added between %d and %d\n",moves[*nbMoves].src_point,moves[*nbMoves].dest_point);
        
        // On incrémente le nombre de moves
	    *nbMoves = *nbMoves + 1;

        // On réinitialise les valeurs du tableau contenant les hitbox touchées
        indiceHBTab[0] = -1;
        indiceHBTab[1] = -1;
    }
    else
    {
        // Sinon on affiche que tous le nombre de maximum est atteint donc on fait pas de moves
        printf("Tous les moves ont déjà été faits : %d MOVES \n", cpt);
    }


}
 
 
