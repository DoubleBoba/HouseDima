// Do not remove the include below
#include "AgroDima.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };
byte ip[] = {192,168,0,228};
EthernetServer server(80);
SdFat sd;
File file;
const uint8_t SD_CS_PIN = SS;
const uint8_t page = 1, static_data =2, path_starts=5;
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
	Serial.println("SD initialized!\nWaiting for clients");
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


void returnData(EthernetClient &client, String &query) {
	uint8_t br_pos = path_starts;
	char c;
	while(c != ' ') {
		c = query.charAt(br_pos++);
	}
	query = query.substring(path_starts, br_pos);
	query.trim();
	if (query == "") {
		query = "index.html";
		client.println("Content-Type: text/html");
		client.println("Connection: close");
#		ifdef DEBUG
		Serial.println("YEAH, BITCHES!");
#		endif
	}
	client.println();
	readAndSendPage(client, query.c_str());
}

