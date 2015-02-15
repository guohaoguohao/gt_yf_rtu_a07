//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : GT-YF-RTU-A07 in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <exception>
#include <memory>
#include "log/Log.h"
#include "config/Configure.h"
#include "daemon/Rtu.h"
using namespace std;
using namespace utility;
using namespace rtu;
int main(int argc, char** argv) {
  //控制台多线程输出
  cout.sync_with_stdio(true);
  try {
    cout << "****************************" << endl;
    cout << "you can type \"quit\" to exit" << endl;
    cout << "****************************" << endl;
    cout << "you can type \"status\" to query" << endl;
    cout << "type something to do something............" << endl;
    // 初始化日志
    Log log;
    log.Init();
    LOG_INFO("**************守护进程启动*****************");
    if (!configure::Configuration::LoadConfigFile()) {
      LOG_ERR("读取配置信息出错，程序无法正常运行");
      // todo:something
    }
    shared_ptr<Rtu> instance = make_shared<Rtu>();
    instance->config = &configure::Configuration::getMainParamConfiguration();
    instance->sconfig = std::make_tuple(
        &configure::Configuration::getServerParamConfiguration().n100config,
        &configure::Configuration::getServerParamConfiguration().n1000config,
        &configure::Configuration::getServerParamConfiguration().s232config,
        &configure::Configuration::getServerParamConfiguration().s485config,
        &configure::Configuration::getServerParamConfiguration().sdebugconfig,
        &configure::Configuration::getServerParamConfiguration().sgprsconfig,
        &configure::Configuration::getServerParamConfiguration().szigbeeconfig,
        &configure::Configuration::getServerParamConfiguration().adioconfig);
    if (!instance->Init()) {
      LOG_ERR("rtu初始化失败!");
      // todo:something
      return -1;
    }
    //启动守护进程提供的服务
    instance->Start();
    string input;
    while (true) {
      getline(cin, input);
      if (input == "quit") {
        instance->Stop();
        break;
      } else if (input == "state") {
        LOG_INFO("这个功能还没有实现^o^");
      }
    }
    LOG_INFO("程序正常退出。");
    LOG_INFO("************Exit without exception***********");
    exit(EXIT_SUCCESS);
  }
  catch (std::exception& e) {
    string msg = "程序异常退出。";
    msg += e.what();
    LOG_ERR(msg);
    LOG_ERR("************ERROR Exit*************");
    // todo:一些辨别异常问题的操作，例如存一个信息文件
    exit(EXIT_FAILURE);
  }
  catch (...) {
    LOG_ERR("程序异常退出，且无法捕获异常信息。");
    LOG_ERR("************ERROR Exit*************");
    exit(EXIT_FAILURE);
  }
}
