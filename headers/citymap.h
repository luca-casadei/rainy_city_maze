/*Indica il tipo di cella, se coperta o meno da un edificio*/
enum Cell_Type { DRY, WET, BUILDING ,BEGINNING, PATH, DRYPATH};

/*Struttura che rappresenta una cella, con il suo tipo e la relativa chiave*/
typedef struct Cell {
	enum Cell_Type tipo;
	int key;
} Cell;

/*Struttura che rappresenta la mappa su cui spostarsi*/
typedef struct Map {
	unsigned int width;
	unsigned int height;
	Cell* cells_array;
} Map;

/*Crea la struttura di una cella a partire dal
suo tipo e la sua chiave e la restituisce.*/
Cell create_node(enum Cell_Type tipo, int key);

/*Crea la mappa a partire dal file di input, leggendo la matrice e creando celle
con il relativo tipo, ovvero se sono asciutte, bagnate, o edifici.*/
Map create_map(unsigned int altezza_matrice,
	unsigned int larghezza_matrice, FILE* filestream);

/*Direzione usata per il controllo della validita di un vicino ad un vertice*/
enum Direction { UP, DOWN, RIGHT, LEFT };

/*Struttura che rappresenza i due tipi di distanza da considerare*/
typedef struct Distance {
	int distance_cells;				/*Celle totali*/
	int distance_wet_cells;			/*Celle bagnate (non coperte da edifici)*/
} Distance;