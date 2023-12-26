#include "kruskal.h"   

//------------------------ Main
int main(){
    int M_Costos[VERTICES][VERTICES], aux;
    rama *arbol = NULL;

    printf("\nIngrese el costo de las aristas: \n");
    for(int i = 0; i < VERTICES; i++){
        for(int j = i + 1; j <= VERTICES - 1; j++){
            int error = 1;
            while(error){       // Caso en el que se ingrese un valor menor a 0, se repite
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
                inserta(i, j, M_Costos[i][j], &arbol);    // Llama a inserta para crear la lista enlazada
            }                                             // de las aristas 
        }
    }
    kruskal(&arbol);        // Llamo a kruskal para conseguir el arbol final
    lista(arbol);           // Llamo el arbol final para mostrar el arbol final
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

// La función 'combina' combina dos conjuntos a y b en un solo conjunto
void combina(tipo_nombre a, tipo_nombre b, conjunto_CE *S){
    int cant1 = S->encabezamientos_conjunto[a].cuenta;  // cantidad de elementos en los conjuntos
    int cant2 = S->encabezamientos_conjunto[b].cuenta;

    if(cant1 > cant2){
        // Recorre todos los elementos del conjunto b
        int elem = S->encabezamientos_conjunto[b].primer_elemento;
        while(S->nombres[elem].siguiente_elemento != -1){   // Recorre todos los elementos del conjunto b
            S->nombres[elem].nombre_conjunto = a;           // Para cambiarlos de b a a
            elem = S->nombres[elem].siguiente_elemento;
        }
        // Actualiza el último elemento del conjunto b para apuntar al primer elemento del conjunto a
        S->nombres[elem].nombre_conjunto = a;
        S->nombres[elem].siguiente_elemento = S->encabezamientos_conjunto[a].primer_elemento;
        //S->encabezamientos_conjunto[a].primer_elemento = S->encabezamientos_conjunto[b].primer_elemento;
       
        S->encabezamientos_conjunto[a].cuenta += S->encabezamientos_conjunto[b].cuenta;     // Actualizo la cantidad de a + b
    } else {    // Lo mismo que antes pero en caso de b con mas elementos que a
        int elem = S->encabezamientos_conjunto[a].primer_elemento;
        while(S->nombres[elem].siguiente_elemento != -1){
            S->nombres[elem].nombre_conjunto = b;
            elem = S->nombres[elem].siguiente_elemento;
        }
        S->nombres[elem].nombre_conjunto = b;
        S->nombres[elem].siguiente_elemento = S->encabezamientos_conjunto[b].primer_elemento;
        //S->encabezamientos_conjunto[b].primer_elemento = S->encabezamientos_conjunto[a].primer_elemento;
        
        S->encabezamientos_conjunto[b].cuenta += S->encabezamientos_conjunto[a].cuenta;
    }
}

// Devuelve el nombre de la componente a la que pertenece el vertice
tipo_nombre encuentra (int x, conjunto_CE *S){
    return ((S->nombres)[x].nombre_conjunto);
}

// Funcion para el algoritmo de kruskal
void kruskal(rama **arbol){     
    conjunto_CE *S = (conjunto_CE*)malloc(sizeof(conjunto_CE));
    rama *final = NULL;
    arista minimaArista;
    int uComponente, vComponente, nVertices = VERTICES; 

    for(int i = 0; i < VERTICES; i++){
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

        if(uComponente != vComponente){
            printf("Arista seleccionada: (%d,%d) con costo %d\n", minimaArista.u, minimaArista.v, minimaArista.costo);
            combina(uComponente, vComponente, S);
            nVertices --;
            inserta(u, v, costo, &final); 
        }
        
    }

    printf("-----------------------------------------------\n");
    rama * aux = *arbol;
    while(aux != NULL){
        *arbol = aux;
        aux = (*arbol)->sig;
        free(*arbol);
    }
    *arbol = final; 
}

// Inserta en la lista enlazada (arbol) la nueva componente (u, v y costo)
void inserta(int u, int v, int costo, rama **arbol){
    rama *nuevo;        // Creo una rama con los datos nuevos 
    nuevo = (rama *)malloc(sizeof(rama)); 
    nuevo->a.u = u;     // Asigno su primer vertice
    nuevo->a.v = v;     // Asigno su segundo vertice
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

    rama *aux = *arbol;
    arista minimoCosto = (*arbol)->a;   // Esta arista va a guardar finalmente la arista de menor costo

    while(aux != NULL){     // Recorre el arbol hasta encontrar la minima, siempre que no sea nulo
        
        if(aux->a.costo < minimoCosto.costo){   // Si es la arista menor, actualiza
            minimoCosto = aux->a;
        }
        aux = aux->sig;         // Caso contrario sigue buscando
    }
    // Reinicia aux al inicio del árbol
    aux = *arbol;
    // Caso de que la raiz sea la arista de menor costo
    if(aux->a.costo == minimoCosto.costo){  
        *arbol = (*arbol)->sig;
        //free(aux);
    } else {
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

// Muestra por consola el arbol final al aplicarle el algoritmo de kruskal
void lista(rama* arbol){
    rama *copia = arbol;    // Creo una copia del arbol original para poder imprimirlo 
    printf("\nEl camino final queda: \n");
    for(int i=0; copia != NULL; i++){           // Imprime las aristas seleccionadas del arbol final. 
        printf("| (%d)", copia->a.u);           // El formato de visualizacion de los vertices es:
        printf("---[%d]---", copia->a.costo);   // (vertice)---[costo]---(vertice) | ... 
        printf("(%d) ", copia->a.v);
        copia = copia->sig;
    }
    printf("|\n..........................................................................\n");
}
