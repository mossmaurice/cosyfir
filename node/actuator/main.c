/*!
 * \file      main.c
 *
 * \brief     cosyfir-actuator-node based on LoRaMac classA device
 * implementation
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

#include "Commissioning.h"
#include "LoRa.h"
#include "board.h"

#include <stdio.h>

/*!
 * Defines the application data transmission duty cycle. value in [ms].
 */
#ifdef DEBUG
#define APP_TX_DUTYCYCLE_RUN_MS 30000
#else
#define APP_TX_DUTYCYCLE_RUN_MS 1800000
#endif

/*!
 * User application data size
 */
uint8_t AppDataSize = 1;

extern uint8_t AppDataBuffer[];

/*!
 * \brief   Prepares the payload of the frame
 */
void PrepareTxFrame(uint8_t port)
{
    switch (port)
    {
    case 2:
    {
        AppDataBuffer[0] = BoardGetBatteryLevel();
    }
    break;
    default:
        break;
    }
}

/*!
 * \brief   Called after a downlink message was received, in order to process it
 */
void ParseRxFrame(uint8_t* buffer, uint8_t size)
{
    /// @todo Parse downlink message from server here and activate valve
}

void BoardInitActuatorNode(void)
{
}

/**
 * Main application entry point.
 */
int main(void)
{
    uint8_t devEui[] = LORAWAN_DEVICE_EUI;
    uint8_t joinEui[] = LORAWAN_JOIN_EUI;
    uint8_t NwkKey[] = LORAWAN_NWK_KEY;

    BoardInitMcu();
    BoardInitPeriph();
    BoardInitActuatorNode();

    /// @todo take version string from version.h.in
    printf("###### ===== cosyfir-actuator-node v0.1.0 ==== ######\r\n\r\n");

    RunLoRaStateMachine(devEui, joinEui, NwkKey, APP_TX_DUTYCYCLE_RUN_MS);
}
