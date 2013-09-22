/**
 * @file Cosa/LCD/Driver/HD44780_IO_Port4b.cpp
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2013, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/Board.hh"
#if !defined(__ARDUINO_TINYX5__)
#include "Cosa/LCD/Driver/HD44780.hh"

/**
 * Data direction and port register for data transfer (D4..D7).
 */
#if defined(__ARDUINO_STANDARD__)
# define DDR DDRD
# define PORT PORTD
#elif defined(__ARDUINO_TINYX4__)
# define DDR DDRA
# define PORT PORTA
#elif defined(__ARDUINO_MEGA__) || defined(__ARDUINO_MIGHTY__)
# define DDR DDRB
# define PORT PORTB
#endif

#if defined(__ARDUINO_TINYX4__)
bool
HD44780::Port4b::setup()
{
  synchronized {
    DDR |= 0x0f;
  }
  return (false);
}

void 
HD44780::Port4b::write4b(uint8_t data)
{
  synchronized {
    PORT = ((data & 0x0f) | (PORT & 0xf0));
    m_en.toggle();
    m_en.toggle();
  }
}

void 
HD44780::Port4b::write8b(uint8_t data)
{
  synchronized {
    PORT = (((data >> 4) & 0x0f) | (PORT & 0xf0));
    m_en.toggle();
    m_en.toggle();
    PORT = ((data & 0x0f) | (PORT & 0xf0));
    m_en.toggle();
    m_en.toggle();
  }
  DELAY(SHORT_EXEC_TIME);
}

#else

bool
HD44780::Port4b::setup()
{
  synchronized {
    DDR |= 0xf0;
  }
  return (false);
}

void 
HD44780::Port4b::write4b(uint8_t data)
{
  synchronized {
    PORT = ((data << 4) | (PORT & 0x0f));
    m_en.toggle();
    m_en.toggle();
  }
}

void 
HD44780::Port4b::write8b(uint8_t data)
{
  synchronized {
    PORT = ((data & 0xf0) | (PORT & 0x0f));
    m_en.toggle();
    m_en.toggle();
    PORT = ((data << 4) | (PORT & 0x0f));
    m_en.toggle();
    m_en.toggle();
  }
  DELAY(SHORT_EXEC_TIME);
}
#endif

void 
HD44780::Port4b::set_mode(uint8_t flag)
{
  m_rs.write(flag);
}

void 
HD44780::Port4b::set_backlight(uint8_t flag)
{
  m_bt.write(flag);
}
#endif

