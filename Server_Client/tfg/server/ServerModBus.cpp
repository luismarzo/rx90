/************************************************************************/
/*
*/
/*
SERVIDOR MODBUS PARA RX90
*/
/*
*/
/************************************************************************/
#include "serverModBUS.h"
#include "constant.h"
/**********************************************************/
/* Constructor:Inicializa las variables de la clase
*/
/* modbus
*/
/*
*/
/* Recibe: Nada
*/
/* Devuelve: Nada
*/
/**********************************************************/
serverModBUS::serverModBUS()
{
mb_mapping=NULL;
ctx=NULL;
rc=-1;
}
/****************************************************************************
****/
/* init: Inicializa la tabla de registros y el contexto TCP
*/
/* del cliente
*/
/*
*/
/* Recibe:
*/
/*
ipServer: IP del servidor
*/

/*
portServer: Puerto del servidor
*/
/*
num_holding_registers: Número de registros de lectura y escritura
*/
/*
*/
/* Devuelve: (bool)
*/
/*
true: Servidor iniciado correctamente
*/
/*
false: error al inicializar
*/
/*
*/
/****************************************************************************
****/
bool serverModBUS::init(char* ipServer, uint16_t portServer,int
num_holding_registers)
{
ctx = modbus_new_tcp(ipServer, portServer);
if(ctx==NULL)
{
fprintf(stderr, "Failed to create ctx: %s\n",modbus_strerror(errno));
return false;
}
mb_mapping = modbus_mapping_new(0,0,num_holding_registers,0);
if (mb_mapping == NULL) {
fprintf(stderr, "Failed to allocate the mapping: %s\n",
modbus_strerror(errno));
modbus_free(ctx);
return false;
}
socket = modbus_tcp_listen(ctx, 10);
return true;
}
/****************************************************************************
****/
/* waitClient: Espera la conexión de un cliente
*/
/*
*/
/*
/*
Recibe: Nada
Devuelve:
*/
*/
/*
*/
/****************************************************************************
****/
void serverModBUS::waitClient()
{
modbus_tcp_accept(ctx, &socket);
}
/****************************************************************************
****/
/* read_only_register: Realiza la lectura de un solo registro
Recibe:
addr: direccion del registro a leer
Devuelve:(int)
Valor del registro leído o -1 si ha habido error 
 */

/****************************************************************************
****/
int serverModBUS::read_only_register(uint16_t addr)
{
int value;
if(mb_mapping!=NULL && mb_mapping->nb_registers > addr)
{
value=mb_mapping->tab_registers[addr];
return value;
}
else
return -1;
}
/****************************************************************************
****/
/* read_many_registes: Realiza la lectura de varios registros
*/
/*
*/
/* Recibe:
*/
/*
addr: direccion del primer registro a leer
/*
num_registers: número de registros a leer
/* Devuelve: vector <int>
/*
Vector con los valores de los registros o vector vacio
*/
/*
si ha habido error
*/
*/
*/
*/
/*
*/
/****************************************************************************
****/
vector<int> serverModBUS::read_many_registers(uint16_t addr,int num_registers)
{
vector <int> values;
if(mb_mapping!=NULL && (mb_mapping->nb_registers > addr + num_registers))
{
for(int i=0;i<num_registers;i++)
values.push_back(mb_mapping->tab_registers[addr+i]);
}
return values;
}
/****************************************************************************
****/
/* write_only_register: Realiza la escritura de un solo registro
*/
/*
*/
/* Recibe:
/*
addr: direccion del registro a escribir
/*
value: valor del registro a escribir
/* Devuelve:(bool)
/*
true: Operación realizada correctamente
/*
false: Operación fallida
*/
*/
*/
*/
*/
*/
/*
*/
/****************************************************************************
****/

bool serverModBUS::write_only_register(uint16_t addr,int value)
{
if(mb_mapping!=NULL && (mb_mapping->nb_registers > addr))
{
//printf("Valor a escribir: %d\n", value);
printf ("Registro de escritura 0x%04X: %d\n",addr,value);
printf ("-----------------------------------------------\n");
mb_mapping->tab_registers[addr]=value;
return true;
}
else
return false;
}
/****************************************************************************
****/
/* write_many_register: Realiza la escritura de varios registros
*/
/*
*/
/* Recibe:
*/
/*
addr: direccion del primer registro a escribir
/*
num_registers: Numeros de registros a escribir
/*
values: vector con los valores de los registros a escribir
/*
/* Devuelve:(bool)
/*
true: Operación realizada correctamente
/*
false: Operación fallida
*/
*/
*/
*/
*/
*/
*/
/*
*/
/****************************************************************************
****/
bool serverModBUS::write_many_registers(uint16_t addr,int num_registers,vector <int> values)
{
if(num_registers<=values.size())
return false;
else
{
if(mb_mapping!=NULL && (mb_mapping->nb_registers > addr + num_registers))
{
for(int i=0;i<num_registers;i++) {
mb_mapping->tab_registers[addr+i]=values.at(i);
printf ("Registro de escritura 0x%04X: %d\n",addr+i,values[i]);
}
printf ("-----------------------------------------------\n");
return true;
}
else
return false;
}
}
/****************************************************************************
****/

/* receive_command: Se queda esperando hasta recibir una serie de
*/
/* comandos concretos
*/
/*
*/
/*
*/
/* Recibe: Nada
*/
/*
*/
/* Devuelve:(int)
*/
/*
Comandos posibles:
*/
/*
ROBOT COMMAND
1
(Comando hacia el robot)
*/
/*
BREAKER COMMAND ACTION
2
(Comando de breaker)
*/
/*
ROBOT BREAKER COMMAND ACTION 3
(Comando de robot y breaker)
*/
/*
DISCONNECT
4 (Cliente desconectado)
*/
/*
*/
/*
*/
/****************************************************************************
****/
int serverModBUS::receiveCommand()
{
int type_request;
int applyCommand=false;
bool flagRX90=false;
char tecla='0';
do
{
rc = modbus_receive(ctx, query);
//if(kbhit())
//
tecla = getch();
if (rc != -1)
{
type_request=analizeRequest();
//printf("Comando tipo %d\n",type_request);
if(type_request!=READ_ONLY && type_request!=NOT_PERMITTED)
{
//printf("Direccion cliente: 0x%04X:%d\n",clientOperation.addr_write);
int addr;
addr=clientOperation.addr_write/*+i*/;
if(addr==ADDR_CLIENT)
flagRX90=true;
if(flagRX90)
{
flagRX90=false;
applyCommand=RX90_COMMAND;
}
else
{
applyCommand==NO_COMMAND;
sendConfirmation();
}
}
else


{
if(type_request==NOT_PERMITTED)
sendException(1);
else
sendConfirmation();
applyCommand == NO_COMMAND;
}
printf("Pulse 'Ctr+C' si desea salir...");
}
else
{
applyCommand=DISCONNECT;
}
}while(applyCommand == NO_COMMAND && tecla!='q');
return applyCommand;
}
/****************************************************************************
****/
/* sendConfirmation: envía una confirmación de que la petición se ha
*/
/* realizado correctamente
*/
/*
*/
/*
*/
/* Recibe: Nada
*/
/* Devuelve:Nada
*/
/*
*/
/****************************************************************************
****/
void serverModBUS::sendConfirmation()
{
modbus_reply(ctx, query, rc, mb_mapping);
}
/****************************************************************************
****/
/* sendConfirmation: envía una excepción si la petición no se ha
*/
/* realizado correctamente
*/
/*
*/
/*
*/
/* Recibe: Nada
*/
/* Devuelve:Nada
*/
/*
*/
/****************************************************************************
****/
void serverModBUS::sendException(int exception)
{
modbus_reply_exception(ctx,query,exception);
}
/****************************************************************************
****/
/* close: Cierra el servidor modBUS y libera la tabla de registros
*/
/*


/* Recibe: Nada
*/
/* Devuelve:Nada
*/
/*
*/
/****************************************************************************
****/
void serverModBUS::close()
{
modbus_mapping_free(mb_mapping);
modbus_close(ctx);
modbus_free(ctx);
}
/****************************************************************************
****/
/* analizeRequest: Analiza el tipo de petición
*/
/*
*/
/* Recibe: Nada
*/
/* Devuelve:(int)
*/
/*
READ_ONLY 0 (solo lectura)
*/
/*
WRITE_ONLY 1
(solo escritura)
*/
/*
READ_WRITE 2
(lectura y escritura)
*/
/*
NOT_PERMITTED 3
(petición rechazada)
*/
/*
*/
/****************************************************************************
****/
int serverModBUS::analizeRequest()
{
int type;
clientOperation.function=query[7];
switch(clientOperation.function)
{
case READ_REGISTERS:
type=READ_ONLY;
clientOperation.addr_read=(query[8]<<8) +query[9];
clientOperation.numReg_read=(query[10]<<8) + query[11];
break;
case WRITE_ONLY_REGISTER:
type=WRITE_ONLY;
clientOperation.addr_write=(query[8]<<8) +query[9];
clientOperation.numReg_write=1;
break;
case WRITE_MULTIPLE_REGISTERS:
type=WRITE_ONLY;
clientOperation.addr_write=(query[8]<<8) +query[9];
clientOperation.numReg_write=(query[10]<<8) + query[11];
break;
case READ_WRITE_REGISTERS:
type=READ_WRITE;
clientOperation.addr_read=(query[8]<<8) +query[9];
clientOperation.numReg_read=(query[10]<<8) + query[11];
clientOperation.addr_write=(query[12]<<8) +query[13];
clientOperation.numReg_write=(query[14]<<8) + query[15];
break;
case READ_FIFO:
type=READ_ONLY;
clientOperation.requestWriting=false;
clientOperation.addr_read=(query[8]<<8) +query[9];
clientOperation.numReg_read=(query[10]<<8) + query[11];

break;
case WRITE_MASK_REGISTER:
type=WRITE_ONLY;
clientOperation.numReg_write=1;
clientOperation.addr_write=(query[8]<<8) +query[9];
clientOperation.mask_and=(query[10]<<8) +query[11];
clientOperation.mask_or=(query[12]<<8) + query[13];
break;
default:
break;
}
//todos los registros son de lectura y escritura, aqui comprueba que donde intenta escribir es un registro de escritura
if(type!=READ_ONLY)
{
for(int i=0;i < clientOperation.numReg_write;i++)
{
int reg=clientOperation.addr_write+i;
//printf("Registro del cliente: 0x%04X\n",reg);
if(reg<ADDR_CLIENT && reg>ADDR_FIN_POSITION) //comprueba que solo se puede escribir en los registros asignados
{
return NOT_PERMITTED;
}
}
return type;
}
else
return READ_ONLY;
}
