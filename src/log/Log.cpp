#include "Log.h"

namespace utility {
Log::Log() {}

bool Log::Init() {
  logging::register_formatter_factory(
      "FNLN", boost::make_shared<FilenameLineFormatterFactory>());
  logging::register_formatter_factory(
      "TimeStamp", boost::make_shared<TimeStampFormatterFactory>());
  std::ifstream settings("logger.conf");
  if (!settings.is_open()) {
    cout << "日志配置文件无法打开，请检查." << endl;
    return false;
  }

  // Read the settings and initialize logging library
  logging::init_from_stream(settings);
  logging::add_common_attributes();
  logging::core::get()->add_global_attribute("FNLN", attrs::named_scope());

  return true;
}
Log::~Log() {}
}/* namespace utility */



