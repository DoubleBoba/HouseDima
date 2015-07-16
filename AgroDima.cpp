// Do not remove the include below
#include "AgroDima.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF4, 0xEC };
byte ip[] = {192,168,0,22};
EthernetServer server(80);
iBoardRF24 radio(3,8,5,6,7,2);
SdFat sd;
File file;
//iBoardRF24 radio(3,8,5,6,7,2);
const uint8_t SD_CS_PIN = SS;
const uint8_t page = 1, static_data =2, path_starts=5;

void initDB();
void nrf_sensor();
void initRadio();
void processReles();

int target1 = -1, target2 = -1, target3 = -1; // Показания для включения реле
int time1 = -1, time2 = -1, time3 = -1; //Промежутки времени между включениями реле
int duration1 = -1, duration2 = -1, duration3 =-1; //Продолжительность включения реле
int sensor1 = -1, sensor2 = -1, sensor3 = -1;
unsigned long last1 = millis(), last2 = millis(), last3 = millis(); //Промежутки времени между включениями реле


// структура для приема данных с sensor Node
typedef struct{
  int ParamID;        // id
  int ParamNAME;
  float ParamT;         // ТЕмпература
  float ParamP;    // влажность почвы
  bool vcc;
}

SensorNode_OUT;

SensorNode_OUT SN; //созжаем структуру


//структура для отправки на miniBord
typedef struct
{
    int RB_id; //id получателя
    int id_OUT; // id servera
    bool Srele_OUT; //состояния реле
}
miniRB_OUT;
miniRB_OUT RB_IN;



void setup()
{
#	ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Welcome to the Agro Dima project\nIt was compiled in DEBUG mode\nStarting web server...");
#	endif
	Ethernet.begin(mac,ip);
	server.begin();
#	ifdef DEBUG
	Serial.println("Server started!\nInitializing SD card...");
#	endif
	if (!sd.begin(4, SPI_FULL_SPEED)) {
#		ifdef DEBUG
		Serial.println("Error opening sd!");
#		endif
		while (1);
	}
#	ifdef DEBUG
	//Serial.println("Free RAM = " + freeRam());
	Serial.println("SD initialized!\nInit DB...");
#	endif
	//initDB();
#	ifdef DEBUG
	Serial.println("DB inited!");
#	endif
	//initRadio();
#	ifdef DEBUG
	Serial.println("Waiting for clients...");
#	endif
}

void initDB() {
	if(!sd.exists("DB"))
		sd.mkdir("DB");

	if(!sd.exists("DB/plants"))
		sd.mkdir("DB/plants");
	if(!sd.exists("DB/free_nodes"))
		sd.mkdir("DB/free_nodes");

}
/*
bool add_sensor_node(int id) {
	String str = "/DB/free_nodes/";
	str+= id;
	if(!sd.exists(str.c_str())) {
		sd.open();
	}
}
*/

void initRadio() {
#	ifdef DEBUG
	Serial.println("Init radio...");
#	endif
	radio.begin();
	radio.setDataRate(RF24_1MBPS); //выбор скорости
	//radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_MAX);
	radio.setChannel(100); //тут установка канала
	radio.setCRCLength(RF24_CRC_16);
	radio.setRetries(15,15); //надо
	radio.openWritingPipe(0xF0F0F0F0E1LL);
	radio.openReadingPipe(1,0xF0F0F0F0D2LL);
	radio.startListening();
# 	ifdef DEBUG
	Serial.println("Radio good.");
#	endif
}


void readAndSendPage(EthernetClient &client, const char *name)  {
	file = sd.open(name);
#	ifdef DEBUG
	Serial.print("Sending file \"");
	Serial.print(name);
	Serial.println("\"");
#	endif
	if (file) {
		int aval;
		int size;
		while ((aval = file.available())) {
			// malloc buffer of data
			// dont delete this fucking code. without it all gonna break!
			uint8_t  *bytes = (uint8_t *)((aval > BUFFER_SIZE)? malloc(sizeof(uint8_t)*(size = BUFFER_SIZE))
					: malloc(sizeof(uint8_t)*(size = aval)));
#			ifdef DEBUG
			//Serial.print(size);
			//Serial.println(" - buffer size");
#			endif
			file.read(bytes, size);
			client.write(bytes, size);
			free(bytes);
		}
	}
#	ifdef DEBUG
	else {
		Serial.println("Cannot open file!");
	}
#	endif
}
void returnData(EthernetClient &, String &);
// The loop function is called in an endless loop
void loop()
{

	nrf_sensor();
	EthernetClient client = server.available();
	if (client) {
		boolean line_blank = true, new_line = false;
		String query;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				if (!new_line)
					query += c;
				if (c == '\n' && line_blank) {
#					ifdef DEBUG
					Serial.println(query);
#					endif
					client.println("HTTP/1.1 200 OK");

					returnData(client, query);
					break;
				}
				if (c=='\n') {
					line_blank = true;
					new_line = true;
				}else if (c != '\r') {
					line_blank = false;
				}
			}
		}
		delay(1);
		file.close();
		client.stop();
	}
	processReles();
}

void processQuery(EthernetClient &, String &);
void returnData(EthernetClient &client, String &query) {
	uint8_t br_pos = path_starts;
	char c;
	while(c != ' ') {
		c = query.charAt(br_pos++);
	}
	query = query.substring(path_starts, br_pos);
	query.trim();
	bool send_file = true;
	if (query == "") {
		query = "index.html";
		client.println("Content-Type: text/html");
		client.println("Connection: close");
	}else if (query.indexOf(String("ajax")) > -1) {
		send_file = false;
		if (query.indexOf("query")>-1) {
			processQuery(client, query);
		}
	}
	client.println();
	if (send_file)
		readAndSendPage(client, query.c_str());
}

void processQuery(EthernetClient &client, String &query) {
#	ifdef DEBUG
	Serial.println("Processing query");
#	endif

	query = query.remove(0, query.indexOf("&")+1);
	query = query.remove(0, query.indexOf("&")+1);

	target1 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);
	target2 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);
	target3 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);

	time1 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);
	time2 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);
	time3 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);

	duration1 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);
	duration1 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);
	duration1 = atoi(query.indexOf('=')+1, query. indexOf('&')-1);
	query.remove(0, indexOf('&')+1);

	client.println("FUCKING INCREDIBLE MAGIC");
}

void nrf_sensor()
{
	if (radio.available()) 
  		{
    		radio.read( &SN, sizeof(SN)); 
#			ifdef DEBUG
    		Serial.println("Sensor Node send a packet:")
			Serial.println( SN.ParamID);
			Serial.println( SN.ParamNAME);
			Serial.println( SN.ParamT);
			Serial.println( SN.ParamP);
			Serial.println( SN.vcc);
			Serial.println("End of packet!");
#			endif
			sensor1 = SN.paramT;
			sensor2 = SN.ParamP;
			sensor3 = SN.vcc;

    	}
}

void processReles() {
	if (sensor1 >target1 || millis() - last1 > time1){
		RBord_IN(true, RELE1);
	}else if (millis() - duration > time1){
		RBord_IN(false, RELE1);
	}
	if (sensor2 >target2 || millis() - last2 > time2){
		RBord_IN(true, RELE2);
	}else if (millis() - duration > time2){
		RBord_IN(false, RELE2);
	}
	if (sensor3 >target3 || millis() - last3 > time3){
			RBord_IN(true, RELE3);
	}else if (millis() - duration > time3){
		RBord_IN(false, RELE3);
	}

}
/**
 * Set rele status
 */
void RBord_IN(bool Srele,int RB_id_client)
{
  radio.stopListening();
  RB_IN.Srele_OUT=Srele;
  RB_IN.RB_id=RB_id_client;
  bool ok = radio.write( &RB_IN, sizeof(RB_IN)); //Отправка данных структуры
  radio.startListening();
}
