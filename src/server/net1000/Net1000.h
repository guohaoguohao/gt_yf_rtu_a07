/*
 * Net1000.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_NET1000_NET1000_H_
#define SRC_SERVER_NET1000_NET1000_H_
#include <string>
#include "../IServer.h"
#include "../../log/Log.h"
#include "../../config/Configure.h"
namespace net1000m {

class Net1000 : public server::IServer {
 public:
  Net1000();
  Net1000(const configure::Net1000Configuration* config);
  bool Init();
  void DoWork();
  string getNameAbbr() { return "千兆网口服务."; }
  virtual ~Net1000();
  const configure::Net1000Configuration* config;
};

} /* namespace net1000m */

#endif /* SRC_SERVER_NET1000_NET1000_H_ */
