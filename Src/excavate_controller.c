#include "excavate_controller.h"

#include <FreeRTOS.h>
#include <can_manager.h>
#include <main.h>
#include <rt_conf.h>
#include <stm32f3xx_hal_conf.h>
#include <task.h>

#include "VESC.h"

static int targetSpeed;
VESC* leftMotor;
VESC* rightMotor;

void excavateCANCallbak(rmc_can_msg msg) {
  U32 msg_type = msg.id >> 8u;
  if (msg_type == EXCAVATE_MSG_SET_DIG_SPEED && msg.length >= 1) {
    targetSpeed = msg.buf[0];
  }
}

void excavateControllerFunc(void const* argument) {
  // System setup
  registerCANMsgHandler(EXCAVATE_SYS_ID, &excavateCANCallback);
  leftMotor = create_vesc(EXCAVATE_MOTOR_L, EXCAVATE_MOTOR_POLE_PAIRS);
  rightMotor = create_vesc(EXCAVATE_MOTOR_R, EXCAVATE_MOTOR_POLE_PAIRS);

  TickType_t lastWakeTime;
  while (1) {
    vTaskDelayUntil(&lastWakeTime, EXCAVATE_LOOP_MS * portTICK_RATE_MS);
    F32 currentSpeed = 10.0f;//Arbitrary
    vesc_set_current(leftMotor,currentSpeed);
    vesc_set_current(rightMotor,currentSpeed);
  }
}
