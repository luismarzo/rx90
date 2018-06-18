/***********************************************************************/
/*
*/
/*
CLIENTE MODBUS PARA RX90
*/
/*
*/
/***********************************************************************/
#include "ClientModBUS.h"
/**Constructor: Recibe como argumentos la IP y el puerto en los que se va a
realizar la conexión**/
ClientModBUS::ClientModBUS(char* ip,short port,int timer)
{
ctx = modbus_new_tcp(ip, port);
struct timeval response_timeout;
/* Define a new and too short timeout! */
response_timeout.tv_sec = timer;
response_timeout.tv_usec = 0;
modbus_set_response_timeout(ctx, &response_timeout);
}
ClientModBUS::~ClientModBUS()
{
}
/**connect: Conecta el cliente con el servidor ModBUS
Recibe: Nada
Devuelve:
true: conexión realizada correctamente
false: Fallo al conectarse con el servidor
**/
bool ClientModBUS::connect()
{
if (modbus_connect(ctx) == -1)
{
fprintf(stderr, "Connection failed: %s\n",modbus_strerror(errno));
modbus_free(ctx);
return false;
}
else
return true;
}
/**disconnect: Realiza la desconexión con el servidor ModBUS con el servidor
ModBUS
Recibe:Nada
Devuelve: Nada
**/
void ClientModBUS::disconnect()
{
modbus_close(ctx);
}
/**write_only_register: Realiza la escritura de un solo registro
Recibe:
addr: Dirección del registro a escribir
value: Valor que se le quiere asignar a dicho registro
Devuelve:
true: Todo correcto
false: No se pudo realizar la operación
**/
bool ClientModBUS::write_only_register(int addr, int value)
{
int rc;
rc=modbus_write_register(ctx, addr, value);
if (rc != 1)
{
fprintf(stderr, "write_only_register: %s\n",modbus_strerror(errno));
return false;
}
else
return true;
}


/**write_many_registers: Realiza la escritura de varios registros
Recibe:
addr: Dirección del primer registro a escribir
num_registers: Numero de registros a escribir
value: Tabla de valores para dichos registros
Devuelve:
true: Todo correcto
false: No se pudo realizar la operación
**/
bool ClientModBUS::write_many_registers(int addr,int num_registers,uint16_t values[])
{
int rc;
rc=modbus_write_registers(ctx, addr, num_registers, values);
if (rc != num_registers)
{
fprintf(stderr, "write_many_registers:%s\n",modbus_strerror(errno));
return false;
}
else
return true;
}
/**read_only_register: Realiza la lectura de un solo registro
Recibe:
addr: Dirección del registro a leer
value: Parametro por referencia donde se guardara el dato leído
Devuelve:
true: Todo correcto
false: No se pudo realizar la operación
**/
bool ClientModBUS::read_only_register(int addr,uint16_t* value)
{
int rc;
rc=modbus_read_registers(ctx, addr, 1,value);
if (rc != 1)
{
fprintf(stderr, "read_only_register: %s\n",modbus_strerror(errno));
return false;
}
else
return true;
}
/**read_many_registers: Realiza la lectura de varios registros
Recibe:
addr: Dirección del primer registro a leer
num_registers: Numero de registros a leer
value: Tabla donde se guardarán los valores de los registros
leídos
Devuelve:
true: Todo correcto
false: No se pudo realizar la operación
**/

bool ClientModBUS::read_many_registers(int addr,int num_registers,uint16_t values[])
{
int rc;
rc=modbus_read_registers(ctx, addr,num_registers,values);
if (rc != num_registers)
{
fprintf(stderr, "read_many_registers: %s\n",modbus_strerror(errno));
return false;
}
else
return true;
}
