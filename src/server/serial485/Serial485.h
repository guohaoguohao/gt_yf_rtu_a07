/*
 * Serial485.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_SERIAL485_SERIAL485_H_
#define SRC_SERVER_SERIAL485_SERIAL485_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace serial485n {

class Serial485 : public server::IServer {
 public:
  Serial485();
  Serial485(const configure::Serial485Configuration* config);
  virtual ~Serial485();
  bool Init();
  void DoWork();
  string getNameAbbr() { return "串口485服务."; }
  const configure::Serial485Configuration* config;
};

} /* namespace serial485n */

#endif /* SRC_SERVER_SERIAL485_SERIAL485_H_ */
