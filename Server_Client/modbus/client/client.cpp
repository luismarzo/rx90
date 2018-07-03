#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ClientModBUS.h"
#include "constant.h"
#include <unistd.h>
#define NUM_REGISTERS 0x32
#define COMMAND_ADDR 3
#define ADDR_INI 0
int uintToInt(uint16_t value);
void char2int(char newPosition[29], uint16_t position[6]);
void imprimePosicion(uint16_t position[6]);

int main()
{

    char direccion[16] = "127.0.0.1"; // 192.168.109.148
    //char direccion[16]="192.168.109.148";
    //char direccion[16]="10.100.182.173";
    int puerto = 1502;
    char newPosition[29] = "0,0,10,-90,-90,0"; // 0,-150,10,-90,-90,0
    int numReg;

    char option = '0';
    uint16_t value;
    uint16_t tab[NUM_REGISTERS];
    uint16_t position[6];

    printf("//////////////////////////////////////////////////\n");
    printf("\n");
    printf("               CONTROL REMOTO RX 90               \n");
    printf("\n");
    printf("\n");
    printf("             Azahara ªPorras Tejada             \n");
    printf("//////////////////////////////////////////////////\n\n");

    // Cambiar (o no) la dirección IP del cliente
    printf("Su direccion IP es %s. ¿Desea cambiarla? [s/n] ", direccion);
    if (getc(stdin) == 's')
    {
        printf("Introduzca la nueva IP: ");

        scanf("%s", direccion);
    }
    printf("Su IP es: \"%s\" \n\n", direccion);
    //Se crea la interfaz cliente con la direccion IP, en el puerto 1502, y se indica el tamaño del mapa de registros
    ClientModBUS modbus(direccion, puerto, 0x0020);
    if (!modbus.connect())
        return 0;
    /**** Opciones ****/

    while (option != 'q') //||option=='0')//mientras no se pulse salir se vuelve al menu tras realizar las distintas opciones
    {
        printf("\nIntroduzca La opcion que desee\n");
        printf("c-Control manual\n");
        printf("m-Mover a posicion\n");
        printf("o-Mover objeto\n\n");
        printf("i-Mover a posicion inicial (0,0,10,-90,-90,0)\n");
        printf("p-Posición actual\n");
        printf("r-Ver registros actuales\n"); //leer varios registros
        //printf("v-Vover a posicion "160,-150,10,-90,-90,0"\n");
        printf("q-Salir\n\n");
        printf("Opción elegida: ");
        scanf("%s", &option);
        /**** Opciones para el control manual****/
        if (option == 'c')
        {
            printf("CONTROL MANUAL\n");
            while (option != '0') //mientras no se pulse salir se vuelve al menu tras realizar las distintas opciones
            {
                printf("\nControl por teclado numerico:\n");
                printf("	(8)	\n");
                printf("	 ^\n");
                printf("(4)<   >(6)\n");
                printf("	 v\n");
                printf("	(2)\n\n");
                printf("5-Cerrar/Abrir pinza\n\n");
                printf("0-Salir\n\n");
                printf("Opción elegida: ");
                scanf("%s", &option);
                // Arriba (8)
                if (option == '8')
                {
                    printf("UP\n");
                    value = ROBOT_UP;
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                        printf("Valor registro Escrito en %04X es: %d\n ROBOT_UP\n", COMMAND_ADDR & 0xFFFF, value);
                    // Abajo (2)
                    if (option == '2')
                    {
                        printf("DOWN\n");
                        value = ROBOT_DOWN;
                        if (modbus.write_only_register(COMMAND_ADDR, value))
                            printf("Valor registro Escrito en %04X es:%d\n ROBOT_DOWN\n", COMMAND_ADDR & 0xFFFF, value);
                    }
                    // Izquierda (4)
                    if (option == '4')
                    {
                        printf("LEFT\n");
                        value = ROBOT_LEFT;
                        if (modbus.write_only_register(COMMAND_ADDR, value))
                            printf("Valor registro Escrito en %04X es:%d\n ROBOT_LEFT\n", COMMAND_ADDR & 0xFFFF, value);
                    }
                    // Derecha (6)
                    if (option == '6')
                    {
                        printf("RIGHT\n");
                        value = ROBOT_RIGHT;
                        if (modbus.write_only_register(COMMAND_ADDR, value))
                            printf("Valor registro Escrito en %04X es:%d\n ROBOT_RIGHT\n", COMMAND_ADDR & 0xFFFF, value);
                    }
                    // Delante (7)
                    if (option == '7')
                    {
                        printf("FORWARD\n");
                        value = ROBOT_FORWARD;
                    }
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                        printf("Valor registro Escrito en %04X es:%d\n ROBOT_FORWARD\n", COMMAND_ADDR & 0xFFFF, value);
                }
                // Detras (9)
                if (option == '9')
                {
                    printf("BACKWARD\n");
                    value = ROBOT_BACKWARD;
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                        printf("Valor registro Escrito en %04X es:%d\n ROBOT_BACKWARD\n", COMMAND_ADDR & 0xFFFF, value);
                }
                /**/
                // Coger (5)
                if (option == '5')
                {
                    printf("PINZA\n");
                    value = ROBOT_CATCH;
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                    {
                        printf("Valor registro Escrito en %04X es:%d\n ROBOT_CATCH\n", COMMAND_ADDR & 0xFFFF, value);
                        if (modbus.read_only_register(ADDR_PINZA, &value))
                        {
                            if (value == CLOSE_PINCERS)
                            {
                                printf("Cerramos la pinza\n");
                            }
                            else
                            {
                                printf("Abrimos la pinza\n");
                            }
                        }
                    }
                }
            }
        }
        // Mover a la posición indicada
        else if (option == 'm')
        {
            numReg = 7;
            //total de registros a escribir
            getc(stdin);
            printf("Mover a posicion %s. ¿Desea cambiarla? [s/n] ", newPosition);
            if (getc(stdin) == 's')
            {
                printf("Introduzca la nueva posicion: ");
                scanf("%s", newPosition);
                //getc(stdin);
            }
            char2int(newPosition, position);
            printf("Mover a posicion: ");
            imprimePosicion(position);
            value = ROBOT_AUTOMOVE;
            position[0] = value;
            if (modbus.write_many_registers(COMMAND_ADDR, numReg, position))
            {
                for (int i = 0; i < numReg; i++)
                    printf("Registro %04X cambia a:%d\n", (COMMAND_ADDR + i) & 0xFFFF, uintToInt(position[i]));
            }
            else
                printf("Error en ESCRITURA DE POSICION\n");
        }
        // Mover objeto
        else if (option == 'o')
        {
            //sleep(2);
            //newPosition[29]="0,0,10,-90,-90,0"; // 0,-150,10,-90,-90,0
            // Se mueve hasta un poco antes de la posición del objeto
            numReg = 7;
            //total de registros a escribir
            value = ROBOT_AUTOMOVE;
            position[0] = value;
            position[1] = position[1] - 10;
            if (modbus.write_many_registers(COMMAND_ADDR, numReg, position))
            {

                for (int i = 0; i < numReg; i++)
                    printf("Registro %04X cambia a:%d\n", (COMMAND_ADDR + i) & 0xFFFF, uintToInt(position[i]));
            }
            else
                printf("Error en ESCRITURA DE POSICION\n");
            /*****************/
            if (modbus.read_many_registers(COMMAND_ADDR, numReg, position))
            {
                printf("La posición obtenida es: ");
                imprimePosicion(position);
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
            sleep(4);
            // Abrimos la pinza
            if (modbus.read_only_register(ADDR_PINZA, &value))
            {
                if (value == CLOSE_PINCERS)
                {
                    printf("La pinza estaba cerrada\n");
                    value = ROBOT_CATCH;
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                    {
                        printf("Valor registro Escrito en %04X es: %d\n ROBOT_CATCH\n", COMMAND_ADDR & 0xFFFF, value);
                    }
                }
                else
                {
                    printf("La pinza estaba abierta\n");
                }
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
            /*****************/
            sleep(4);
            // Se mueve hasta rodear el objeto
            printf("Mover a posicion %s\n", newPosition);
            value = ROBOT_AUTOMOVE;
            position[0] = value;
            position[1] = position[1] + 10;
            if (modbus.write_many_registers(COMMAND_ADDR, numReg, position))
            {
                for (int i = 0; i < numReg; i++)
                    printf("Registro %04X cambia a: %d\n", (COMMAND_ADDR + i) & 0xFFFF, uintToInt(position[i]));
            }
            else
                printf("Error en ESCRITURA DE POSICION\n");
            /*****************/
            if (modbus.read_many_registers(COMMAND_ADDR, numReg, position))
            {

                printf("La posición obtenida es: ");
                imprimePosicion(position);
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
            /*****************/
            sleep(4);
            // Coge el objeto (cerramos la pinza)
            if (modbus.read_only_register(ADDR_PINZA, &value))
            {
                if (value == OPEN_PINCERS)
                {
                    printf("La pinza estaba abierta\n");
                    value = ROBOT_CATCH;
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                    {
                        printf("Valor registro Escrito en %04X es: %d\n ROBOT_CATCH\n", COMMAND_ADDR & 0xFFFF, value);
                    }
                }
                else
                {
                    printf("La pinza estaba cerrada\n");
                }
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
            /*****************/
            sleep(4);
            // Desplazamos el objeto a la nueva posicion
            value = ROBOT_AUTOMOVE;
            position[0] = value;
            position[1] = position[1] + 90;
            if (modbus.write_many_registers(COMMAND_ADDR, numReg, position))
            {
                for (int i = 0; i < numReg; i++)
                    printf("Registro %04X cambia a: %d\n", (COMMAND_ADDR + i) & 0xFFFF, uintToInt(position[i]));
            }
            else
                printf("Error en ESCRITURA DE POSICION\n");
            /*****************/
            if (modbus.read_many_registers(COMMAND_ADDR, numReg, position))
            {
                printf("La posición obtenida es: ");
                imprimePosicion(position);
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
            /*****************/
            sleep(4);
            // Abrimos la pinza

            if (modbus.read_only_register(ADDR_PINZA, &value))
            {
                if (value == CLOSE_PINCERS)
                {
                    printf("La pinza estaba cerrada\n");
                    value = ROBOT_CATCH;
                    if (modbus.write_only_register(COMMAND_ADDR, value))
                    {
                        printf("Valor registro Escrito en %04X es:%d\n ROBOT_CATCH\n", COMMAND_ADDR & 0xFFFF, value);
                    }
                }
                else
                {
                    printf("La pinza estaba abierta\n");
                }
            }
            else
                printf("Error en operacion LECTURA DE VARIOSREGISTROS\n");
            /*****************/
            sleep(4);
            // Se mueve hasta un poco antes de la posición del objeto
            value = ROBOT_AUTOMOVE;
            position[0] = value;
            position[1] = position[1] - 10;
            if (modbus.write_many_registers(COMMAND_ADDR, numReg, position))
            {
                for (int i = 0; i < numReg; i++)
                    printf("Registro %04X cambia a:%d\n", (COMMAND_ADDR + i) & 0xFFFF, uintToInt(position[i]));
            }
            else
                printf("Error en ESCRITURA DE POSICION\n");
            /*****************/
            if (modbus.read_many_registers(COMMAND_ADDR, numReg, position))
            {
                printf("La posición obtenida es: ");
                imprimePosicion(position);
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
            /*****************/
        }
        // Volver a la posición inicial
        if (option == 'i')
        {
            value = ROBOT_AUTOMOVE;
            position[0] = value;
            position[1] = 0;
            position[2] = 0;
            position[3] = 10;
            position[4] = -90;
            position[5] = -90;
            position[6] = 0;
            printf("Mover a posicion: ");
            imprimePosicion(position);

            if (modbus.write_many_registers(COMMAND_ADDR, numReg, position))
            {
                for (int i = 0; i < numReg; i++)
                    printf("Registro %04X cambia a:%d\n", (COMMAND_ADDR + i) & 0xFFFF, uintToInt(position[i]));
            }
            else
                printf("Error en ESCRITURA DE POSICION\n");
        }
        //Lectura de todos los registros del sistema
        else if (option == 'r')
        {
            printf("REGISTROS DEL SISTEMA\n");
            numReg = 20; //total de registros para mostrar todos por pantall
            if (modbus.read_many_registers(ADDR_INI, numReg, tab))
            {
                for (int i = 0; i < numReg; i++)
                    printf("El valor del registro leido en 0x%04X es: %d\n", (ADDR_INI + i) & 0xFFFF, uintToInt(tab[i]));
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
        }
        else if (option == 'p')
        {
            printf("POSICION ACTUAL\n");
            numReg = 7; //total de registros para mostrar todos por pantall
            if (modbus.read_many_registers(COMMAND_ADDR, numReg, position))
            {
                printf("La posición obtenida es: ");
                imprimePosicion(position);
            }
            else
                printf("Error en operacion LECTURA DE VARIOS REGISTROS\n");
        }
        else if (option != 'q' && option != '0')
        {
            printf("Error: Option %c doesn't exist\n", option);
        }
    }
    return 0;
}
int uintToInt(uint16_t value)
{
    int entero;
    if (value > 65176)
        entero = -(65536 - value);
    else
        entero = value;
    return entero;
}
void char2int(char newPosition[29], uint16_t position[6])
{
    char copia[29];
    char *pos;
    int i = 1;
    strcpy(copia, newPosition);
    //printf ("Splitting string \"%s\" into tokens:\n",newPosition);
    pos = strtok(newPosition, " ,");
    // Aqui deja solo la coma
    while (pos != NULL)
    {
        //printf ("%s\n",pos);
        // Aqui deberias guardar tu dato en el array!
        position[i] = atoi(pos);
        //printf ("%d\n",position[i]);
        pos = strtok(NULL, " ,"); // Aca tambien iria solo la coma.!!
        i++;
    }
    //printf("Position: %d,%d,%d,%d,%d,%d\n",uintToInt(position[1]),uintToInt(position[2]),uintToInt(position[3]),uintToInt(position[4]),uintToInt(position[5]),uintToInt(position[6]));
    strcpy(newPosition, copia);
}
void imprimePosicion(uint16_t position[6])
{
    printf("%d,%d,%d,%d,%d,%d\n", uintToInt(position[1]), uintToInt(position[2]), uintToInt(position[3]), uintToInt(position[4]), uintToInt(position[5]), uintToInt(position[6]));
}
