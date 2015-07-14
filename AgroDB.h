/*
 * NodeDB.h
 *
 *  Created on: 11 июля 2015 г.
 *      Author: tamtaradm
 */

#ifndef AGRODB_H_
#define AGRODB_H_
#include <Arduino.h>
#include <SdFat.h>
#include <Ethernet.h>
#include <SPI.h>
#include "SdFat.h"
#include <iBoardRF24.h>
#include "AgroDB.h"
#include <digitalWriteFast.h>
#include <public_defs.h>

class AgroDB {
private:
	String rootDir;
	SdFat *sd;
	File file;
	/*
	 * This functions only add a data to DB (sd card)!
	 * Its calling from public methods.
	 */
	bool addNode(int id, NodeType type);
	bool addSensor(int id, int nodeId, char *name);
public:
	AgroDB(SdFat *sd, String &rootDir, File &file);
	AgroDB(SdFat *sd);
	~AgroDB();
	/*
	 * This functions only parsing a ajax query!
	 */
	bool addNode(char *query);
	bool removeNode(int id);
	bool addPlant(char *query);
	bool removePlant(int id);
	bool addSensor(char *query);
	bool removeSensor(char *query);
	bool assignSensor(int sensorId, int plantId);
	bool deattachSensor(int sensorId);
	Plant getPlant(int id);
	SensorNode_OUT getSensorNode(int id);
	miniRB_OUT getRele(int id);
	//auto getAllPlants();


};



#endif /* AGRODB_H_ */
