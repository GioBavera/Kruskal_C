/* kruskal.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERTICES 5          //maximo 5 

//------------------------ Definiciones
typedef int tipo_nombre;
typedef int tipo_elemento;
typedef int vertice;

typedef struct _ARISTA{     // (u,v) Costo
    vertice u;
    vertice v;
    int costo;
}arista;

typedef struct _RAMA{       // Arista -> Arista -> ...
    struct _ARISTA a;
    struct _RAMA *sig;
}rama;

typedef struct _ENCABEZADO{
    int cuenta;
    int primer_elemento;
}encabezado;

typedef struct _NOMBRE{
    tipo_nombre nombre_conjunto; 
    int siguiente_elemento;
}nombre;

typedef struct _CONJUNTO_CE{
    nombre nombres[VERTICES];  // 5 estructuras nombre
    encabezado encabezamientos_conjunto[VERTICES];
}conjunto_CE;

//------------------------ Declaraciones
void inicial(tipo_nombre, tipo_elemento, conjunto_CE*);  
void combina(tipo_nombre, tipo_nombre, conjunto_CE*);
tipo_nombre encuentra (int, conjunto_CE*);               
void kruskal (rama**);
void inserta(int, int, int, rama**);
arista sacar_min(rama**);
void lista (rama*); 