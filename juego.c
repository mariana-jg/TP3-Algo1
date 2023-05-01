#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dia_en_la_uni.h"
#include "papeleo.h"
#include <stdlib.h>
#include <time.h>
#include "controlador.h"

#define JUGANDO 0

#define ACTIVADO 'A'
#define DESACTIVADO 'D'

void imprimir_personaje_ayudante(char personaje_tp1) {

    if (personaje_tp1 == JASMIN) {
        printf("Tu personaje ayudante es Jasmín, ella te otorgará 1 martillo extra en cada nivel.\n");
    } else if (personaje_tp1 == RAYO_MCQUEEN) {
        printf("Tu personaje ayudante es Rayo Mcqueen, él te dará 10 movimientos extras al comenzar el primer nivel.\n");
    } else if (personaje_tp1 == OLAF) {
        printf("Tu personaje ayudante es Olaf, él te extinguirá 2 fuegos del primer nivel y 1 del segundo.\n");
    } else if (personaje_tp1 == STITCH) {
        printf("Tu personaje ayudante es Stitch, él te quemará un papeleo del último nivel.\n");
    }
} 

char estado_ahuyenta_randall(bool estado){

    char activado_o_desactivado = DESACTIVADO;

    if (estado) {

        activado_o_desactivado = ACTIVADO;
    }    

    return activado_o_desactivado;
}
void imprimir_mensajes (juego_t juego){

    printf("Estás en el nivel: %i\n"
        "Cantidad de movimiento disponibles: %i\n"
        "Cantidad de movimientos realizados: %i\n"
        "Cantidad de martillos disponibles: %i\n"
        "Cantidad de extintores disponibles: %i\n"
        "Estado del Ahuyenta Randall (A: activado / D: desactivado): %c\n", 
        juego.nivel_actual, juego.jugador.movimientos, juego.jugador.movimientos_realizados, juego.jugador.martillos, juego.jugador.extintores, 
        estado_ahuyenta_randall(juego.jugador.ahuyenta_randall)); 
}


int main () {

    srand (( unsigned)time(NULL));

   char personaje;
    
   juego_t juego;

   personaje = ejec_cuestionario(&personaje);

   inicializar_controlador ();
    
   inicializar_juego(&juego, personaje);

    do {

        imprimir_terreno(juego);
        imprimir_personaje_ayudante(personaje);
        imprimir_mensajes(juego);
        realizar_jugada(&juego);
   
    } while (estado_juego(juego) == JUGANDO);

   terminar_controlador ();

   return 0;
}