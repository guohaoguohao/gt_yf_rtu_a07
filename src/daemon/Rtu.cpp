/*
 * Rtu.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "Rtu.h"
#include "../common/SERVER_PROTOCAL.h"
#include "../common/MEMORY_SIZE.h"
using namespace std;
namespace rtu {

Rtu::Rtu() : config(nullptr), memory(nullptr), memory_size(0) {}

Rtu::~Rtu() {}

bool Rtu::Init() {
  LOG_COM_INFO(this, "正在初始化");
  this->memory_size =
      static_cast<unsigned int>(MEMORY_SIZES::MEMORY_SIZE_CARD1);
  if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::NET_INTERFACE_100M)>(
           sconfig))->AvailabilitySign) {
    server_n100 = make_shared<Net100>(
        get<static_cast<unsigned int>(SERVER_PROTOCAL::NET_INTERFACE_100M)>(
            sconfig));
    server_n100->setDaemon(this);
    if (!server_n100->Init()) return false;
    this->RegisterController(server_n100);
  }
  if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::NET_INTERFACE_1000M)>(
           sconfig))->AvailabilitySign) {
    server_n1000 = make_shared<Net1000>(
        get<static_cast<unsigned int>(SERVER_PROTOCAL::NET_INTERFACE_1000M)>(
            sconfig));
    server_n1000->setDaemon(this);
    if (!server_n1000->Init()) return false;
    this->RegisterController(server_n1000);
  }
  if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_232)>(
           sconfig))->AvailabilitySign) {
    server_s232 = make_shared<Serial232>(
        get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_232)>(
            sconfig));
    server_s232->setDaemon(this);
    if (!server_s232->Init()) return false;
    this->RegisterController(server_s232);
  }
  if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_485)>(
           sconfig))->AvailabilitySign) {
    server_s485 = make_shared<Serial485>();
    server_s485->setDaemon(this);
    if (!server_s485->Init()) return false;
    this->RegisterController(server_s485);
  }
  if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_DEBUG)>(
           sconfig))->AvailabilitySign) {
    server_sdebug = make_shared<SerialDebug>(
        get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_DEBUG)>(
            sconfig));
    server_sdebug->setDaemon(this);
    if (!server_sdebug->Init()) return false;
    this->RegisterController(server_sdebug);
  }
  if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_GPRS)>(
           sconfig))->AvailabilitySign) {
    server_sgprs = make_shared<SerialGprs>(
        get<static_cast<unsigned int>(SERVER_PROTOCAL::SERIAL_INTERFACE_GPRS)>(
            sconfig));
    server_sgprs->setDaemon(this);
    if (!server_sgprs->Init()) return false;
    this->RegisterController(server_sgprs);
  }
  if ((get<static_cast<unsigned int>(
           SERVER_PROTOCAL::SERIAL_INTERFACE_ZIGBEE_WIA)>(sconfig))
          ->AvailabilitySign) {
    server_szigbeewia =
        make_shared<SerialZigBeeWia>(get<static_cast<unsigned int>(
            SERVER_PROTOCAL::SERIAL_INTERFACE_ZIGBEE_WIA)>(sconfig));
    server_szigbeewia->setDaemon(this);
    if (!server_szigbeewia->Init()) return false;
    this->RegisterController(server_szigbeewia);
  }
  /*  //建立ADIO服务
    if ((get<static_cast<unsigned int>(SERVER_PROTOCAL::AD_IO)>(sconfig))
            ->AvailabilitySign) {
      server_n100 = make_shared<ADIO>();
      server_n100->setDaemon(this);
      if (!server_n100->Init()) return false;
      this->RegisterController(server_n100);
    }*/
  /**
   * 内存对象
   */
  this->memory = make_shared<CircleBuffer<short>>();
  LOG_COM_INFO(this, "内存.正在初始化");
  if (!this->memory->Init(
          this->memory_size,
          static_cast<unsigned int>(MEMORY_WRITE_PER::WRITE_PER))) {
  LOG_COM_ERR(this, "内存.初始化失败");
    return false;
  }
  LOG_COM_INFO(this, "内存.初始化完成");
  this->setId(config->DeviceId);
  this->setName(config->StationName);
  this->setInitialized(true);
  LOG_COM_INFO(this, "已初始化");
  return true;
}

bool Rtu::Start() {
  LOG_COM_INFO(this, "正在启动");
  if (!IDaemon::Start()) return false;
  LOG_COM_INFO(this, "已启动");
  return true;
}

void Rtu::Stop() {
  LOG_COM_INFO(this, "正在停止");
  IDaemon::StopWait();
  LOG_COM_INFO(this, "已停止");
}

inline string Rtu::getNameAbbr() { return "守护进程."; }

} /* namespace rtu */
