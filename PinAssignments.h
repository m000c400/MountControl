//#define _ARDUINO_
#define _STM_32_

#ifdef _ARDUINO_

#define RA_STEP   2
#define RA_DIRECTION 3
#define RA_ENABLE 4
#define RA_MODE2 A2
#define RA_MODE1 A1
#define RA_MODE0 A0

#define DEC_STEP 5
#define DEC_DIRECTION 6
#define DEC_ENABLE 7
#define DEC_MODE2 A5
#define DEC_MODE1 A4
#define DEC_MODE0 A3

#define STATUSPIN 13
#define IR_RX 11
#endif

#ifdef _STM_32_
#define RA_DIRECTION PC13
#define RA_STEP   PC14
#define RA_nSLEEP PC15
#define RA_nRESET PA0
#define RA_MODE2 PA1
#define RA_MODE1 PA2
#define RA_MODE0 PA3
#define RA_nENABLE PA4

#define DEC_DIRECTION PA5
#define DEC_STEP PA6
#define DEC_nSLEEP PA7
#define DEC_nRESET PB0
#define DEC_MODE2 PB1
#define DEC_MODE1 PB10
#define DEC_MODE0 PB11
#define DEC_nENABLE PB14

#define STATUSPIN PB9
#define IR_RX PA12
#endif


