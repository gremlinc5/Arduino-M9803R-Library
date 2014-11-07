/**
@file
Arduino library for communicating with M9803R Multimeter over RS232.

@defgroup setup M9803RMaster Object Instantiation/Initialization
@defgroup buffer M9803RMaster Buffer Management
@defgroup constant M9803R Exception Codes
*/
/*

  M9803RMaster.h - Arduino library for communicating with M9803R Multimeter
  over RS232.
  
  This file is part of M9803RMaster.
  
  M9803RMaster is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  M9803RMaster is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with M9803RMaster.  If not, see <http://www.gnu.org/licenses/>.
  
  Written by Doc Chiesa Michele
  Copyright © 2014-2015 Doc Chiesa Michele <gremlinc5 at github dot com>
  
*/

  
#ifndef M9803RMaster_h
#define M9803RMaster_h


/**
@def __M9803RMASTER_DEBUG__ (1).
*/
#define __M9803RMASTER_DEBUG__ (1)


/* _____STANDARD INCLUDES____________________________________________________ */
// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* _____UTILITY MACROS_______________________________________________________ */


/* _____PROJECT INCLUDES_____________________________________________________ */


/* _____CLASS DEFINITIONS____________________________________________________ */
/**
Arduino class library for communicating with Modbus slaves over 
RS232/485 (via RTU protocol).
*/
class M9803RMaster
{
  public:
    M9803RMaster();
    M9803RMaster(uint8_t);
    
    void begin();
    void begin(uint16_t);
    
    // M9803R exception codes

	/**
	M9803R Frame Malformed exception.

	M9803R data have carriage return and line feed but there are not 11 byte.

	@ingroup constant
	*/
	static const uint8_t ku8M9803RFrameMalformed = 0x02;

    /**-
    M9803R Over Range exception.
    
    M9803R is out of range during measure.
    
    @ingroup constant
    */
    static const uint8_t ku8M9803ROverRange            = 0x01;

    /**
    M9803R Success.
    
    Data are successfully read.
      
    @ingroup constant
    */
    static const uint8_t ku8M9803RSuccess                    = 0x00;

	// M9803R convertion costants

	/**
	M9803R Convertion Constants array.

	M9803R data must be converted from 4 digits, unit and range values to absolute float value.

	@ingroup constant
	*/
	/*static const float ku8M9803RFloatConvertion[7][13] = { 
		{ 0.0001, 0.001, 0.01, 0.1, 1.0, 0.0, 0.0 }, // DC VOLT
		{ 0.0001, 0.001, 0.01, 0.1, 1.0, 0.0, 0.0 }, // AC VOLT
		{ 0.000001, 0.00001, 0.0001, 0.001, 0.001, 0.0, 0.0 }, // DC milliAMPERE
		{ 0.000001, 0.00001, 0.0001, 0.001, 0.001, 0.0, 0.0 }, // AC milliAMPERE
		{ 0.1, 1.0, 10.0, 100.0, 1000.0, 10000.0, 0.0 }, // Ohm
		{ 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // Buzzer need check
		{ 0.001, 0.001, 0.0, 0.0, 0.0, 0.0, 0.0 }, // Diode need check
		{ 10.0, 100.0, 1000.0, 0.0, 0.0, 0.1, 1.0 }, // ADP Adaptive Hz
		{ 0.01, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0 }, // DC A
		{ 0.01, 0.0, 0.01, 0.0, 0.0, 0.0, 0.0 }, // AC A
		{ 10.0, 100.0, 1000.0, 0.0, 0.0, 0.1, 1.0 }, // Hz
		{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // Nothing
		{ 0.000000000001, 0.00000000001, 0.0000000001, 0.000000001, 0.00000001, 0.0, 0.0 } // CAP capacitor
	};*/

	// M9803R convertion costants

	/**
	M9803R Convertion Constants array.

	M9803R data must be converted from 4 digits, unit and range values to absolute float value.

	@ingroup constant
	*/
	/*static const char ku8M9803RUnit[13] = { 'V', 'V', 'A', 'A', 'O', 'O', 'D', 'H', 'A', 'A', 'H', ' ', 'F' };*/


	struct m9803r_data_t {
		uint8_t status;
		uint8_t d0;
		uint8_t d1;
		uint8_t d2;
		uint8_t d3;
		uint8_t unit;
		uint8_t range;
		uint8_t special_1;
		uint8_t special_2;
	};

	void RefreshM9803R(void);
	float GetM9803RValue(void);
	char GetM9803RValueUnit(void);

  private:
    uint8_t  _u8SerialPort;                                     ///< serial port (0..3) initialized in constructor
    uint16_t _u16BaudRate;                                      ///< baud rate (300..115200) initialized in begin()
	uint8_t _u8M9803RStatus;
	static const uint8_t ku8MaxBufferSize = 32;					///< size of data buffers    
    uint8_t _u8ResponseBuffer[ku8MaxBufferSize];				///< buffer to store M9803R data
    uint8_t* rxBuffer;
    uint8_t _u8ResponseBufferIndex;
	m9803r_data_t _M9803RData;
	float m9803r_absolute_value;
	char m9803r_absolute_value_unit;

	void InitResponseBuffer(void);
	uint8_t getEndOfPacket(void);
	char ConvertUnitToChar(uint8_t unit);
	float ku8M9803RFloatConvertion(uint8_t unit, uint8_t range);
	char ku8M9803RUnit(uint8_t unit);

	uint8_t M9803RCore(int8_t index_crlf);
};
#endif
