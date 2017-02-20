/* 
    GEM Control System
    Copyright (C) <2015>  <Mike Roberts>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//#define STM32_DEV_BOARD
#define V4_PROTO_SHIELD


#ifdef STM32_DEV_BOARD

#define RA_DIRECTION PC14
#define RA_STEP   PC15
#define RA_ENABLE PA5

#define RA_nSLEEP PA0
#define RA_nRESET PA1

#define DEC_DIRECTION PA6
#define DEC_STEP PA7
#define DEC_ENABLE PB12

#define DEC_nSLEEP PB0
#define DEC_nRESET PB1

#define STATUSPIN PC13

#endif

#ifdef V4_PROTO_SHIELD

#define RA_DIRECTION 2
#define RA_STEP   5
#define RA_ENABLE 8

#define RA_nSLEEP A0
#define RA_nRESET A0

#define DEC_DIRECTION 3
#define DEC_STEP 6
#define DEC_ENABLE 8

#define DEC_nSLEEP A0
#define DEC_nRESET A0

#define STATUSPIN 13

#endif


