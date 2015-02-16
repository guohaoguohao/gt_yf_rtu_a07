/*
 * Serial232.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_SERIAL232_SERIAL232_H_
#define SRC_SERVER_SERIAL232_SERIAL232_H_
#include <string>
#include <modbus/modbus.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>

#include "../../common/PHYSICAL_PORT.h"
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
# ifndef _MSC_VER
# include <stdint.h>
# else
# include "stdint.h"
# endif
#endif

#define SERVER_ID         17
#define INVALID_SERVER_ID 18
namespace serial232n {

class Serial232 : public server::IServer {
 public:
  Serial232();
  Serial232(const configure::Serial232Configuration* config);
  virtual ~Serial232();
  bool Init();
  void DoWork();
  string getNameAbbr() { return "串口232服务."; }
  const configure::Serial232Configuration* config;
 private:
  /*server member*/
  int nb_points; //memory size
  uint8_t *tab_rp_bits; //memory for bit
  uint16_t *tab_rp_registers; //memory for registers
  uint16_t *tab_rp_registers_bad; //memory for registers bad
  /*server member*/
  static map< int, string> port;
  SERVER_OR_CLIENT modal; //server/client
  PHYSICAL_PORT use_backend;//RTU
  int baud;
  int header_length;
  string serialnum;
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

} /* namespace serial232n */

#endif /* SRC_SERVER_SERIAL232_SERIAL232_H_ */
