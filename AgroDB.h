/*
 * NodeDB.h
 *
 *  Created on: 11 июля 2015 г.
 *      Author: tamtaradm
 */

#ifndef AGRODB_H_
#define AGRODB_H_

enum nodeType {
	sensor_node = 0,
	rele = 1
};

class AgroDB {

public:
	AgroDB(SdFat *sd, String &rootDir);
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
private:
	String rootDir;
	SdFat *sd;
	/*
	 * This functions only add a data to DB (sd card)!
	 * Its calling from public methods.
	 */
	bool addNode(int id, nodeType type);
	bool addSensor(int id, int nodeId, char *name);

};



#endif /* AGRODB_H_ */
