#ifndef NASA_RMC_RT_INTAKE_CONTROLLER_H
#define NASA_RMC_RT_INTAKE_CONTROLLER_H

#include "can_manager.h"

typedef enum { UP=0, MOVING_UP, MOVING_DOWN, DOWN } IntakeState;

#define INTAKE_MSG_SET_POSITION 40u
#define INTAKE_MSG_STATUS 41u

#define INTAKE_LOOP_MS 20u

void intakeCANCallback(rmc_can_msg msg);

IntakeState getIntakeState();

#endif