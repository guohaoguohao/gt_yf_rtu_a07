/*
 * SerialDebug.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_SERIALDEBUG_SERIALDEBUG_H_
#define SRC_SERVER_SERIALDEBUG_SERIALDEBUG_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace serialdebugn {

class SerialDebug : public server::IServer {
 public:
  SerialDebug();
  SerialDebug(const configure::SerialDebugConfiguration* config);
  virtual ~SerialDebug();
  bool Init();
  void DoWork();
  string getNameAbbr() { return "串口DEBUG服务."; }
  const configure::SerialDebugConfiguration* config;
};

} /* namespace serialdebugn */

#endif /* SRC_SERVER_SERIALDEBUG_SERIALDEBUG_H_ */
