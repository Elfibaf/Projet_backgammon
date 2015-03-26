//// Fichier pour l'interface ////

//void deroulement_du_jeu()	
int main()
{
	//*****// � chaque utilisation de gameState, ne pas oublier de faire une copie de tous les �l�ments (pas fait ici)
	SGameState gameState;
	SMove moves[4];

	//*****// � faire pour chaque joueur
	
	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions d�crites dans "backgammon.h")
	// -> � ermplacer par votre code de chargement
	pfInitLibrary j1InitLibrary=InitLibrary;
	pfStartMatch j1StartMatch=StartMatch;
	pfStartGame j1StartGame=StartGame;
	pfEndGame j1EndGame=EndGame;
	pfEndMatch j1EndMatch=EndMatch;
	pfDoubleStack j1DoubleStack=DoubleStack;
	pfTakeDouble j1TakeDouble=TakeDouble;
	pfPlayTurn j1PlayTurn=PlayTurn;
	// ...
	pfTakeDouble j2TakeDouble=TakeDouble;

	// Initialisation de la librairie
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(5);

	unsigned int nbMoves;
	unsigned char dices[2];

		//*****// � faire pour chaque jeu
		j1StartGame(BLACK);
			//*****// pour chaque joueur, tant que ce n'est pas fini
			if (j1DoubleStack(&gameState))
				j2TakeDouble(&gameState);
			j1PlayTurn(&gameState,dices,moves,&nbMoves,3);
		j1EndGame();

	j1EndMatch();

	return(0);
}