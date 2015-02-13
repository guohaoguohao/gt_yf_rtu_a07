/*
 * Net1000.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "../net1000/Net1000.h"

namespace net1000m {

Net1000::Net1000() : config(nullptr) {}

Net1000::Net1000(const configure::Net1000Configuration* config) : config(config) {}

bool Net1000::Init() {
  LOG_COM_INFO(this, "初始化");
  LOG_COM_INFO(this, "初始化完成");
}

void Net1000::DoWork() {}

Net1000::~Net1000() {}

} /* namespace net1000m */
