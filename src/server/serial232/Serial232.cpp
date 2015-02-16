/*
 * Serial232.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include <thread>
#include <chrono>
#include <string.h>
#include <boost/format.hpp>
#include "Serial232.h"
using namespace std;
namespace serial232n {

map<int, string> Serial232::port = {{0, "/dev/ttyUSB0"},
                                    {1, "/dev/ttyS1"},
                                    {2, "/dev/ttyS2"},
                                    {3, "/dev/ttyS3"},
                                    {4, "/dev/ttyS4"},
                                    {5, "/dev/ttyS5"},
                                    {6, "/dev/ttyS6"},
                                    {7, "/dev/ttyS7"},
                                    {8, "/dev/ttyS8"}};

// uint8_t Serial232::UT_BITS_TAB[5] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
// uint8_t Serial232::UT_INPUT_BITS_TAB[3] = { 0xAC, 0xDB, 0x35 };
// uint16_t Serial232::UT_REGISTERS_TAB[3] = { 0x022B, 0x0001, 0x0064 };
// uint16_t Serial232::UT_INPUT_REGISTERS_TAB[1] = { 0x000A };

Serial232::Serial232()
    : config(nullptr),
      use_backend(PHYSICAL_PORT::RTU),
      ctx(nullptr),
      mb_mapping(nullptr),
      query(nullptr),
      modal(SERVER_OR_CLIENT::CLIENT),
      header_length(0),
      UT_BITS_TAB{0xCD, 0x6B, 0xB2, 0x0E, 0x1B},
      UT_INPUT_BITS_TAB{0xAC, 0xDB, 0x35},
      UT_REGISTERS_TAB{0x022B, 0x0001, 0x0064},
      UT_INPUT_REGISTERS_TAB{0x000A},
      baud(0),
      nb_points(0),
      tab_rp_bits(nullptr),
      tab_rp_registers(nullptr),
      tab_rp_registers_bad(nullptr) {}

Serial232::Serial232(const configure::Serial232Configuration *config)
    : config(config),
      use_backend(PHYSICAL_PORT::RTU),
      ctx(nullptr),
      mb_mapping(nullptr),
      query(nullptr),
      modal(SERVER_OR_CLIENT::CLIENT),
      header_length(0),
      UT_BITS_TAB{0xCD, 0x6B, 0xB2, 0x0E, 0x1B},
      UT_INPUT_BITS_TAB{0xAC, 0xDB, 0x35},
      UT_REGISTERS_TAB{0x022B, 0x0001, 0x0064},
      UT_INPUT_REGISTERS_TAB{0x000A},
      baud(0),
      nb_points(0),
      tab_rp_bits(nullptr),
      tab_rp_registers(nullptr),
      tab_rp_registers_bad(nullptr) {
  modal = static_cast<SERVER_OR_CLIENT>(config->modal);
  baud = config->Baud;
  serialnum = port[config->SerialPortNum];
}

bool Serial232::Init() {
  if (modal == SERVER_OR_CLIENT::SERVER) {
    LOG_COM_INFO(this, "初始化服务端");
    use_backend = PHYSICAL_PORT::RTU;
    ctx = modbus_new_rtu(serialnum.data(), baud, 'N', 8, 1);
    modbus_set_slave(ctx, SERVER_ID);
    query = reinterpret_cast<uint8_t *>(malloc(MODBUS_RTU_MAX_ADU_LENGTH));
    header_length = modbus_get_header_length(ctx);

    modbus_set_debug(ctx, TRUE);

    mb_mapping = modbus_mapping_new(
        UT_BITS_ADDRESS + UT_BITS_NB, UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,
        UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,
        UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);
    if (mb_mapping == NULL) {
      boost::format msg = boost::format("Failed to allocate the mapping: %s") %
                          modbus_strerror(errno);
      modbus_free(ctx);
      LOG_COM_INFO(this, msg.str());
      return false;
    }
    /* Examples from PI_MODBUS_300.pdf.
       Only the read-only input values are assigned. */

    /** INPUT STATUS 不需要bit操作**/
    //    modbus_set_bits_from_bytes(mb_mapping->tab_input_bits,
    //                               UT_INPUT_BITS_ADDRESS, UT_INPUT_BITS_NB,
    //                               UT_INPUT_BITS_TAB);

    /** INPUT REGISTERS **/
    //    for (int i = 0; i < UT_INPUT_REGISTERS_NB; i++) {
    //      mb_mapping->tab_input_registers[UT_INPUT_REGISTERS_ADDRESS + i] =
    //          UT_INPUT_REGISTERS_TAB[i];
    //      ;
    //    }
    for (int i = 0; i < UT_REGISTERS_NB; i++) {
      mb_mapping->tab_registers[UT_REGISTERS_ADDRESS + i] = i;
    }
    if (modbus_connect(ctx) == -1) {
      boost::format msg =
          boost::format("Unable to connect %s") % modbus_strerror(errno);
      modbus_free(ctx);
      LOG_COM_INFO(this, msg.str());
      return false;
    }
    LOG_COM_INFO(this, "初始化服务端完成");
    this->setInitialized(true);
  }
  /**
   * 客户端--读取寄存器地址内数据，不需要侦听串口添加客户端操作方法，dowork()直接退出
   */
  else if (modal == SERVER_OR_CLIENT::CLIENT) {
    LOG_COM_INFO(this, "初始化客户端");
    use_backend = PHYSICAL_PORT::RTU;
    ctx = modbus_new_rtu(serialnum.data(), baud, 'N', 8, 1);
    if (ctx == NULL) {
      boost::format msg = boost::format("Unable to allocate libmodbus context");
      return false;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(
        ctx, static_cast<modbus_error_recovery_mode>(
                 MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));
    if (modbus_connect(ctx) == -1) {
      boost::format msg =
          boost::format("Connection failed: %s") % modbus_strerror(errno);
      modbus_free(ctx);
      return false;
    }
    /* Allocate and initialize the memory to store the bits */
    nb_points = (UT_BITS_NB > UT_INPUT_BITS_NB) ? UT_BITS_NB : UT_INPUT_BITS_NB;
    tab_rp_bits = (uint8_t *)malloc(nb_points * sizeof(uint8_t));
    memset(tab_rp_bits, 0, nb_points * sizeof(uint8_t));

    /* Allocate and initialize the memory to store the registers */
    nb_points = (UT_REGISTERS_NB > UT_INPUT_REGISTERS_NB)
                    ? UT_REGISTERS_NB
                    : UT_INPUT_REGISTERS_NB;
    tab_rp_registers = (uint16_t *)malloc(nb_points * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));
    LOG_COM_INFO(this, "初始化客户端完成");
  }
  return true;
}

void Serial232::DoWork() {
  LOG_COM_INFO(this, "启动");
  boost::format msg;
  while (this->running) {
    //    this_thread::sleep_for(chrono::seconds(1));
    LOG_COM_INFO(this, "receive data");
    int rc = modbus_receive(ctx, query);
    LOG_COM_INFO(this, "receive data ok");
    if (rc == -1) {
      /* Connection closed by the client or error */
      LOG_COM_INFO(this, "Connection closed by the client or error");
      break;
    }
    /* Read holding registers */
//    if (query[header_length] == 0x03) {
//      if (MODBUS_GET_INT16_FROM_INT8(query, header_length + 3) ==
//          UT_REGISTERS_NB_SPECIAL) {
//        LOG_COM_INFO(this, "Set an incorrect number of values");
//        MODBUS_SET_INT16_TO_INT8(query, header_length + 3,
//                                 UT_REGISTERS_NB_SPECIAL - 1);
//      } else if (MODBUS_GET_INT16_FROM_INT8(query, header_length + 1) ==
//                 UT_REGISTERS_ADDRESS_SPECIAL) {
//        LOG_COM_INFO(this,
//                     "Reply to this special register address by an exception");
//        modbus_reply_exception(ctx, query,
//                               MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY);
//        continue;
//      }
//    }
    rc = modbus_reply(ctx, query, rc, mb_mapping);
    if (rc == -1) {
      break;
    }
  }  // while(true)
  modbus_mapping_free(mb_mapping);
  free(query);
  modbus_free(ctx);
}

Serial232::~Serial232() {}

} /* namespace serial232n */
