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

#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <stdbool.h>
#include <stdio.h>

void SetupUartForDs18b20(void);

bool Ds18b20SendByte(const uint8_t sendByte, uint8_t* const readByte);

bool Ds18b20ReadByte(uint8_t* byte);

bool Ds18b20SendSkipRom(void);

bool Ds18b20StartTempConversion(void);

bool Ds18b20ReadScratchpad(void);

bool Ds18b20Reset(void);

bool Ds18b20ReadTemperature(void);

uint16_t Ds18b20GetLastTemperature(void);

#endif
