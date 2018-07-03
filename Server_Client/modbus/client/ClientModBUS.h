#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus.h>
#include "constant.h"

class ClientModBUS
{
  private:
    modbus_t *ctx;

  public:
    ClientModBUS(char *ip, short port, int timeout);
    ~ClientModBUS();
    bool connect();
    void disconnect();
    bool write_only_register(int addr, int value);
    bool write_many_registers(int addr, int num_registers, uint16_t values[]);
    bool read_only_register(int addr, uint16_t *);
    bool read_many_registers(int addr, int num_registers, uint16_t[]);
};
