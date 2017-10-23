/*
 * lightos_config.h
 *
 *  Created on: 2017.1.25
 *      Author: xzy47
 */

#ifndef INCLUDE_LIGHTOS_CONFIG_H_
#define INCLUDE_LIGHTOS_CONFIG_H_

#define LCONFIG_OS_TIMESLICE 5

#define LCONFIG_TASK_MAX_NUMBER 4
#define LCONFIG_TASK_MAX_PRIORITY 8

#define LPortPendSVHandler PendSV_Handler

#define LCONFIG_BYTE_ALIGNMENT 8

#define LCONFIG_TASK_PRIVILEGE_LEVELS

#define LCONFIG_IDLE_STACK_DEPTH 16

#endif /* INCLUDE_LIGHTOS_CONFIG_H_ */
