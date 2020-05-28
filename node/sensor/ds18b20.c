// Copyright 2020 Simon Hoinkis
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ds18b20.h"
#include "board-config.h"
#include "delay.h"
#include "uart.h"

Uart_t Uart1;

/*!
 * Temperature measured via DS18B20
 *
 * \remark Resolution is 0.0625 degree celcius (default)
 */
static uint16_t LastMeasuredTemperature = 0;

void SetupUartForDs18b20(void)
{
    UartSingleWireInit(&Uart1, UART_1, UART1_TX, UART1_RX);

    // Init for first phase: reset and presence pulse
    UartSingleWireConfig(&Uart1,
                         RX_TX,
                         9600,
                         UART_8_BIT,
                         UART_1_STOP_BIT,
                         NO_PARITY,
                         NO_FLOW_CTRL);
}

bool Ds18b20SendByte(const uint8_t sendByte, uint8_t* const readByte)
{
    uint8_t txBuffer[8];
    uint8_t rxBuffer[8];

    // Send single byte for each bit
    for (int i = 0; i < 8; i++)
    {
        if (sendByte & (1 << i))
        {
            txBuffer[i] = 0xFF;
        }
        else
        {
            txBuffer[i] = 0x00;
        }
    }

    if (!UartSingleWireTxRx(txBuffer, rxBuffer, 8))
    {
        return false;
    }

    if (readByte != NULL)
    {
        uint8_t answer = 0;

        for (int i = 0; i < 8; i++)
        {
            if (rxBuffer[i] == 0xFF)
            {
                answer |= 0x01 << i;
            }
        }
        *readByte = answer;
    }

    return true;
}

bool Ds18b20ReadByte(uint8_t* byte)
{
    // To read over single wire, send 1's and see what ds18b20 transmits
    return Ds18b20SendByte(0xFF, byte);
}

bool Ds18b20SendSkipRom(void)
{
    // Send command which will skip specific selection of device
    // if multiple DS18B20 are connected
    return Ds18b20SendByte(0xCC, NULL);
}

bool Ds18b20StartTempConversion(void)
{
    return Ds18b20SendByte(0x44, NULL);
}

bool Ds18b20ReadScratchpad(void)
{
    return Ds18b20SendByte(0xBE, NULL);
}

bool Ds18b20Reset(void)
{
    // Send reset and presence pulse
    UartSingleWireConfig(&Uart1,
                         RX_TX,
                         9600,
                         UART_8_BIT,
                         UART_1_STOP_BIT,
                         NO_PARITY,
                         NO_FLOW_CTRL);

    uint8_t answer = 0;
    uint8_t resetRequest = 0xF0;

    if (!UartSingleWireTxRx(&resetRequest, &answer, 1))
    {
        return false;
    }

    if (answer == 0xF0)
    {
        // Reset didn't work
        return false;
    }

    // Init for transmission phase, read and write slots
    UartSingleWireConfig(&Uart1,
                         RX_TX,
                         115200,
                         UART_8_BIT,
                         UART_1_STOP_BIT,
                         NO_PARITY,
                         NO_FLOW_CTRL);

    return true;
}

bool Ds18b20ReadTemperature(void)
{
    // Start the temperature conversion
    if (!Ds18b20Reset())
    {
        LastMeasuredTemperature = 0;
        return false;
    }
    Ds18b20SendSkipRom();
    Ds18b20StartTempConversion();

    DelayMs(750);

    // Read the measured temperature from the sensor
    if (!Ds18b20Reset())
    {
        LastMeasuredTemperature = 0;
        return false;
    }
    Ds18b20SendSkipRom();
    Ds18b20ReadScratchpad();

    uint8_t lsb, msb;
    if (!Ds18b20ReadByte(&lsb) || !Ds18b20ReadByte(&msb))
    {
        // Reading the temperature failed
        LastMeasuredTemperature = 0;
        return false;
    }

    LastMeasuredTemperature = (msb << 0x08) | lsb;

    return true;
}

uint16_t Ds18b20GetLastTemperature(void)
{
    return LastMeasuredTemperature;
}
