#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

/////////////////////REPETIMOS LA DEFINICIÓN DE LOS TIPOS DE DATOS
//abin es un puntero a struct celda
typedef struct celda *abin;
//contenido de cada componenteLexico del árbol
//es un árbol de componentes lexico

typedef struct componenteLexico componenteLexico;

struct componenteLexico{
	int identificador;
    char *lexema;
};

/////////////////////ESTRUCTURAS DE DATOS
struct celda{
	componenteLexico info;
	struct celda *izq, *der;
};

///////////////////////FUNCIONES

//Crea el árbol vacío
void crear(abin *A){
	*A=NULL;
}

//Suprime las ramas izquierda y derecha de un nodo del arbol
void supizq(abin *A);
void supder(abin *A);
//Comprueba si un nodo del arbol esta vacío
unsigned esVacio(abin A);

void destruir(abin *A){
	abin aux;
	aux=*A;
	if(!esVacio(aux)){
	  supizq(&aux);
	  supder(&aux);
	  free(aux->info.lexema); // Liberar memoria de la cadena de caracteres
	  free(aux);
	  *A=NULL;
	}	
}

//Comprueba si el árbol está vacío
unsigned esVacio(abin A){
	return (A==NULL);
}

//Devuelve el subárbol izquierdo de A
abin izq(abin A){
	return A->izq;
}

//Devuelve el subárbol derecho de A
abin der(abin A){
	return A->der;
}

//Recupera la informacion de la celda de la raiz del arbol
void leer(abin A, componenteLexico *E){
	*E=A->info;
}

//Suprime el subárbol a la izquierda de A
void supizq(abin *A){
	abin aux;
	aux=izq(*A);
	if(!esVacio(aux)){
	  supizq(&aux);
	  supder(&aux);	
	   free(aux->info.lexema); // Liberar memoria de la cadena de caracteres
	  (*A)->izq=NULL;
	  free(aux);
	}	
}

//Suprime el subárbol a la derecha de A
void supder(abin *A){
	abin aux;
	aux=der(*A);
	if(!esVacio(aux)){
	  supizq(&aux);
	  supder(&aux);	
	   free(aux->info.lexema); // Liberar memoria de la cadena de caracteres
	  (*A)->der=NULL;
	  free(aux);
	}	
}

//Recorrido recursivo preorden: R-I-D
void imprimir(abin A) {
    componenteLexico E;
    if (!esVacio(A)) {
        leer(A, &E);
        printf("%d | %s\n", E.identificador, E.lexema);
        imprimir(izq(A));
        imprimir(der(A));
    }
}

int insertar_buscar(abin *A, char *lexema, int componente){
	abin aux;
	//Al comienzo, el componente léxico a introducir será una cadena alfanumérica
	int componenteResultado = componente;

	//Si un nodo del árbol está vacío, se reserva memoria para el mismo
	//También se reserva memoria suficiente para almacenar el lexema
	if(esVacio(*A)){
		aux=(abin)malloc(sizeof(struct celda));
		aux->info.identificador =  componente;
		aux->info.lexema = (char *)malloc(strlen(lexema)+1);
		//Se copia el contenido del lexema, en el campo correspondiente del componente léxcio
		strcpy(aux->info.lexema, lexema);
		aux->izq=NULL;
		aux->der=NULL;
		*A=aux;
	}
	//Si el nodo no está vacío, hay que comparar si el lexema que se quiere meter, ya está guardado en el árbol
	else{
		//En este caso, ya se había insertado en el árbol previamente
		if(strcmp(lexema, (*A)->info.lexema) == 0){
			//Se actualiza el entero del componente léxico, ya que podría ser una palabra reservada
			componenteResultado = (*A)->info.identificador;
		}
		//Al usar un árbol binario de búsqueda, las inserciones se realizarán en función del orden alfabético de los lexemas 
		//Si el lexema a insertar es mayor que el lexema con el que se compara, se inserta a su derecha
		//En otro caso se inserta a la izquierad
		else if(strcmp(lexema, (*A)->info.lexema) > 0){
			componenteResultado = insertar_buscar(&(*A)->der, lexema, componente);
		}
		else{
			componenteResultado = insertar_buscar(&(*A)->izq, lexema, componente);
		}
	}
	//Se devuelve el componenete lexico
	return componenteResultado;
}
