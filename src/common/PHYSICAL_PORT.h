/*
 * PHYSICAL_ PORT.h
 *
 *  Created on: Feb 15, 2015
 *      Author: kevin
 */

#ifndef SRC_COMMON_PHYSICAL_PORT_H_
#define SRC_COMMON_PHYSICAL_PORT_H_

enum class PHYSICAL_PORT : int {
  TCP = 0,
  TCP_PI = 1,
  RTU = 2
};

enum class SERVER_OR_CLIENT : int {
  SERVER = 0,
  CLIENT = 1
};



#endif /* SRC_COMMON_PHYSICAL_PORT_H_ */
