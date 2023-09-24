/*Struttura Grafo per il BFS*/
/* struttura arco */
typedef struct Edge {
	int src;            /* nodo sorgente        */
	int dst;            /* nodo destinazione    */
	struct Edge* next;
} Edge;

/* struttura grafo */
typedef struct {
	int n;              /* numero di nodi               */
	int m;              /* numero di archi              */
	Edge** edges;       /* array di liste di adiacenza  */
} Graph;

/*Crea la struttura di un grafo a partire dal numero di nodi.*/
Graph* create_graph(int n);
/*Dato un grafo non nullo ritorna il numero di nodi.*/
int get_nodes(const Graph* g);
/*Dealloca completamente il grafo.*/
void destroy_graph(Graph* g);
/*Crea un arco dalla sorgente src alla destinazione dst*/
Edge* create_edge(int src, int dst);
/*Crea e aggiunge un arco al grafo*/
void add_edge_to_graph(Graph* g, int src, int dst);
/*Restituisce il primo arco della lista di adiacenza del vertice v*/
Edge* first_adjacent_edge(const Graph* g, int v);