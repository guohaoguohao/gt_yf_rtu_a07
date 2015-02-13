/*
 * Net100.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_NET100_NET100_H_
#define SRC_SERVER_NET100_NET100_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace net100m {

class Net100 : public server::IServer {
 public:
  Net100();
  Net100(const configure::Net100Configuration* config);
  bool Init();
  void DoWork();
  string getNameAbbr() { return "百兆网口服务."; }
  virtual ~Net100();
  const configure::Net100Configuration* config;
};

} /* namespace net100m */

#endif /* SRC_SERVER_NET100_NET100_H_ */
