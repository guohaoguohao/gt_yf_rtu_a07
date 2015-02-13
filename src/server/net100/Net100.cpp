/*
 * Net100.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "../net100/Net100.h"

namespace net100m {

Net100::Net100() : config(nullptr) {}

Net100::Net100(const configure::Net100Configuration* config) : config(config) {}

bool Net100::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
}

void Net100::DoWork() {}

Net100::~Net100() {}

} /* namespace net100m */
