/*!
 * \file      main.c
 *
 * \brief     cosyfir-sensor-node based on LoRaMac classA device implementation
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
#include "adc-board.h"
#include "board.h"
#include "ds18b20.h"

#include <stdio.h>

/// @todo Future GPIO config for sensor nodes
#define ADC_1 PA_0
#define ADC_2 PA_1
#define ADC_3 PA_4

Adc_t AdcWatermark;

/*!
 * Defines the application data transmission duty cycle. value in [ms].
 */
#ifdef DEBUG
#define APP_TX_DUTYCYCLE_RUN_MS 30000
#else
#define APP_TX_DUTYCYCLE_RUN_MS 1800000
#endif

uint8_t AppDataSize = 3;

extern uint8_t AppDataBuffer[];

void BoardInitSensorNode(void)
{
    /// @todo fix second param ADC_CHANNEL_1
    AdcInit(&AdcWatermark, NC);
    SetupUartForDs18b20();
}

/*!
 * \brief   Prepares the payload of the frame
 */
void PrepareTxFrame(uint8_t port)
{
    switch (port)
    {
    case 2:
    {
        if (!Ds18b20ReadTemperature())
        {
            printf("Could not read temperature\n");
        }

        AppDataBuffer[0] = BoardGetBatteryLevel();
        AppDataBuffer[1] = Ds18b20GetLastTemperature() >> 8;
        AppDataBuffer[2] = Ds18b20GetLastTemperature() & 0xFF;
        /// @todo Read watermark values here
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
    // We do nothing here
    // Poweroff? Reset? Change of cycle time?
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
    BoardInitSensorNode();

    /// @todo take version string from version.h.in
    printf("###### ===== cosyfir-sensor-node v0.1.0 ==== ######\r\n\r\n");

    RunLoRaStateMachine(devEui, joinEui, NwkKey, APP_TX_DUTYCYCLE_RUN_MS);
}
