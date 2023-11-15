#ifndef PEDIDO_H_INCLUDED
#define PEDIDO_H_INCLUDED
#include "combo.h"
#include <time.h>

typedef struct{
    int dia;
    int mes;
    int anio;
}Fecha;

typedef struct{
    char nombre[30];
    char id_pedido[7];
    int vendedor;
    int combo_pedido[MAX];
    int forma_de_pago; //1 debito 2 credito 3 qr 4 efectivo
    float subtotal;
    int consume_en_local; //1 si 0 no
    float total;
    Fecha fecha_de_compra;
    int entregado; //1 si 0 no
}Pedido;

//INICIALIZADOR

void init_pedido(Pedido *pedido, int i){
    (*pedido).consume_en_local = 1; //si
    (*pedido).entregado = 0; //no
    (*pedido).subtotal = 0;
    for (i = 0; i < 10; i++){
        (*pedido).combo_pedido[i] = 0;
    }
    (*pedido).total = 0;
    set_fecha_actual(pedido);
}

//SETTERS

void set_nombre(Pedido *pedido,char nombre[]){
    strcpy((*pedido).nombre,nombre);
}

void set_id_pedido(Pedido *pedido,char id_pedido[]){
    strcpy((*pedido).id_pedido, id_pedido);
}


void set_vendedor(Pedido *pedido,int vendedor){
    (*pedido).vendedor = vendedor;
}

void set_combo_pedido(Pedido *pedido, int opcion, int unidades){
        (*pedido).combo_pedido[opcion] = unidades;
}

void set_forma_de_pago(Pedido *pedido,int forma_de_pago){
    (*pedido).forma_de_pago = forma_de_pago;
}

void set_subtotal(Pedido *pedido,float subtotal){
    (*pedido).subtotal = subtotal;
}

void set_consume_en_local(Pedido *pedido,int consume_en_local){
    (*pedido).consume_en_local = consume_en_local;
}

void set_total(Pedido *pedido,float total){
    (*pedido).total = total;
}

void set_fecha_dia(Pedido *pedido, int dia){
    (*pedido).fecha_de_compra.dia = dia;
}


void set_entregado(Pedido *pedido,int entregado){
    (*pedido).entregado = entregado;
}

void set_fecha_actual(Pedido *pedido) {
    time_t tiempoActual;
    struct tm *infoTiempo;
    time(&tiempoActual);
    infoTiempo = localtime(&tiempoActual);
    (*pedido).fecha_de_compra.dia = infoTiempo->tm_mday;
    (*pedido).fecha_de_compra.mes = infoTiempo->tm_mon + 1;
    (*pedido).fecha_de_compra.anio = infoTiempo->tm_year + 1900;
}

void set_dia(Pedido *pedido, int dia){
    (*pedido).fecha_de_compra.dia = dia;
}

void set_mes(Pedido *pedido, int mes){
    (*pedido).fecha_de_compra.mes = mes;
}

void set_anio(Pedido *pedido, int anio){
    (*pedido).fecha_de_compra.anio = anio;
}

//GETTERS

char* get_nombre(Pedido pedido){
    char *nombre;
    nombre = (char*)malloc(strlen(pedido.nombre)+1);
    if(nombre!=NULL){
        strcpy(nombre,pedido.nombre);
    }
    return nombre;
}

int get_id_pedido(Pedido pedido){
    return pedido.id_pedido;
}

int get_vendedor(Pedido pedido){
    return pedido.vendedor;
}

int get_combo_pedido(Pedido pedido, int i) {
    return pedido.combo_pedido[i];
}

int get_forma_de_pago(Pedido pedido){
    return pedido.forma_de_pago;
}

float get_subtotal(Pedido pedido){
    return pedido.subtotal;
}

int get_consume_en_local(Pedido pedido){
    return pedido.consume_en_local;
}

float get_total(Pedido pedido){
    return pedido.total;
}

int get_fecha_de_compra_dia(Pedido pedido){
    return pedido.fecha_de_compra.dia;
}

int get_fecha_de_compra_mes(Pedido pedido){
    return pedido.fecha_de_compra.mes;
}

int get_fecha_de_compra_anio(Pedido pedido){
    return pedido.fecha_de_compra.anio;
}

int get_entregado(Pedido pedido){
    return pedido.entregado;
}

#endif // PEDIDO_H_INCLUDED
