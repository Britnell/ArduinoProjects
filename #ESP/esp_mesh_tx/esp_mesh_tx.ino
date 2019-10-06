#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>

unsigned int request_i = 0;
unsigned int response_i = 0;

String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */

uint8_t expecting = 0;

String manageRequest(String request)
{
  if(expecting){
    /* Print out received message */
    Serial.print(response_i++);
    Serial.print("\treceived ackn : ");
    Serial.println(request);
    expecting = 0;
  }
  else {
    Serial.print(response_i++);
    Serial.print("\treceived unexpected : ");
    Serial.println(request);
    
  }
	
	/* return a string to send back */
  //	char response[60];
  //	sprintf(response, "Hello world response #%d from Mesh_Node%d.", response_i++, ESP.getChipId());
  //	return response;
}

long timer;

void setup()
{
	Serial.begin(115200);
	delay(10);

	Serial.println();
	Serial.println();
	Serial.print("Setting up mesh node ");
  Serial.println(ESP.getChipId());

	/* Initialise the mesh node */
	mesh_node.begin();
  timer = millis();
  // Eo setup
}



void loop()
{
	/* Accept any incoming connections */
	mesh_node.acceptRequest();

  if(millis()-timer > 500){
    timer = millis();
    /* Scan for other nodes and send them a message */
    char request[60];
    sprintf(request, "Data:packet:%d", request_i++, ESP.getChipId());
    Serial.println("Sending request : ");
    Serial.println(request);
    expecting = 1;
    mesh_node.attemptScan(request);
  }
	delay(10);
}


//*
