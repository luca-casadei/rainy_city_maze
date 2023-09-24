#include "graph.h"
#include <stdlib.h>
#include <assert.h>

Graph* create_graph(int n)
{
	int i;
	Graph* g = (Graph*)malloc(sizeof(*g));
	assert(g != NULL);
	assert(n > 0);

	g->n = n;
	g->m = 0;
	g->edges = (Edge**)malloc(n * sizeof(Edge*));
	assert(g->edges != NULL);
	for (i = 0; i < n; i++) {
		g->edges[i] = NULL;
	}
	return g;
}

int get_nodes(const Graph* g)
{
	assert(g != NULL);
	return g->n;
}

void destroy_graph(Graph* g)
{
	int i;

	assert(g != NULL);

	for (i = 0; i < g->n; i++) {
		Edge* arco = g->edges[i];
		while (arco != NULL) {
			Edge* next = arco->next;
			free(arco);
			arco = next;
		}
		g->edges[i] = NULL;
	}
	free(g->edges);
	g->n = 0;
	g->edges = NULL;
	free(g);
}

Edge* create_edge(int src, int dst) {
	Edge* arco = malloc(sizeof(Edge));
	assert(arco != NULL);
	arco->dst = dst;
	arco->src = src;
	arco->next = NULL;
	return arco;
}

void add_edge_to_graph(Graph* g, int src, int dst)
{
	Edge* next;
	Edge* arco;
	assert((src >= 0) && (src < get_nodes(g)));
	assert((dst >= 0) && (dst < get_nodes(g)));

	next = g->edges[src];
	arco = create_edge(src, dst);

	g->edges[src] = arco;
	g->edges[src]->next = next;

	g->m++;
}

Edge* first_adjacent_edge(const Graph* g, int v)
{
	assert(g != NULL);
	assert((v >= 0) && (v < get_nodes(g)));

	return g->edges[v];
}
