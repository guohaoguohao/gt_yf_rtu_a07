/**
 * @file Log.h
 * @author GuoHao
 * @date Feb 2, 2015
 */

#ifndef LOG_H_
#define LOG_H_
#include "../common/SERVER_PROTOCAL.h"
#include "../common/LOG_SEVERITY_LEVEL.h"
#include <iostream>
#include <fstream>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/core/record.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/value_visitation.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
using namespace std;

//  Global logger declaration
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(thds_lg, src::severity_logger_mt<>)

struct FilenameLineFormatter {
  typedef void result_type;
  typedef attrs::named_scope::value_type scope_stack;

  explicit FilenameLineFormatter(logging::attribute_name const& name)
      : name_(name) {}
  void operator()(logging::record_view const& rec,
                  logging::formatting_ostream& strm) const {
    logging::visit<scope_stack>(
        name_, rec.attribute_values(),
        boost::bind(&FilenameLineFormatter::format, _1, boost::ref(strm)));
  }

 private:
  static void format(scope_stack const& scopes,
                     logging::formatting_ostream& strm) {
    scope_stack::const_iterator it = scopes.begin(), end = scopes.end();
    for (; it != end; ++it) {
      strm << it->scope_name << " [" << it->file_name << ":" << it->line << "]";
    }
  }
  logging::attribute_name name_;
};

struct TimeStampFormatter {
  typedef void result_type;
  typedef boost::posix_time::ptime clock;
  explicit TimeStampFormatter(logging::attribute_name const& name)
      : name_(name) {}
  void operator()(logging::record_view const& rec,
                  logging::formatting_ostream& strm) const {
    logging::visit<clock>(
        name_, rec.attribute_values(),
        boost::bind(&TimeStampFormatter::format, _1, boost::ref(strm)));
  }

 private:
  static void format(clock const& ts, logging::formatting_ostream& strm) {
    strm << boost::posix_time::to_iso_extended_string(ts);
  }
  logging::attribute_name name_;
};

class FilenameLineFormatterFactory : public logging::formatter_factory<char> {
 public:
  formatter_type create_formatter(logging::attribute_name const& attr_name,
                                  args_map const& args) {
    return formatter_type(FilenameLineFormatter(attr_name));
  }
};

class TimeStampFormatterFactory : public logging::formatter_factory<char> {
 public:
  formatter_type create_formatter(logging::attribute_name const& attr_name,
                                  args_map const& args) {
    return formatter_type(TimeStampFormatter(attr_name));
  }
};


/**
 * @brief 调用的INFO日志宏，存入Main文件中，
 * 可从传入的参数中获取该控制器或设备的指针中获取该指针的名称
 * @param a 调用方控制器或设备的指针，一般用法为直接传this
 * @param b 要记录的文字内容
 */
#define LOG_COM_INFO(a, b)                                         \
  do {                                                                  \
    BOOST_LOG_FUNCTION();                                               \
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", "Main");                         \
    BOOST_LOG_SEV(thds_lg::get(), info) << (((a))->getTraceName() + b); \
  } while (0)
/**
 * @brief 供控制器及设备调用的TRACE日志宏，存入Main文件中，
 * 可从传入的参数中获取该控制器或设备的指针中获取该指针的名称
 * @param a 调用方控制器或设备的指针，一般用法为直接传this
 * @param b 要记录的文字内容
 */
#define LOG_COM_TRACE(a, b)                                         \
  do {                                                                   \
    BOOST_LOG_FUNCTION();                                                \
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", "Main");                          \
    BOOST_LOG_SEV(thds_lg::get(), trace) << (((a))->getTraceName() + b); \
  } while (0)
/**
 * @brief 供控制器及设备调用的ERROR日志宏，存入Main文件中，
 * 可从传入的参数中获取该控制器或设备的指针中获取该指针的名称
 * @param a 调用方控制器或设备的指针，一般用法为直接传this
 * @param b 要记录的文字内容
 */
#define LOG_COM_ERR(a, b)                                           \
  do {                                                                   \
    BOOST_LOG_FUNCTION();                                                \
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", "Main");                          \
    BOOST_LOG_SEV(thds_lg::get(), error) << (((a))->getTraceName() + b); \
  } while (0)


//非守护对象，服务对象，调用不带有this参数的日志宏
#define LOG_INFO(a)                             \
  do {                                          \
    BOOST_LOG_FUNCTION();                       \
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", "Main"); \
    BOOST_LOG_SEV(thds_lg::get(), info) << (a); \
  } while (0);
#define LOG_TRACE(a)                             \
  do {                                           \
    BOOST_LOG_FUNCTION();                        \
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", "Main");  \
    BOOST_LOG_SEV(thds_lg::get(), trace) << (a); \
  } while (0);
#define LOG_ERR(a)                               \
  do {                                           \
    BOOST_LOG_FUNCTION();                        \
    BOOST_LOG_SCOPED_THREAD_TAG("Tag", "Main");  \
    BOOST_LOG_SEV(thds_lg::get(), error) << (a); \
  } while (0);

namespace utility {

/**
 * @brief 日志类
 * @details 提供日志记录功能
 * 使用：1.在main时创建奔类实例，调用Init函数，通过配置文件初始化日志模块
 * 2.在需要时，调用本文件提供的宏来实现日志记录功能
 */
class Log {
 public:
  /**
   * @brief 构造函数
   */
  Log();

  /**
   * @brief 析构函数
   */
  virtual ~Log();

  /**
   * @brief
   * 初始化函数，用来从配置文件中读取日志记录的配置信息，并初始化日志实例
   * @return 若日志模块初始化成功，返回true
   */
  bool Init();
};

} /* namespace utility */
#endif /* LOG_H_ */
