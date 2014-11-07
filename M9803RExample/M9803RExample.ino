

/*

  Basic.ino - example using M9803R library
  
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
  Copyright © 2014-2015 Doc Chiesa Michele <gremlinc5 at github dot com>
  
*/

#include <M9803R.h>

M9803RMaster multimeter(1); // Multimeter is on Serial_1

void setup()
{
  // initialize M9803R communication as default 9600 baudrate
  multimeter.begin();
  pinMode(13, OUTPUT);
}


void loop()
{
  float value;
  multimeter.RefreshM9803R();
  value = multimeter.GetM9803RValue();
  
  if (value > 10.0)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
}

