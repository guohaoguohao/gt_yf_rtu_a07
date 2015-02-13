/*
 * Serial485.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "Serial485.h"

namespace serial485n {

Serial485::Serial485() : config(nullptr) {}

Serial485::Serial485(const configure::Serial485Configuration* config)
    : config(config) {}

bool Serial485::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
}

void Serial485::DoWork() {}

Serial485::~Serial485() {}

} /* namespace serial485n */
