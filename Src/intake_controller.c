
#include "intake_controller.h"

#include <FreeRTOS.h>
#include <can_manager.h>
#include <main.h>
#include <rt_conf.h>
#include <stm32f3xx_hal_conf.h>
#include <task.h>

#include "VESC.h"

static IntakeState targetState = UP;
static IntakeState currentState = UP;

VESC* intakeMotor;
void intakeCANCallback(rmc_can_msg msg) {
  U32 msg_type = msg.id >> 8u;
  if (msg_type == INTAKE_MSG_SET_POSITION && msg.length >= 1) {
    U8 param = msg.buf[0];
    switch (param) {
      case 1:
        targetState = DOWN;
        break;
      default:
      case 0:
        targetState = UP;
    }
  }
}
void intakeControllerFunc(void const* argument) {
  registerCANMsgHandler(INTAKE_SYS_ID, &intakeCANCallback);
  intakeMotor = create_vesc(INTAKE_MOTOR, INTAKE_MOTOR_POLE_PAIRS);
  TickType_t lastWakeTime;
  while (1) {
    vTaskDelayUntil(&lastWakeTime, INTAKE_LOOP_MS * portTICK_RATE_MS);
    if (targetState == DOWN &&
        (currentState == UP || currentState == MOVING_UP)) {
      currentState = MOVING_DOWN;
    } else if (targetState == UP &&
               (currentState == DOWN || currentState == MOVING_DOWN)) {
      currentState = MOVING_UP;
    }
    //I don't know where this be at
    bool lowLimitR =
        HAL_GPIO_ReadPin(INTAKE_LimitRL_GPIO_Port, INTAKE_LimitRL_Pin) == 0;
    bool lowLimitL =
        HAL_GPIO_ReadPin(INTAKE_LimitLL_GPIO_Port, INTAKE_LimitLL_Pin) == 0;
    bool highLimitR =
        HAL_GPIO_ReadPin(INTAKE_LimitRH_GPIO_Port, INTAKE_LimitRH_Pin) == 0;
    bool highLimitL =
        HAL_GPIO_ReadPin(INTAKE_LimitLH_GPIO_Port, INTAKE_LimitLH_Pin) == 0;
    // Check if our movement has completed
    if (lowLimitR && lowLimitL && currentState == MOVING_DOWN) {
      currentState = DOWN;
    }
    if (highLimitR && highLimitL && currentState == MOVING_UP) {
      currentState = UP;
    }

    // Set VESC movement
    F32 currentR = 0.0f;
    if (currentState == MOVING_UP && !highLimitR) {
      currentR = 10.0f;
    } else if (currentState == MOVING_DOWN && !lowLimitR) {
      currentR = -10.0f;
    }
    vesc_set_current(intakeMotor, currentR);

    // Send status message
    U8 data[1];
    data[0] = currentState;
    do_send_can_message((INTAKE_MSG_STATUS << 8u) | INTAKE_SYS_ID, data, 1);
  }
}