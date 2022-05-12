#include "labo.h"
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#define MAX 100

#define MAX_NODE 4004001 / 64
#define OPTICK_ENABLE_GPU (0)
#include "optick.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define HEAP_SIZE UINT32_MAX * UINT32_MAX * 9999999999
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

AdjMatrix* create_graph(size_t max_nodes) {
	AdjMatrix* graph = (AdjMatrix*)allocate(sizeof(AdjMatrix));
	graph->len = 0;
	graph->max_size = max_nodes;
	graph->nodes = (Node*)allocate(sizeof(Node) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		Node* n = &graph->nodes[i];
		n->cost = UINT64_MAX;
		n->visited = 0;
		n->data = "NONE";
		n->path_from = UINT64_MAX;
	}
	return graph;
}

void add_node(AdjMatrix* graph, void* data, Vector2 pos) 
{
	Node* n = &graph->nodes[graph->len++];
	n->max = 0;
	n->data = data;
	n->position = pos;
	n->idx = graph->len - 1;
}

void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost)
{
	int max = graph->nodes[fromNode].max;
	graph->nodes[fromNode].adj[max] = &graph->nodes[toNode];
	graph->nodes[fromNode].max++;
}

void build_groups(AdjMatrix* graph)
{
	/*int groupCount = 1;
	for (int i = 0; i < graph->len; i++)
	{
		if (graph->nodes[i].visited == 0)
		{
			graph->nodes[i].visited = 1;
			graph->nodes[i].graph_group = groupCount;
			for (int c = 0; c < graph->len; c++)
			{
				if (graph->adjGraph[i][c] == 0 && graph->nodes[c].graph_group == 0)
				{
					graph->nodes[c].visited = 1;
					graph->nodes[c].graph_group = groupCount;
				}
			}
		}
		else
		{
			for (int c = 0; c < graph->len; c++)
			{
				if (graph->adjGraph[i][c] == 1 && graph->nodes[c].graph_group == 0)
				{
					graph->nodes[c].visited = 1;
					graph->nodes[c].graph_group = groupCount;
				}
			}
		}
		groupCount++;
	}*/
}

void astar(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{
	printf("in the hood");
	Queue* q = (Queue*)allocate(sizeof(Queue));
	queue_init(q);
	Node* n = &graph->nodes[startNodeIndex];
	Node* endNode = &graph->nodes[endNodeIndex - 1];
	n->visited = 1;
	n->cost = 0;
	while (n != NULL)
	{
		//boucle infinie
		for (int c = 0; c < n->max ; c++)
		{
			if (n->adj[c]->visited == 0)
			{
				//s'il n'a pas ete visiter, elle se fait push dans la queue, elle est mit a la fin
				queue_push(q, n->adj[c]);
			}

			//on le met a visiter
			n->adj[c]->visited = 1;

			//on calcule la distance entre notre noeud present et celui de la fin
			Vector2 position = {0};
			position.x = endNode->position.x - n->adj[c]->position.x;
			position.y = endNode->position.y - n->adj[c]->position.y;
			int distance = sqrt(pow(position.x, 2) + pow(position.y, 2));
			int cost = n->cost + distance;

			//on change son cost si celui calculer est plus petit
			if (cost < n->adj[c]->cost)
			{
				n->adj[c]->cost = cost;
				n->adj[c]->path_from = n->idx;
			}
		}
		//on pop la queue pour avoir les noeuds qui n'ont pas ete vus
		n = (Node*)queue_pop(q);
	}

	//on ne se rend pas encore a sa
	n = &graph->nodes[endNodeIndex-1];
	while (n->path_from != UINT64_MAX)
	{
		stack_push(solvedPath, n);
		n = &graph->nodes[n->path_from];
		if (n == &graph->nodes[startNodeIndex])
		{
			stack_push(solvedPath, n);
			break;
		}
	}
}

void trente_et_un_bmp()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/31.bmp", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if(graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "31_Solution.bmp";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void soixante_quatre_bmp()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/64.bmp", &w, &h, &channels, 0);
	if (img == nullptr)
	{
		printf("Erreur : Image non trouver");
	}
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "64_Solution.bmp";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void cent_vingt_huit_bmp()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/128.bmp", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "128_Solution.bmp";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void cinq_cent_douse_bmp()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/512.bmp", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "512_Solution.bmp";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void quarante_bmp()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/45565.bmp", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "45565_Solution.bmp";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void braid2k()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/braid2k.png", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "braid2k_Solution.png";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void combo()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/combo400.png", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "combo400_Solution.png";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}

void perfect2k()
{
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");

	AdjMatrix* graph = NULL;

	//pour pouvoir changer des couleurs
	int w, h, channels;
	unsigned char* img = stbi_load("Labyrinthe/perfect2k.png", &w, &h, &channels, 0);
	size_t img_size = w * h * channels;
	uint32_t count = 0;
	int startNodeIdx;
	int endNodeIdx;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255)
		{
			count++;
		}
	}
	graph = create_graph(count);
	int grosseurGraph = count;
	count = 0;
	Node* t_node = NULL;
	int idx = 0;
	for (unsigned char* pixel = img; pixel < img + img_size; pixel += 3)
	{
		if (pixel[0] == 255) {
			Vector2 position;
			position.x = count % w;
			position.y = count / h;
			add_node(graph, pixel, position);
			//Regarde pour celui qui est en en haut
			if (idx != 0)
			{
				//Les add_edge fonctionnent comme il le devrait
				for (int i = 0; i < graph->len; i++)
				{
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y - 1 &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
					if (graph->nodes[i].position.y == graph->nodes[idx].position.y &&
						graph->nodes[i].position.x == graph->nodes[idx].position.x - 1)
					{
						add_edge(graph, idx, i, 1);
						add_edge(graph, i, idx, 1);
					}
				}
			}
			idx++;
		}
		count++;
	}
	Stack path = { 0 };
	path = stack_init(grosseurGraph);
	astar(graph, 0, grosseurGraph, &path);

	while (path.top != -1)
	{
		Node* elem = (Node*)stack_pop(&path);
		unsigned char* pixel = (unsigned char*)elem->data;
		pixel[1] = 0;
		pixel[2] = 0;
	}
	//variable pour pouvoir changer le nom du fichier
	const char* filename = "perfect2k_Solution.png";
	stbi_write_png(filename, w, h, channels, img, w * 3);
}