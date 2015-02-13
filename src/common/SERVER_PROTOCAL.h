/*
 * SERVER_PROTOCAL.h
 *
 *  Created on: Feb 2, 2015
 *      Author: kevin
 */

#ifndef SRC_COMMON_SERVER_PROTOCAL_H_
#define SRC_COMMON_SERVER_PROTOCAL_H_

enum class SERVER_PROTOCAL : unsigned int {
  NET_INTERFACE_100M = 0,
  NET_INTERFACE_1000M = 1,
  SERIAL_INTERFACE_232 = 2,
  SERIAL_INTERFACE_485 = 3,
  SERIAL_INTERFACE_DEBUG = 4,
  SERIAL_INTERFACE_GPRS = 5,
  SERIAL_INTERFACE_ZIGBEE_WIA = 6,
  AD_IO = 7
};

#endif /* SRC_COMMON_SERVER_PROTOCAL_H_ */