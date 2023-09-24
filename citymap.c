#include <stdio.h>
#include <stdlib.h>
#include "citymap.h"
#include "utility.h"

Cell create_node(enum Cell_Type type, int key) {
	Cell nuovo_nodo;
	nuovo_nodo.tipo = type;
	nuovo_nodo.key = key;
	return nuovo_nodo;
}

Map create_map(unsigned int height,
	unsigned int width, FILE* filestream) {

	Map map;
	Cell* cells_array;
	unsigned int i;
	unsigned int j;
	unsigned int k;
	int dry_count;		/*Usato per controllare se una cella
									è coperta dalla pioggia o meno.*/
	int current_cell_value;

	cells_array = (Cell*)malloc(height * width * sizeof(Cell));

	k = 0;
	for (i = 0; i < height; i++) {
		dry_count = 0;
		for (j = 0; j < width; j++) {
			current_cell_value = TOINT(fgetc(filestream));
			/*Se trovo un edificio, prima di riassegnare quante celle copre a destra,
			devo verificare che non ci sia una copertura più alta prima, perché,
			in tal caso, la conta precedente della copertura è quella che deve
			rimanere valida.*/
			if (current_cell_value != 0) {
				cells_array[k] = create_node(BUILDING, k);
				if (current_cell_value >= dry_count) {
					dry_count = current_cell_value;
				}
				else {
					dry_count--;
				}
			}
			else {
				if (dry_count > 0) {
					cells_array[k] = create_node(DRY, k);
				}
				else {
					cells_array[k] = create_node(WET, k);
				}
				(dry_count > 0) ? dry_count-- : dry_count;
			}
			k++;
		}
		fgetc(filestream);
	}

	map.cells_array = cells_array;
	map.height = height;
	map.width = width;
	return map;
}