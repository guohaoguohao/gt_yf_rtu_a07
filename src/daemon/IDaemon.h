/*
 * IDaemon.h
 *
 *  Created on: Feb 2, 2015
 *      Author: kevin
 */

#ifndef SRC_DAEMON_IDAEMON_H_
#define SRC_DAEMON_IDAEMON_H_
#include <string>
#include <vector>
#include <memory>
#include "../common/SERVER_PROTOCAL.h"
#include "../server/IServer.h"
using namespace std;
using namespace server;

namespace daemon_rtu {

class IDaemon {
 public:
  IDaemon();
  virtual ~IDaemon();
  //初始化守护
  virtual bool Init()=0;
  //启动守护程序全部服务
  virtual bool Start();
  //停止全部服务
  virtual void Stop();
  //停止等待服务
  virtual void StopWait();

/*  //实时启动守护程序某个服务,这个方法以后再实现
  virtual void StartSingle(SERVER_PROTOCAL);
  //实时停止一个服务，这个方法以后再实现
  virtual void StopSingle(SERVER_PROTOCAL);
  //实时重启一个服务，这个方法以后在实现
  virtual void ReStartSingle(SERVER_PROTOCAL);*/


  void RegisterController(shared_ptr<IServer> server);
  void RemoveRegisteredController(shared_ptr<IServer> server);
  inline virtual string getNameAbbr();

  bool isEnabled() const {
    return enabled;
  }

  void setEnabled(bool enabled = false) {
    this->enabled = enabled;
  }

  bool isRunning() const {
    return running;
  }

  void setRunning(bool running = false) {
    this->running = running;
  }

  bool isInitialized() const {
    return initialized;
  }

  void setInitialized(bool initialized = false) {
    this->initialized = initialized;
  }

  void setName(const string& name) {
    this->name = name;
  }

  const string& getName() const {
    return name;
  }

  void setId(int id) {
    this->id = id;
  }

  int getId() const {
    return id;
  }

  IDaemon* getParent() {
    return parent;
  }

  void setParent(IDaemon* parent) {
    this->parent = parent;
  }

  string getTraceName();

 private:
  /**
   * 是否启用标志
   */
  bool enabled;
  /**
   * 是否正在运行标志
   */
  bool running;
  /**
   * 是否初始化
   */
  bool initialized;
  /**
   * 父节点指针
   */
  IDaemon * parent;
  /**
   * 名称
   */
  string name;
  /**
   * ID
   */
  int id;
  /**
   * 提供的所有服务协议集合
   */
  vector<shared_ptr<IServer>> servers;
};

} /* namespace daemon_rtu */

#endif /* SRC_DAEMON_IDAEMON_H_ */
