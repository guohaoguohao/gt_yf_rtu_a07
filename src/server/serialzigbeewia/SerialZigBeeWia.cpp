/*
 * SerialZigBeeWia.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "SerialZigBeeWia.h"

namespace serialzigbeewian {

SerialZigBeeWia::SerialZigBeeWia() : config(nullptr) {}

SerialZigBeeWia::SerialZigBeeWia(const configure::SerialZigBeeConfiguration* config)
    : config(config) {}

SerialZigBeeWia::~SerialZigBeeWia() {}

bool SerialZigBeeWia::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
}

void SerialZigBeeWia::DoWork() {
  LOG_COM_INFO(this, "启动");
  while(this->running) {

  }
}

} /* namespace serialzigbeewian */
