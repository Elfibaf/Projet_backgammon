#ifndef BACKGAMMON_H
	#define BACKGAMMON_H

/////////// Strutures � utiliser

/**
 * @brief D�finit quel joueur contr�le la case.
 *
 * Les nombres sont l� pour faciliter les tests.
 */
typedef enum {
    BLACK = 0,
    NOBODY = -1,
    WHITE = 1
} Player;

/**
 * @brief D�finit le contenu d'une case de jeu.
 */
typedef struct {
    Player owner;           /*!<  Joueur contr�lant la case */
    unsigned int nbDames;   /*!<  Nombre de dames sur la case (non utilis� si
                                  `owner == NOBODY.`) */
} Square;


/**
 * @brief Contient toutes les informations relatives � une partie en cours.
 *
 * Une struct contenant toutes les informations du jeu permet des tests plus
 * faciles.
 */
typedef struct {
    Square board[24];           /*!< Les 24 cases du tableau, en partant du bord blanc. */
	unsigned int bar[2];
	unsigned int out[2];
    unsigned int whiteScore;    /*!< Score du joueur blanc */
    unsigned int blackScore;    /*!< Score du joueur noir */
    unsigned int turn;          /*!< Num�ro du tour en cours (initialis� � 0) */
	unsigned int stake;			/*!< Mise courante de la partie */
} SGameState;

// Structure de repr�sentation d'un mouvement
typedef struct
{
	unsigned int src_point;
	unsigned int dest_point;
} SMove;

/////////// Fonctions � utiliser
// Les prototypes en commentaires sont � impl�menter dans la librairie
// Les types pointeur de fonction dans l'application

/**
 * Initialiser la librairie
 * @param char name[50]
 *	nom associ� � la librairie
 */
//void InitLibrary(char name[50]);
typedef void (*pfInitLibrary)(char[50]);

/**
 * Initialiser l'IA pour un match
 * @param const unsigned int target_score
 *	score cible pour gagner un match
 */
//void StartMatch(const unsigned int target_score);
typedef void (*pfStartMatch)(const unsigned int);

/**
 * Initialiser l'IA pour une manche (d'un match)
 */
//void StartGame(Player p);
typedef void (*pfStartGame)(Player);

/**
 * Fin d'une manche (d'un match)
 */
//void EndGame();
typedef void (*pfEndGame)();

/**
 * Fin d'un match
 */
//void EndMatch();
typedef void (*pfEndMatch)();

/**
 * Doubler la mise
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @return int
 *	vrai si on propose de doubler : faux sinon
 */
//int DoubleStack(const SGameState * const gameState);
typedef int (*pfDoubleStack)(const SGameState * const);

/**
 * Accepter ou refuser la nouvelle mise
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @return int
 *	vrai si on accepte la nouvelle mise ; faux sinon
 */
//int TakeDouble(const SGameState * const gameState);
typedef int (*pfTakeDouble)(const SGameState * const);

/**
 * Prise de d�cision de la part de l'IA
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @param SMove moves[4]
 *	tableau des mouvements � effectuer par l'IA
 * @param unsigned int tries
 *	nombre d'essais restants (3 initialement).
 */
//void PlayTurn(const SGameState * const gameState, const unsigned char dices[2], SMove moves[4], unsigned int *nbMove, unsigned int tries);
typedef void (*pfPlayTurn)(const SGameState * const,  const unsigned char[2], SMove[4], unsigned int*, unsigned int);


#endif