#ifndef COMBO_H_INCLUDED
#define COMBO_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#define MAX 10
#include <malloc.h>
#include <string.h>

typedef struct {
    int id_combo;
    char descripcion[100];
    int stock;
    float precio_unitario;
    int descuento;
} Combo;

void set_IdCombo(Combo* combo, int id) {
    (*combo).id_combo = id;}

void set_Descripcion(Combo* combo, char* descripcion) {
    strcpy((*combo).descripcion, descripcion);}

void set_Stock(Combo *combo, int stock) {
    (*combo).stock = stock;}

void set_PrecioUnitario(Combo* combo, float precio_unitario) {
    (*combo).precio_unitario = precio_unitario;}

void set_Descuento(Combo* combo, int calculo) {
    (*combo).descuento = calculo;
}

void aplicar_descuento(Combo* combo, int calculo) {
    (*combo).descuento = (*combo).descuento + calculo * 0.85;
}

int get_IdCombo(Combo combo) {
    return combo.id_combo;}

char* get_Descripcion(Combo combo) {
    char *aux;
    aux = (char*)malloc(strlen(combo.descripcion) + 1);
    strcpy(aux, combo.descripcion);
    return aux;}

int get_Stock(Combo combo) {
    return combo.stock;}

float get_PrecioUnitario(Combo combo) {
    return combo.precio_unitario;}

int get_descuento(Combo combo){
    return combo.descuento;
}

#endif // COMBO_H_INCLUDED
