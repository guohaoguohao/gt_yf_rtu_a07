/*
 * Rtu.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_DAEMON_RTU_H_
#define SRC_DAEMON_RTU_H_

#include <memory>
#include <tuple>
#include "IDaemon.h"
#include "../log/Log.h"
#include "../config/Configure.h"
#include "../memorymanager/CircleBuffer.h"
#include "../server/net100/Net100.h"
#include "../server/net1000/Net1000.h"
#include "../server/serial232/Serial232.h"
#include "../server/serial485/Serial485.h"
#include "../server/serialdebug/SerialDebug.h"
#include "../server/serialgprs/SerialGprs.h"
#include "../server/serialzigbeewia/SerialZigBeeWia.h"
//#include "../server/adio"
using namespace configure;
using namespace memorymanager;
using namespace net100m;
using namespace net1000m;
using namespace serialdebugn;
using namespace serial232n;
using namespace serial485n;
using namespace serialgprsn;
using namespace serialzigbeewian;

namespace rtu {

class Rtu : public daemon_rtu::IDaemon {
 public:
  Rtu();
  virtual ~Rtu();
  bool Init();
  bool Start();
  void Stop();
  string getNameAbbr();
  const MainParamConfiguration* config;
  std::tuple<const Net100Configuration*,
             const Net1000Configuration*,
             const Serial232Configuration*,
             const Serial485Configuration*,
             const SerialDebugConfiguration*,
             const SerialGPRSConfiguration*,
             const SerialZigBeeConfiguration*,
             const ADIOConfiguration*> sconfig;
  std::shared_ptr<CircleBuffer<short>> getMemory() {
    return this->memory;
  }
  //服务对象
  std::shared_ptr<Net100> server_n100;
  std::shared_ptr<Net1000> server_n1000;
  std::shared_ptr<SerialDebug> server_sdebug;
  std::shared_ptr<Serial232> server_s232;
  std::shared_ptr<Serial485> server_s485;
  std::shared_ptr<SerialGprs> server_sgprs;
  std::shared_ptr<SerialZigBeeWia> server_szigbeewia;
  //  std::shared_ptr<adio::ADIO> server_adio;
 private:
  std::shared_ptr<CircleBuffer<short>> memory;
  unsigned long memory_size;
};

} /* namespace rtu */

#endif /* SRC_DAEMON_RTU_H_ */
