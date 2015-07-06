// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _AgroDima_H_
#define _AgroDima_H_
#include "Arduino.h"
#define DEBUG
#define BUFFER_SIZE 512
//add your includes for the project AgroDima here
#include <Ethernet.h>
#include <SPI.h>
#include "SdFat.h"
#include <iBoardRF24.h>
//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project AgroDima here




//Do not add code below this line
#endif /* _AgroDima_H_ */
