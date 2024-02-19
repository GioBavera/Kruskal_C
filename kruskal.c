#include "kruskal.h"   

//------------------------ Main
int main(){
    int M_Costos[VERTICES][VERTICES], aux;
    rama *arbol = NULL;

    printf("\nIngrese el costo de las aristas: \n");

    for(int i = 0; i < VERTICES; i++){
        for(int j = i + 1; j <= VERTICES - 1; j++){
            int error = 1;
            while(error){       // Chequea que el costo sea valido
                printf("Arista %d y %d = ", i, j);
                scanf("%d", &aux);
                if(aux > 0){
                    error = 0;
                    M_Costos[i][j] = aux;
                } else {
                    printf("Error. Vuelva a intentarlo. \n");
                }   // Si el valor es > 0, entonces lo guarda en M_Costos
            }
        }
    }
    
    for(int i=0; i<= VERTICES-1; i++){
        for(int j = i + 1; j <= VERTICES - 1; j++){
            if(M_Costos[i][j] != 0){
                inserta(i, j, M_Costos[i][j], &arbol);    // Crea la lista enlazada de los vertices y costos
            }                                              
        }
    }
    kruskal(&arbol);        // Llamo a kruskal para conseguir el arbol final
    lista(arbol);           // Imprime el arbol final tras el algoritmo
    return 0;
}

//------------------------ Funciones
// Agrega un vertice al conjunto 
void inicial(tipo_nombre x, tipo_elemento s, conjunto_CE *S){
    (S->nombres)[s].nombre_conjunto = x;            // Agrega el nomre
    (S->nombres)[s].siguiente_elemento = -1;        // Fin de la lista
    (S->encabezamientos_conjunto)[x].cuenta = 1;    // Inicia la cuenta
    (S->encabezamientos_conjunto)[x].primer_elemento = s;   
}

// La función une a los dos conjuntos A y B en un solo conjunto C.
void combina(tipo_nombre a, tipo_nombre b, conjunto_CE *S){
    int cant1 = S->encabezamientos_conjunto[a].cuenta;  // cant1 = cantidad de elementos conjunto A
    int cant2 = S->encabezamientos_conjunto[b].cuenta;  // cant2 = cantidad de elementos conjunto B

    // Al conjunto mas grande se le une el mas chico
    if(cant1 > cant2){
        int elem = S->encabezamientos_conjunto[b].primer_elemento;
        while(S->nombres[elem].siguiente_elemento != -1){   // Recorre todos los elementos del conjunto b
            S->nombres[elem].nombre_conjunto = a;           // Para cambiarlos de b a a
            elem = S->nombres[elem].siguiente_elemento;
        }
        // Actualiza el último elemento del conjunto b para apuntar al primer elemento del conjunto a
        S->nombres[elem].nombre_conjunto = a;
        S->nombres[elem].siguiente_elemento = S->encabezamientos_conjunto[a].primer_elemento;   // Primer elemento de A, ultimo en C
        S->encabezamientos_conjunto[a].primer_elemento = S->encabezamientos_conjunto[b].primer_elemento;
        S->encabezamientos_conjunto[a].cuenta += S->encabezamientos_conjunto[b].cuenta;     // Actualizo la cantidad de elementos del conjunto
    
    } else {    // Mismo codigo pero caso contrario
        int elem = S->encabezamientos_conjunto[a].primer_elemento;
        while(S->nombres[elem].siguiente_elemento != -1){
            S->nombres[elem].nombre_conjunto = b;
            elem = S->nombres[elem].siguiente_elemento;
        }
        S->nombres[elem].nombre_conjunto = b;
        S->nombres[elem].siguiente_elemento = S->encabezamientos_conjunto[b].primer_elemento;
        S->encabezamientos_conjunto[b].primer_elemento = S->encabezamientos_conjunto[a].primer_elemento;
        S->encabezamientos_conjunto[b].cuenta += S->encabezamientos_conjunto[a].cuenta;
    }
}

// Simplemente retorna el nombre del conjunto que pertenece el vertice
tipo_nombre encuentra (int v, conjunto_CE *S){
    return ((S->nombres)[v].nombre_conjunto);
}

// Algoritmo de Kruskal
void kruskal(rama **arbol){     
    conjunto_CE *S = (conjunto_CE*)malloc(sizeof(conjunto_CE));
    rama *final = NULL;
    arista minimaArista;
    int uComponente, vComponente, nVertices = VERTICES; 

    // Se crean las componentes
    for(int i = 0; i < nVertices; i++){
        inicial(i, i, S);
    }

    printf("\n-----------------------------------------------\n");

    while(nVertices > 1){
        minimaArista = sacar_min(arbol);
        int u = minimaArista.u;
        int v = minimaArista.v;
        int costo = minimaArista.costo; 
        uComponente = encuentra(u, S);
        vComponente = encuentra(v, S);

        // Si los conjuntos son distintos, se combina las componentes.
        if(uComponente != vComponente){
            printf("Arista seleccionada: (%d,%d) con costo %d\n", minimaArista.u, minimaArista.v, minimaArista.costo);
            combina(uComponente, vComponente, S);
            nVertices --;
            inserta(u, v, costo, &final); 
        }
        
    }

    printf("-----------------------------------------------\n");
    // Libera la memoria del grafo original
    rama *aux = *arbol;
    while(aux != NULL){
        *arbol = aux;
        aux = (*arbol)->sig;
        free(*arbol);
    }
    *arbol = final; 
}

// Inserta en la lista enlazada (arbol) la nueva arista (u, v y costo)
void inserta(int u, int v, int costo, rama **arbol){
    rama *nuevo;        // Creo una rama con los datos nuevos 
    nuevo = (rama *)malloc(sizeof(rama)); 
    nuevo->a.u = u;         // Asigno su primer vertice
    nuevo->a.v = v;         // Asigno su segundo vertice
    nuevo->a.costo = costo; // Asigno su costo 
    nuevo->sig = NULL;  // Momentanemanete no apunta a nada 

    // Si el arbol esta vacio, entonces la nueva rama se convierte en la raiz
    if(*arbol == NULL){
        *arbol = nuevo;
    }else{  // Caso contrario, se encuentra el final del arbol para insertar la nueva rama
        rama *aux = *arbol;
        while(aux->sig != NULL){
            aux = aux->sig;
        }
        aux->sig = nuevo;
    }
}

// Esta función devuelve la arista de menor costo dentro de la lista enlazada, en este caso, árbol
arista sacar_min(rama **arbol){
    rama *aux = *arbol;     // Copia para evitar manipular el arbol inicial
    arista minimoCosto = (*arbol)->a;   // Arista donde se termina guardando la menor

    while(aux != NULL){     // Recorre el arbol hasta encontrar la minima siendo no nulo
        if(aux->a.costo < minimoCosto.costo){   // Si es la arista menor, actualiza
            minimoCosto = aux->a;               // Guardo la arista de menor costo
        }
        aux = aux->sig;         // Caso contrario sigue buscando
    }
    aux = *arbol;   // Reinicia aux al inicio del árbol

    // Caso de que la raiz sea la arista de menor costo
    if(aux->a.costo == minimoCosto.costo){  
        *arbol = (*arbol)->sig;
        free(aux);
    }else{
        // Si no, encuentra el nodo que contiene la arista de minimoCosto
        while(aux->sig->a.costo != minimoCosto.costo){
            aux = aux->sig;
        }
        // Elimina ese nodo del árbol
        rama *borrar = aux->sig;
        aux->sig = aux->sig->sig; // al elemento previo del elemento a borrar lo anexamos con el siguiente al que se va a borrar
        free(borrar);
    }
    return minimoCosto;     // Devuelvo la arista de menor costo
}

// Imprime el arbol final
void lista(rama* arbol){
    rama *copia = arbol;    // Creo una copia del arbol original para no modificar el original 
    printf("\nEl camino de menor costo queda: \n");
    for(int i=0; copia != NULL; i++){           // Imprime las aristas seleccionadas del arbol final. 
        printf("| (%d)", copia->a.u);           // El formato de visualizacion de los vertices es:
        printf("---[%d]---", copia->a.costo);   // (vertice)---[costo]---(vertice) | ... 
        printf("(%d) ", copia->a.v);
        copia = copia->sig;
    }
    printf("|\n..........................................................................\n");
}
