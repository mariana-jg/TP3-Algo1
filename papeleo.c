#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "utiles.h"
#include "papeleo.h"
#include "dia_en_la_uni.h"

#define MAX_FILAS 30
#define MAX_COLUMNAS 30

#define CERO_EN_ASCII 48
#define TIEMPO_DE_MUESTREO_DE_MENSAJE 1
#define ESPACIO ' '
#define PARED '*'
#define PERSONAJE 'W'

#define DIMENSION_N1 22
#define DIMENSION_N2 17
#define DIMENSION_N3 12

#define NUMERO_NIVEL1 1
#define NUMERO_NIVEL2 2
#define NUMERO_NIVEL3 3

#define CANT_FUEGOS_N1 10
#define CANT_MEDIAS_N1 5
#define CANT_BOTELLAS_N1 4
#define CANT_INTERRUPTOR_N1 1
#define CANT_PAPELEOS_N1 2
#define CANT_MOVIMIENTOS_N1 40
#define CANT_MARTILLOS_N1 4
#define CANT_EXTINTORES_N1 4

#define CANT_FUEGOS_N2 5
#define CANT_MEDIAS_N2 4
#define CANT_BOTELLAS_N2 3
#define CANT_INTERRUPTOR_N2 1
#define CANT_PAPELEOS_N2 3
#define CANT_MOVIMIENTOS_N2 30
#define CANT_MARTILLOS_N2 5
#define CANT_EXTINTORES_N2 2

#define CANT_FUEGOS_N3 3
#define CANT_MEDIAS_N3 3
#define CANT_BOTELLAS_N3 2
#define CANT_INTERRUPTOR_N3 0
#define CANT_PAPELEOS_N3 4
#define CANT_MOVIMIENTOS_N3 20
#define CANT_MARTILLOS_N3 6
#define CANT_EXTINTORES_N3 2

#define FUEGOS_EXTINTOS_OLAF_N1 2
#define FUEGOS_EXTINTOS_OLAF_N2 1
#define PAPELEOS_QUEMADOS_STITCH_N3 0
#define MOVIMIENTOS_EXTRAS_RAYO_N1 10
#define MARTILLO_EXTRA_JASMIN 1

#define FUEGOS 'F'
#define MEDIAS 'M'
#define BOTELLAS 'G'
#define INTERRUPTOR 'I'
#define PAPELEOS 'P'

#define IZQUIERDA 'A'
#define DERECHA 'D'
#define ARRIBA 'W'
#define ABAJO 'S'
#define ROTACION_HORARIA 'E'
#define ROTACION_ANTIHORARIA 'Q'
#define USAR_MARTILLO 'Z'
#define USAR_EXTINTOR 'C'

#define MOVIMIENTOS_PERDIDOS_POR_MEDIA 10
#define MOVIMIENTOS_GANADOS_POR_BOTELLA 7

#define JUGANDO 0
#define GANADO 1
#define PERDIDO -1

#define R_N1 7
#define R_N2 5
#define R_N3 3

#define MOV_PAREDES_ALEATORIAS_N1 40
#define MOV_PAREDES_ALEATORIAS_N2 30
#define MOV_PAREDES_ALEATORIAS_N3 20

/*Pre-condiciones: 
    -
Post-condiciones: 
    el procedimiento inicializara los campos que representan a los topes en 0 (cero) de los vectores que conforman el registro nivel_t.*/
void inicializar_topes (nivel_t* nivel) {

    nivel->tope_paredes = 0;
    nivel->tope_obstaculos = 0;
    nivel->tope_herramientas = 0;
    nivel->tope_papeleos = 0;  

}    

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    la función retorna "dimension_tablero" que es DIMENSION_N1, DIMENSION_N2, DIMENSION_N3.*/
int definir_dimension_tablero (int numero_nivel) {

    int dimension;

    if (numero_nivel == NUMERO_NIVEL1) {
        dimension = DIMENSION_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
        dimension = DIMENSION_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        dimension = DIMENSION_N3;        
    }

    return dimension;

}    

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    el procedimiento otorga una coordenada aleatoria (fila y columna) que tenga concordancia con el tamaño del tablero segun el nivel ingresado.*/
void coordenada_random (coordenada_t* ref_coordenada, int numero_nivel) {

    (*ref_coordenada).fil = rand () % definir_dimension_tablero(numero_nivel) + 0;
    (*ref_coordenada).col = rand () % definir_dimension_tablero(numero_nivel) + 0;

}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función compara dos coordenadas del tablero ingresadas, regresando una respuesta equivalente a 'true' si se cumple la igualdad.*/
bool mismas_coordenadas (coordenada_t coordenada_1, coordenada_t coordenada_2) {

    return ((coordenada_1.fil == coordenada_2.fil) && (coordenada_1.col == coordenada_2.col));

}


/*Pre-condiciones: 
    el vector 'paredes' ya debera estar cargado para que sea posible compararlo.
Post-condiciones: 
    la función comparara dos coordenadas (una referente a un objeto al cual quiero ubicar y una correspondiente al vector 'paredes'), regresando una 
        respuesta equivalente a 'true' si se cumple la igualdad.*/
bool coordenada_ocupada_con_pared (coordenada_t objeto, coordenada_t paredes[MAX_PAREDES], int tope_paredes) {
    
    bool ocupado_con_pared = false;

    for (int i = 0; i < tope_paredes ; i++) {

        if (mismas_coordenadas(paredes[i], objeto) == true) {
        ocupado_con_pared = true;
        }  

    }

    return ocupado_con_pared;
}

/*Pre-condiciones: 
    la 'pos_inicial_jugador' ya debera estar cargada para que sea posible compararla.
Post-condiciones: 
    la función comparara dos coordenadas (una referente a un objeto al cual quiero ubicar y la otra correspondiente a la pos_inicial_jugador), regresando una 
        respuesta equivalente a 'true' si se cumple la igualdad.*/
bool coordenada_ocupada_con_jugador (coordenada_t objeto, coordenada_t pos_inicial_jugador) {
    
    bool ocupado_con_jugador = false;

        if (mismas_coordenadas(pos_inicial_jugador, objeto) == true) {
        ocupado_con_jugador = true;    
        }  
    
    return ocupado_con_jugador;
}

/*Pre-condiciones: 
    el vector 'paredes' ya debera estar cargado para que sea posible compararlo.
Post-condiciones: 
    la función comparara dos coordenadas (una referente a un objeto (caso especifico: FUEGO) al cual quiero ubicar y la otra correspondiente al vector 'paredes', 
        regresando una respuesta equivalente a 'true' si se cumple ambas coordenadas son adyacentes.*/
bool adyacente (coordenada_t paredes[MAX_PAREDES], int tope_paredes, coordenada_t objeto) {

    bool es_adyacente = false;

    for (int i = 0; i < tope_paredes ; i++){

        int distancia = abs(paredes[i].fil - objeto.fil) + abs(paredes[i].col - objeto.col);

        if (distancia == 1){
            es_adyacente = true;    
        }
    }

    return es_adyacente;
}

/*Pre-condiciones: 
    el vector 'papeleos' debe estar cargado anteriormente para poder compararlo.
Post-condiciones: 
    la función comparara dos coordenadas (una referente a un objeto al cual quiero ubicar y la otra una correspondiente al vector 'papeleos'), regresando una 
        respuesta equivalente a 'true' si se cumple la igualdad.*/
bool coordenada_ocupada_con_papeleo (coordenada_t objeto, papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos) {

    bool ocupado_con_papeleo = false;

    for (int i = 0; i < tope_papeleos ; i++) {

        if (mismas_coordenadas(papeleos[i].posicion, objeto) == true) {
            ocupado_con_papeleo = true;    
        }  
    }

    return ocupado_con_papeleo;
}

/*Pre-condiciones: 
    el vector 'herramientas' debe estar cargado anteriormente para poder compararlo.
Post-condiciones: 
    la función comparara dos coordenadas (una referente a un objeto al cual quiero ubicar y la otra una correspondiente al vector 'herramientas'), regresando una 
        respuesta equivalente a 'true' si se cumple la igualdad.*/
bool coordenada_ocupada_con_herramienta (coordenada_t objeto, objeto_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas) {
    
    bool ocupado_con_herramienta = false;

    for (int i = 0; i < tope_herramientas ; i++) {

        if (mismas_coordenadas(herramientas[i].posicion, objeto) == true) {
            ocupado_con_herramienta = true;    
        }  
    }

    return ocupado_con_herramienta;
}

/*Pre-condiciones: 
    el vector 'obstaculos' debe estar cargado anteriormente para poder compararlo.
Post-condiciones: 
    la función comparara dos coordenadas (una referente a un objeto al cual quiero ubicar y la otra una correspondiente al vector 'obstaculos'), regresando una 
        respuesta equivalente a 'true' si se cumple la igualdad.*/
bool coordenada_ocupada_con_obstaculo (coordenada_t objeto, objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos) {

    bool ocupado_con_obstaculo = false;

    for (int i = 0; i < tope_obstaculos; i++) {

        if (mismas_coordenadas(obstaculos[i].posicion, objeto) == true) {
            ocupado_con_obstaculo = true;    
        }  
    }

    return ocupado_con_obstaculo;
}

/*Pre-condiciones: 
    el vector 'obstaculos' debe estar cargado anteriormente para poder buscar el obstaculo buscado.
Post-condiciones: 
    la función recorre el vector 'obstaculos' hasta encontrar la posicion del obstaculo que tiene las mismas coordenadas que el objeto, devolviendo qué tipo de obstáculo 
    es (FUEGOS o MEDIAS).*/
char tipo_de_obstaculo (coordenada_t objeto, objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos) {

    char tipo_de_obstaculo = ' ';
    bool encontrado = false;
    int i = 0;

    while (i < tope_obstaculos && !encontrado) {

        if (mismas_coordenadas(obstaculos[i].posicion, objeto) == true) {
            tipo_de_obstaculo = obstaculos[i].tipo;   
            encontrado = true; 
        }
        i++;      
    }

    return tipo_de_obstaculo;
}

/*Pre-condiciones: 
    el vector 'herramientas' debe estar cargado anteriormente para poder buscar el objeto buscado.
Post-condiciones: 
    la función recorre el vector 'herramientas' hasta encontrar la posicion de la herramienta que tiene las mismas coordenadas que el objeto, devolviendo qué tipo de herramienta 
    es (INTERRUPTOR o BOTELLAS).*/
char tipo_de_herramienta (coordenada_t objeto, objeto_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas) {

    char tipo_de_herramienta = ' ';
    bool encontrado = false;
    int i = 0;

    while (i < tope_herramientas && !encontrado) {

        if (mismas_coordenadas(herramientas[i].posicion, objeto) == true) {
            tipo_de_herramienta = herramientas[i].tipo;    
            encontrado = true;
        }
        i++;      
    }


    return tipo_de_herramienta;
}

/*Pre-condiciones: 
        "coordenada_a_ocupar" y "nivel" deben estar cargados;
Post-condiciones: 
    la función devuelve True si la "coordenada_a_ocupar" se encuentra ocupada por una pared, un papeleo, el jugador, un obstaculo o una herramienta.
    En caso contrario devuelve False.*/
bool coordenada_ocupada(coordenada_t coordenada_a_ocupar, nivel_t nivel) {

    return (coordenada_ocupada_con_pared(coordenada_a_ocupar, nivel.paredes, nivel.tope_paredes)  ||
                 coordenada_ocupada_con_jugador(coordenada_a_ocupar, nivel.pos_inicial_jugador) || 
                 coordenada_ocupada_con_papeleo(coordenada_a_ocupar, nivel.papeleos, nivel.tope_papeleos) ||
                 coordenada_ocupada_con_obstaculo(coordenada_a_ocupar, nivel.obstaculos, nivel.tope_obstaculos) ||
                 coordenada_ocupada_con_herramienta(coordenada_a_ocupar, nivel.herramientas, nivel.tope_herramientas));
}

/*Pre-condiciones: 
        "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    el procedimiento carga el vector 'obstaculos' del tipo objeto_t (campo del struct nivel_t), llenando tipo y posicion (la cual se verifica que ya no se encuentre ocupada, ademas
    de que si el objeto es FUEGO, comprobara que sea una posicion adyacente a una pared).*/
void cargar_vector_objetos_obstaculos(int cant_objeto, char objeto, int numero_nivel, nivel_t* nivel) {

   for (int i = 0; i < cant_objeto; i++) {

        nivel->obstaculos[nivel->tope_obstaculos].tipo = objeto;
        coordenada_t coordenada;

        if (objeto == FUEGOS) {

            do {
                coordenada_random(&coordenada, numero_nivel);
                nivel->obstaculos[nivel->tope_obstaculos].posicion = coordenada;
            } while (coordenada_ocupada(coordenada, *nivel) || (!adyacente(nivel->paredes, nivel->tope_paredes, coordenada)));

            (nivel->tope_obstaculos)++;

        } else {

            do {
                coordenada_random(&coordenada, numero_nivel);
                nivel->obstaculos[nivel->tope_obstaculos].posicion = coordenada;
            } while (coordenada_ocupada(coordenada, *nivel));

            (nivel->tope_obstaculos)++;
        }
       
    }
}
/*Pre-condiciones: 
        "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    el procedimiento carga el vector 'herramientas' del tipo objeto_t (campo del struct nivel_t), llenando tipo y posicion (la cual se verifica que ya no se encuentre ocupada).*/
void cargar_vector_objetos_herramientas(int cant_objeto, char objeto, int numero_nivel, nivel_t* nivel) {
   
    for (int i = 0; i < cant_objeto; i++) {
        nivel->herramientas[nivel->tope_herramientas].tipo = objeto;
        coordenada_t coordenada;

        do {
        coordenada_random(&coordenada, numero_nivel);
        nivel->herramientas[nivel->tope_herramientas].posicion = coordenada;
        } while (coordenada_ocupada(coordenada, *nivel));
      
        (nivel->tope_herramientas)++;
    
    }
}
/*Pre-condiciones: 
        "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    el procedimiento carga el vector 'papeleos' del tipo papeleo_t (campo del struct nivel_t), llenando posicion (la cual se verifica que ya no se encuentre ocupada),
    id y estado de recoleccion.*/
void cargar_vector_papeleos (int cant_papeleos, int numero_nivel, nivel_t* nivel) {

    for (int i = 0; i < cant_papeleos; i++) {

    	nivel->papeleos[nivel->tope_papeleos].id_papeleo = (i+1);
        nivel->papeleos[nivel->tope_papeleos].recolectado = false;
        coordenada_t coordenada;

        do {
            coordenada_random(&coordenada, numero_nivel);
            nivel->papeleos[nivel->tope_papeleos].posicion = coordenada;
        } while (coordenada_ocupada(coordenada, *nivel));

        (nivel->tope_papeleos)++;
    }
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3; 
    "amigo_pide_favor" debe ser JASMIN, OLAF, RAYO_MCQUEEN O STITCH (CONSTANTES en dia_en_la_uni).
Post-condiciones: 
    la función retorna "cantidad_fuegos" con su valor segun el nivel y el personaje enviado por parametro.*/
int cantidad_fuegos (int numero_nivel, char amigo_pide_favor) {

    int cantidad_fuegos;

    if (numero_nivel == NUMERO_NIVEL1) {
        if (amigo_pide_favor == OLAF) {
            cantidad_fuegos = CANT_FUEGOS_N1 - FUEGOS_EXTINTOS_OLAF_N1;
        } else {
            cantidad_fuegos = CANT_FUEGOS_N1;
        }    
    } else if (numero_nivel == NUMERO_NIVEL2) {
        if (amigo_pide_favor == OLAF) {
            cantidad_fuegos = CANT_FUEGOS_N2 - FUEGOS_EXTINTOS_OLAF_N2;
        } else {
            cantidad_fuegos = CANT_FUEGOS_N2;
        }   
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_fuegos = CANT_FUEGOS_N3;
    }

    return cantidad_fuegos;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    la función retorna "cantidad_medias" que es CANT_MEDIAS_N1, CANT_MEDIAS_N2 o CANT_MEDIAS_N3.*/
int cantidad_medias (int numero_nivel) {

    int cantidad_medias;

    if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_medias = CANT_MEDIAS_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
        cantidad_medias = CANT_MEDIAS_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_medias = CANT_MEDIAS_N3;
    }
    return cantidad_medias;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    la función retorna "cantidad_interruptores" que es CANT_INTERRUPTOR_N1, CANT_INTERRUPTOR_N2 o CANT_INTERRUPTOR_N3.*/
int cantidad_interruptores (int numero_nivel) {

    int cantidad_interruptores;

    if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_interruptores = CANT_INTERRUPTOR_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
        cantidad_interruptores = CANT_INTERRUPTOR_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_interruptores = CANT_INTERRUPTOR_N3;
    }

    return cantidad_interruptores;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    la función retorna "cantidad_botellas" que es CANT_BOTELLAS_N1, CANT_BOTELLAS_N2 o CANT_BOTELLAS_N3.*/
int cantidad_botellas (int numero_nivel) {

    int cantidad_botellas;

    if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_botellas = CANT_BOTELLAS_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
        cantidad_botellas = CANT_BOTELLAS_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_botellas = CANT_BOTELLAS_N3;
    }

    return cantidad_botellas;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3; 
    "amigo_pide_favor" debe ser JASMIN, OLAF, RAYO_MCQUEEN O STITCH (CONSTANTES en dia_en_la_uni).
Post-condiciones: 
    la función retorna "cantidad_papeleos" con su valor segun el nivel y el personaje enviado por parametro.*/
int cantidad_papeleos (int numero_nivel, char amigo_pide_favor) {

    int cantidad_papeleos;

    if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_papeleos = CANT_PAPELEOS_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
        cantidad_papeleos = CANT_PAPELEOS_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        if (amigo_pide_favor == STITCH) {
            cantidad_papeleos = CANT_PAPELEOS_N3 - PAPELEOS_QUEMADOS_STITCH_N3;
        } else {
        cantidad_papeleos = CANT_PAPELEOS_N3;
        }
    }

    return cantidad_papeleos;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3; 
    "amigo_pide_favor" debe ser JASMIN, OLAF, RAYO_MCQUEEN O STITCH (CONSTANTES en dia_en_la_uni).
Post-condiciones: 
    la función retorna "cantidad_movimientos_iniciales" con su valor segun el nivel y el personaje enviado por parametro.*/
int cantidad_movimientos_iniciales (int numero_nivel, char amigo_pide_favor) {

    int cantidad_movimientos_iniciales;

    if (numero_nivel == NUMERO_NIVEL1) {
        if (amigo_pide_favor == RAYO_MCQUEEN) {
            cantidad_movimientos_iniciales = CANT_MOVIMIENTOS_N1 + MOVIMIENTOS_EXTRAS_RAYO_N1;
        } else {
            cantidad_movimientos_iniciales = CANT_MOVIMIENTOS_N1;
        }
    } else if (numero_nivel == NUMERO_NIVEL2) {
        cantidad_movimientos_iniciales = CANT_MOVIMIENTOS_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_movimientos_iniciales = CANT_MOVIMIENTOS_N3;
    }

    return cantidad_movimientos_iniciales;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3; 
Post-condiciones: 
    la función retorna "cantidad_movimientos" con su valor segun el nivel.*/
int cantidad_movimientos_para_agregar_paredes (int numero_nivel) {

    int cantidad_movimientos;

    if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_movimientos = MOV_PAREDES_ALEATORIAS_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
        cantidad_movimientos = MOV_PAREDES_ALEATORIAS_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_movimientos = MOV_PAREDES_ALEATORIAS_N3;
    }

    return cantidad_movimientos;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3; 
    "amigo_pide_favor" debe ser JASMIN, OLAF, RAYO_MCQUEEN O STITCH (CONSTANTES en dia_en_la_uni).
Post-condiciones: 
    la función retorna "cantidad_martillos" con su valor segun el nivel y el personaje enviado por parametro.*/
int cantidad_martillos (int numero_nivel, char amigo_pide_favor) {

    int cantidad_martillos;

    if (amigo_pide_favor == JASMIN) {
        if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_martillos = CANT_MARTILLOS_N1 + MARTILLO_EXTRA_JASMIN;
        } else if (numero_nivel == NUMERO_NIVEL2) {
           cantidad_martillos = CANT_MARTILLOS_N2 + MARTILLO_EXTRA_JASMIN;
        } else if (numero_nivel == NUMERO_NIVEL3) {
            cantidad_martillos = CANT_MARTILLOS_N3 + MARTILLO_EXTRA_JASMIN;
        }
    } else {   
        if (numero_nivel == NUMERO_NIVEL1) {
            cantidad_martillos = CANT_MARTILLOS_N1;
        } else if (numero_nivel == NUMERO_NIVEL2) {
           cantidad_martillos = CANT_MARTILLOS_N2;
        } else if (numero_nivel == NUMERO_NIVEL3) {
            cantidad_martillos = CANT_MARTILLOS_N3;
        }
    }

    return cantidad_martillos;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    la función retorna "cantidad_extintores" que es CANT_EXTINTORES_N1, CANT_EXTINTORES_N2 o CANT_EXTINTORES_N3.*/
int cantidad_extintores (int numero_nivel) {

    int cantidad_extintores;

    if (numero_nivel == NUMERO_NIVEL1) {
        cantidad_extintores = CANT_EXTINTORES_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
       cantidad_extintores = CANT_EXTINTORES_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        cantidad_extintores = CANT_EXTINTORES_N3;
    }
    return cantidad_extintores;
}

/*Pre-condiciones: 
    "numero_nivel" debe ser 1, 2, o 3;
Post-condiciones: 
    la función retorna "divisor_de_randall" que es R_N1, R_N2 o R_N3.*/
int divisor_de_randall (int numero_nivel) {

    int divisor_de_randall;

    if (numero_nivel == NUMERO_NIVEL1) {
        divisor_de_randall = R_N1;
    } else if (numero_nivel == NUMERO_NIVEL2) {
       divisor_de_randall = R_N2;
    } else if (numero_nivel == NUMERO_NIVEL3) {
        divisor_de_randall = R_N3;
    }
    return divisor_de_randall;
}

// Pre y post en papeleo.h
void inicializar_nivel(nivel_t* nivel, int numero_nivel, char amigo_pide_favor) {

    inicializar_topes(nivel);

    obtener_paredes(numero_nivel, nivel->paredes, &(nivel->tope_paredes), &(nivel->pos_inicial_jugador)); 

    cargar_vector_papeleos(cantidad_papeleos(numero_nivel, amigo_pide_favor), numero_nivel, nivel);

    cargar_vector_objetos_obstaculos(cantidad_fuegos(numero_nivel, amigo_pide_favor), FUEGOS, numero_nivel, nivel);
    cargar_vector_objetos_obstaculos(cantidad_medias(numero_nivel), MEDIAS, numero_nivel, nivel);


    cargar_vector_objetos_herramientas(cantidad_botellas(numero_nivel), BOTELLAS, numero_nivel, nivel);
    cargar_vector_objetos_herramientas(cantidad_interruptores(numero_nivel), INTERRUPTOR, numero_nivel, nivel);

}

/*Pre-condiciones: 
        "numero_nivel" debe ser 1, 2, o 3;
        "amigo_pide_favor" debe ser JASMIN, OLAF, RAYO_MCQUEEN O STITCH (CONSTANTES en dia_en_la_uni).
Post-condiciones: 
    el procedimiento carga un registro del tipo juego_t, llenando cantidad de martillos, movimientos y extintores,
    ademas de la posicion (que debe ser la inicial al comenzar cada nivel) y el estado de ahuyenta_randall.
    Y agregando que los movimientos realizados al principio de cada nivel sean 0 sumandole los movimientos restantes del nivel anterior.*/
void cargar_jugador (jugador_t* jugador, int numero_nivel, char amigo_pide_favor, coordenada_t posicion_inicial, int movimientos_restantes) {

    jugador->martillos = cantidad_martillos(numero_nivel, amigo_pide_favor);
    jugador->movimientos = cantidad_movimientos_iniciales (numero_nivel, amigo_pide_favor) + movimientos_restantes;
    jugador->extintores = cantidad_extintores (numero_nivel);
    jugador->posicion = posicion_inicial;
    jugador->ahuyenta_randall = false;
    jugador->movimientos_realizados = 0;

}

// Pre y post en papeleo.h
void inicializar_juego (juego_t* juego, char amigo_pide_favor) {

    juego->nivel_actual = NUMERO_NIVEL1;
    juego->personaje_tp1 = amigo_pide_favor;

    inicializar_nivel(&(juego->niveles[0]), NUMERO_NIVEL1, amigo_pide_favor);
    inicializar_nivel(&(juego->niveles[1]), NUMERO_NIVEL2, amigo_pide_favor);
    inicializar_nivel(&(juego->niveles[2]), NUMERO_NIVEL3, amigo_pide_favor); 

    cargar_jugador (&(juego->jugador), juego->nivel_actual, amigo_pide_favor, juego->niveles[(juego->nivel_actual)-1].pos_inicial_jugador, 0);

} 

/*Pre-condiciones: 
     -
Post-condiciones: 
    el procedimiento inicializa una matriz de tamaño dimensionxdimension, primeramente con ' ' en cada posicion.*/
void inicializar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], int dimension) {

        for (int i = 0; i < dimension; i++)
            for (int j = 0; j < dimension; j++)
                mapa[i][j] = ESPACIO;
}

/*Pre-condiciones: 
    'juego' ya debe estar cargado.
Post-condiciones: 
    el procedimiento ubica en las coordenadas cargadas de los vectores 'papeleos', 'paredes', 'obstaculos', 'herramientas' sus respectivos caracteres 
    representativos en la matriz 'mapa'.*/
void cargar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego) {
  
    for (int i = 0; i < juego.niveles[(juego.nivel_actual-1)].tope_paredes; i++) {
        mapa[juego.niveles[(juego.nivel_actual-1)].paredes[i].fil][juego.niveles[(juego.nivel_actual-1)].paredes[i].col] = PARED;
    }

    for (int i = 0; i < juego.niveles[(juego.nivel_actual-1)].tope_papeleos; i++) {
        if (juego.niveles[(juego.nivel_actual-1)].papeleos[i].recolectado == false) {
            mapa[juego.niveles[(juego.nivel_actual-1)].papeleos[i].posicion.fil][juego.niveles[(juego.nivel_actual-1)].papeleos[i].posicion.col] = 
                (char)(juego.niveles[(juego.nivel_actual-1)].papeleos[i].id_papeleo + CERO_EN_ASCII);
        }
    }

    for (int i = 0; i < juego.niveles[(juego.nivel_actual-1)].tope_obstaculos; i++) {
        mapa[juego.niveles[(juego.nivel_actual-1)].obstaculos[i].posicion.fil][juego.niveles[(juego.nivel_actual-1)].obstaculos[i].posicion.col] = 
            juego.niveles[(juego.nivel_actual-1)].obstaculos[i].tipo;
    }

    for (int i = 0; i < juego.niveles[(juego.nivel_actual-1)].tope_herramientas; i++) {
        mapa[juego.niveles[(juego.nivel_actual-1)].herramientas[i].posicion.fil][juego.niveles[(juego.nivel_actual-1)].herramientas[i].posicion.col] = 
            juego.niveles[(juego.nivel_actual-1)].herramientas[i].tipo;
    }

    mapa[juego.jugador.posicion.fil][juego.jugador.posicion.col] = PERSONAJE;  
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    el procedimiento imprime la matriz 'mapa'.*/
void imprimir_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], int dimension){

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                printf(" %c ", mapa[i][j]);
        }
        printf("\n");
    }

}


// Pre y post en papeleo.h
void imprimir_terreno(juego_t juego) {

    system("clear");
				
	int dimension = definir_dimension_tablero(juego.nivel_actual);

	char mapa[MAX_FILAS][MAX_COLUMNAS];

    inicializar_mapa(mapa, dimension);
    cargar_mapa(mapa, juego);
    imprimir_mapa(mapa, dimension);

}

/*Pre-condiciones: 
    -
Post-condiciones: 
    imprime mensaje en ascii.*/
void mostrar_mensaje_de_perdedor() {

    system("clear");
    printf(       "██████╗ ███████╗██████╗ ██████╗ ██╗███████╗████████╗███████╗ \n");
    printf(       "██╔══██╗██╔════╝██╔══██╗██╔══██╗██║██╔════╝╚══██╔══╝██╔════╝ \n");
    printf(       "██████╔╝█████╗  ██████╔╝██║  ██║██║███████╗   ██║   █████╗   \n");
    printf(       "██╔═══╝ ██╔══╝  ██╔══██╗██║  ██║██║╚════██║   ██║   ██╔══╝   \n");
    printf(       "██║     ███████╗██║  ██║██████╔╝██║███████║   ██║   ███████╗ \n");
    printf(       "╚═╝     ╚══════╝╚═╝  ╚═╝╚═════╝ ╚═╝╚══════╝   ╚═╝   ╚══════╝ \n");
    detener_el_tiempo(TIEMPO_DE_MUESTREO_DE_MENSAJE);
    system("clear");

}

/*Pre-condiciones: 
    -
Post-condiciones: 
    imprime mensaje en ascii.*/
void mostrar_mensaje_de_ganador() {

    system("clear");
    printf(       "\n");
    printf(       "   ███████╗ ███████╗ ██╗      ██╗  ██████╗ ██╗ ████████╗  █████╗   ██████╗ ██╗  ██████╗  ███╗   ██╗ ███████╗ ███████╗ \n");
    printf(       "   ██╔════╝ ██╔════╝ ██║      ██║ ██╔════╝ ██║ ╚══██╔══╝ ██╔══██╗ ██╔════╝ ██║ ██╔═══██╗ ████╗  ██║ ██╔════╝ ██╔════╝ \n");
    printf(       "   █████╗   █████╗   ██║      ██║ ██║      ██║    ██║    ███████║ ██║      ██║ ██║   ██║ ██╔██╗ ██║ █████╗   ███████╗ \n");
    printf(       "   ██╔══╝   ██╔══╝   ██║      ██║ ██║      ██║    ██║    ██╔══██║ ██║      ██║ ██║   ██║ ██║╚██╗██║ ██╔══╝   ╚════██║ \n");
    printf(       "   ██║      ███████╗ ███████╗ ██║ ╚██████╗ ██║    ██║    ██║  ██║ ╚██████╗ ██║ ╚██████╔╝ ██║ ╚████║ ███████╗ ███████║ \n");
    printf(       "   ╚═╝      ╚══════╝ ╚══════╝ ╚═╝  ╚═════╝ ╚═╝    ╚═╝    ╚═╝  ╚═╝  ╚═════╝ ╚═╝  ╚═════╝  ╚═╝  ╚═══╝ ╚══════╝ ╚══════╝ \n");
    printf(       "\n");
    printf(       "                    ██████╗  █████╗ ███╗   ██╗ █████╗ ███████╗████████╗███████╗ \n");
    printf(       "                   ██╔════╝ ██╔══██╗████╗  ██║██╔══██╗██╔════╝╚══██╔══╝██╔════╝ \n");
    printf(       "                   ██║  ███╗███████║██╔██╗ ██║███████║███████╗   ██║   █████╗   \n");
    printf(       "                   ██║   ██║██╔══██║██║╚██╗██║██╔══██║╚════██║   ██║   ██╔══╝   \n");
    printf(       "                   ╚██████╔╝██║  ██║██║ ╚████║██║  ██║███████║   ██║   ███████╗ \n");
    printf(       "                    ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝   ╚═╝   ╚══════╝ \n");
    detener_el_tiempo(TIEMPO_DE_MUESTREO_DE_MENSAJE);
    system("clear");
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función devuelve True si 'comando' cumple con alguna de las condiciones impuestas, devolverá False en caso contrario.*/
bool es_comando_de_movimiento_valido (char comando) {

    return (comando == DERECHA || comando == IZQUIERDA || comando == ROTACION_HORARIA || comando == ROTACION_ANTIHORARIA || comando == USAR_EXTINTOR || comando == USAR_MARTILLO);
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función devuelve True si 'comando' cumple con alguna de las condiciones impuestas, devolverá False en caso contrario.*/
bool es_direccion_valida_para_eliminar (char comando) {

    return (comando == DERECHA || comando == IZQUIERDA || comando == ARRIBA || comando == ABAJO);
}

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función devuelve True si 'movimientos_dispoibles' es menor a 0, False en caso contrario.*/
bool hay_movimientos_para_usar (int movimientos_disponibles) {

    return (movimientos_disponibles > 0);
}

/*Pre-condiciones: 
    'coordenada' debe estar cargada.
Post-condiciones: 
    la función devuelve 'coordenada' rotada en sentido horario.*/
coordenada_t coordenada_rotada_horario (coordenada_t* coordenada, int dimension) {

    int auxiliar;
    int menos_filas = dimension - 1;
    auxiliar = (*coordenada).fil;
    (*coordenada).fil = (*coordenada).col;
    (*coordenada).col = abs(auxiliar - menos_filas);

    return *coordenada;

}

/*Pre-condiciones: 
    'coordenada' debe estar cargada.
Post-condiciones: 
    la función devuelve 'coordenada' rotada en sentido antihorario.*/
coordenada_t coordenada_rotada_antihorario (coordenada_t* coordenada, int dimension) {

    int auxiliar;
    int menos_columnas = dimension - 1;    
    auxiliar = (*coordenada).col;
    (*coordenada).col = (*coordenada).fil;
    (*coordenada).fil = abs(auxiliar - menos_columnas);

    return *coordenada;
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento suma un movimiento y resta a los movimientos disponibles.*/
void sumar_movimiento(juego_t* juego) {

    (juego->jugador.movimientos_realizados)++;
    (juego->jugador.movimientos)--;

}

/*Pre-condiciones: 
    el vector 'papeleos' debe estar cargado y 'id_papeleo' debe ser válido.
Post-condiciones: 
    la función devuelve True si se encuentra que en el vector 'papeleos', el papeleo que tiene como campo id el anterior a 'id_papeleo' ingresado como parámetro está recolectado.*/
bool papeleo_anterior_fue_recogido(papeleo_t papeleos[MAX_PAPELEOS], int id_papeleo) {

    return (papeleos[id_papeleo - 2].recolectado);

}

/*Pre-condiciones: 
    'posicion_papeleo' y 'papeleos' deben estar cargados.
Post-condiciones: 
    la función devuelve el numero de papeleo (su id) del papeleo que tenga como posicion 'posicion_papeleo' dentro del vector 'papeleos'.*/
int papeleo_encontrado(coordenada_t posicion_papeleo, papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos) {
    
    int numero_de_papeleo = 0;

    for (int i = 0; i < tope_papeleos; i++) {
        if (papeleos[i].posicion.fil == posicion_papeleo.fil && papeleos[i].posicion.col == posicion_papeleo.col) {
            numero_de_papeleo = papeleos[i].id_papeleo;
        }
    }

    return numero_de_papeleo;
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento vuelve a cero los movimientos que tiene para usar el jugador.*/
void pisar_fuego(juego_t* juego) {

    juego->jugador.movimientos = 0; 

}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento le resta a los movimientos del jugador los correspondientes como castigo por haber pisado una media.*/
void pisar_media(juego_t* juego) {

    juego->jugador.movimientos = juego->jugador.movimientos - MOVIMIENTOS_PERDIDOS_POR_MEDIA;
  
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento le suma a los movimientos del jugador los correspondientes como recompensa por haber pisado una botella de gritos.*/
void pisar_botella(juego_t* juego) {

    juego->jugador.movimientos = juego->jugador.movimientos + MOVIMIENTOS_GANADOS_POR_BOTELLA;

}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento activa o desactiva el interruptor ahuyenta randall (si está desactivado lo activa, si está activado lo desactiva).*/
void pisar_interruptor(juego_t* juego) {

    if (!juego->jugador.ahuyenta_randall){

        juego->jugador.ahuyenta_randall = true;

    } else {

        juego->jugador.ahuyenta_randall = false;

    }
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento pregunta qué tipo de obstáculo se pisó y según cuál haya sido se aplicará su castigo.*/
void pisar_obstaculo(juego_t* juego) {

    if (tipo_de_obstaculo(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].obstaculos,
    juego->niveles[juego->nivel_actual-1].tope_obstaculos) == FUEGOS) {

        pisar_fuego(juego);

    } else if (tipo_de_obstaculo(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].obstaculos,
    juego->niveles[juego->nivel_actual-1].tope_obstaculos) == MEDIAS) {

        pisar_media(juego);
    } 
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento pregunta qué tipo de herramienta se pisó y según cuál haya sido se aplicará su recompensa.*/
void pisar_herramienta(juego_t* juego) {
    
    if (tipo_de_herramienta(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].herramientas, 
    juego->niveles[juego->nivel_actual-1].tope_herramientas) == BOTELLAS) {

        pisar_botella(juego);

    } else if (tipo_de_herramienta(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].herramientas, 
    juego->niveles[juego->nivel_actual-1].tope_herramientas) == INTERRUPTOR) {

        pisar_interruptor(juego);
    }   

}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento recoge un papeleo si el papeleo anterior a numero_papeleo ya fue recogido, dejando su campo recolectado como True, 
    en caso contrario no se recolectará..*/
void pisar_papeleo(juego_t* juego, int numero_papeleo) {

    if (papeleo_anterior_fue_recogido(juego->niveles[juego->nivel_actual-1].papeleos, numero_papeleo)) {

        juego->niveles[juego->nivel_actual-1].papeleos[numero_papeleo - 1].recolectado = true;
    }
}    

/*Pre-condiciones: 
    -
Post-condiciones: 
    la función devuelve True si 'movimientos' es divisible por 'divisor_randall'.*/
bool son_movimientos_divisibles_por_R (int movimientos, int divisor_randall) {

    return (movimientos % divisor_randall == 0);
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento según se establezcan ciertas condiciones ('ahuyenta_randall' en False y los 'movimientos_realizados' divisibles por el divisor de randall), 
    moverá un papeleo aleatorio en una posición libre y válida.*/
void ahuyentar_a_randall (nivel_t* nivel, bool ahuyenta_randall, int nivel_actual, int movimientos_realizados) {

    if (!ahuyenta_randall && son_movimientos_divisibles_por_R(movimientos_realizados, divisor_de_randall(nivel_actual))) {

        int i = rand () % (nivel->tope_papeleos) + 0;
        coordenada_t coordenada;
        do {
            coordenada_random(&coordenada, nivel_actual);
        } while (coordenada_ocupada(coordenada, *nivel));
         
         nivel->papeleos[i].posicion = coordenada;
    }

}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento según se establezcan ciertas condiciones (los movimientos realizados por el jugador deben ser menores a los movimientos límites que se deben realizar 
    para agregar paredes aleatorias (depende de cada nivel)), generará una pared aleatoria en una posición libre y válida.*/
void agregar_pared_aleatoria (juego_t* juego) {
    
    int movimientos_limite = cantidad_movimientos_para_agregar_paredes(juego->nivel_actual);

    if (juego->jugador.movimientos_realizados <= movimientos_limite) {

        coordenada_t nueva_pared;

        do {
            coordenada_random(&nueva_pared, juego->nivel_actual); 
        } while (coordenada_ocupada(nueva_pared, juego->niveles[juego->nivel_actual-1]));
         
        juego->niveles[juego->nivel_actual-1].paredes[juego->niveles[juego->nivel_actual-1].tope_paredes] = nueva_pared;
        (juego->niveles[juego->nivel_actual-1].tope_paredes)++;
    }

}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    en el procedimiento según qué posición tenga el jugador, se comparará si coincide con algún obstáculo, herramienta o papeleo, accionando como se debe en cada uno de los casos.*/
void interactuar_con_objetos_y_papeleos (juego_t* juego) {

    if (coordenada_ocupada_con_obstaculo(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].obstaculos,
            juego->niveles[juego->nivel_actual-1].tope_obstaculos)) {

        pisar_obstaculo(juego); 

    } else if (coordenada_ocupada_con_herramienta(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].herramientas,
        juego->niveles[juego->nivel_actual-1].tope_herramientas)) {

        pisar_herramienta(juego);

    } else if (coordenada_ocupada_con_papeleo(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].papeleos,
        juego->niveles[juego->nivel_actual-1].tope_papeleos)) {
        
        int papeleo = papeleo_encontrado(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].papeleos, juego->niveles[juego->nivel_actual-1].tope_papeleos);
        pisar_papeleo(juego, papeleo);    

    }
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento hace 'caer' al jugador hacia abajo de su posición hasta que se encuentre con una pared. Tambíén se tiene en cuenta si este pasó por un objeto o por papeleos.*/
void efectuar_gravedad (juego_t* juego) {

    while (!(coordenada_ocupada_con_pared(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].paredes, 
    juego->niveles[juego->nivel_actual-1].tope_paredes)) && hay_movimientos_para_usar(juego->jugador.movimientos)) {
            
        (juego->jugador.posicion.fil)++;

        interactuar_con_objetos_y_papeleos(juego);

    }

    if (coordenada_ocupada_con_pared(juego->jugador.posicion, juego->niveles[juego->nivel_actual-1].paredes, 
    juego->niveles[juego->nivel_actual-1].tope_paredes)){

        (juego->jugador.posicion.fil)--; 
    
    }
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento suma un movimiento al jugador y rota todas las coordenadas de la matriz tamaño dimensionxdimension de manera horaria. Además de tener en cuenta la gravedad
    luego de rotar, el procedimiento de ahuyenta_randall y el agregar paredes aleatorias.*/
void rotar_horario (juego_t* juego) {

    sumar_movimiento(juego);

    int dimension = definir_dimension_tablero(juego->nivel_actual);
    
    coordenada_rotada_horario(&(juego->jugador.posicion), dimension);

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_paredes; i++) {
        coordenada_rotada_horario(&(juego->niveles[(juego->nivel_actual-1)].paredes[i]), dimension);
    }

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_papeleos; i++) {
        coordenada_rotada_horario(&(juego->niveles[(juego->nivel_actual-1)].papeleos[i].posicion),dimension);
    }

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_obstaculos; i++) {
        coordenada_rotada_horario(&(juego->niveles[(juego->nivel_actual-1)].obstaculos[i].posicion), dimension);
    }        

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_herramientas; i++) {
        coordenada_rotada_horario(&(juego->niveles[(juego->nivel_actual-1)].herramientas[i].posicion), dimension);
    }

    efectuar_gravedad (juego);

    ahuyentar_a_randall(&(juego->niveles[(juego->nivel_actual)-1]), juego->jugador.ahuyenta_randall, juego->nivel_actual, juego->jugador.movimientos_realizados);
    agregar_pared_aleatoria(juego);    
}

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento suma un movimiento al jugador y rota todas las coordenadas de la matriz tamaño dimensionxdimension de manera antihoraria. Además de tener en cuenta la gravedad
    luego de rotar, el procedimiento de ahuyenta_randall y el agregar paredes aleatorias.*/
void rotar_antihorario (juego_t* juego) {

    sumar_movimiento(juego);

    int dimension = definir_dimension_tablero(juego->nivel_actual);
    
    coordenada_rotada_antihorario(&(juego->jugador.posicion), dimension);

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_paredes; i++) {
        coordenada_rotada_antihorario(&(juego->niveles[(juego->nivel_actual-1)].paredes[i]), dimension);
    }

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_papeleos; i++) {
        coordenada_rotada_antihorario(&(juego->niveles[(juego->nivel_actual-1)].papeleos[i].posicion),dimension);
    }

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_obstaculos; i++) {
        coordenada_rotada_antihorario(&(juego->niveles[(juego->nivel_actual-1)].obstaculos[i].posicion), dimension);
    }        

    for (int i = 0; i < juego->niveles[(juego->nivel_actual-1)].tope_herramientas; i++) {
        coordenada_rotada_antihorario(&(juego->niveles[(juego->nivel_actual-1)].herramientas[i].posicion), dimension);
    }

    efectuar_gravedad(juego);

    ahuyentar_a_randall(&(juego->niveles[(juego->nivel_actual)-1]), juego->jugador.ahuyenta_randall, juego->nivel_actual, juego->jugador.movimientos_realizados);
    agregar_pared_aleatoria(juego);

}


/*Pre-condiciones: 
    -
Post-condiciones: 
    la función devuelve True si 'cantidad_de_herramientas' es menor a 0, False en caso contrario.*/
bool hay_herramientas_disponibles (int cantidad_de_herramientas) {

    return (cantidad_de_herramientas > 0);
}   

/*Pre-condiciones: 
    el vector 'paredes' debe estar cargado.
Post-condiciones: 
la función devuelve la posicion en el vector 'paredes' del elemento de este que tenga la misma coordenada que 'pared_buscada'.*/
int posicion_pared_a_eliminar(coordenada_t paredes[MAX_PAREDES], int tope_paredes, coordenada_t pared_buscada) {
    
    int posicion = 0;

    for (int i = 0; i < tope_paredes; i++) {
        if (paredes[i].fil == pared_buscada.fil && paredes[i].col == pared_buscada.col) {
            posicion = i;
        }
    }

    return posicion;
}

/*Pre-condiciones: 
    el vector 'paredes' debe estar cargado.
Post-condiciones: 
    la función devuelve la posicion en el vector 'obstaculos' del elemento de este que tenga la misma coordenada que 'fuego_buscado' y que sea del tipo FUEGOS.*/
int posicion_fuego_a_eliminar(objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos, coordenada_t fuego_buscado) {
    
    int posicion = 0;

    for (int i = 0; i < tope_obstaculos; i++) {
        if (obstaculos[i].posicion.fil == fuego_buscado.fil && obstaculos[i].posicion.col == fuego_buscado.col && obstaculos[i].tipo == FUEGOS){
            posicion = i;
        }
    }

    return posicion;
}

/*Pre-condiciones: 
    el vector 'paredes' debe estar cargado.
Post-condiciones: 
    el procedimiento elimina (si se encuentra en este) el elemento con 'posicion' como ubicación en el vector 'paredes'.*/
void eliminar_pared(coordenada_t paredes[MAX_PAREDES], int* tope_paredes, int posicion) {

    for (int j = posicion; j < (*tope_paredes)-1; j++) {
        paredes[j] = paredes[j+1];
    }

    (*tope_paredes)--;
}

/*Pre-condiciones: 
    el vector 'obstaculos' debe estar cargado.
Post-condiciones: 
    el procedimiento elimina (si se encuentra en este) el elemento con 'posicion' como ubicación en el vector 'obstaculos'.*/
void eliminar_fuego(objeto_t obstaculos[MAX_OBSTACULOS], int* tope_obstaculos, int posicion) {

    for (int j = posicion; j < (*tope_obstaculos)-1; j++) {
        obstaculos[j] = obstaculos[j+1];
    }

    (*tope_obstaculos)--;
}

/*Pre-condiciones: 
   'original' debe estar cargada.
Post-condiciones: 
    el procedimiento carga en 'posicion_a_cargar' la coordenada a la izquierda de 'original'.*/
void cargar_coordenada_izquierda(coordenada_t* posicion_a_cargar, coordenada_t original) {

    posicion_a_cargar->fil = original.fil;
    posicion_a_cargar->col = original.col - 1;
}

/*Pre-condiciones: 
   'original' debe estar cargada.
Post-condiciones: 
    el procedimiento carga en 'posicion_a_cargar' la coordenada a la derecha de 'original'.*/
void cargar_coordenada_derecha(coordenada_t* posicion_a_cargar, coordenada_t original) {

    posicion_a_cargar->fil = original.fil;
    posicion_a_cargar->col = original.col + 1;
}

/*Pre-condiciones: 
   'original' debe estar cargada.
Post-condiciones: 
    el procedimiento carga en 'posicion_a_cargar' la coordenada arriba de 'original'.*/
void cargar_coordenada_arriba(coordenada_t* posicion_a_cargar, coordenada_t original) {

    posicion_a_cargar->fil = original.fil - 1;
    posicion_a_cargar->col = original.col;
}

/*Pre-condiciones: 
   'original' debe estar cargada.
Post-condiciones: 
    el procedimiento carga en 'posicion_a_cargar' la coordenada debajo de 'original'.*/
void cargar_coordenada_abajo(coordenada_t* posicion_a_cargar, coordenada_t original) {

    posicion_a_cargar->fil = original.fil + 1;
    posicion_a_cargar->col = original.col;
}


/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento pregunta si la siguiente coordenada a la izq tiene una pared que la ocupa, en ese caso no se mueve al personaje. En caso contrario, el jugador se mueve hacia la
    izquierda, teniendo en cuenta la interaccion con objetos y papeleos, el ahuyenta randall y el agregado de una pared aleatoria. Además de usar gravedad.*/
void realizar_movimiento_hacia_la_izquierda (juego_t* juego) {
    
    coordenada_t siguiente_posicion_izquierda;
    cargar_coordenada_izquierda(&siguiente_posicion_izquierda, juego->jugador.posicion);

    if (!coordenada_ocupada_con_pared(siguiente_posicion_izquierda, juego->niveles[juego->nivel_actual-1].paredes,
        juego->niveles[juego->nivel_actual-1].tope_paredes)) {

        (juego->jugador.posicion.col)--;

        sumar_movimiento(juego);

        interactuar_con_objetos_y_papeleos(juego);

        ahuyentar_a_randall(&(juego->niveles[(juego->nivel_actual)-1]), juego->jugador.ahuyenta_randall, juego->nivel_actual, juego->jugador.movimientos_realizados);
        agregar_pared_aleatoria(juego);

    }

    efectuar_gravedad(juego);

}   

/*Pre-condiciones: 
    'juego' debe estar cargado.
Post-condiciones: 
    el procedimiento pregunta si la siguiente coordenada a la derecha tiene una pared que la ocupa, en ese caso no se mueve al personaje. En caso contrario, el jugador se mueve hacia la
    derecha, teniendo en cuenta la interaccion con objetos y papeleos, el ahuyenta randall y el agregado de una pared aleatoria. Además de usar gravedad.*/
void realizar_movimiento_hacia_la_derecha (juego_t* juego) {

    coordenada_t siguiente_posicion_derecha;
    cargar_coordenada_derecha(&siguiente_posicion_derecha, juego->jugador.posicion);

    if (!coordenada_ocupada_con_pared(siguiente_posicion_derecha, juego->niveles[juego->nivel_actual-1].paredes,
        juego->niveles[juego->nivel_actual-1].tope_paredes)) {

        (juego->jugador.posicion.col)++;

        sumar_movimiento(juego);

        interactuar_con_objetos_y_papeleos(juego);

        ahuyentar_a_randall(&(juego->niveles[(juego->nivel_actual)-1]), juego->jugador.ahuyenta_randall, juego->nivel_actual, juego->jugador.movimientos_realizados);
        agregar_pared_aleatoria(juego);
    } 

    efectuar_gravedad(juego);
}

/*Pre-condiciones: 
   -
Post-condiciones: 
    la función devuelve True si la 'pared' comprende un borde de la matriz tamaño dimensionxdimension.*/
bool es_borde(coordenada_t pared, int dimension) {

    return (pared.col == 0 || pared.col == (dimension - 1) || pared.fil == 0 || pared.fil == (dimension - 1));
}

/*Pre-condiciones: 
   'juego' y 'pared' deben estar cargados.
Post-condiciones: 
    el procedimiento elimina una pared del vector 'paredes' (campo del struct juego_t) si ésta no forma parte del borde de la matriz de tamaño dimensionxdimension.*/
void martllar_pared (juego_t* juego, coordenada_t pared) {

    int posicion = posicion_pared_a_eliminar(juego->niveles[juego->nivel_actual-1].paredes,juego->niveles[juego->nivel_actual-1].tope_paredes, pared);
    int dimension = definir_dimension_tablero(juego->nivel_actual);

    if (coordenada_ocupada_con_pared(pared, &(juego->niveles[juego->nivel_actual-1].paredes)[posicion], juego->niveles[juego->nivel_actual-1].tope_paredes) && !es_borde(pared, dimension)) {

        (juego->jugador.martillos)--;
        eliminar_pared(juego->niveles[juego->nivel_actual-1].paredes,&(juego->niveles[juego->nivel_actual-1].tope_paredes),posicion);

    } 

}

/*Pre-condiciones: 
   'juego' y 'pared' deben estar cargados.
Post-condiciones: 
    el procedimiento elimina un fuego del vector 'obstaculos' (campo del struct juego_t).*/
void extinguir_fuego (juego_t* juego, coordenada_t fuego){

    int posicion = posicion_fuego_a_eliminar(juego->niveles[juego->nivel_actual-1].obstaculos,juego->niveles[juego->nivel_actual-1].tope_obstaculos, fuego);

    if (coordenada_ocupada_con_obstaculo(fuego, &(juego->niveles[juego->nivel_actual-1].obstaculos)[posicion], 
    juego->niveles[juego->nivel_actual-1].tope_obstaculos)) {

        (juego->jugador.extintores)--;
        eliminar_fuego(juego->niveles[juego->nivel_actual-1].obstaculos, &(juego->niveles[juego->nivel_actual-1].tope_obstaculos), posicion);

    } 
}

/*Pre-condiciones: 
   'comando' y 'juego' deben estar cargados.
Post-condiciones: 
    según qué comando sea, cargará la coordenada correspondiente a eliminar y se procede a llamar al procedimiento que se encargará de eliminar
     la pared en ese lugar si es posible.*/
void definir_direccion_de_uso_de_martillo(char comando, juego_t* juego, coordenada_t a_eliminar){

    if (comando == IZQUIERDA) {

        cargar_coordenada_izquierda(&a_eliminar, juego->jugador.posicion);
        martllar_pared(juego,a_eliminar);

    } else if (comando == DERECHA) {

        cargar_coordenada_derecha(&a_eliminar, juego->jugador.posicion);
        martllar_pared(juego, a_eliminar);
                    
    } else if (comando == ARRIBA) {

        cargar_coordenada_arriba(&a_eliminar, juego->jugador.posicion);
        martllar_pared(juego,a_eliminar);

    } else if (comando == ABAJO) {

        cargar_coordenada_abajo(&a_eliminar, juego->jugador.posicion);
        martllar_pared(juego, a_eliminar);
        efectuar_gravedad (juego);
    }

}

/*Pre-condiciones: 
   'comando' y 'juego' deben estar cargados.
Post-condiciones: 
    según qué comando sea, cargará la coordenada correspondiente a eliminar y se procede a llamar al procedimiento que eliminará el fuego en ese lugar.*/
void definir_direccion_de_uso_de_extintor(char comando, juego_t* juego, coordenada_t a_eliminar) {

    if (comando == IZQUIERDA) {

        cargar_coordenada_izquierda(&a_eliminar, juego->jugador.posicion);
        extinguir_fuego(juego,a_eliminar);

    } else if (comando == DERECHA) {

        cargar_coordenada_derecha(&a_eliminar, juego->jugador.posicion);
        extinguir_fuego(juego, a_eliminar);
                    
    } else if (comando == ARRIBA) {

        cargar_coordenada_arriba(&a_eliminar, juego->jugador.posicion);
        extinguir_fuego(juego,a_eliminar);

    }

}

/*Pre-condiciones: 
   'comando' y 'juego' deben estar cargados.
Post-condiciones: 
    si hay martillos para usar, se pide la dirección de la pared a eliminar, si es válida se elimina, si la dirección no es válida, sigue preguntando hasta que lo sea.*/
void preparar_martillos (juego_t* juego, char comando){
    
    if (hay_herramientas_disponibles(juego->jugador.martillos)) {

        printf("\nEn qué dirección querés martillar?:");
        scanf(" %c", &comando);

        while (!es_direccion_valida_para_eliminar(comando)) {

            printf("\nEse comando NO es válido, intentá otro:");
            scanf(" %c", &comando);            
        } 
        coordenada_t a_eliminar;
        definir_direccion_de_uso_de_martillo(comando, juego, a_eliminar);

    }

}             

/*Pre-condiciones: 
   'comando' y 'juego' deben estar cargados.
Post-condiciones: 
    si hay extintores para usar, se pide la direccion del fuego a eliminar, si es válida se elimina, si la dirección no es válida, sigue preguntando hasta que lo sea.*/
void preparar_extintores (juego_t* juego, char comando){

    if (hay_herramientas_disponibles(juego->jugador.extintores)) {

        printf("\nEn qué dirección querés extinguir? a la derecha, izquierda o arriba: ");
        scanf(" %c", &comando);

        while (!es_direccion_valida_para_eliminar(comando)) {

   
            printf("\nEse comando NO es válido, intentá otro:");
            scanf(" %c", &comando);            
        }

        coordenada_t a_eliminar;
        definir_direccion_de_uso_de_extintor(comando, juego, a_eliminar);

    }  
} 

// Pre y post en papeleo.h
int estado_nivel(papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos){

    int estado = GANADO;

    for (int i = 0; i < tope_papeleos; i++) {

        if (papeleos[i].recolectado == false) {

            estado = JUGANDO;
        }
    }

    return estado;
}

// Pre y post en papeleo.h
int estado_juego(juego_t juego) {

    int estado = JUGANDO;

    if (juego.nivel_actual == NUMERO_NIVEL3 && estado_nivel(juego.niveles[(juego.nivel_actual)-1].papeleos, juego.niveles[(juego.nivel_actual)-1].tope_papeleos) == GANADO) {

        estado = GANADO;

    } else if (!(hay_movimientos_para_usar(juego.jugador.movimientos))) {

        estado = PERDIDO;
    }

    return estado;
}

/*Pre-condiciones: 
   -
Post-condiciones: 
    Se ingresa 'comando' y se realiza su validación.*/
void ingresar_y_verificar_comando (char* comando, juego_t juego) {

    printf("¿Cuál es tu siguiente movimiento?: ");
    scanf(" %c", comando);

    while (!es_comando_de_movimiento_valido(*comando)) {

        printf("\nEse comando NO es válido, intentá otro: ");
        scanf(" %c", comando);            
    } 

}

/*Pre-condiciones: 
   'comando' y 'juego' deben estar cargados.
Post-condiciones: 
    Según qué comando sea, se realizarán las acciones correspondientes a cada uno.*/
void aplicar_acciones_segun_comando(char comando, juego_t* juego) {

    if (comando == IZQUIERDA) {
          
        realizar_movimiento_hacia_la_izquierda(juego);

    } else if (comando == DERECHA) {

        realizar_movimiento_hacia_la_derecha(juego);
    
    } else if (comando == ROTACION_HORARIA) {

        rotar_horario(juego);
        
    } else if (comando == ROTACION_ANTIHORARIA) {

        rotar_antihorario(juego);

    } else if (comando == USAR_MARTILLO) {

        preparar_martillos(juego, comando);

    } else if (comando == USAR_EXTINTOR) {

        preparar_extintores(juego, comando);

    }
}

// Pre y post en papeleo.h
void realizar_jugada(juego_t* juego) {

    char comando;

    int estado_del_nivel = JUGANDO;
    int estado_del_juego = JUGANDO;

        ingresar_y_verificar_comando(&comando, *juego);

        aplicar_acciones_segun_comando(comando, juego);

        estado_del_nivel = estado_nivel(juego->niveles[(juego->nivel_actual)-1].papeleos, juego->niveles[(juego->nivel_actual)-1].tope_papeleos);

        estado_del_juego = estado_juego(*juego);

    if (estado_del_nivel == GANADO && estado_del_juego == JUGANDO) {

        (juego->nivel_actual)++;
        cargar_jugador (&(juego->jugador), juego->nivel_actual, juego->personaje_tp1, juego->niveles[(juego->nivel_actual)-1].pos_inicial_jugador, juego->jugador.movimientos);

    } else if (estado_del_juego == GANADO) {

        mostrar_mensaje_de_ganador();

    } else if (estado_del_juego == PERDIDO) {

        mostrar_mensaje_de_perdedor();
    }

}


