/*
 * Net1000.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include <thread>
#include <chrono>
#include <string.h>
#include <boost/format.hpp>
#include "../net1000/Net1000.h"

namespace net1000m {

Net1000::Net1000() :
    config(nullptr),
    use_backend(PHYSICAL_PORT::TCP),
    ctx(nullptr),
    mb_mapping(nullptr),
    query(nullptr),
    modal(SERVER_OR_CLIENT::CLIENT),
    header_length(0),
    UT_BITS_TAB{0xCD, 0x6B, 0xB2, 0x0E, 0x1B},
    UT_INPUT_BITS_TAB{0xAC, 0xDB, 0x35},
    UT_REGISTERS_TAB{0x022B, 0x0001, 0x0064},
    UT_INPUT_REGISTERS_TAB{0x000A},
    port(502),
    nb_points(0),
    tab_rp_bits(nullptr),
    tab_rp_registers(nullptr),
    tab_rp_registers_bad(nullptr),
    socket(0){}

Net1000::Net1000(const configure::Net1000Configuration* config):
    config(config),
    use_backend(PHYSICAL_PORT::TCP),
    ctx(nullptr),
    mb_mapping(nullptr),
    query(nullptr),
    modal(SERVER_OR_CLIENT::CLIENT),
    header_length(0),
    UT_BITS_TAB{0xCD, 0x6B, 0xB2, 0x0E, 0x1B},
    UT_INPUT_BITS_TAB{0xAC, 0xDB, 0x35},
    UT_REGISTERS_TAB{0x022B, 0x0001, 0x0064},
    UT_INPUT_REGISTERS_TAB{0x000A},
    port(502),
    nb_points(0),
    tab_rp_bits(nullptr),
    tab_rp_registers(nullptr),
    tab_rp_registers_bad(nullptr),
    socket(0) {
      modal = static_cast<SERVER_OR_CLIENT>(config->modal);
      port = config->port;
      ip = config->ip;
    }

bool Net1000::Init() {
  if (modal == SERVER_OR_CLIENT::SERVER) {
    LOG_COM_INFO(this, "初始化服务端");
    use_backend = PHYSICAL_PORT::TCP;
    ctx = modbus_new_tcp(ip.data(), port);
    query = reinterpret_cast<uint8_t *>(malloc(MODBUS_TCP_MAX_ADU_LENGTH));
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
    socket = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, &socket);
    LOG_COM_INFO(this, "初始化服务端完成");
    this->setInitialized(true);
  } else if(modal == SERVER_OR_CLIENT::CLIENT) {
    LOG_COM_INFO(this, "初始化客户端");
    //todo:
    LOG_COM_INFO(this, "初始化客户端完成");
  }
}

void Net1000::DoWork() {
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
  close(socket);
  modbus_mapping_free(mb_mapping);
  free(query);
  modbus_free(ctx);
}

Net1000::~Net1000() {}

} /* namespace net1000m */
