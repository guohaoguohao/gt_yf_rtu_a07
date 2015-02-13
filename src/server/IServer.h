/*
 * IServer.h
 *
 *  Created on: Feb 2, 2015
 *      Author: kevin
 */

#ifndef SRC_SERVER_ISERVER_H_
#define SRC_SERVER_ISERVER_H_
#include <thread>
#include <atomic>
namespace daemon_rtu {
class IDaemon;
}
using namespace daemon_rtu;

namespace server {

class IServer {
 public:
  IServer();
  virtual ~IServer();
  /**
  * @brief
  * 纯需初始化方法，在创建本类继承类后必须调用本方法，只有当返回true时才可继续下一步操作
  * @return 初始化成功返回true，否则返回false
  */
  virtual bool Init() = 0;

  /**
  * @brief 启动服务方法
  * @details
  * 基类的Start方法中包括判断是否成功初始化、设置运行标志为true，启动主控线程的功能，
  * 继承类若要修改Start方法，自行实现（覆盖）本方法，
  * 也可在实现方法中通过IController::Start()调用本基类的Start方法
  */
  virtual void Start();

  /**
  * @brief
  * 停止服务方法，本方法为异步停止方法，调用后立即返回，服务是否真实停止未知
  * @details
  * 基类的Stop方法提供了设置运行标志为false的功能，继承类可视情况选择是否实现（覆盖）本方法
  */
  virtual void Stop();

  /**
  * @brief 停止控制器方法，本方法为同步停止方法，
  * 调用后调用方会被阻塞，直到守护服务主控线程停止后，方法解除阻塞
  * @details 基类的StopandWait方法提供了设置运行标志为false的功能，
  * 继承类可视情况选择是否实现（覆盖）本方法
  */
  virtual void StopandWait();

  /**
  * @brief 禁止本类的复制构造函数
  */
  IServer(IServer const &) = delete;

  /**
  * @brief 禁止复制运算符
  * @param 被复制的对象
  * @return 复制后对象
  */
  IServer &operator=(IServer const &) = delete;
  /**
  * @brief 获取初始化状态
  * @return true表示初始化成功，反之为false
  */
  bool isInitialized() const { return initialized; }

  /**
  * @brief 设置初始化状态
  * @param initialized 初始化状态，true表示成功，false表示失败
  */
  void setInitialized(bool initialized) { this->initialized = initialized; }

  /**
  * @brief 通过服务的基类获取本服务所属守护对象指针的函数
  * @return 本服务所属守护
  */
  IDaemon *getDaemon() const { return daemon; }

  /**
  * @brief 设置本控制器所属的设备的函数
  * @param device 本控制器所属设备指针
  */
  void setDaemon(IDaemon *daemon) { this->daemon = daemon; }

  /**
  * @brief 获取本服务在记录日志时输出的简要名称
  * @return 本服务的简要名称
  */
  std::string getTraceName();

  /**
  * @brief INLINE函数，获取本服务的简要名称
  * @return 本服务的简要名称
  */
  inline virtual std::string getNameAbbr() { return "未定义."; }

 protected:
  /**
  * @brief 纯需的工作函数，继承类需实现本函数
  */
  virtual void DoWork() = 0;

  /**
  * @brief
  * 运行标志位，true表示正在运行，false表示停止运行，为了能够被其他线程控制，因此采用原子类型，本标志位线程安全
  */
  std::atomic_bool running;

 private:
  /**
  * @brief 初始化标志，false表示尚未初始化或初始化失败，true表示初始化成功
  */
  bool initialized = false;
  /**
  * @brief 本类的线程实例，本类所有继承类提供的服务都在worker中承载
  */
  std::thread worker;
  /**
  * @brief 存储哪个守护实例创建的服务
  */
  IDaemon *daemon;
};

} /* namespace server */

#endif /* SRC_SERVER_ISERVER_H_ */
