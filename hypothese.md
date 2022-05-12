- Votre hypothèse sur la structure la plus performante et ce qui pourrait justifier cette rapidité. Remettez votre information en fichier .md
Il faut enlever cette partie de create_graph :
```
/*graph->adjGraph = (int**)allocate(sizeof(int*) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		graph->adjGraph[i] = (int*)allocate(sizeof(int) * max_nodes);
		for (int y = 0; y < max_nodes; ++y) {
			graph->adjGraph[i][y] = 0;
		}
	}*/
```
parce que quand on veut creer un graph, surtout si c'est celui de "perferct2k.png", sa rend le programme lourd parce que la matrice d'ajdacence prent trop d'espace. Parce qu'il essai de creer des liens sur les nodes. 