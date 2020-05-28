/*!
 * \file      LoRa.h
 *
 * \brief     LoRaStateMachine based on LoRaMac classA device implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Simon Hoinkis
 */

#ifndef __LORA_H__
#define __LORA_H__

#include "LoRaMac.h"

#ifndef ACTIVE_REGION

#define ACTIVE_REGION LORAMAC_REGION_EU868

#endif

/*!
 * When set to 1 the application uses the Over-the-Air activation procedure
 * When set to 0 the application uses the Personalization activation procedure
 */
#define OVER_THE_AIR_ACTIVATION 1

/*!
 * Indicates if the end-device is to be connected to a private or public network
 */
#define LORAWAN_PUBLIC_NETWORK true

/*!
 * Default datarate
 */
#define LORAWAN_DEFAULT_DATARATE DR_0

/*!
 * LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON false

/*!
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_ON 1

/*!
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT 2

/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE 242

/*!
 * User application data size
 */
extern uint8_t AppDataSize;

/*!
 * Defines the duty cycle before the node has joined the network. value in [ms].
 */
#define APP_TX_DUTYCYCLE_JOIN_MS 5000

/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND_MS 1000

/*!
 * Device states
 */
typedef enum
{
    DEVICE_STATE_RESTORE,
    DEVICE_STATE_START,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_CYCLE_JOIN,
    DEVICE_STATE_CYCLE_RUN,
    DEVICE_STATE_SLEEP
} eDeviceState;

/*!
 *
 */
typedef enum
{
    LORAMAC_HANDLER_UNCONFIRMED_MSG = 0,
    LORAMAC_HANDLER_CONFIRMED_MSG = !LORAMAC_HANDLER_UNCONFIRMED_MSG
} LoRaMacHandlerMsgTypes_t;

/*!
 * Application data structure
 */
typedef struct LoRaMacHandlerAppData_s
{
    LoRaMacHandlerMsgTypes_t MsgType;
    uint8_t Port;
    uint8_t BufferSize;
    uint8_t* Buffer;
} LoRaMacHandlerAppData_t;

/*!
 * \brief
 */
void OnMacProcessNotify(void);

/*!
 * \brief   MLME-Indication event function
 *
 * \param   [IN] mlmeIndication - Pointer to the indication structure.
 */
void MlmeIndication(MlmeIndication_t* mlmeIndication);

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] mlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
void MlmeConfirm(MlmeConfirm_t* mlmeConfirm);

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
void McpsIndication(McpsIndication_t* mcpsIndication);

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] mcpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
void McpsConfirm(McpsConfirm_t* mcpsConfirm);

/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
void OnTxNextPacketTimerEvent(void* context);

/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
bool SendFrame(void);

/*!
 * Executes the network Join request
 */
void JoinNetwork(void);

/*!
 * Prints the provided buffer in HEX
 *
 * \param buffer Buffer to be printed
 * \param size   Buffer size to be printed
 */
void PrintHexBuffer(uint8_t* buffer, uint8_t size);
extern void PrepareTxFrame(uint8_t port);
extern void ParseRxFrame(uint8_t* buffer, uint8_t size);

/*!
 * Main state machine of the node
 * @note Will never return
 */
void RunLoRaStateMachine(uint8_t devEui[],
                         uint8_t joinEui[],
                         uint8_t NwkKey[],
                         uint32_t dutyCycle);

#endif
