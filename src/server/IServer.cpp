/*
 * IServer.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: kevin
 */

#include "IServer.h"
#include "../daemon/Rtu.h"
using namespace std;
using namespace daemon_rtu;
namespace server {

IServer::IServer() : daemon(nullptr){}

IServer::~IServer() {}

void IServer::Start() {
  if (this->initialized) {
    this->running = true;
    this->worker = thread([this] { DoWork(); });
  }
}

void IServer::Stop() {
  if (this->running) {
    this->running = false;
  }
}

void IServer::StopandWait() {
  if (this->running) {
    this->running = false;
    if (this->worker.joinable()) {
      this->worker.join();
    }
  }
}

string IServer::getTraceName() {
  string ret = this->getNameAbbr();
  ret = this->getDaemon()->getTraceName() + "#" + ret;
  return ret;
}

} /* namespace server */
