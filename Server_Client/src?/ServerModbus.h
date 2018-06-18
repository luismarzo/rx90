#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <modbus/modbus.h>
#include "constant.h"

using namespace std;
//Estructura donde se guardan los datos del comando recibido
typedef struct data_modBUS
{
uint16_t function;
uint16_t addr_read;
uint16_t numReg_read;
uint16_t addr_write;
uint16_t numReg_write;
uint16_t mask_and;
uint16_t mask_or;
uint16_t requestWriting;
}Operation;

class serverModBUS
{
private:
modbus_t *ctx;
//Contexto TCP delservidor Modbus
modbus_mapping_t *mb_mapping;
//Tabla de registros delservidor Modbus
uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH]; //Cadena de bytes con lapeticion del cliente
int rc;
Operation clientOperation;
//Datos de la peticiónrecibida
int socket;
//Socket por el queescucha el servidor
private:
int analizeRequest();
//Funcion que analiza larespuesta y rellena clientOperation
public:
serverModBUS();
//Constructor de la clase
bool init(char* ip,uint16_t port,int num_holding_registers);
//Inicia el servidor y la tabla de registro
void waitClient();
int read_only_register(uint16_t addr);
//Función para leer un solo registro
vector<int> read_many_registers(uint16_t addr,int num_registers);
//Función para leer varios registros
bool write_many_registers(uint16_t addr,int num_registers,
vector<int> values); //Función para escribir varios registros
bool write_only_register(uint16_t addr,int value); //Función para escribir en un solo registro
int receiveCommand();
//Función que recibe los comandos del cliente
void sendConfirmation();
//Función que envía la confirmación de una petición
void sendException(int exception);
//Función que envía una excepción a una petición
void close();
};
