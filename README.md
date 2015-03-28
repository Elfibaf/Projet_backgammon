# Projet_backgammon


Suivez ce tuto pour lier votre C9 à gitHub :

http://doc.ubuntu-fr.org/git



Pour commit, effectuer ces 3 commandes :

git pull

git commit -m "commentaire" nomfichier

git push



/!\ Pour la compilation /!\
-Il faut d'abord compiler la librairie du bot :
    gcc -fPIC -c bot.c
    gcc -shared -o bot.so bot.o

-Ensuite vous devez ajouter l'emplacement de la librairie dans la variable d'environnement LD_LIBRARY_PATH :
    export LD_LIBRARY_PATH=:chemindaccesenabsolu

-Enfin pour compiler le main :
    gcc -Wall main.c -o main -ldl