/*
 * IDaemon.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: kevin
 */

#include <iostream>
#include <algorithm>
#include "IDaemon.h"
using namespace std;
namespace daemon_rtu {

IDaemon::IDaemon()
    : initialized(false),
      enabled(false),
      running(false),
      parent(nullptr),
      id(-1) {}

IDaemon::~IDaemon() { parent = nullptr; }

bool IDaemon::Start() {
  for (auto &server : this->servers) {
    server->Start();
    this->running = true;
  }
  this->setRunning(true);
  return true;
}

void IDaemon::Stop() {
  if (this->initialized && this->running) {
    for(auto& server : this->servers) {
      server->Stop();
    }
  }
  this->setRunning(false);
}

void IDaemon::StopWait() {
  if (this->initialized && this->running) {
    for (auto &server : this->servers) {
      cout << getNameAbbr()  << " " << server->getNameAbbr() << " stopping" << endl;
      server->StopandWait();
      cout << getNameAbbr()  << " " << server->getNameAbbr() << " stopped" << endl;
    }
  }
  this->setRunning(false);
}

void IDaemon::RegisterController(shared_ptr<IServer> server) {
  this->servers.push_back(server);
}

void IDaemon::RemoveRegisteredController(shared_ptr<IServer> server) {
  this->servers.erase(
      std::remove_if(
          this->servers.begin(), this->servers.end(),
          [server](shared_ptr<IServer> p) { return p == server; }),
      this->servers.end());
}

inline string IDaemon::getNameAbbr() { return ""; }

string IDaemon::getTraceName() {
  string ret = this->getNameAbbr();
  /**
   * 下面这个while判断条件是，当parent不为空，且不是顶级节点时，记录对应的名字
   */
  IDaemon *p = this->parent;
  while (p != NULL) {
    ret = ((p)->getNameAbbr()) + ret;
    p = (p)->parent;
  }
  return ret;
}

} /* namespace daemon_rtu */
