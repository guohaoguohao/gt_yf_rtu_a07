/*
 * SerialGprs.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_SERIALGPRS_SERIALGPRS_H_
#define SRC_SERVER_SERIALGPRS_SERIALGPRS_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace serialgprsn {

class SerialGprs : public server::IServer {
 public:
  SerialGprs();
  SerialGprs(const configure::SerialGPRSConfiguration* config);
  virtual ~SerialGprs();
  bool Init();
  void DoWork();
  string getNameAbbr() { return "串口GPRS服务."; }
  const configure::SerialGPRSConfiguration* config;
};

} /* namespace serialgprsn */

#endif /* SRC_SERVER_SERIALGPRS_SERIALGPRS_H_ */
