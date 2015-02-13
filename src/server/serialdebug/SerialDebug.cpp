/*
 * SerialDebug.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "SerialDebug.h"

namespace serialdebugn {

SerialDebug::SerialDebug() : config(nullptr) {}

SerialDebug::SerialDebug(const configure::SerialDebugConfiguration* config)
    : config(config) {}

bool SerialDebug::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
  return true;
}

void SerialDebug::DoWork() {}

SerialDebug::~SerialDebug() {}

} /* namespace serialdebugn */
