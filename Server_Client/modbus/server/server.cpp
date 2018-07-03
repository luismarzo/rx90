/***********************************************************************/
/*
*/
/*
SERVIDOR TCP PARA RX90
*/
/*
*/
/***********************************************************************/
#include "serverModBUS.h"
#include "constant.h"
#include "Rx90.h"
void mover(serverModBUS server, int reg, Rx90 rx90);
void pinza(serverModBUS server, int reg, Rx90 rx90);
int main()
{
    // Brazo robótico Rx90
    char dispositivo[] = "/dev/ttyUSB0";
    //char dispositivo[] = "/dev/pts/18";

    //char posicion[]="122,-77,-43,55,45,-43";
    char posicion[] = "150,-150,10,-90,-90,0";
    char ipServer[16] = "192.168.109.148"; // 127.0.0.1
    int puerto = 1502;
    int nClient = 1;
    int reg = 0; //entero en el que se guarda el tipo de comando recibido
    uint16_t addr;
    // Servidor modbus
    serverModBUS server;
    printf("Iniciando servidor...\n");
    //Se inicia el servidor
    int sinit = server.init(ipServer, puerto, NUM_REG); //ip, puerto, numero de registros para crear
    /*
REGISTROS DEL SISTEMA:
0x0000: Número de conexiones de clientes                                       (servidor)
0x0001: Estado del fichero de registro (0:cerrado/1:abierto)                   (servidor)  
0x0002: Estado del RX90                                                        (servidor)
0x0003: Comando recibido por el cliente                                        (cliente)
0x0004: Posicion de rotacion de la base del RX90                               (cliente)
0x0005: Posicion de rotacion de la 1a articulacion RX90                        (cliente)
0x0006: Posicion de rotacion de la 2a articulacion RX90                        (cliente)
0x0007: Posicion de rotacion de la base de la articulación pinza del RX90      (cliente)
0x0008: Posicion de rotacion de la articulacion de la pinza del RX90           (cliente)
0x0009: Posicion de rotacion de la base de la pinza del RX90                   (cliente)
0x000A: Estado de la pinza (0: abierta / 1: cerrada)                           (servidor)
0x000B: Código de error
*/

    if (sinit)
    {
        server.write_only_register(ADDR_RX90_STATE, DISCONNECT);
        server.write_only_register(ADDR_CLIENT, NO_COMMAND);
        for (addr = ADDR_INI_POSITION; addr <= ADDR_FIN_POSITION; addr++)
        {
            server.write_only_register(addr, NO_COMMAND);
        }
        server.write_only_register(ADDR_PINZA, OPEN_PINCERS);
        printf("Servidor iniciado corectamente\n");
    }
    else
    {
        printf("Error al iniciar el servidor Modbus\n");
        return 0;
    }
    Rx90 rx90(dispositivo, posicion);
    Rx90::Action action = Rx90::NONE;
    server.write_only_register(ADDR_RX90_STATE, STATE_POWERINGON);
    //Se espera que el cliente se conecte
    printf("Esperando conexión del cliente\n");
    server.waitClient();
    printf("Cliente conectado al servidor\n");
    //server.write_only_register(NCLIENT, nClient);
    server.write_only_register(ADDR_CLIENT, COMMAND_START);
    while (true)
    {
        printf("Se espera la recepción de un comando\n");
        //Se espera la recepción de un comando
        int command = server.receiveCommand();
        if (command == RX90_COMMAND)
        {                                                 //si se recibe comando del RX90
            server.sendConfirmation();                    // Enviamos confirmación TCP
            reg = server.read_only_register(ADDR_CLIENT); //obtiene el comando escrito por el cliente
            printf("Comando recibido: %d\n", reg);
            printf("Realizando accion...\n");
            if (reg == ROBOT_UP)
            {
                action = Rx90::UP;
                server.write_only_register(ADDR_RX90_STATE, reg);
                rx90.move(action);
                printf("RX90 UP\n");
            }
            else if (reg == ROBOT_DOWN)
            {
                action = Rx90::DOWN;
                server.write_only_register(ADDR_RX90_STATE, reg);
                rx90.move(action);
                printf("RX90 DOWN\n");
            }
            else if (reg == ROBOT_LEFT)
            {
                action = Rx90::LEFT;
                server.write_only_register(ADDR_RX90_STATE, reg);
                rx90.move(action);
                printf("RX90 LEFT\n");
            }
            else if (reg == ROBOT_RIGHT)
            {
                action = Rx90::RIGHT;
                server.write_only_register(ADDR_RX90_STATE, reg);
                rx90.move(action);
                printf("RX90 RIGHT\n");
            }
            else if (reg == ROBOT_BACKWARD)
            {
                action = Rx90::BACKWARD;
                server.write_only_register(ADDR_RX90_STATE, reg);
                rx90.move(action);
                printf("RX90 BACKWARD\n");
            }
            else if (reg == ROBOT_FORWARD)
            {
                action = Rx90::FORWARD;
                server.write_only_register(ADDR_RX90_STATE, reg);
                rx90.move(action);
                printf("RX90 FORWARD\n");
            }
            else if (reg == ROBOT_CATCH)
            {
                if (server.read_only_register(ADDR_PINZA) == OPEN_PINCERS)
                {
                    printf("La pinza estaba abierta: %d\n", server.read_only_register(ADDR_PINZA));
                    reg == ROBOT_CATCH;
                    server.write_only_register(ADDR_RX90_STATE, reg);
                    action = Rx90::CATCH;
                    rx90.move(action);
                    server.write_only_register(ADDR_PINZA, CLOSE_PINCERS);
                    printf("Pinza cerrada: %d\n", server.read_only_register(ADDR_PINZA));
                }
                else if (server.read_only_register(ADDR_PINZA) == CLOSE_PINCERS)
                {
                    printf("La pinza estaba cerrada: %d\n", server.read_only_register(ADDR_PINZA));
                    reg == ROBOT_DROP;
                    server.write_only_register(ADDR_RX90_STATE, reg);
                    action = Rx90::DROP;
                    rx90.move(action);
                    server.write_only_register(ADDR_PINZA, OPEN_PINCERS);
                    printf("Pinza abierta: %d\n", server.read_only_register(ADDR_PINZA));
                }
                else
                {
                    printf("No se conoce el estado de la pinza\n");
                }
            }
            else if (reg == ROBOT_AUTOMOVE)
            {
                server.write_only_register(ADDR_RX90_STATE, ROBOT_AUTOMOVE);
                vector<int> leePos = server.read_many_registers(ADDR_INI_POSITION, 6);
                action = Rx90::AUTOMOVE;
                rx90.automove(leePos);
            }
            else
            {

                action = Rx90::NONE;
            }
            Rx90::printAction(action);
        }
        if (command == DISCONNECT)
        {
            printf("Cliente desconectado del servidor\n");
            server.write_only_register(ADDR_CLIENT, COMMAND_FINISH);
            server.waitClient();
            printf("Cliente conectado al servidor\n");
            nClient++;
            server.write_only_register(NCLIENT, nClient);
            server.write_only_register(ADDR_CLIENT, COMMAND_START);
        }
    }
    server.write_only_register(ADDR_RX90_STATE, STATE_POWERINGOFF);
    rx90.~Rx90();
    server.write_only_register(ADDR_RX90_STATE, DISCONNECT);
    server.close();
}
