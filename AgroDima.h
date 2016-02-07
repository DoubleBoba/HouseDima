// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _AgroDima_H_
#define _AgroDima_H_
#include "Arduino.h"
#define BUFFER_SIZE 256
#define DEBUG
//add your includes for the project AgroDima here
#include <Ethernet.h>
#include <SPI.h>
#include <SdFat.h>
#include <iBoardRF24.h>
#include <digitalWriteFast.h>
#include <MemoryFree.h>
//end of add your includes here

// Аёдишники реле и нодов
#define RELES_IDS {1,2,3,4}

#define NODE 4

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
