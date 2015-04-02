int min(int a, int b);


int max(int a,int b);

int IsMoveRight(int numCaseDep, int dice, const SGameState * const gameState);


int IsCaseOurs(int numCase, const SGameState * const gameState);


int IsCaseJanInt(int numCase);


int IsDiceUsed(int dice);



int IsDiceDouble(const unsigned char dice[2]);


int IsCaseEmpty(int caseDep, int nbMove, SMove moves[4], const SGameState * const gameState);