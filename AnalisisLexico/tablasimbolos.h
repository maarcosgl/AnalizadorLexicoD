#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

typedef void *tabla; 

//Función para la creación de la tabla de símbolos
void crearTabla(tabla *tablaSimbolos);

//Función para la destrucción de la tabla de símbol0s
void destruirTabla(tabla *tablaSimbolos);

//Función para imprimir la tabla de símbolos
void imprimirTabla(tabla tablaSimbolos);

//Función para buscar e inserta componentes léxicos en la tabla
int insertar_buscar_tabla(tabla *tablaSimbolos, char *lexema, int componente);

#endif  // TABLA_H