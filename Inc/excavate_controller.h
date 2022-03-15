#ifndef NASA_RMC_RT_EXCAVATE_CONTROLLER_H
#define NASA_RMC_RT_EXCAVATE_CONTROLLER_H

#include "can_manager.h"

#define ACHOO_MOTOR_POLE_PAIRS 7

#define EXCAVATE_MSG_SET_DIG_SPEED  50u

#define EXCAVATE_LOOP_MS 20u

void excavateCANCallback(rmc_can_msg msg);


#endif