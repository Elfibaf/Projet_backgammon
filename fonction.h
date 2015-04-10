/**
 * fonctions permettant de trouver le min ou le max
 * @param int a
 * 	valeur de a
 * @param int b
 * 	valeur de b
 */
int min(int a, int b);


int max(int a,int b);


/**
 * Vérifie que la case sur laquelle le pion va se déplacer est libre, de sa couleur ou n'a qu'un pion dessus
 * @param int numCaseDep
 * 	valeur de la case de départ
 * @param int dice
 * 	valeur du dé
 * @param const SGameState * const gameState
 * 	état du jeu courant
 */
int IsMoveRight(int numCaseDep, int dice, const SGameState * const gameState);

/**
 * Vérifie qu'une case est bien de la couleur de l'IA
 * @param int numCase
 * 	valeur de la case en question
 * @param int dice
 * 	valeur du dé
 * @param const SGameState * const gameState
 * 	état du jeu courant 
 */
int IsCaseOurs(int numCase, int dice, const SGameState * const gameState);

/**
 * Vérifie qu'une case est dans le Jan intérieur
 * @param int numCase
 * 	valeur de la case vérifié
 */
int IsCaseJanInt(int numCase);

/**Indique si le dé a été utilisé ou non (1 s'il a déjà été utilisé, 0 sinon)
 * @param int dice
 * 	valeur du dé
 */
int IsDiceUsed(int dice);


/**Indique si les dés forment un double
 * @param const unsigned char dice[2]
 * 	tableau représentant les deux dés
 */
int IsDiceDouble(const unsigned char dice[2]);

/**Indique si la case a été vidée de ses pions lors du tour courant
 * @param int caseDep
 * 	valeur de la case de départ
 * @param int nbMove
 * 	nombre de move à effectuer
 * @param SMove moves[4]
 * 	Tableau contenant les moves
 * @param  const SGameState * const gameState
 * 	état du jeu courant
 */
int IsCaseEmpty(int caseDep, int nbMove, SMove moves[4], const SGameState * const gameState);

/** Effectue la somme des dés non encore utilisés
 * @param int *dice
 * 	tableau contenant les valeurs des dés
 * @param int sizeDice
 * 	nombre de dés
 */
int SumDice(int *dice,int sizeDice);

/** Retourne le nombre de dés non encore utilisés
 * @param int *dice
 * 	tableau contenant les valeurs des dés
 * @param int sizeDice
 * 	nombre de dés
 */
int NbDiceLeft(int *dice, int sizeDice);

/** fonction permettant d'efféctuer un move
 * @param unsigned int *nbMove
 * 	représente le nombre de moves
 * @param SMove moves[4]
 * 	Tableau contenant les moves
 * @param int caseSrc
 * 	valeur de la case de départ
 * @param int *dice
 * 	tableau contenant les valeurs des dés
 * @param int j
 * 	valeur de j
 */
void ApplyMove(unsigned int *nbMove, SMove moves[4], int caseSrc, int *dice, int j);

/** Définition des diffénrents Etats
 * @param const SGameState * const gameState
 * 	état courant du jeu
 */
Etat DefEtat(const SGameState * const gameState);