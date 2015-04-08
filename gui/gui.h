#include <SDL2/SDL.h>
#include "../bot.h"


// Définition du type Hitbox
typedef struct {
    SDL_Rect *rectHB; 
} Hitbox;


/**
 * Initialisation des Hitbox et du tableau contenant les Hitbox
 * @param Hitbox *hitboxesTab 
 *      Tableau contenant des Hitbox
 * @param SDL_Surface* screen (DEBUG purpose)
 *	    surface de l'écran (pour dessiner les hitboxes à l'écran)
*/

void initHitBoxesTab(Hitbox *hitboxesTab,  SDL_Surface* screen);


/**
 * Renvoie l'indice de la hitbox atteinte par le clic, renvoie -1 sinon
 * @param Hitbox *hitboxesTab
 *	tableau de Hitbox
 * @param int x
 *	abscisse du pointeur de la souris
 * @param int y
 *	ordonnée du pointeur de la souris
 */
int detectClickIntoHitbox(Hitbox *hitboxesTab, int x, int y);




/**
 * Permet d'interpréter les "clicks" du joueur et en fait des moves 
 * @param int* indiceHBTab
 *	tableau contenant les indices des hitbox touchées [0] -> Premier clic, [1] -> Deuxième clic
 * @param SMove* moves
 *	Moves qui seront joués
 * @param unsigned int *nbMoves
 *  Nombre de moves qui seront joués
 * @param Player curPlayer
 *  Joueur courant, qui réalise les clics
 */
 
 
void clickToSMoves(int* indiceHBTab, SMove* moves,unsigned int *nbMoves, Player curPlayer);



