/*
 * SerialZigBeeWia.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_SERIALZIGBEEWIA_SERIALZIGBEEWIA_H_
#define SRC_SERVER_SERIALZIGBEEWIA_SERIALZIGBEEWIA_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace serialzigbeewian {

class SerialZigBeeWia : public server::IServer {
 public:
  SerialZigBeeWia();
  SerialZigBeeWia(const configure::SerialZigBeeConfiguration* config);
  virtual ~SerialZigBeeWia();
  bool Init();
  void DoWork();
  string getNameAbbr() { return "串口Zigbee/Wia服务."; }
  const configure::SerialZigBeeConfiguration* config;
};

} /* namespace serialzigbeewian */

#endif /* SRC_SERVER_SERIALZIGBEEWIA_SERIALZIGBEEWIA_H_ */
