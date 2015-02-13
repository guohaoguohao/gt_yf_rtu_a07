/*
 * Serial232.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_SERIAL232_SERIAL232_H_
#define SRC_SERVER_SERIAL232_SERIAL232_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
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
};

} /* namespace serial232n */

#endif /* SRC_SERVER_SERIAL232_SERIAL232_H_ */
