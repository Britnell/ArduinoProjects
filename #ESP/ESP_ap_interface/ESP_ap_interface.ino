/*
 * 
 *  Credits to : 
 *  
 *  Access Point Example from
 *    https://randomnerdtutorials.com/esp8266-nodemcu-access-point-ap-web-server/
 *    
 *    
 *  Captive Portal from
 *    https://www.hackster.io/rayburne/esp8266-captive-portal-5798ff
 *   
 *  Reference :
 *  
 *   - Async Web server
 *     https://github.com/me-no-dev/ESPAsyncWebServer
 * 
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

// ################################### HTML
const char index_html[] PROGMEM = R"rawliteral(       // ################################### HTML

<!DOCTYPE HTML><html>
<head>
  <meta content="text/html;charset=utf-8" http-equiv="Content-Type">
  <meta content="utf-8" http-equiv="encoding">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>

    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     background-color: black;
     color: white;
    }

    h2 { font-size: 2.0rem; }
    p { font-size: 2.0rem; }

    #switch {
      font-size: 2.0rem;
      background-color: lightblue;
    }

    #state {
      background-color: grey;
      padding: 8px;
    }

    #program {
      padding: 4px 20px;
      margin: 0px 30px;
    }

    td, tr {
      border: none;
    }
    td {
      width: 150px;
    }

    tr {
      margin: 100px 0px;
    }

  </style>
</head>
<body>
  <h2>Wifi Timer-Switch</h2>
  <p>
    <span id="time">20:00</span> - the device is currently : <span id="state">ON</span> 
  </p>
  
  
  <table id="table">

    <tr id="in_row">  
      <td id="in">Turn on in :</td> 
      <td><span id="inval">2h</span></td>  
      <td><input type="range" id="inslider" class="slider" min="0.25" max="12" step="0.25" value="2.0" oninput="update_slider('inval', this.value)"/></td> 
    </tr>

    <tr id="then_row">  
      <td id="then">Then Turn off after :</td> 
      <td><span id="thenval">2h</span></td>
      <td><input type="range" id="thenslider" class="slider" min="0.25" max="12" step="0.25" value="2.0"  oninput="update_slider('thenval', this.value)"/></td> 
    </tr>

    <tr id="at_row">  
      <td id="at">Turn on at :</td> 
      <td><span id="atval">12:00</span></td>  
      <td><input type="range" id="atslider" class="slider" min="0.25" max="24" step="0.25" value="12.0" oninput="update_slider_time('atval', this.value)"/></td> 
    </tr>
    <tr id="after_row">  
      <td id="after">Then Turn off at :</td> 
      <td><span id="afterval">21:00</span></td>  
      <td><input type="range" id="afterslider" class="slider" min="0.25" max="12" step="0.25" value="21.0" oninput="update_slider_time('afterval', this.value)"/></td> 
    </tr>

  </table>
  <div>
    <button id="program" onclick="program()">Program</button>
    <span id="cnsl"></span>
  </div>

  
</body>
<script>

var $state = document.getElementById('state');

var $inval = document.getElementById('inval');
var $in = document.getElementById('in');
var $at = document.getElementById('at');
var $then = document.getElementById('then');
var $after = document.getElementById('after');

var $in_row = document.getElementById('in_row');
var $then_row = document.getElementById('then_row');
var $at_row = document.getElementById('at_row');
var $after_row = document.getElementById('after_row');

var $cnsl = document.getElementById('cnsl');

var active_commands = [];
var all_commands = [ $at_row, $after_row, $in_row, $then_row ];


$state.onclick = function(){
  // Send http call
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/click", true);
  xhttp.send();

  get_state();

  // Eo f
}

// Highlight  active_commands
$in.onclick = function(){
  select_command($in_row);
}
$then.onclick = function(){
  select_command($then_row);
}
$at.onclick = function(){
  select_command($at_row);
}
$after.onclick = function(){
  select_command($after_row);
}


function select_command(div){
  if(active_commands.length==0){
    // first selected > add
    active_commands.push(div);
  }
  else if( active_commands.includes(div) ){
    // already > remove
    remove_from_list(active_commands,div);
  }
  else {
    // check about deactivating other

    if(div==$at_row){
      remove_from_list(active_commands,$in_row);
      remove_from_list(active_commands,$then_row);
    }
    else if(div==$after_row){
      remove_from_list(active_commands,$in_row);
      remove_from_list(active_commands,$then_row);
    }
    else if(div==$in_row){
      remove_from_list(active_commands,$at_row); 
      remove_from_list(active_commands,$after_row); 
    }
    else if(div==$then_row){
      remove_from_list(active_commands,$at_row); 
      remove_from_list(active_commands,$after_row); 
    }
    active_commands.push(div);
  }
  

  // loop through list & colour
  all_commands.forEach(function(cmd){
    if( active_commands.includes(cmd) )
      cmd.children[0].style.backgroundColor = '#1060B0';
    else
      cmd.children[0].style.backgroundColor = 'black';
  })
}


function remove_from_list(list, x){
  let idx = list.indexOf(x);
  if(idx!=-1)
    list.splice(idx,1);
}

function update_slider_time(id,val){
  let label = document.getElementById(id);
  // float to 1h 15 mins
  let hrs = Math.floor(val);
  let mins = val - hrs;
  mins *= 60;
  label.innerHTML = "";
  label.innerHTML += hrs + ":";
  if(mins==0)
    label.innerHTML += '00';
  else
    label.innerHTML += mins;
}

function update_slider(id,val){
  let label = document.getElementById(id);
  // float to 1h 15 mins
  let hrs = Math.floor(val);
  let mins = val - hrs;
  mins *= 60;
  label.innerHTML = "";

  if(hrs!=0)
    label.innerHTML += hrs + "h ";
  if(mins!=0)
    label.innerHTML += mins + "mins ";
}

// get relay state
function get_state(){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) 
    {
      // $state = this.responseText;
      if(this.responseText=='1')
      {
        $state.innerHTML = 'ON'
        $in.innerHTML = 'Turn off in : ';
        $then.innerHTML = 'Then Turn on after :';
        $at.innerHTML = 'Turn off at :';
        $after.innerHTML = 'Then Turn on at :';
      }
      else { 
        $state.innerHTML = 'OFF'
        $in.innerHTML = 'Turn on in : ';
        $then.innerHTML = 'Then Turn off after :';
        $at.innerHTML = 'Turn on at :';
        $after.innerHTML = 'Then Turn off at :';
        
      }
    }
  };
  xhttp.open("GET", "/get_state", true);
  xhttp.send();
}

get_state();

function program(){
  // send current program
  // lets just do no checking for now : )
  active_commands.forEach(function(cmd){
    let val = cmd.children[2].firstChild.value;
    console.log(cmd.id, val);
  });
}


</script>
</html>

)rawliteral";       // ################################### HTML
// ################################### HTML

// * Initialisations

const char* ssid     = "ESP8266-Access-Point";
const char* password = "eeesssppp";

AsyncWebServer server(80);
DNSServer         dnsServer;             


// * Variables

uint8_t LEDstate = 0;
uint8_t LEDpin = D1;    // LED_BUILTIN


// * Functions

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
//  if(var == "TEMPERATURE"){
//    return String(t);
//  }
//  else if(var == "HUMIDITY"){
//    return String(h);
//  }
//  return String();
}



//      ***************     SETUP     ***************


void setup(){
  
  Serial.begin(115200);  
  Serial.print("Setting AP (Access Point)…");
  
  // .softAP(const char* ssid, const char* password, int channel, int ssid_hidden, int max_connection)
  WiFi.softAP(ssid ); 
  IPAddress IP = WiFi.softAPIP();
//  Serial.println(WiFi.localIP()); // AP IP addr
  dnsServer.start(53, "*", IP);   // DNS & Captive
  
  // * Server Routing
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html );
  });
  server.on("/get_state", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(LEDstate).c_str());
  });
  
  server.on("/click", HTTP_GET,  [](AsyncWebServerRequest *request){
    
    // * Switch 
    LEDstate = !LEDstate;
    digitalWrite(LEDpin, LEDstate);
    // reply with new LED state
    //request->send_P(200, "text/plain", String(LEDstate).c_str());
    request->send_P(200, "text/html", index_html );
  });

  // * Captive answer all
  
  server.onNotFound([](AsyncWebServerRequest *request){
    //request->send(404);
    request->send_P(200, "text/html", index_html );
  });
  
//  server.onNotFound(NULL); // remove handler
  
  // * Outputs
  // TODO : store state in EEPROM
  // TODO : Input button wake up
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LEDstate);
  
  // * Start server
  server.begin();
}
 
void loop(){
  // TODO : Sleep mode
  // TODO : check current time
  
  // Not asynchronous?
  dnsServer.processNextRequest();
  
  // Eo loop
}

