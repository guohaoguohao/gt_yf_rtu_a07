/*
 * Net1000.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_NET1000_NET1000_H_
#define SRC_SERVER_NET1000_NET1000_H_
#include <string>
#include <modbus/modbus.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>

#include "../../common/PHYSICAL_PORT.h"
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace net1000m {

class Net1000 : public server::IServer {
 public:
  Net1000();
  Net1000(const configure::Net1000Configuration* config);
  bool Init();
  void DoWork();
  string getNameAbbr() { return "千兆网口服务."; }
  virtual ~Net1000();
  const configure::Net1000Configuration* config;
 private:
  /*server member*/
  int socket;
  int nb_points; //memory size
  uint8_t *tab_rp_bits; //memory for bit
  uint16_t *tab_rp_registers; //memory for registers
  uint16_t *tab_rp_registers_bad; //memory for registers bad
  /*server member*/
  SERVER_OR_CLIENT modal; //server/client
  PHYSICAL_PORT use_backend;//RTU
  int header_length;
  string ip;
  int port;
  uint8_t *query;
  modbus_t *ctx;
  modbus_mapping_t *mb_mapping;
  /**
   *
   */
  const uint16_t UT_BITS_ADDRESS = 0x00;
  const uint16_t UT_BITS_NB = 0x00;
  uint8_t UT_BITS_TAB[5];

  const uint16_t UT_INPUT_BITS_ADDRESS = 0x00;
  const uint16_t UT_INPUT_BITS_NB = 0x00;
  uint8_t UT_INPUT_BITS_TAB[3];


  /*-----------------register--0x03------------------------*/
  const uint16_t UT_REGISTERS_ADDRESS = 0x00;
  /* Raise a manual exception when this adress is used for the first byte */
  const uint16_t UT_REGISTERS_NB = 20000;
  uint16_t UT_REGISTERS_TAB[20000];
  /*---------------------------------------------------------*/
  const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x00;
  /* If the following value is used, a bad response is sent.
     It's better to test with a lower value than
     UT_REGISTERS_NB_POINTS to try to raise a segfault. */
  const uint16_t UT_REGISTERS_NB_SPECIAL = 0x0;

  const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x00;
  const uint16_t UT_INPUT_REGISTERS_NB = 0x0;
  uint16_t UT_INPUT_REGISTERS_TAB[1];

  const float UT_REAL = 916.540649;
  const uint32_t UT_IREAL = 0x4465229a;
};

} /* namespace net1000m */

#endif /* SRC_SERVER_NET1000_NET1000_H_ */
