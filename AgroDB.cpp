/*
 * NodeBD.cpp
 *
 *  Created on: 11 июля 2015 г.
 *      Author: tamtaradm
 */
#include "AgroDB.h"

AgroDB::AgroDB(SdFat *sd, String &rootDir) :
	rootDir(rootDir), sd(sd) {
	char *temp = (rootDir + "/plants").c_str();
	if (!sd->exists(temp))
		sd->mkdir(temp);

	temp = (rootDir + "/nodes").c_str();
	if (!sd->exists(temp))
		sd->mkdir(temp);

	temp = (rootDir + "/nodes");
}


