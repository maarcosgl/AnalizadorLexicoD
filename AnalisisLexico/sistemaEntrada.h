typedef struct dobleBuffer dobleBuffer;

struct dobleBuffer{
    //Los buffers podrán almacenar 32 caracteres
    //Se mete un espacio de mas en el buffer
    //ya que ese va a ser el EOF
	char buffer1[33];
    char buffer2[33];
    //Punteros a la posición de inicio del componente lexcio a procesar
    //y el caracter que se esta procesando actualmente
    //Se guardan como posiciones de los buffers
    int inicio;
    int delantero;
    //Indica que buffer se está usando
    int turno;
};

/*
*Función que inicializa la estructura del centinela
* y abre el archivo a procesar pasado como argumento
*/
void iniciarSistemaEntrada(char *nombreFichero);

/*
* Función que indica la terminación del procesamiento y que cierra el fichero
*/
void cerrarSistemaEntrada();

//Función que devuelve el siguiente caraccter a procesar
//del buffer correspondiente del centinela
char siguienteCaracter();

//Retrocede el puntero delantero una posición en los buffers
void retroceder();

//Función que, en el componente léxico que recibe como argumento
//construye el lexema en función de las posiciones de los punteros inicio y delantero
void aceptar(cLexico *CL);

//Función especial para aceptar comentario, ya que estos pueden sobrepasar el tamaño
//máximo de los lexemas. 
//Como estos no van a ser mostrados por pantalla, pueden aceptarse independientemente de su longitud
void aceptarComentario();