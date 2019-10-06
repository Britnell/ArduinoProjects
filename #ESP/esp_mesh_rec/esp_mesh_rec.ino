/*
 * Testing WifiMesh, with one receiver and one sender
 * 
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMeshT.h>

unsigned int request_i = 0;
unsigned int response_i = 0;

String manageRequest(String request);

/* Create the mesh node object */
ESP8266WiFiMeshT mesh_node = ESP8266WiFiMeshT(ESP.getChipId(), manageRequest);

/**
 * Callback for when other nodes send you data
 *
 * @request The string received from another node in the mesh
 * @returns The string to send back to the other node
 */
String manageRequest(String request)
{
	/* Print out received message */
	Serial.print(response_i++);
	Serial.print(" received packet ");
	Serial.println(request);
  
	/* return a string to send back */
	char response[60];
	//sprintf(response, "Hello world response #%d from Mesh_Node%d.", response_i++, ESP.getChipId());
  sprintf(response, "Ack");
	return response;
}

void setup()
{
	Serial.begin(115200);
	delay(10);

	Serial.println();
	Serial.print("Setting up mesh node ");
  Serial.println(ESP.getChipId());

	/* Initialise the mesh node */
	mesh_node.begin();
}

void loop()
{
	/* Accept any incoming connections */
  //mesh_node.acceptRequest();
  mesh_node.meshReceiver();
  //mesh_node.clientLoop();
  
	/* Scan for other nodes and send them a message */
//	char request[60];
//	sprintf(request, "Hello world request #%d from Mesh_Node%d.", request_i++, ESP.getChipId());
//	mesh_node.attemptScan(request);
	delay(50);
}


