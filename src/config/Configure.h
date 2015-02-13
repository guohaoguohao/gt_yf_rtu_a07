/*
 * Configure.h
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#ifndef SRC_CONFIG_CONFIGURE_H_
#define SRC_CONFIG_CONFIGURE_H_
#include <string>
namespace configure {
/*************MainParam*****************/
typedef struct {
  std::string StorePath;
  std::string ProgramName;
  int DeviceId;
  std::string StationName;
}MainParamConfiguration;

/*************ServerParam*****************/
typedef struct {
  bool AvailabilitySign;
  int SerialPortNum;
  int Baud;
}SerialDebugConfiguration;

typedef struct {
  bool AvailabilitySign;
  int SerialPortNum;
  int Baud;
}Serial232Configuration;

typedef struct {
  bool AvailabilitySign;
  int SerialPortNum;
  int Baud;
}Serial485Configuration;

typedef struct {
  bool AvailabilitySign;
  int SerialPortNum;
  int Baud;
}SerialGPRSConfiguration;

typedef struct {
  bool AvailabilitySign;
  int SerialPortNum;
  int Baud;
}SerialZigBeeConfiguration;

typedef struct {
  bool AvailabilitySign;
}Net100Configuration;

typedef struct {
  bool AvailabilitySign;
}Net1000Configuration;

typedef struct {
  bool AvailabilitySign;
}ADIOConfiguration;

typedef struct {
  SerialDebugConfiguration sdebugconfig;
  Serial232Configuration s232config;
  Serial485Configuration s485config;
  SerialGPRSConfiguration sgprsconfig;
  SerialZigBeeConfiguration szigbeeconfig;
  Net100Configuration n100config;
  Net1000Configuration n1000config;
  ADIOConfiguration adioconfig;
}ServerParamConfiguration;

class Configuration {
 public:
  Configuration();
  virtual ~Configuration();
  //加载配置文件
  static bool LoadConfigFile();
  //重新加载配置文件
  static bool ReLoadConfigFile();
  //保存配置文件.运行中动态修改配置参数保存配置文件，完成后需重新加载配置文件，重启服务
  static bool SaveConfigFile();
  //设置用户配置文件绝对路径
  static void SetPath(std::string namepath);
  //扫描文件加载默认配置
  static void ScanPathandUpbulid(std::string name);

  static const MainParamConfiguration& getMainParamConfiguration() {
    return mainParamConfig;
  }

  static const ServerParamConfiguration& getServerParamConfiguration() {
    return serverParamConfig;
  }

 private:
  static std::string configfilename;
  static MainParamConfiguration mainParamConfig;
  static ServerParamConfiguration serverParamConfig;

};

} /* namespace configure */

#endif /* SRC_CONFIG_CONFIGURE_H_ */
