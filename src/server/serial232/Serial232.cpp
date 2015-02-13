/*
 * Serial232.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "Serial232.h"
#include <thread>
#include <chrono>
using namespace std;
namespace serial232n {

Serial232::Serial232() : config(nullptr) {}

Serial232::Serial232(const configure::Serial232Configuration* config)
    : config(config) {}

bool Serial232::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
  this->setInitialized(true);
  return true;
}

void Serial232::DoWork() {
  LOG_COM_INFO(this,"启动");
  while(this->running) {
    this_thread::sleep_for(chrono::seconds(1));
  }
}

Serial232::~Serial232() {}

} /* namespace serial232n */
