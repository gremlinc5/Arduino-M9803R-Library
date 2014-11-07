/**
@file
Arduino library for communicating with M9803R Multimeter over RS232.
*/
/*

  M9803R.cpp - Arduino library for communicating with M9803R Multimeter
  over RS232.
  
  This file is part of M9803R.
  
  M9803R is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  M9803R is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with M9803R.  If not, see <http://www.gnu.org/licenses/>.
  
  Written by Doc Chiesa Michele
  Copyright © 2014-2015 Chiesa Michele <gremlinc5 at github dot com>
  
*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "M9803R.h"


/* _____GLOBAL VARIABLES_____________________________________________________ */
HardwareSerial M9803RSerial = Serial; ///< Pointer to Serial class object


/* _____PUBLIC FUNCTIONS_____________________________________________________ */
/**
Constructor.

Creates class object using default serial port 0.

@ingroup setup
*/
M9803RMaster::M9803RMaster(void)
{
  _u8SerialPort = 0;
  InitResponseBuffer();
}

/**
Constructor.

Creates class object using specified serial port.

@overload void M9803RMaster::M9803RMaster(uint8_t u8SerialPort)
@param u8SerialPort serial port (Serial, Serial1..Serial3)
@ingroup setup
*/
M9803RMaster::M9803RMaster(uint8_t u8SerialPort)
{
  _u8SerialPort = (u8SerialPort > 3) ? 0 : u8SerialPort;
}


/**
Initialize class object.

Sets up the serial port using default 9600 baud rate.
Call once class has been instantiated, typically within setup().

@ingroup setup
*/
void M9803RMaster::begin(void)
{
  begin(9600);
}

/**
Initialize class object.

Sets up the serial port using specified baud rate.
Call once class has been instantiated, typically within setup().

@overload M9803RMaster::begin(uint16_t u16BaudRate)
@param u16BaudRate baud rate, in standard increments (300..115200)
@ingroup setup
*/
void M9803RMaster::begin(uint16_t u16BaudRate)
{
	InitResponseBuffer();

	switch (_u8SerialPort)
	{
#if defined(UBRR1H)
	case 1:
		M9803RSerial = Serial1;
		break;
#endif

#if defined(UBRR2H)
	case 2:
		M9803RSerial = Serial2;
		break;
#endif

#if defined(UBRR3H)
	case 3:
		M9803RSerial = Serial3;
		break;
#endif

	case 0:
	default:
		M9803RSerial = Serial;
		break;
	}

	M9803RSerial.begin(u16BaudRate);
}

/**
M9803R RefreshM9803R manage data. This function need to be called at least one time in a second when collecting data.

@param void
@return void
*/
void M9803RMaster::RefreshM9803R(void)
{
	int8_t index_of_crlf = 0;
	// Read data if available
	while (M9803RSerial.available())
	{
		_u8ResponseBuffer[_u8ResponseBufferIndex] = M9803RSerial.read();
		_u8ResponseBufferIndex++;
	}

	index_of_crlf = getEndOfPacket();
	if (index_of_crlf != -1)
	{
		_u8M9803RStatus = M9803RCore(index_of_crlf);
	}
}

/**
M9803R GetM9803RValue

@param void
@return float the float current value measured
*/
float M9803RMaster::GetM9803RValue(void)
{
	return m9803r_absolute_value;
}

/**
M9803R GetM9803RValue

@param void
@return float the float current value measured
*/
char M9803RMaster::GetM9803RValueUnit(void)
{
	return m9803r_absolute_value_unit;
}

/* _____PRIVATE FUNCTIONS____________________________________________________ */

/**
M9803R Response Buffer Initialize.

@param void
@return void
*/
void M9803RMaster::InitResponseBuffer(void)
{
	for (uint8_t i = 0; i < ku8MaxBufferSize; i++){
		_u8ResponseBuffer[i] = 0;
	}
	_u8ResponseBufferIndex = 0;
}

/**
M9803R Response Buffer Initialize.

@param void
@return uint8_t value is 0 if there aren't CarriageReturn and LineFeed in _u8ResponseBuffer, else return 1
*/
uint8_t M9803RMaster::getEndOfPacket(void)
{
	if (_u8ResponseBufferIndex >= 2)
	{
		for (uint8_t i = 0; i < (_u8ResponseBufferIndex - 1); i++){
			if (_u8ResponseBuffer[i] == 0x0D)
			{
				if (_u8ResponseBuffer[i + 1] == 0x0A)
				{
					return i;
				}
			}
		}
	}
	return -1;
}

/**
M9803R Convert Unit value to corresponding char Unit.

@param uint8_t M9803R unit value
@return char letter of corresponding unit (V, A, O, F, D, H) for (Volt, Ampere, Ohm, Farad, Diode, Hz)
*/
char M9803RMaster::ConvertUnitToChar(uint8_t unit)
{
	if (unit > 12)
	{
		return ' ';
	}
	return ku8M9803RUnit(unit);
}

float M9803RMaster::ku8M9803RFloatConvertion(uint8_t unit, uint8_t range)
{
	switch (unit)
	{
	case 0:
	case 1:
		switch (range)
		{
		case 0:
			return 0.0001;
			break;
		case 1:
			return 0.001;
			break;
		case 2:
			return 0.01;
			break;
		case 3:
			return 0.1;
			break;
		case 4:
			return 1.0;
			break;
		case 5:
		case 6:
		default:
			return 0.0;
		}
		break;
	case 2:
	case 3:
		switch (range)
		{
		case 0:
			return 0.000001;
			break;
		case 1:
			return 0.00001;
			break;
		case 2:
			return 0.0001;
			break;
		case 3:
		case 4:
			return 0.001;
			break;
		case 5:
		case 6:
		default:
			return 0.0;
		}
		break;
	case 4:
	case 5:
		switch (range)
		{
		case 0:
			return 0.1;
			break;
		case 1:
			return 1.0;
			break;
		case 2:
			return 10.0;
			break;
		case 3:
			return 100.0;
			break;
		case 4:
			return 1000.0;
			break;
		case 5:
			return 10000.0;
			break;
		case 6:
		default:
			return 0.0;
		}
		break;
	case 6: // DIODE
		switch (range)
		{
		case 0:
		case 1:
			return 0.001;
			break;
		default:
			return 0.0;
		}
		break;
	case 7: // ADP
	case 10: // Hz
		switch (range)
		{
		case 0:
			return 10.0;
			break;
		case 1:
			return 100.0;
			break;
		case 2:
			return 1000.0;
			break;
		case 3:
		case 4:
			return 0.0;
			break;
		case 5:
			return 0.1;
			break;
		case 6:
			return 1.0;
			break;
		default:
			return 0.0;
		}
		break;
	case 8:
	case 9:
		switch (range)
		{
		case 0:
		case 2:
			return 0.01;
			break;
		default:
			return 0.0;
		}
		break;
	case 11:
		return 0.0;
		break;
	case 12:
		switch (range)
		{
		case 0:
			return 0.000000000001;
			break;
		case 1:
			return 0.00000000001;
			break;
		case 2:
			return 0.0000000001;
			break;
		case 3:
			return 0.000000001;
			break;
		case 4:
			return 0.00000001;
			break;
		default:
			return 0.0;
		}
		break;
	default:
		return 0.0;
	}

}

char M9803RMaster::ku8M9803RUnit(uint8_t unit)
{
	switch (unit)
	{
	case 0:
	case 1:
		return 'V';
	case 2:
	case 3:
	case 8:
	case 9:
		return 'A';
	case 4:
	case 5: 
		return 'O';
	case 6: 
		return 'D';
	case 7:
	case 10:
		return 'H';
	case 11:
		return ' ';
	case 12:
		return 'F';
	default:
		return ' ';
	}
}

/**
M9803R Core.
Sequence:
  - evaluate/disassemble data
  - return status (success/exception)

@param index where is CarriageReturn and LineFeed
@return 0 on success; exception number on failure
*/
uint8_t M9803RMaster::M9803RCore(int8_t index_crlf)
{
	uint8_t u8Status = ku8M9803RSuccess;
	uint8_t u8_sign = 0;
	uint8_t u8_unit;
	uint8_t u8_range;

	if (index_crlf < 9)
	{
		u8Status = ku8M9803RFrameMalformed;
		InitResponseBuffer();
	}
	else
	{
		// Store data
		_M9803RData.status = _u8ResponseBuffer[index_crlf - 9];
		_M9803RData.d0 = _u8ResponseBuffer[index_crlf - 8];
		_M9803RData.d1 = _u8ResponseBuffer[index_crlf - 7];
		_M9803RData.d2 = _u8ResponseBuffer[index_crlf - 6];
		_M9803RData.d3 = _u8ResponseBuffer[index_crlf - 5];
		_M9803RData.unit = _u8ResponseBuffer[index_crlf - 4];
		_M9803RData.range = _u8ResponseBuffer[index_crlf - 3];
		_M9803RData.special_1 = _u8ResponseBuffer[index_crlf - 2];
		_M9803RData.special_2 = _u8ResponseBuffer[index_crlf - 1];
		// Clear buffer
		InitResponseBuffer();

		// Manage data
		if (_M9803RData.status & 0x01) // OL Over Range
		{
			return ku8M9803ROverRange;
		}
		if (_M9803RData.status & 0x08) // if this bit is set sign is negative
		{
			u8_sign = 1;
		}
		u8_unit = _M9803RData.unit & 127;
		u8_range = _M9803RData.range & 127;
		m9803r_absolute_value_unit = ConvertUnitToChar(u8_unit);
		m9803r_absolute_value = (float(_M9803RData.d0) + float(_M9803RData.d1 * 10) + float(_M9803RData.d2 * 100) + float(_M9803RData.d3 * 1000)) * ku8M9803RFloatConvertion(u8_unit,u8_range);
		if (u8_sign)
		{
			m9803r_absolute_value = m9803r_absolute_value * -1.0;
		}
	}

	return u8Status;
}
