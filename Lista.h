#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "Pedido.h"

typedef struct {
    Pedido vipd;
    struct Nodo *ps;}Nodo;

typedef struct{
    Nodo *acceso;
    Nodo *cursor;
    Nodo *cursor_aux;
}Lista;

void init_lista(Lista *l){
    (*l).acceso=NULL;
    (*l).cursor=NULL;
    (*l).cursor_aux=NULL;}

void reset_lista(Lista *l){
    (*l).cursor = (*l).acceso;
    (*l).cursor_aux = (*l).acceso;
}

void forward_lista(Lista *l){
    if((*l).cursor == (*l).cursor_aux){
        (*l).cursor = (*l).cursor->ps;}
    else{
        (*l).cursor_aux = (*l).cursor;
        (*l).cursor = (*l).cursor->ps;
    }
}

int isOss_lista(Lista l){
    if(l.cursor == NULL)
        return 1;
    else
        return 0;
}

int isFull_lista(Lista l){
    Nodo *temp = (Nodo*)malloc(sizeof(Nodo));
    if (temp == NULL){
        free(temp);
        return 1;}
    else{
        free(temp);
        return 0;}}

int isEmpty_lista(Lista l){
    if(l.acceso == NULL)
        return 1;
    else
        return 0;}

void insert_lista(Lista *l,Pedido pedido){
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    (*nuevo).vipd = pedido;
    (*nuevo).ps = NULL;
        if((*l).acceso == (*l).cursor){
            (*l).acceso = nuevo;
            (*nuevo).ps = (*l).cursor;
            (*l).cursor = (*l).acceso;
            (*l).cursor_aux = (*l).cursor;}
        else{
            (*l).cursor_aux->ps = nuevo;
            (*nuevo).ps = (*l).cursor;
            (*l).cursor = nuevo;
            }
}

void  supress_lista(Lista *l){
    if((*l).cursor == (*l).cursor_aux){
            (*l).acceso = (*l).cursor->ps;
            (*l).cursor_aux = (*l).cursor_aux->ps;
            free((void*)(*l).cursor);
            (*l).cursor = (*l).cursor_aux;}
        else{
            (*l).cursor_aux->ps = (*l).cursor->ps;
            free((void*)(*l).cursor);
            (*l).cursor = (*l).cursor_aux->ps;
            }
}

Pedido copy_lista(Lista l){
    return (l.cursor)->vipd;
}

#endif // LISTA_H_INCLUDED
