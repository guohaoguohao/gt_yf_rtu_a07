/*
 * SerialGprs.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "SerialGprs.h"

namespace serialgprsn {

SerialGprs::SerialGprs() : config(nullptr) {}

SerialGprs::SerialGprs(const configure::SerialGPRSConfiguration* config)
    : config(config) {}

bool SerialGprs::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
}
void SerialGprs::DoWork() {}

SerialGprs::~SerialGprs() {}

} /* namespace serialgprsn */
