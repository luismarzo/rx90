/***********************************************************************/
/*
*/
/*
CONSTANTES DEL SERVIDOR DEL RX90
*/
/*
*/
/***********************************************************************/
#define NUM_REG 20

/*
REGISTROS DEL SISTEMA:								(Usado por)
0x0000: Número de conexiones de clientes					(servidor)
0x0001: Estado del fichero de registro (0:cerrado/1:abierto)			(servidor)
0x0002: Estado del RX90								(servidor)
0x0003: Comando recibido por el cliente						(cliente)
0x0004: Posicion de rotacion de la base del RX90				(cliente)
0x0005: Posicion de rotacion de la 1a articulacion RX90				(cliente)
0x0006: Posicion de rotacion de la 2a articulacion RX90				(cliente)
0x0007: Posicion de rotacion de la base de la articulación pinza del RX90	(cliente)
0x0008: Posicion de rotacion de la articulacion de la pinza del RX90		(cliente)
0x0009: Posicion de rotacion de la base de la pinza del RX90			(cliente)
0x000A: Estado de la pinza (0: abierta / 1: cerrada)				(servidor)
0x000B: Código de error								(servidor)
*/ 


#define NCLIENT 0x0000
#define F_REGISTRO 0x0001
#define ADDR_RX90_STATE 0x0002 // ADDR_RX90_STATE y ADDR_CLIENT deben coincidir al ejecutar un comando
#define ADDR_CLIENT 0x0003
// Las direcciones 4, 5, 6, 7, 8 y 9 se usan para guardar el ángulo de rotación (posicion) de los ejes del RX90
#define ADDR_INI_POSITION 0x0004
#define ADDR_FIN_POSITION 0x0009
#define ADDR_PINZA 0x000A
// Estado pinza (ADDR_PINZA)
#define OPEN_PINCERS 0
#define CLOSE_PINCERS 1


// Tipos de registros
#define READ_ONLY 0
#define WRITE_ONLY 1
#define READ_WRITE 2
#define NOT_PERMITTED 3
// Comandos Modbus
#define READ_REGISTERS 0x03 // Valor que indica el comando para leer registros
#define WRITE_MULTIPLE_REGISTERS 0x10 // Valor que indica el comando para escribir varios registros
#define WRITE_ONLY_REGISTER 0x06 // Valor que indica el comando para escribir un único registro
#define READ_WRITE_REGISTERS 0x17 // Valor que indica el comando para leer/escribir registros
#define WRITE_MASK_REGISTER 0X16
#define READ_FIFO 0x18 // Valor que indica el comando para leer registros de la pila

//Estado del sistema (ADDR_RX90_STATE)
#define DISCONNECT -1
#define RX90_COMMAND 1
#define NO_COMMAND 0
#define STATE_POWERINGON 101
#define STATE_POWERINGOFF 102

//Robot command (ADDR_CLIENT)
#define COMMAND_START 501
#define COMMAND_FINISH 502
#define ROBOT_UP 908
#define ROBOT_DOWN 902
#define ROBOT_LEFT 904
#define ROBOT_RIGHT 906
#define ROBOT_BACKWARD 907
#define ROBOT_FORWARD 909
#define ROBOT_CATCH 905
#define ROBOT_DROP 0


#define ROBOT_AUTOMOVE 307
//Errores
#define ERROR_NO_ERROR 801
#define ERROR_UNKNOW 802
#define ERROR_SENSOR 803
#define ERROR_ACTUADOR 804
#define ERROR_MODBUS 800




