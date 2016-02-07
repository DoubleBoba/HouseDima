// Do not remove the include below
#include "AgroDima.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xF4, 0xEC };
byte ip[] = {192,168,0,122};
EthernetServer server(80);
iBoardRF24 radio(3,8,5,6,7,2);
SdFat sd;
File file;
//iBoardRF24 radio(3,8,5,6,7,2);
const uint8_t SD_CS_PIN = SS;
const uint8_t page = 1, static_data =2, path_starts=5;

void nrf_sensor();
void initRadio();
<<<<<<< HEAD
void RBord_IN(bool, int);

struct Rele {
	uint8_t pin;
	bool status;
};

Rele reles[] = {{9, false}, {8, false}, {7, false}, {0, false}};

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
	Serial.println("Welcome to the Agro Dima project\nIt was compiled in DEBUG mode\nStarting web server... YES");
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
	initRadio();
#	ifdef DEBUG
//	Serial.println(freeMemory());
	Serial.println("Waiting for clients...");
#	endif
	RBord_IN(true, 1);
}


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

}

void toggleStatus(EthernetClient &, String &);
void returnStatus(EthernetClient &, String &);
void returnData(EthernetClient &client, String &query) {
	uint8_t br_pos = path_starts;
	char c;
	while(c != ' ') {
		c = query.charAt(br_pos++);
		Serial.print(c);
	}
	query.remove(0, path_starts);
	query.remove(br_pos-path_starts, 9);
	bool send_file = true;
#	ifdef DEBUG
	Serial.println(query);
#	endif
	query.trim();
	if (query == "") {
		query = "index.html";
		client.println("Content-Type: text/html");
		client.println("Connection: close");
	}else if (query.indexOf("ajax") > -1) {
		send_file = false;
		if (query.indexOf("GET_S")>-1) {
#			ifdef DEBUG
			Serial.println("YES MOTHERFUCKER");
#			endif
			returnStatus(client, query);
		} else if (query.indexOf("SET_S")) {
			toggleStatus(client, query);

		}
	}
	client.println();
	if (send_file)
		readAndSendPage(client, query.c_str());
}

void returnStatus(EthernetClient &client, String &query) {

	String result = "";

	for (int i = 0; i < 4; i++) {
		result = result + reles[i].pin + ":" + ((reles[i].status) ? 1 : 0) += ";";
	}
#	ifdef DEBUG
	Serial.println("Status: "+result);
#	endif
	client.println(result);
}

void toggleStatus(EthernetClient &client, String &query) {
	char chs[] = {query.charAt(query.length()-1)};
	uint8_t num = atoi(chs);
#	ifdef DEBUG
	Serial.print("Toggle ");
	Serial.println(num);
#	endif
	if (!(num > 3 && num < 0)) {
		Rele r = reles[num];
		if (num != 3) {
			(r.status) ? digitalWrite(r.pin, LOW) : digitalWrite(r.pin, HIGH);
			r.status = !r.status;
		} else {
			RBord_IN(!r.status, 21);
			r.status = !r.status;
		}
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
