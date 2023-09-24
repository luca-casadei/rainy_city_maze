/*Strutture lista per gestire la coda (usata nel BFS)*/
typedef int ListInfo;

typedef struct ListNode {
	ListInfo val;
	struct ListNode* succ, * pred;
} ListNode;

typedef struct {
	int list_len;
	ListNode sentinel;
} List;

/*Funzioni di utilità per le liste.*/
ListNode* list_first(const List* L);
ListNode* create_list_node(int v);
/* Restituisce l'indirizzo di memoria della sentinella di L */
const ListNode* list_get_last_node(const List* L);
List* create_list(void);
void empty_list(List* L);
void list_destroy(List* L);
int is_list_empty(const List* L);
/*Crea il nodo con valore k e lo inserisce dopo il nodo n*/
void list_add_after(List* L, ListNode* n, ListInfo k);
/* Inserisce un nuovo nodo contenente k alla fine della lista */
void list_add_end(List* L, ListInfo k);
/* Rimuove il nodo n dalla lista L */
void list_remove(List* L, ListNode* n);
ListInfo list_remove_first(List* L);