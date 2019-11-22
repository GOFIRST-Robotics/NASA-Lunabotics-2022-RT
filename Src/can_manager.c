//
// Created by nick on 10/22/19.
//

#include <stm32f3xx.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "queue.h"
#include "task.h"

#include "can_manager.h"
#include "string.h"

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart2;
uint32_t TxMailbox;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
CANMsgHandlerPair msgHandlers[CAN_HANDLERS_SIZE];
int canHandlersCt = 0;

void do_send_can_message(unsigned int id, uint8_t* buf, int length) {
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.ExtId = id;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.DLC = length;
  TxHeader.TransmitGlobalTime = DISABLE;
  // todo mutex?
  int retval = HAL_CAN_AddTxMessage(&hcan, &TxHeader, buf, &TxMailbox);
}

void registerCANMsgHandler(unsigned int mask,
                           void (*callback)(rmc_can_msg msg)) {
  msgHandlers[canHandlersCt++] =
      (CANMsgHandlerPair){.mask = mask, .callback = callback};
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
  /* Get RX message */
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
    /* Reception Error */
    Error_Handler();
  }
  rmc_can_msg msg;
  msg.id = RxHeader.ExtId;
  msg.length = RxHeader.DLC;
  memcpy(&msg.buf, RxData, msg.length);
  for (int i = 0; i < canHandlersCt; ++i) {
    CANMsgHandlerPair handlerPair = msgHandlers[i];
    if (msg.id & handlerPair.mask) {
      handlerPair.callback(msg);
    }
  }
}