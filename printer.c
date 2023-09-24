#include <stdio.h>
#include "citymap.h"
#include "printer.h"
#include "utility.h"

#define UPPERCORNERLEFT "%c",218
#define UPPERCORNERRIGHT "%c",191
#define HZNTLINE "%c",196
#define BOTTOMCORNERLEFT "%c",192
#define BOTTOMCORNERRIGHT "%c",217
#define VERTLINE "%c",179

/*Implementazione ricorsiva della stampa del percorso*/
static void stampa_percorso_ricorsivo(Map mappa, int s, int d,
	const int* p,
	int corr,				/*Vertice corrente, alla fine della ricorsione
							sarà uguale alla sorgente*/
	int succ				/*Il successivo del vertice corrente, serve per
							stabilire in che direzione ci si è mossi.*/
)
{
	mappa.cells_array[corr].tipo = (mappa.cells_array[corr].tipo == WET) ? 
		PATH : DRYPATH;

	if (corr != s) {
		stampa_percorso_ricorsivo(mappa, s, d, p, p[corr], corr);
	}
	if (corr == (succ - mappa.width)) {
		printf("%c", 'S');
	}
	else if (corr == succ + 1) {
		printf("%c", 'O');
	}
	else if (corr == succ - 1) {
		printf("%c", 'E');
	}
	else if (corr != succ) {
		printf("%c", 'N');
	}
}

static void print_upper_frame(unsigned int len) {
	unsigned int i;
	printf(UPPERCORNERLEFT);
	for (i = 0; i < len; i++) {
		printf(HZNTLINE);
	}
	printf(UPPERCORNERRIGHT);
}

static void print_bottom_frame(unsigned int len) {
	unsigned int i;
	printf(BOTTOMCORNERLEFT);
	for (i = 0; i < len; i++) {
		printf(HZNTLINE);
	}
	printf(BOTTOMCORNERRIGHT);
}

void print_map_graphics(Map map) {
	unsigned int i;
	unsigned int j;
	unsigned int k = 0;
	print_upper_frame(map.width);
	printf("\n");
	for (i = 0; i < map.height; i++) {
		if (k == 0) {
			printf("%c", 221);
		}
		else {
			printf(VERTLINE);
		}
		for (j = 0; j < map.width; j++) {
			switch (map.cells_array[k].tipo){
			case BUILDING: {
				printf("%c", 219);
				break;
				}
			case DRY: {
				printf("%c", 176);
				break;
			}
			case PATH: {
				printf("%c",158);
				break;
			}
			case DRYPATH: {
				printf("%c", 178);
				break;
			}
			default:
				printf(" ");
				break;
			}
			k++;
		}
		if (k == map.height * map.width) {
			printf("%c", 221);
		}
		else {
			printf(VERTLINE);
		}
		printf("\n");
	}
	print_bottom_frame(map.width);
}

/*Stampa a video sia il numero di celle percorse che il percorso seguito.*/
void print_path(int sorgente, int destinazione,
	Map mappa, Distance* d, int* predecessori) {
	if (predecessori[destinazione] == NODE_UNDEF) {				/*Se non esistono padri*/
		printf("%d %d\n", NODE_UNDEF, NODE_UNDEF);
	}
	else {
		/*Aumento le distanze di 1 perché va considerata la cella di partenza*/
		printf("%-10s%-10s%-10s\n","Path","Wet","Dry");
		printf("%-10d%-10d%-10d\n", d[destinazione].distance_cells + 1,
			d[destinazione].distance_wet_cells + 1,
			d[destinazione].distance_cells - d[destinazione].distance_wet_cells);
		printf("\nShortest path:\t");
		stampa_percorso_ricorsivo(mappa, sorgente, destinazione,
			predecessori, destinazione, destinazione);
		printf("\n");
	}
}