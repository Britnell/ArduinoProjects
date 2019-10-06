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

uint8_t expecting = 0;
long timer, resp_time;
uint32_t tgt_id = 7626322;
String tgt_ssid = "Mesh_Node7626322";

uint8_t wait_for_ack = 0;
uint8_t only_once = 1;


String manageRequest(String request)
{
  if(expecting){
    Serial.print("response-time : ");
    Serial.println(millis()-resp_time);
    
    /* Print out received message */
    Serial.print(response_i++);
    Serial.print("\treceived ackn : ");
    Serial.println(request);
    expecting = 0;
    only_once = 1;
    timer = millis();
  }
  else {
    Serial.print(response_i++);
    Serial.print("\treceived unexpected : ");
    Serial.println(request);
    
  }

  //return;
	/* return a string to send back */
  //	char response[60];
  //	sprintf(response, "Hello world response #%d from Mesh_Node%d.", response_i++, ESP.getChipId());
  //	return response;
}



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
  
  delay(500);
  Serial.println("Wifi scan ");
  mesh_node.WifiLearnings();

  
  Serial.println("Connecting Mesh directly");
  mesh_node.meshConnect(tgt_ssid);
  
  timer = millis();
  // Eo setup
}



void loop()
{
	/* Accept any incoming connections */

  if(only_once)
	if(millis()-timer > 100){
    only_once = 0;
    
    /* Scan for other nodes and send them a message */
    char request[60];
    sprintf(request, "Data:packet:%d", request_i++, ESP.getChipId());
    Serial.println("Sending request : ");
    Serial.println(request);
    expecting = 1;
    resp_time = millis();
    
    //mesh_node.attemptScan(request);
    //mesh_node.attemptDirect(tgt_ssid, request);
    Serial.println(mesh_node.meshSend(request)?"Send OK":"No response");
    //mesh_node.meshSend(request);
    mesh_node.acceptRequest();  
    
  }
	delay(10);
  mesh_node.acceptRequest();
  // Eo loop
}


//*
