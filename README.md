# Projet_backgammon


Suivez ce tuto pour lier votre C9 à gitHub :

http://doc.ubuntu-fr.org/git



Pour commit, effectuer ces 3 commandes :

git pull

git commit -m "commentaire" nomfichier

git push



/!\ Pour la compilation /!\
-Il faut d'abord compiler la librairie du bot :
    gcc -Wall -std=c99 -fPIC -c bot.c
    gcc -Wall -std=c99 -shared -o bot.so bot.o

-Ensuite vous devez ajouter l'emplacement de la librairie dans la variable d'environnement LD_LIBRARY_PATH :
    export LD_LIBRARY_PATH=:chemindaccesenabsolu

-Enfin pour compiler le main :
    gcc -Wall main.c -o main -ldl

gcc -Wall main2.c arbitrage.c gui/gui.c -o main `sdl2-config --cflags --libs` -ldl -lSDL2_image -lSDL2_ttf

    
    
    
    
///// EXPLICATION DU BOT /////

-La stratégie:
    Pour établir notre stratégie nous avons établis au préalable plusieurs points à travailler, ces différents points se regroupaient en deux grandes phases:
    L'attaque et la défense
    Afin de jouer la carte de la surteté, notre bot cherche à être éfficace tout en étant en sécurité.
    Par conséquent, il fallait prendre en compte les différents éléments essentiels au backgammon, comme les doubles, le bar, l'out, mais également le facteur chance et essayé d'anticiper les mouvements de l'adversaire
    Voici les différents points qui nous ont paru essentiels à travailler :
        -Ne pas laisser de pion solitaire pour éviter de se faire "manger" par l'adversaire
        -Lorsqu'un double est réalisé, déplacer nos pions de façon homogène si possible, c'est-à-dire de déplacer 4 pions provenant de la même case ou faire deux puis deux d'une autre case, afin de ne pas éparpiller nos pions n'importe où
        -Tenter de bloquer l'adversaire en remplissant le plus vite possible les cases proches de notre out.
        -Garder tout de même les 2 pions les plus éloignés de notre out, pour pouvoir capturer l'adversaire si possible.
    
-L'évolution au cours du temps:    
    Pour garder cette stratégie la plus claire possible nous pensions créer diffénrents états afin de savoir dans quel avancement du jeu sommes nous. Il exsite donc pour nous 3 états, un état normal.
    du jeu (comme par exemple sortir les pions de "prison")
    Un état de transition où on déplace nos pions vers l'out de la meilleure façon possible
    Puis un état fin qui est destiné à déplacer nos pions dans l'out.
    
    
-Le codage:
    Afin de rendre cette stratégie réalisable, on a dû mettre en place un grande nombre de fonctions et de variables
    
    -Connaître les caractéristiques du plateau:
        Pour avoir le maximum d'informations sur nos pions mais également sur l'adversaire nous devons effectuer une bonne lecture du plateau.
        Pour cela on remplit un tableau (board appartenant à notre structure) contenant les indices des cases sur lesquelles sont présents les pions du bot cependant 
        on ne le remplit pas dans le même sens en fonction de la couleur du bot, cela facilite l'implémentation de l'IA pour les 2 couleurs.
        De plus nous effectuons des analyses sur les cases, telles que:
            -Vérification de l'appartenance de la case
            -Si une case est vide
            -Si c'est une case du jan intérieur
    
    
    -Les moves:
        Tout d'abord, pour que notre bot ne se trompe pas de mouvement, nous avons vérifié quelles possibilités s'offrent à lui lorsque c'est à son tour. Par exemple imaginons que notre bot ait deux moves à éffectuer, alors on vérifie
        toutes les cases atteignables, ainsi que si le move est possible (donc s'il n'y a pas plus d'un pion adverse ou si on va vers l'avant).
        En fait nous avons une variable dices représentant le nombre de dés, puis une variable dice qui représente la valeur de notre dé.
        La variable moves, elle, représente un tableau de move qui ne peut pas dépasser 4 éléments car il est impossible d'éffectuer plus de 4 déplacements tandis que nbMove représente le nombre de move éffectué
        D'ailleurs nbMove évolue tout au long du jeu afin de savoir à quel avancement nous en sommes au cours de la partie. Cette variable est manipulée de sorte que notre stratégie soit claire et fluide toute au long de la partie.
        Il existe également la variable nbMoveInter qui est utile lorsque nous voulons effectué un double déplacement avec un pion (par exemple de 2 à 3 puis de 3 à 6 avec un seul pion en faisant 1/3 avec les dés)
        De plus on vérifie si nos dés ont déjà été joué afin de ne pas jouer une fois de trop par exemple, on vérifie si le dés forment un double et le nombre de dés qu'il reste.
        Lorsque nous déplaçons nos pions nous utilisons la fonction ApplyMove afin de déplacer notre pions d'une case source vers une case de destination.
    
    -Les différents états:
        
        -L'Etat TRANSITION: (Etat dans lequel on ramene les pions les plus éloignés en priorité)
        
        Dans cet Etat, l'objectif est simple, il s'agit uniquement d'un déplacement de nos pions vers notre jan intérieur. 
        Cependant nous déplaçons en priorité l'ancre c'est-à-dire une case où on a au moins deux pions afin de le mettre dans le côté adverse. Ici l'avantage est double car on pourra bloquer l'adevrsaire si jamais il veut sortir
        du bar mais on peut également être en sécurité et déplacer nos pions sur cet ancre si c'est nécéssaire
        
        -L'Etat NORMAL: (Là où se passe l'essentiel des moves, c'est-à-dire du début du jeu jusqu'à l'étape de transition où nous déplaçons nos pions vers le jan intérieur)
        
        Dans cet Etat on prendre évidemment en compte le bar. C'est-à-dire que si notre bar n'est pas vide nous sommes obligés de jouer ce ou ces pions en priorité. Voici l'odre de priorité des moves :
        
            1.Sinon nous effectuons des moves afin de nous rapprocher du jan intérieur tout en restant sécurisé. Donc quand indiqué sur notre point stratégie nous déplaçons en priorité deux pions sur la même case.
            2.Ou sinon lorsque l'on a un double, déplacer deux pions d'une même case vers une autre case (qui pour nous est plus rentable que de déplacer 4 pions sur un même case)
            3.Ou si tout cela n'est pas possible, bouger un seul pion sur un case lointaine, car statistiquement nous avons moins de chance d'être "attrapé" par l'adversaire
            4.Ou si on ne peut faire aucun de ces moves, bouger un pion vers une case alliée
            5.Bouger les pions qui sont le plus éloigné sur une case vide et disponible ou sur une case composé d'un seul pion
            6.Bouger l'ancre
            
        -L'Etat FIN:
        
        Ici il s'agit de déplacer nos pions dans l'out lorsque c'est possible, ou si ce n'est pas possible se rapprocher au mieux de l'out (donc de la case finale).
        
        NB : Bien évidemment dans chacun de ces Etat les doubles sont pris en compte
        
        
