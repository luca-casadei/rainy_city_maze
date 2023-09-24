#include "list.h"
#include <assert.h>
#include <stdlib.h>

/*Funzioni di utilità per le liste.*/
ListNode* list_first(const List* L)
{
	assert(L != NULL);
	return L->sentinel.succ;
}

ListNode* create_list_node(int v)
{
	ListNode* r = (ListNode*)malloc(sizeof(ListNode));
	assert(r != NULL); /* evitiamo un warning con VS */
	r->val = v;
	r->succ = r->pred = r;
	return r;
}

/* Restituisce l'indirizzo di memoria della sentinella di L */
const ListNode* list_get_last_node(const List* L)
{
	assert(L != NULL);
	return &(L->sentinel);
}

List* create_list(void)
{
	List* L = (List*)malloc(sizeof(List));
	assert(L != NULL);

	L->list_len = 0;
	L->sentinel.pred = L->sentinel.succ = &(L->sentinel);
	return L;
}

void empty_list(List* L)
{
	int i;
	ListNode* tail = L->sentinel.pred;
	ListNode* corpse;
	for (i = 0; i < L->list_len; i++) {
		corpse = tail;
		tail = tail->pred;
		free(corpse);
	}
	L->sentinel.pred = &(L->sentinel);
	L->sentinel.succ = &(L->sentinel);
	L->list_len = 0;
}

void list_destroy(List* L)
{
	empty_list(L);
	free(L);
}

int is_list_empty(const List* L)
{
	assert(L != NULL);
	return (list_first(L) == list_get_last_node(L));
}

/*Crea il nodo con valore k e lo inserisce dopo il nodo n*/
void list_add_after(List* L, ListNode* n, ListInfo k) {
	ListNode* nuovo_nodo = create_list_node(k);
	nuovo_nodo->succ = n->succ;
	nuovo_nodo->pred = n;
	n->succ->pred = nuovo_nodo;
	n->succ = nuovo_nodo;
	L->list_len++;
}

/* Inserisce un nuovo nodo contenente k alla fine della lista */
void list_add_end(List* L, ListInfo k)
{
	assert(L != NULL);
	list_add_after(L, L->sentinel.pred, k);
}

/* Rimuove il nodo n dalla lista L */
void list_remove(List* L, ListNode* n)
{
	assert(L != NULL);
	assert(n != NULL);
	assert(n != list_get_last_node(L));
	n->pred->succ = n->succ;
	n->succ->pred = n->pred;
	free(n);
	L->list_len--;
}

ListInfo list_remove_first(List* L)
{
	ListInfo val = L->sentinel.succ->val;
	list_remove(L, L->sentinel.succ);
	return val;
}