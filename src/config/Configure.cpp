/*
 * Configure.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: kevin
 */

#include "Configure.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/exceptions.hpp>

using namespace std;
namespace configure {

string Configuration::configfilename = "./config.info";

MainParamConfiguration Configuration::mainParamConfig;
ServerParamConfiguration Configuration::serverParamConfig;

void Configuration::SetPath(string namepath) { configfilename = namepath; }

void Configuration::ScanPathandUpbulid(string name) {
  using boost::filesystem::path;
  using boost::filesystem::current_path;
  try {
    //当前运行文件路径
    path fullpath = current_path();
    path file_path = fullpath / "config.info";
    if (is_directory(fullpath)) {
      if (!exists(file_path) || (file_size(file_path) == 0)) {
        //配置文件不存在，在当前工作目录下创建config.info写入默认配置
        cout << "配置文件不存在，创建文件写入默认配置" << endl;
      }
    }
  }
  catch (boost::filesystem::filesystem_error& e) {
    std::cout << e.what() << std::endl;
  }
}

bool Configuration::LoadConfigFile() {
  mainParamConfig = MainParamConfiguration();
  serverParamConfig = ServerParamConfiguration();
  using boost::property_tree::ptree;
  ptree pt;
  read_info(Configuration::configfilename, pt);
  try {
    // MainParam
    mainParamConfig.StationName =
        pt.get<string>("MainParam.StationName", "站名");
    mainParamConfig.StorePath =
        pt.get<string>("MainParam.StorePath", "/home/kevin/rtu");
    mainParamConfig.ProgramName =
        pt.get<string>("MainParam.ProgramName", "rtu-a07");
    mainParamConfig.DeviceId = pt.get<int>("MainParam.DeviceId", 0);
    // ServerParam.SerialDebug
    serverParamConfig.sdebugconfig.AvailabilitySign =
        pt.get<bool>("ServerParam.SerialDebug.AvailabilitySign", false);
    serverParamConfig.sdebugconfig.SerialPortNum =
        pt.get<int>("ServerParam.SerialDebug.SerialPortNum", 0);
    serverParamConfig.sdebugconfig.Baud =
        pt.get<int>("ServerParam.SerialDebug.Baud", 12800);
    // ServerParam.Serial232
    serverParamConfig.s232config.AvailabilitySign =
        pt.get<bool>("ServerParam.Serial232.AvailabilitySign", false);
    serverParamConfig.s232config.SerialPortNum =
        pt.get<int>("ServerParam.Serial232.SerialPortNum", 0);
    serverParamConfig.s232config.Baud =
        pt.get<int>("ServerParam.Serial232.Baud", 12800);
    // ServerParam.Serial485
    serverParamConfig.s485config.AvailabilitySign =
        pt.get<bool>("ServerParam.Serial485.AvailabilitySign", false);
    serverParamConfig.s485config.SerialPortNum =
        pt.get<int>("ServerParam.Serial485.SerialPortNum", 0);
    serverParamConfig.s485config.Baud =
        pt.get<int>("ServerParam.Serial485.Baud", 0);
    // ServerParam.SerialGPRS
    serverParamConfig.sgprsconfig.AvailabilitySign =
        pt.get<bool>("ServerParam.SerialGPRS.AvailabilitySign", false);
    serverParamConfig.sgprsconfig.SerialPortNum =
        pt.get<int>("ServerParam.SerialGPRS.SerialPortNum", 0);
    serverParamConfig.sgprsconfig.Baud =
        pt.get<int>("ServerParam.SerialGPRS.Baud", 0);
    // ServerParam.SerialZigbee
    serverParamConfig.szigbeeconfig.AvailabilitySign =
        pt.get<bool>("ServerParam.SerialZigbee.AvailabilitySign", false);
    serverParamConfig.szigbeeconfig.SerialPortNum =
        pt.get<int>("ServerParam.SerialZigbee.SerialPortNum", 0);
    serverParamConfig.szigbeeconfig.Baud =
        pt.get<int>("ServerParam.SerialZigbee.Baud", 0);
    // ServerParam.Net100
    serverParamConfig.n100config.AvailabilitySign =
        pt.get<bool>("ServerParam.n100config.AvailabilitySign", false);
    // ServerParam.Net1000
    serverParamConfig.n1000config.AvailabilitySign =
        pt.get<bool>("ServerParam.n1000config.AvailabilitySign", false);
    // ServerParam.ADIO
    serverParamConfig.adioconfig.AvailabilitySign =
        pt.get<bool>("ServerParam.adioconfig.AvailabilitySign", false);
  }
  catch (boost::property_tree::ptree_error& ex) {
    string r = "读取用户配置时出错：";
    r += ex.what();
    // LOG(r)
  }
  return true;
}

bool Configuration::ReLoadConfigFile() {
  Configuration::LoadConfigFile();
  return true;
}

bool Configuration::SaveConfigFile() {
  // todo:
  return true;
}

Configuration::Configuration() {}

Configuration::~Configuration() {}

} /* namespace configure */
