#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../bot.h"


// Définition du type Hitbox
typedef struct {
    SDL_Rect *rectHB; 
} Hitbox;


/**
 * 
 * Calcul des coordonnees des elements affiches a l'ecran dans l'interface graphique
 * @param const SGameState * const state 
 * 	Structure contenant les informations sur l'etat du jeu
 * @param SDL_Rect noirs[15] 
 * 	tableau contenant les SDL_Rect des jetons noirs à modifier
 * @param SDL_Rect rouges[15] 
 * 	tableau contenant les SDL_Rect des jetons blancs à modifier
 * @param SDL_Rect *rectPlateau 
 * 	SDL_Rect du plateau
 * 
 */
void setBoardTokens(const SGameState * const state, SDL_Rect noirs[15] , SDL_Rect rouges[15], SDL_Rect *rectPlateau);


/**
 * Affichage des elements de l'interface graphique
 * @param SDL_Surface *surfPlateau 
 * 	Surface du plateau dans la GUI
 * @param SDL_Surface *surfJetonNoir 
 * 	Surface du jeton noir dans la GUI
 * @param SDL_Surface *surfJetonBlanc 
 * 	Surface du jeton blanc dans la GUI
 * @param SDL_Rect noirs[15] 
 * 	tableau contenant les SDL_Rect des jetons noirs à modifier
 * @param SDL_Rect blancs[15] 
 * 	tableau contenant les SDL_Rect des jetons blancs à modifier
 * @param SDL_Rect *rectPlateau 
 * 	SDL_Rect du plateau
 * @param SDL_Rect *rectDes 
 * 	SDL_Rect des dés
 * @param SDL_Surface *screen 
 * 	Surface de l'ecran
 * 
 */
void afficher(SDL_Surface *surfPlateau, SDL_Surface *surfJetonNoir, SDL_Surface *surfJetonBlanc, SDL_Rect noirs[15] , SDL_Rect blancs[15], SDL_Rect *rectPlateau, SDL_Rect *rectDes, SDL_Surface *screen);

/**
 * @param SDL_Surface *des 
 * 	Surface d'affichage des dés
 * @param SDL_Rect *rectDes 
 * 	SDL_Rect des dés
 * @param unsigned char dices[2] 
 * 	tableau contenant les dés courants
 * @param char stringDes[10] 
 * 	chaine de caractères correspondant à l'affichage des dés
 * @param SDL_Color colorFont 
 * 	Couleur de la police 
 * @param TTF_Font *font 
 * 	Police 
 * @param SDL_Surface *screen 
 * 	Surface de l'ecran dans la GUI
 */

void afficherDes(SDL_Surface *des, SDL_Rect *rectDes, unsigned char dices[2], char stringDes[10], SDL_Color colorFont, TTF_Font *font, SDL_Surface *screen);

void afficherScore(SDL_Surface *titleBlack, SDL_Surface *titleWhite, SDL_Surface *scoreBlack, SDL_Surface *scoreWhite, SDL_Rect *rectScoreBlack, SDL_Rect *rectScoreWhite, SDL_Rect *rectTitleBlack, SDL_Rect *rectTitleWhite, char stringScoreBlack[20], char stringScoreWhite[20], SDL_Color colorFont, TTF_Font *font, SDL_Surface *screen, unsigned int blackScore, unsigned int whiteScore);

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
 
 
void clickToSMoves(int* indiceHBTab, SMove* moves,unsigned int *nbMoves, Player curPlayer, int cpt);



