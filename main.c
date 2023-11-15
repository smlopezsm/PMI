#include "Lista.h"

///Funciones del id_pedido para generar codigo random.
char randomLetter() {
    return 'a' + rand() % 26;
}
char randomDigit() {
    return '0' + rand() % 10;
}
void generateRandomCode(char *code) {
    int i;
    for (i = 0; i < 3; i++) {
        code[i] = randomLetter();
    }
    for (i = 3; i < 6; i++) {
        code[i] = randomDigit();
    }
    code[6] = '\0';
}

int vendedor = 1;
int contador12 = 0;
int cant_pedidos = 0;

///Inicializa todos los combos desde el archivo. Funcion (ñ).
void inic_combo(Combo opciones_combo[]){
    int i;
    FILE *all_combos;
    all_combos = fopen("menu.txt", "r");
    if (all_combos == NULL) {
        printf("Error al abrir el archivo\n");
        return 1;}
    for (i = 0; i < 10; i++) {
        fscanf(all_combos, "%d", &opciones_combo[i].id_combo);
        fscanf(all_combos, "%*[\n]");
        fgets(opciones_combo[i].descripcion, sizeof(opciones_combo[i].descripcion), all_combos);
        opciones_combo[i].descripcion[strcspn(opciones_combo[i].descripcion, "\n")] = '\0';
        fscanf(all_combos, "%d", &opciones_combo[i].stock);
        fscanf(all_combos, "%f", &opciones_combo[i].precio_unitario);
        fscanf(all_combos, "%d", &opciones_combo[i].descuento);
        fscanf(all_combos, "%*[\n]");
    }
    fclose(all_combos);}


///Carga el pedido. Función (a). Menu <1>.
void carga_pedido(Combo opciones_combo[], Lista *l, char code[]){
    int i, opcion, unidades, cupon, mas = 1, stock_aux, unidades_combo;
    float calculo=0;
    char nombre[30];
    Pedido pedido;
    init_pedido(&pedido, i);

    printf("\nCodigo de pedido: %s\n", code);
    set_id_pedido(&pedido, code);
    set_vendedor(&pedido, vendedor);
    printf("\nFecha: %d/%d/%d\n", get_fecha_de_compra_dia(pedido), get_fecha_de_compra_mes(pedido), get_fecha_de_compra_anio(pedido));
    printf("\nEl vendedor que va a tomar su pedido es el: %d\n", vendedor);
    printf("\nIngrese su nombre: ");
    fflush(stdin);
    gets(nombre);
    set_nombre(&pedido, nombre);
    do{
    printf("\nOPCIONES DE COMBOS\n\n");
    for (i=0;i<10;i++){
        printf("%d - %s\n", i, get_Descripcion(opciones_combo[i]));
    }

    do{
    do{
        printf("\nIngrese que opcion del menu desea: ");
        fflush(stdin);
        while(scanf("%d", &opcion)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(opcion < 0 || opcion > 9)
            printf("\nPor favor, ingrese una opcion entre 0 y 9.\n");
    }while(opcion < 0 || opcion > 9);
        if(get_Stock(opciones_combo[opcion]) == 0){
            printf("\nNo hay stock del combo que desea, por favor elija otro.\n");
        }
    }while(get_Stock(opciones_combo[opcion]) == 0);

    do{
        printf("\nIngrese cuantas unidades del combo desea: ");
        fflush(stdin);
        while(scanf("%d", &unidades)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(unidades > get_Stock(opciones_combo[opcion])){
            printf("\nNo hay suficiente stock del combo solicitado. Por favor, ingrese %d o menos unidades.\n", get_Stock(opciones_combo[opcion]));
        }
        if(unidades <= 0){
            printf("\nPor favor, ingrese una o mas unidades.\n");
        }
    }while(unidades > get_Stock(opciones_combo[opcion]) || unidades <= 0);

    unidades_combo = unidades + get_combo_pedido(pedido, opcion);
    set_combo_pedido(&pedido, opcion, unidades_combo);
    set_Stock(&opciones_combo[opcion], get_Stock(opciones_combo[opcion])-unidades);

    do{
        printf("\nDesea pedir algo mas? (1- Si 0- No):  ");
        fflush(stdin);
        while(scanf("%d", &mas)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(mas < 0 || mas > 1){
            printf("\nPor favor, ingrese 1 o 0\n");
        }
    }while(mas < 0 || mas > 1);


    }while(mas==1);

    do{
        printf("\nTiene cupon de descuento? (1- Si 0- No): ");
        fflush(stdin);
        while(scanf(" %d", &cupon)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(cupon < 0 || cupon > 1){
            printf("\nPor favor, ingrese 1 o 0\n");
        }
    }while(cupon < 0 || cupon > 1);


    for(i=0;i<10;i++){
        calculo += (get_combo_pedido(pedido, i) * get_PrecioUnitario(opciones_combo[i]));
    }
    set_subtotal(&pedido, calculo);
    calculo = 0;
    if(cupon == 1){
    for(i=0;i<10;i++){
    if(get_descuento(opciones_combo[i])==1){
        calculo += (get_combo_pedido(pedido, i) * get_PrecioUnitario(opciones_combo[i])) * 0.15;
    }}}
    set_subtotal(&pedido, get_subtotal(pedido)-calculo);

    do{
    printf("\nConsume en el local? (1- Si 0- No): ");
    fflush(stdin);
    while(scanf("%d", &opcion)!= 1){
        fflush(stdin);
        printf("\nError: Debe ingresar un numero entero");
        printf("\nIntentelo nuevamente: ");
    }
        if(opcion < 0 || opcion > 1){
            printf("\nPor favor, ingrese 1 o 0\n");
        }
    }while(opcion < 0 || opcion > 1);

    set_consume_en_local(&pedido, opcion);
    if(get_consume_en_local(pedido)==0)
        set_total(&pedido, get_subtotal(pedido)+500);
    else
        set_total(&pedido, get_subtotal(pedido));

    do{
        printf("\nIngrese forma de pago (1- Debito 2- Credito 3- QR 4- Efectivo): ");
        fflush(stdin);
        while(scanf("%d", &opcion)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(opcion < 1 || opcion > 4){
            printf("\nPor favor, ingrese un numero entre 1 y 4.\n");
        }
    }while(opcion < 1 || opcion > 4);

    set_forma_de_pago(&pedido, opcion);
    printf("Subtotal: $%.2f\n", get_subtotal(pedido));
    if(get_consume_en_local(pedido)==0)
        printf("Costo de delivery: $500\n");
        else
        printf("Costo de delivery: $0\n");
    printf("Total: $%.2f\n\n\n\n", get_total(pedido));
    insert_lista(l, pedido);
}


///Busca un pedido por id. Solo busca. Función (b). Menu <2>.
int buscar_un_pedido_por_id(Lista *l,char id_pedido_pasado[]){
    Pedido var_aux;
    reset_lista(l);
    int x = 0;
    while(!isOss_lista(*l)){
        var_aux=copy_lista(*l);
        forward_lista(l);
        if(strcmp(get_id_pedido(var_aux),id_pedido_pasado)==0)
            x=1;}
        if(x==1){
            return 1;}
        else{
                return 0;}
}


///Busca un pedido por id. Solo muestra. Función (c). Menu <2>.
void mostrar_un_pedido_por_id(Lista *l, char id_pedido_pasado[]){
    int i, x=0;
    Pedido var_aux;
    reset_lista(l);
        while(!isOss_lista(*l)){
            var_aux = copy_lista(*l);
            if(strcmp(get_id_pedido(var_aux),id_pedido_pasado)==0){
                x=1;
                printf("\nNombre del cliente: %s\n",get_nombre(var_aux));
                printf("ID del pedido: %s \n", get_id_pedido(var_aux));
                if(get_forma_de_pago(var_aux)==1){
                    printf("Forma de pago: Debito.\n");}
                else if(get_forma_de_pago(var_aux)==2){
                    printf("Forma de pago: Credito.\n");}
                else if(get_forma_de_pago(var_aux)==3){
                    printf("Forma de pago: QR.\n");}
                else if(get_forma_de_pago(var_aux)==4){
                    printf("Forma de pago: Efectivo.\n");}
                for(i=0; i<10; i++){
                    if(get_combo_pedido(var_aux,i)!=0){
                        printf("Combo %d: %d unidades.\n", i ,get_combo_pedido(var_aux,i));}}
                    if(get_consume_en_local(var_aux)==1){
                        printf("Consume en el local.\n");}
                    else{
                    printf("Pedido enviado por delivery\n");}
                    printf("El sub total es: $%0.2f\n",get_subtotal(var_aux));
                    printf("El total es:$%0.2f \n",get_total(var_aux));
                    printf("Fecha %i/%i/%i. \n", get_fecha_de_compra_dia(var_aux), get_fecha_de_compra_mes(var_aux), get_fecha_de_compra_anio(var_aux));
                    if(get_entregado(var_aux)){
                        printf("Estado del pedido: Entregado.\n\n");}
                    else{
                        printf("Estado del pedido: No entregado.\n\n");
                    }
                    }
                    forward_lista(l);
                    }
            if(x==0){
                printf("El pedido no esta en la lista.\n\n");
            }
        }

///Mostrar los pedidos de un mes ordenados por fecha. Función (d). Menu <3>.
void mostrar_pedidos_mes(Lista l,int mes_pasado,int anio_pasado){
    int arr[32], i;
    printf("\n");
    for (i=1;i<32;i++){
        reset_lista(&l);
        while(!isOss_lista(l)){
            if (get_fecha_de_compra_mes(copy_lista(l)) == mes_pasado && get_fecha_de_compra_anio(copy_lista(l)) == anio_pasado && get_fecha_de_compra_dia(copy_lista(l)) == i){
                printf("%d/%d/%d\n", get_fecha_de_compra_dia(copy_lista(l)), get_fecha_de_compra_mes(copy_lista(l)), get_fecha_de_compra_anio(copy_lista(l)));
                printf("Nombre del cliente: %s",get_nombre(copy_lista(l)));
                printf("\nID del pedido: %s",get_id_pedido(copy_lista(l)));
                printf("\nForma de pago: ");
            switch(get_forma_de_pago(copy_lista(l))){
            case 1:
                printf("Debito\n");
                break;
            case 2:
                printf("Credito\n");
                break;
            case 3:
                printf("QR\n");
                break;
            case 4:
                printf("Efectivo\n");
                break;
            }
            if(get_consume_en_local(copy_lista(l))==1){
                printf("Consume en el local\n");
            }
            else{
                printf("No consume en el local\n");
            }
        printf("Subtotal pagado: %.2f\n",get_subtotal(copy_lista(l)));
        printf("Total pagado: %.2f\n",get_total(copy_lista(l)));
        if(get_entregado(copy_lista(l)) == 1){
            printf("Estado del pedido: Entregado.\n\n");}
        else{
             printf("Estado del pedido: No entregado.\n\n");
        }
            }
        forward_lista(&l);
        }
    }
}

///Mostrar los pedidos por nombre, paginar la muestra de los datos. Función (e). Menu <4>.
void mostrar_aux(Lista l){
        printf("Nombre del cliente: %s",get_nombre(copy_lista(l)));
        printf("\nID del pedido: %s",get_id_pedido(copy_lista(l)));
        printf("\nFecha de compra: %d/%d/%d\n",get_fecha_de_compra_dia(copy_lista(l)),get_fecha_de_compra_mes(copy_lista(l)),get_fecha_de_compra_anio(copy_lista(l)));
        printf("Forma de pago: ");
            switch(get_forma_de_pago(copy_lista(l))){
            case 1:
                printf("Debito\n");
                break;
            case 2:
                printf("Credito\n");
                break;
            case 3:
                printf("QR\n");
                break;
            case 4:
                printf("Efectivo\n");
                break;
            }
            if(get_consume_en_local(copy_lista(l))==1){
                printf("Consume en el local\n");
            }
            else{
                printf("No consume en el local\n");
            }
        printf("Subtotal pagado: %.2f\n",get_subtotal(copy_lista(l)));
        printf("Total pagado: %.2f\n",get_total(copy_lista(l)));
        if(get_entregado(copy_lista(l)) == 1){
            printf("Estado del pedido: Entregado.\n\n");}
        else{
             printf("Estado del pedido: No entregado.\n\n");
        }

}
void mostrar_pedidos_nombre(Lista l,char nom_pasado[]){
    Pedido aux;
    int mostrar_mas=0,se_muestra=0;
    char letra;
    system("cls");
    reset_lista(&l);
    while(!isOss_lista(l)){
        aux = copy_lista(l);
        if(strcmp(get_nombre(aux),nom_pasado)==0){
            mostrar_aux(l);
            se_muestra++;
            mostrar_mas++;
       }
        forward_lista(&l);
        if(mostrar_mas==3){
            do{
                printf("Presione (M) si desea ver mas pedidos. Presione (S) si no lo desea\n");
                scanf(" %c",&letra);
                if(letra == 's' || letra == 'S'){
                        system("cls");
                    return 0;
                }

                if(letra!= 'm' && letra!= 'M'){
                    printf("\nEl valor ingresado no esta en los parametros.\n");
                }

            }while(letra!= 'm' && letra!= 'M');
            mostrar_mas=0;
            system("cls");
        }

    }

    if(se_muestra==0){
            printf("\nNo se encontro ningun pedido con el nombre pasado\n");
        }
       else  {
            printf("Se mostraron todos los pedidos exitosamente\n\n");
       }

    do{
        printf("Presione (V) para volver al menu\n");
        scanf(" %c", &letra);
        if(letra!= 'v' && letra!= 'V'){
            printf("\nEl valor ingresado no esta en los parametros.\n");
        }
    }while(letra!= 'v' && letra!= 'V' );

    system("cls");
}

///Mostrar los pedidos realizados por un vendedor solo mostrar total e idpedido. Función (f). Menu <5>.
int recursiva(int vendedor_pasado,Lista l){
    if(isOss_lista(l)){
        return 0;
    }
    else if(get_vendedor(copy_lista(l))==vendedor_pasado){
        printf("ID Pedido: %s:\n",get_id_pedido(copy_lista(l)));
        printf("Total facturado: %.2f\n",get_total(copy_lista(l)));
        forward_lista(&l);
        return recursiva(vendedor_pasado, l);
    }
    else{
        forward_lista(&l);
        return recursiva(vendedor_pasado, l);
    }
}

///Contar los pedidos realizados por un vendedor, resetear lista antes de entrar en la funcion. Función (g). Menu <6>.
int cuenta_pedidos(Lista li,int ven){
    if(isOss_lista(li)){
        return 0;
    }else{
        if(get_vendedor(copy_lista(li))==ven){
            forward_lista(&li);
            return 1 + cuenta_pedidos(li,ven);
        }else{
            forward_lista(&li);
            return 0 + cuenta_pedidos(li,ven);
        }
    }
}

///Modificar el estado del pedido a entregado por idpedido. Función (i). Menu <7>.
void cambia_entrega(Lista *l, char id[]){
    int x=0;
    Pedido var_aux;
    reset_lista(l);
    while(!isOss_lista(*l)){
        var_aux = copy_lista(*l);
        if(strcmp(get_id_pedido(var_aux), id) == 0){
            set_entregado(&var_aux, 1);
            printf("\nEl pedido ha sido entregado.\n\n\n");
            supress_lista(l);
            insert_lista(l, var_aux);
            x++;
            forward_lista(l);
        }
        else{
            forward_lista(l);
        }
    }
        if(x==0){
            printf("\nEl codigo ingresado no pertenece a ningun pedido.\n\n\n");}}


///Modificar la forma de pago de un pedido según su idpedido. Función (j). Menu <8>.
void cambia_pago(Lista *l){
    char id [7];
    int x=0, opcion;
    Pedido var_aux;
    do{
    printf("Ingrese el ID del pedido que quiere modificar la forma de pago: ");
    fflush(stdin);
    gets(id);
    if(strlen(id) != 6){
        printf("\nCodigo invalido. Recuerde que el id consta de 3 letras seguidas de 3 numeros.\n");}
    }while(strlen(id) != 6);
    reset_lista(l);
    while(!isOss_lista(*l)){
        var_aux = copy_lista(*l);
        if(strcmp(get_id_pedido(var_aux), id) == 0){
            printf("\nLas formas de pago son: 1- Debito 2- Credito 3- QR 4- Efectivo\n");
            printf("Usted habia seleccionado la opcion %d\n", get_forma_de_pago(var_aux));
            do{
                printf("Seleccione la nueva forma de pago: ");
                fflush(stdin);
                while(scanf("%d", &opcion)!= 1){
                    fflush(stdin);
                    printf("\nError: Debe ingresar un numero entero");
                    printf("\nIntentelo nuevamente: ");
                }
                if(opcion < 1 || opcion > 4){
                    printf("\nPor favor, ingrese un numero entre 1 y 4.\n");
                }
            }while(opcion<1 || opcion>4);
            set_forma_de_pago(&var_aux, opcion);
            if(get_forma_de_pago(var_aux) == opcion)
                printf("\nEl cambio ha sido guardado exitosamente.\n");
            supress_lista(l);
            insert_lista(l, var_aux);
            x++;
            forward_lista(l);
        }
        else{
            forward_lista(l);
        }}
    if(x==0)
        printf("\nEl codigo ingresado no pertenece a ningun pedido.\n");
}


///Modificar nombre de un pedido según su idpedido. Función (k). Menu <9>.
void cambia_nombre(Lista *l, char id[]){
    char nombre[30];
    int x=0, opcion;
    Pedido var_aux;
    reset_lista(l);
    while(!isOss_lista(*l)){
        var_aux = copy_lista(*l);
        if(strcmp(get_id_pedido(var_aux), id) == 0){
            printf("\nEl nombre actual del cliente es: %s\n", get_nombre(var_aux));
            printf("Ingrese el nuevo nombre: ");
            fflush(stdin);
            gets(nombre);
            set_nombre(&var_aux, nombre);
            if(strcmp(get_nombre(var_aux), nombre)==0)
                printf("\nEl cambio ha sido guardado exitosamente.\n\n\n");
            supress_lista(l);
            insert_lista(l, var_aux);
            x++;
            forward_lista(l);
        }
        else{
            forward_lista(l);
        }}
        if(x==0)
            printf("\nEl codigo ingresado no pertenece a ningun pedido.\n\n\n");
}


///Anular pedido por id, almacenar los datos del pedido borrado en un archivo. Función (l). Menu <10>.
int anular_pedido(Lista *l, Combo opciones_combo[]){
    char id [7], guiones[50]="---------------------------------------------", id_aux[7];
    int x, i, id_encontrado;
    Pedido var_aux;
    FILE *fp, *fp2, *fp3;
    fp = fopen("PedidosBorrados.txt", "a+");
    fp3 = fopen("IDBorrados.txt","r");
    fp2 = fopen("IDBorrados.txt","a");
    printf("Ingrese el ID del pedido que quiere anular: ");
    do{
    fflush(stdin);
    gets(id);
    if(strlen(id) != 6){
    printf("\nCodigo invalido. Recuerde que el id consta de 3 letras seguidas de 3 numeros.\n");}
    }while(strlen(id) != 6);
    reset_lista(l);
    while(!isOss_lista(*l)){
        var_aux = copy_lista(*l);
        if(strcmp(get_id_pedido(var_aux), id) == 0){
            if(get_entregado(var_aux) == 1){
                    printf("\nEl pedido ha sido entregado, por lo tanto no puede ser anulado.\n");
                    fclose(fp);
                    fclose(fp2);
                    fclose(fp3);
                    return 0;
                }
            else{
            fseek(fp3,0,SEEK_SET);
            id_encontrado=1;
            while(fscanf(fp3,"%s\n", id_aux)==1){
                if(!strcmp(id_aux,get_id_pedido(var_aux))){
                    id_encontrado=0;
                }
            }
            if(id_encontrado == 0){
                    printf("\nEl pedido borrado ya ha sido cargado anteriormente en el archivo de pedidos borrados, aun asi ha sido borrado.\n\n");
                    supress_lista(l);
            }
            if(id_encontrado){
            fprintf(fp, "%s\n", get_nombre(var_aux));
            fprintf(fp, "%s\n", get_id_pedido(var_aux));
            fprintf(fp2, "%s\n", get_id_pedido(var_aux));
            fprintf(fp, "%d\n", get_vendedor(var_aux));
            for (i=0;i<10;i++){
                fprintf(fp, "%d\n", get_combo_pedido(var_aux, i));
                set_Stock(&opciones_combo[i], get_Stock(opciones_combo[i]) + get_combo_pedido(var_aux, i));
            }
            fprintf(fp, "%d\n", get_forma_de_pago(var_aux));
            fprintf(fp, "%f\n", get_subtotal(var_aux));
            fprintf(fp, "%d\n", get_consume_en_local(var_aux));
            fprintf(fp, "%f\n", get_total(var_aux));
            fprintf(fp, "%d\n", get_fecha_de_compra_dia(var_aux));
            fprintf(fp, "%d\n", get_fecha_de_compra_mes(var_aux));
            fprintf(fp, "%d\n", get_fecha_de_compra_anio(var_aux));
            fprintf(fp, "%d\n", get_entregado(var_aux));
            fprintf(fp, "%s\n", guiones);
            supress_lista(l);
            x++;
            printf("\n\nSe ha anulado correctamente el pedido %s.\n\n", get_id_pedido(var_aux));
            fclose(fp);
            fclose(fp2);
            fclose(fp3);
            return 0;}
        }}

        forward_lista(l);
}
    if(x==0){
        printf("\nEl codigo ingresado no pertenece a ningun pedido.\n");
        fclose(fp);
        fclose(fp2);
        fclose(fp3);
    }
}

///Almacenar los pedidos de cierta forma de pago pasada por parametro en un archivo. Función (ll). Menu <11>.
void forma_de_pago_archivo(Lista *l, int pago){
    FILE *fp,*fp2,*fp3;
    Pedido var_aux;
    char id_aux[7];
    int i, x=0,id_encontrado,Archivo_vacio=1;
    char guiones[50]="-------------------------------------------------";
    fp3 = fopen("IDFormasDePago.txt","r");
    fp2 = fopen("IDFormasDePago.txt","a");
    fp = fopen("FormasDePago.txt","a");
    reset_lista(l);
    while(!isOss_lista(*l)){
        var_aux = copy_lista(*l);
        if(get_forma_de_pago(var_aux)==pago){
            fseek(fp3,0,SEEK_SET);
            id_encontrado=1;
            while(fscanf(fp3,"%s\n", id_aux)==1){
                if(!strcmp(id_aux,get_id_pedido(var_aux))){
                    id_encontrado=0;
                }
            }
                    if(id_encontrado){
                        fprintf(fp, "%s\n", get_nombre(var_aux));
                        fprintf(fp, "%s\n", get_id_pedido(var_aux));
                        fprintf(fp2, "%s\n", get_id_pedido(var_aux));
                        fprintf(fp, "%d\n", get_vendedor(var_aux));
                        for (i=0;i<10;i++){
                            fprintf(fp, "%d\n", get_combo_pedido(var_aux, i));
                        }
                        fprintf(fp, "%d\n", get_forma_de_pago(var_aux));
                        fprintf(fp, "%f\n", get_subtotal(var_aux));
                        fprintf(fp, "%d\n", get_consume_en_local(var_aux));
                        fprintf(fp, "%f\n", get_total(var_aux));
                        fprintf(fp, "%d\n", get_fecha_de_compra_dia(var_aux));
                        fprintf(fp, "%d\n", get_fecha_de_compra_mes(var_aux));
                        fprintf(fp, "%d\n", get_fecha_de_compra_anio(var_aux));
                        fprintf(fp, "%d\n", get_entregado(var_aux));
                        fprintf(fp, "%s\n", guiones);
                        x++;
                    }
            }
        forward_lista(l);
    }
    printf("\nSe cargaron %d pedidos en el archivo.\n\n", x);
    fclose(fp);
    fclose(fp2);
    fclose(fp3);
}

///Precarga del historial de pedidos. Función (m). Menu <12>.
void pedidos_anteriores(Lista *l){
    FILE *fp;
    Pedido aux;
    int i, x, p_vendedor, p_combo_pedido[10], p_forma_de_pago, p_consume_en_el_local, p_entregado, p_dia, p_mes, p_anio;
    char p_nombre[30], p_id[7], guiones[50];
    float p_subtotal, p_total;
    fp = fopen("CargaPedido.txt", "r");
    for (i=0;i<10;i++){
        reset_lista(l);

        fscanf(fp,"%[^\n]%*c",p_nombre);
        set_nombre(&aux, p_nombre);

        fgets(p_id, 7, fp);
        set_id_pedido(&aux, p_id);

        fscanf(fp, "%d\n", &p_vendedor);
        set_vendedor(&aux, p_vendedor);

        for(x=0;x<10;x++){
            fscanf(fp, "%d\n", &p_combo_pedido[x]);
            set_combo_pedido(&aux, x, p_combo_pedido[x]);
        }

        fscanf(fp, "%d\n", &p_forma_de_pago);
        set_forma_de_pago(&aux, p_forma_de_pago);

        fscanf(fp, "%f\n", &p_subtotal);
        set_subtotal(&aux, p_subtotal);

        fscanf(fp, "%d\n", &p_consume_en_el_local);
        set_consume_en_local(&aux, p_consume_en_el_local);

        fscanf(fp, "%f\n", &p_total);
        set_total(&aux, p_total);

        fscanf(fp, "%d\n", &p_dia);
        set_dia(&aux, p_dia);

        fscanf(fp, "%d\n", &p_mes);
        set_mes(&aux, p_mes);

        fscanf(fp, "%d\n", &p_anio);
        set_anio(&aux, p_anio);

        fscanf(fp, "%d\n", &p_entregado);
        set_entregado(&aux, p_entregado);

        fgets(guiones, 50, fp);

        insert_lista(l, aux);

    }
    contador12 = 1;
    printf("\nSe han cargado los pedidos antiguos de manera exitosa.\n\n");
    fclose(fp);
}

///Mostrar todos los pedidos no entregados. Función (n). Menu <13>.
void mostrar_no_entregados(Lista *l){
    reset_lista(l);
    int x=0;
    printf("\nPedido/s no entregado/s:\n\n", get_id_pedido(copy_lista(*l)));
    while(!isOss_lista(*l)){
        if(get_entregado(copy_lista(*l)) == 0){
            mostrar_aux(*l);
            x++;
        }
        forward_lista(l);
        }
}

/// Modificar precio y stock del combo según idcombo. Función (o). Menu <14>.
void modificar_precio_y_stock(Combo opciones_combo[],int id){
    int i,indice,stock;
    float precio;
    indice=-1;
    for(i=0;i<10;i++){
        if(get_IdCombo(opciones_combo[i])==id){
            indice=i;
            i=10;
        }
    }
    if(indice != -1){
        do{
        printf("Ingrese el precio nuevo: ");
        fflush(stdin);
        while(scanf(" %f", &precio)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(precio<0){
            printf("\nError: el precio no puede ser un valor negativo\n");
        }
        }while(precio<0);

        do{
        printf("Ingrese el stock nuevo: ");
        fflush(stdin);
        while(scanf(" %d", &stock)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(stock<0){
            printf("\nError: el stock no puede ser un valor negativo\n");
        }
        }while(stock<0);

        set_PrecioUnitario(&opciones_combo[indice],precio);
        set_Stock(&opciones_combo[indice],stock);
    }else{
        printf("\nError: no se ha encontrado ningun combo con su id\n");
    }
    printf("\n\nEl precio y stock se han modificado exitosamente :)\n\n");
}

///Mostrar todos los combos. Función (p). Menu <15>.
void mostrar_todos_los_combos(Combo opciones_combo[]){
    int i;
    printf("\nOPCIONES DE COMBOS\n\n");
    for (i=0;i<10;i++){
        printf("%d - %s\n", i, get_Descripcion(opciones_combo[i]));
        printf("Precio Unitario: $%.2f\n",get_PrecioUnitario(opciones_combo[i]));
        printf("Stock: %d\n\n\n",get_Stock(opciones_combo[i]));
    }
}

/// Mostrar precio y stock de un combo por idcombo. Función (q). Menu <16>.
void mostrar_precio_y_stock(Combo opciones_combo[],int id){
    int i,existe;
    existe=0;

    //Busca el combo con el id pasado por parametro
    for(i=0;i<10;i++){
        if(get_IdCombo(opciones_combo[i])==id){
            existe=1;
            i=10;
        }
    }

    //analiza si se a encontrado un combo con el id pasado por parametro
    if(existe){
        printf("El precio unitario del combo %d es: %.2f\n",id,get_PrecioUnitario(opciones_combo[id]));
        printf("El stock del combo %d es: %d\n",id,get_Stock(opciones_combo[id]));
    }else{
        printf("Error: no se ha encontrado ningún combo con su id\n\n");
    }
}

/// Mostrar los combos sin stock. Suponiendo que hay combos sin stock, aplicar control antes. Función (r). Menu <17>.
void mostrar_combos_sin_stock(Combo opciones_combo[]){
    int i;
    printf("\n\nCombos sin stock\n");
    for(i=0;i<10;i++){
        if(get_Stock(opciones_combo[i])==0){
            printf("id combo = %d\n",get_IdCombo(opciones_combo[i]));
            printf("Descripcion = %s\n\n\n",get_Descripcion(opciones_combo[i]));
        }
    }
}

/// Informar cual es el vendedor que realizo mas pedidos en el mes. Función (s). Menu <18>.
void informar_pedidos_del_mes(Lista l,int mes){
    int vendedores[4];
    int i, mayor_ventas,empleado;
    for(i=0;i<4;i++){
        vendedores[i]=0;
    }
    Pedido pe;
    reset_lista(&l);
    while(!isOss_lista(l)){
        pe=copy_lista(l);
        if(get_fecha_de_compra_mes(pe)==mes){
            switch(get_vendedor(pe)){
                case 1:
                    vendedores[0]++;
                    break;
                case 2:
                    vendedores[1]++;
                    break;
                case 3:
                    vendedores[2]++;
                    break;
                case 4:
                    vendedores[3]++;
                    break;
            }
        }
        forward_lista(&l);
    }
    mayor_ventas=vendedores[0];
    for(i=1;i<4;i++){
        if(mayor_ventas<vendedores[i]){
            mayor_ventas=vendedores[i];
        }
    }

    printf("\nVendedor/es con mas ventas en el mes %d: \n\n",mes);

    for(i=0;i<4;i++){
        if(mayor_ventas==vendedores[i]){
            printf("Vendedor %d: %d venta/s\n",i+1,mayor_ventas);
        }
    }

}

/// Permitir cambiar el vendedor, ingresando uno de los valores establecido. Función (t). Menu <19>.
void cambiar_vendedor(){
    do{
        printf("Ingresar el codigo del nuevo vendedor (de 1 a 4): ");
        fflush(stdin);
        while(scanf(" %d",&vendedor)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        if(vendedor < 1 || vendedor > 4){
            printf("\nError: el valor ingresado no se encuentra en los establecidos\n");
        }
    }while(vendedor < 1 || vendedor > 4);
    printf("\nEl vendedor ha sido cambiado exitosamente.\n\n");
}


int main() {
    int i, menu,cant=0,vendedor_pasado, ven, aux, pago;
    Combo opciones_combo[10];
    Combo combo;
    Lista l;
    Pedido pedido;
    Fecha fecha;
    inic_combo(opciones_combo);
    init_pedido(&pedido, i);
    init_lista(&l);
    char id_pedido_pasado[7], id[7];
    int mes_pasado;
    int anio_pasado;
    char nom_pasado[30];
    do{
    printf("--------------------------------------------- MENU ---------------------------------------------\n");
    printf("<1> Ingresar un pedido.\n");
    printf("<2> Buscar y Mostrar los datos de un pedido por el ID del pedido.\n");
    printf("<3> Mostrar los pedidos de un mes ordenados por fecha.\n");
    printf("<4> Mostrar los pedidos por nombre de cliente.\n");
    printf("<5> Mostrar los pedidos realizados por un vendedor.\n");
    printf("<6> Contar los pedidos realizados por un vendedor.\n");
    printf("<7> Modificar el estado del pedido a entregado.\n");
    printf("<8> Modificar la forma de pago de un pedido. \n");
    printf("<9> Modificar el nombre de cliente de un pedido.\n");
    printf("<10> Anular un pedido.\n");
    printf("<11> Almacenar los datos de los pedidos facturados con cierta forma de pago.\n");
    printf("<12> Historial de los pedidos.\n");
    printf("<13> Mostrar los pedidos no entregados.\n");
    printf("<14> Modificar precio y stock de los combos.\n");
    printf("<15> Mostrar todos los combos.\n");
    printf("<16> Mostrar precio y stock de un combo especifico.\n");
    printf("<17> Mostrar los combos sin stock.\n");
    printf("<18> Mostrar cual es el vendedor que realizo mas ventas en el mes.\n");
    printf("<19> Cambiar el vendedor.\n");
    printf("<20> Salir.\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("\nIngrese la opcion deseada: ");
    while(scanf("%d", &menu)!= 1){
        fflush(stdin);
        printf("\nError: Debe ingresar un numero entero");
        printf("\nIntentelo nuevamente: ");
    }

    if(menu < 1 || menu > 20)
        printf("\nOpcion invalida. Ingrese un numero entre 1 y 20.\n\n");
    switch(menu){
    case 1:
        srand(time(NULL));
        char code[7];
        generateRandomCode(code);
        carga_pedido(opciones_combo, &l, code);
        break;
    case 2:
         if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se pudo buscar ningun pedido aun.\n\n");
            else{
                do{
                printf("Ingrese el id que esta buscando: ");
                fflush(stdin);
                gets(id_pedido_pasado);
                if(strlen(id_pedido_pasado) != 6){
                    printf("\nCodigo invalido. Recuerde que el id consta de 3 letras seguidas de 3 numeros.\n");
                }
                }while(strlen(id_pedido_pasado) != 6);
                if(buscar_un_pedido_por_id(&l, id_pedido_pasado)==1){
                    mostrar_un_pedido_por_id(&l, id_pedido_pasado);}
                else{
                    printf("\nEl pedido no esta en la lista.\n\n");}
            }
        break;
    case 3:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        printf("\nIngrese el anio a buscar: ");
        fflush(stdin);
        while(scanf("%d", &anio_pasado)!= 1){
            fflush(stdin);
            printf("\nError: Debe ingresar un numero entero");
            printf("\nIntentelo nuevamente: ");
        }
        do{
            printf("\nIngrese el mes a buscar: ");
            fflush(stdin);
            while(scanf("%d", &mes_pasado)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(mes_pasado < 1 || mes_pasado > 12){
                printf("\nPor favor, ingrese entre 1 y 12.\n");
            }
        }while(mes_pasado < 1 || mes_pasado > 12);
        mostrar_pedidos_mes(l,mes_pasado,anio_pasado);
        }
        break;
    case 4:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        printf("\nIngrese el nombre que desea buscar: ");
        fflush(stdin);
        gets(nom_pasado);
        mostrar_pedidos_nombre(l,nom_pasado);
        }
        break;
    case 5:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        do{
            printf("\nIngrese el numero de vendedor que desea ver: ");
            fflush(stdin);
            while(scanf("%d",&vendedor_pasado)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(vendedor_pasado < 1 || vendedor_pasado > 4){
                printf("\nPor favor, ingrese un numero entre 1 y 4.\n");
            }
        }while(vendedor_pasado < 1 || vendedor_pasado > 4);
        reset_lista(&l);
        recursiva(vendedor_pasado, l);}
        break;
    case 6:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede contar ningun pedido aun.\n\n");
        else{
        do{
            printf("\nIngrese de que vendedor quiere ver sus pedidos realizados (1-4): ");
            fflush(stdin);
            while(scanf("%d", &ven)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(ven<1 || ven>4){
                printf("\nPor favor, ingrese un numero entre 1 y 4\n");
            }
        }while(ven<1 || ven>4);
        reset_lista(&l);
        printf("Los pedidos realizados por el vendedor %d son: %d\n\n",ven,cuenta_pedidos(l, ven));}
        break;
    case 7:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede modificar ningun pedido aun.\n\n");
        else{
        do{
        printf("Ingrese el ID del pedido que quiere entregar: ");
        fflush(stdin);
        gets(id);
        if(strlen(id) != 6){
        printf("\nCodigo invalido. Recuerde que el id consta de 3 letras seguidas de 3 numeros.\n");}
        }while(strlen(id) != 6);
        cambia_entrega(&l, id);}
        break;
    case 8:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede modificar ningún pedido aun.\n\n");
        else{
        cambia_pago(&l);}
        break;
    case 9:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        do{
        printf("Ingrese el ID del pedido que quiere modificar el nombre: ");
        fflush(stdin);
        gets(id);
        if(strlen(id) != 6){
        printf("\nCodigo invalido. Recuerde que el id consta de 3 letras seguidas de 3 numeros.\n");}
        }while(strlen(id) != 6);
        cambia_nombre(&l, id);}
        break;
    case 10:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede eliminar ningun pedido aun.\n\n");
        else{
        anular_pedido(&l, opciones_combo);}
        break;
    case 11:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        do{
            printf("Ingrese la forma de pago (1- Debito, 2- Credito, 3- QR, 4- Efectivo): ");
            fflush(stdin);
            while(scanf("%d", &pago)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(pago < 1 || pago > 4){
                printf("\nPor favor, ingrese un numero entre 1 y 4. \n");
            }
        }while(pago < 1 || pago > 4);
        forma_de_pago_archivo(&l, pago);}
        break;
    case 12:
        if(contador12 == 0){
            pedidos_anteriores(&l);
        }
        else{
            printf("\nLa carga de esos pedidos ya ha sido realizada anteriormente.\n\n");
        }
        break;
    case 13:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        reset_lista(&l);
        aux=0;
        while(!isOss_lista(l)){
            if(get_entregado(copy_lista(l))== 0){
                aux++;
            }
            forward_lista(&l);
        }
        if(aux>0){
            mostrar_no_entregados(&l);
        }}
        break;
    case 14:
        do{
            printf("Ingresar el id del combo que desea modificar (entre 0 y 9)\n");
            fflush(stdin);
            while(scanf(" %d", &aux)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(aux<0 || aux>9){
                printf("\nPor favor, ingrese un numero entre 0 y 9\n");
            }
        }while(aux<0 || aux>9);
        modificar_precio_y_stock(opciones_combo,aux);
        break;
    case 15:
        mostrar_todos_los_combos(opciones_combo);
        break;
    case 16:
        do{
            printf("Ingresar el id del combo que desea buscar (entre 0 y 9): ");
            fflush(stdin);
            while(scanf(" %d", &aux)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(aux<0 || aux>9){
                printf("\nPor favor, ingrese un numero entre 0 y 9\n");
            }
        }while(aux<0 || aux>9);
        mostrar_precio_y_stock(opciones_combo, aux);
        break;
    case 17:
        aux=0;
        for(i=0;i<10;i++){
            if(get_Stock(opciones_combo[i])==0){
                mostrar_combos_sin_stock(opciones_combo);
                aux=1;
                i=10;
            }
        }
        if(aux==0){
            printf("\nNo se encuentran combos sin stock\n\n");
        }
        break;
    case 18:
        if(isEmpty_lista(l)==1)
                printf("\nLa lista esta vacia, por lo tanto no se puede mostrar ningun pedido aun.\n\n");
        else{
        printf("\nIngresar el mes (numero entero) en el que desee ver que vendedor realizo mas ventas: ");
        do{
            fflush(stdin);
            while(scanf(" %d", &aux)!= 1){
                fflush(stdin);
                printf("\nError: Debe ingresar un numero entero");
                printf("\nIntentelo nuevamente: ");
            }
            if(aux<1 || aux>12){
                printf("\nPor favor, ingrese un numero entre 1 y 12\n");
            }
        }while(aux<1 || aux>12);
        ven=0;
        reset_lista(&l);
        while(!isOss_lista(l)){
            if(get_fecha_de_compra_mes(copy_lista(l))==aux){
                ven++;
            }
            forward_lista(&l);
        }
        if(ven==0){
            printf("\nNo hay pedidos en ese mes\n");
        }else{
            informar_pedidos_del_mes(l,aux);
        }}
        break;
    case 19:
        cambiar_vendedor();
        break;
    case 20:
        printf("\nSe finalizo la ejecucion del programa.\n\n");
        exit(1);
        break;
}}while(menu!=20);
    return 0;
}
