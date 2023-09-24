/*Casadei Luca, 0001069237, Gruppo B, luca.casadei27@studio.unibo.it*/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "graph.h"
#include "citymap.h"
#include "utility.h"
#include "printer.h"


/*Controlla che la cella "key" sia nella stessa riga di "key_confronto", 
e che spostandosi verso una certa direzione non si vada out of bounds.*/
static int free_cell(Map map, int key, 
	int confrontation_key, enum Direction dir) {
	
	int valid = 1;
	int row_beginning;

	/*Calcolo il limite inferiore della riga corrente, se muovendomi
	se muovendomi in una direzione che non sia SU o GIU mi sposto di riga,
	il movimento non è valido.
	Es: Se ho key_confronto = 27 e la larghezza della matrice è 10, 
	ottengo 2 * 10 = 20, che è l'inizio della riga corrente.*/
	row_beginning = (confrontation_key / map.width) * map.width;

	/*Controllo out of bounds*/
	if (key < 0 || ((unsigned int)key >= (map.height * map.width))) {
		valid = 0;
	}
	else {
		if (dir == RIGHT || dir == LEFT) {
			/*Controllo sulla riga corrente*/
			if ((key >= (row_beginning + map.width)) || (key < row_beginning)) {
				valid = 0;
			}
		}
		else {
			if (map.cells_array[key].tipo == BUILDING) {
				valid = 0;
			}
		}
	}
	return valid;
}

/*Restituisce la key della cella vicina a "cella" in una certa direzione, 
considerando che le celle sono salvate su array, ma la mappa è una matrice.
Input:
- mappa: La struttura che contiene l'array delle celle della città, altezza
		 e larghezza della matrice.
- cella: La cella della mappa di cui si vuole calcolare il vicino.
- dir:	 La direzione del vicino di cui si vuole la key, rispetto alla "cella".
Output:
La key della cella corrispondente al vicino di "cella" nella direzione "dir".
*/
static int get_adjacent_key(Map mappa, enum Direction dir, Cell cella) {
	int adjacent_key = 0;
	int current_key = cella.key;
	switch (dir)
	{
	case UP:
		adjacent_key = (current_key - mappa.width);
		break;
	case DOWN:
		adjacent_key = (current_key + mappa.width);
		break;
	case RIGHT:
		adjacent_key = (current_key + 1);
		break;
	case LEFT:
		adjacent_key = (current_key - 1);
		break;
	default:
		break;
	}
	return adjacent_key;
}

/*Controlla se il vicino è una cella in cui si puo andare, 
in tal caso, crea il relativo arco nel grafo*/
static void create_adjacent_edge(Graph *g, Map map, enum Direction dir,
	int current_vertex) {

	Cell current_node;
	int neighbor;
	current_node = map.cells_array[current_vertex];

	neighbor = get_adjacent_key(map, dir, current_node);
	if (free_cell(map, neighbor, current_node.key, dir)) {
		add_edge_to_graph(g, current_node.key, neighbor);
	}
	
}

/*Controlla ogni cella della mappa e crea l'arco per ogni 
vicino del vertice corrispondente, gli edifici non vengono considerati.*/
static void build_edges(Map map, Graph* g) {
	int i;
	int len = map.width * map.height;
	for (i = 0; i < len; i++) {
		if (map.cells_array[i].tipo != BUILDING) {
			create_adjacent_edge(g, map, RIGHT, i);
			create_adjacent_edge(g, map, LEFT, i);
			create_adjacent_edge(g, map, UP, i);
			create_adjacent_edge(g, map, DOWN, i);
		}
	}
}

/*Esegue l'algoritmo BFS per trovare il percorso minimo, 
è stato riadattato in modo tale da poter gestire il conteggio
delle celle attraversate con la pioggia.*/
static void bfs(Map map,	/*Mappa con array delle celle.*/
	const Graph* g,				/*Grafo con gli archi già allocati*/
	int s,						/*La sorgente da cui partire per il bfs*/
	Distance* d,				/*Array delle distanze per la stampa del 
								numero di celle percorse*/
	int* p						/*Array dei padri per mantenere il percorso.*/
) {
	const int n = get_nodes(g);
	List* q;
	int i;

	for (i = 0; i < n; i++) {
		d[i].distance_wet_cells = -1;
		d[i].distance_cells = -1;
		p[i] = NODE_UNDEF;
	}

	d[s].distance_wet_cells = 0;
	d[s].distance_cells = 0;

	q = create_list();
	list_add_end(q, s);

	while (!is_list_empty(q)) {
		/*Dequeue*/
		const int u = list_remove_first(q);
		const Edge* arco;
		for (arco = first_adjacent_edge(g, u); arco != NULL; arco = arco->next) {
			const int v = arco->dst;
			if (d[v].distance_cells < 0) { /* il nodo v non è ancora stato visitato */
				/*Modifica effettuata per il conteggio delle celle bagnate*/
				if (map.cells_array[v].tipo == WET) {
					d[v].distance_wet_cells = d[u].distance_wet_cells + 1;
				}
				else {
					d[v].distance_wet_cells = d[u].distance_wet_cells;
				}
				d[v].distance_cells = d[u].distance_cells + 1;
				p[v] = u;
				list_add_end(q, v);
			}
		}
	}
	list_destroy(q);
}

/*Alloca e inizializza tutte le strutture necessarie per il BFS, compresa
l'individuazione degli archi dei collegamenti tra celle.*/
static void inizializza_strutture(
	FILE* filestream,				/*Lo stream già aperto per 
									la lettura della matrice*/
	Graph** g,
	Map* map,
	Distance** distances,			/*Gli array delle distanze e dei predecessori
									vengono allocati direttamente in questa funzione.*/
	int** predecessors
){
	unsigned int altezza_matrice;
	unsigned int larghezza_matrice;

	fscanf(filestream, "%u %u\n", &altezza_matrice, &larghezza_matrice);

	*map = create_map(altezza_matrice, larghezza_matrice, filestream);
	*distances = malloc(sizeof(Distance) * altezza_matrice * larghezza_matrice);
	*predecessors = malloc(sizeof(int) * altezza_matrice * larghezza_matrice);
	*g = create_graph(larghezza_matrice * altezza_matrice);
	build_edges(*map, *g);
}

int main(int argc, char* argv[]) {
	Map mappa;
	FILE* filestream;
	Distance* distance_array;
	int* predecessors_array;
	Graph* g;

	/*Controlli sul file di input*/
	if (argc != 2) {
		fprintf(stderr, "Uso: %s nome_file\n", argv[0]);
		return EXIT_FAILURE;
	}

	filestream = fopen(argv[1], "r");

	if (filestream == NULL) {
		fprintf(stderr, "Impossibile aprire: %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	inizializza_strutture(filestream,&g, &mappa, 
		&distance_array, &predecessors_array);
	
	fclose(filestream);

	bfs(mappa, g, 0, distance_array, predecessors_array);

	print_path(0, ((mappa.width * mappa.height) - 1), 
		mappa, distance_array, predecessors_array);

	printf("\n");
	print_map_graphics(mappa);
	printf("\n");

	/*Deallocazione di tutta la memoria.*/
	destroy_graph(g);
	free(distance_array);
	free(mappa.cells_array);
	free(predecessors_array);

	return EXIT_SUCCESS;
}