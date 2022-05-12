## Intro
Le but de ce TP est de construire, à partir d'une image représentant un labyrinthe, un graph. À partir de ce graph, il vous sera demandé de trouver le chemin le plus court vers la sortie et de créer une nouvelle image contenant la solution avec la couleur rouge.

## Propriété des labyrinthes:
- L'entrée se retrouvera dans la ligne du haut
- La sortie se retrouvera dans la ligne du bas
- Un pixel blanc représente un chemin accessible
- Un pixel noir représente un mur
- combo400 peut avoir plusieurs solutions, mais perfect2k a seulement une solution.

## Besoin
- Choisir une structure d'adjacence, soit une matrice d'adjacence ou une liste d'adjacence. Essayez de penser en terme d'espace suscité et de performance d'accès. Au début, choisissez une structure aléatoire, et au fur et à mesure que vous aggrandissez de graph, faites du profiling afin de voir si une structure pourrait être plus rapide.
- Avoir une structure de données représentant un Node du graph
- Chaque pixel représente un node. À vous de décider si vous prennez les pixels mur et chemin comme node.
- Utilisez leur position x et y dans le graph afin d'avoir leur position
- Votre application devrait générer une image solution pour chaque labyrinthe.
- Pour lire et générer les images, utilisez ces singles files library: [read](https://github.com/nothings/stb/blob/master/stb_image.h) et [write](https://github.com/nothings/stb/blob/master/stb_image_write.h). Pour vous aider, voici un [lien](https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/)

## À remettre
- Votre code avec les deux structures d'adjacences utilisées et l'algorithme A*. Par simplicité, vous pouvez remettre une version d'A* pour chacune des structures.
- Les fichiers de profiling fournis par Optick. Je veux avoir soit deux fichiers pour chacune des fonctions A* ou un seul fichier qui contient l'appel des deux fonctions. Vous pouvez remettre ces fichiers dans votre repos. Idéalement, essayez de profiler plus que les fonctions A*.
- Votre hypothèse sur la structure la plus performante et ce qui pourrait justifier cette rapidité. Remettez votre information en fichier .md
- Une des fonctions A* doit pouvoir parcourir tous les graphs fournis.

## ATTENTION
Si vous vous aidez entre vous, assurez vous que la personne qui vous aide ou celui qui est aidé n'a pas exactement le même code. Sinon,vous aurez 0 tous les deux. De plus, mettez des commentaires à votre code.
