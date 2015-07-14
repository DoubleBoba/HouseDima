/*
 * NodeBD.cpp
 *
 *  Created on: 11 июля 2015 г.
 *      Author: tamtaradm
 */
#include "AgroDB.h"
void createIfExistDir(const char *dir, SdFat *sd) {
	if (!sd->exists(dir))
		sd->mkdir(dir);
}
void createIfExistFile(const char *dir, SdFat *sd) {
	if (!sd->exists(dir))
		sd->open(dir, O_CREAT).close();
}
AgroDB::AgroDB(SdFat *sd, String &rootDir, File &file) :
	rootDir(rootDir), sd(sd), file(file) {
	// Check and create directories for DB
#	ifdef DEBUG
	Serial.println("Creating DB dirs...");
#	endif
	createIfExistDir((rootDir + "/plants").c_str(), sd);
	createIfExistDir((rootDir + "/nodes").c_str(), sd);
	createIfExistDir((rootDir + "/sensors").c_str(), sd);

	createIfExistFile((rootDir + "/plants/plants_ids").c_str(), sd);
	createIfExistFile((rootDir + "/nodes/nodes_ids").c_str(), sd);
	createIfExistFile((rootDir + "/sensors/sensorsids").c_str(), sd);
#	ifdef DEBUG
	Serial.println("Creating DB dirs...");
#	endif


}


