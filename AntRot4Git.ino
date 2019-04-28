//added timeout
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>        // Include the mDNS library - permits local name

char* SSIDh = "GrassHopper";
const char* PWDh = "TakesPebble"; 
int Channel = 10;

const char* ssid = "SmoothPapa";
const char* password = "ShavesAll";

const int irLED = 13; // IR Emitor - IR LED D7
const int cwPin = 12; // RELAY CH           D6
const int ccwPin = 14; //RELAY CH           D5
const int sensorPin = A0; // IR Detector    A0
int irT = 793; // irT mid brightness (infra red threshold)  should be verified per actual hardware
int irTd = 793; //irTd is the darker readings - extreme used to calibrate*
int irTl = 793; //irTl is the lighter readings - extreme used to calibrate*
const int bT = 8; // count to be exceeded //NO APPLICABLE WITH SWITCH
int NT = 0; // toggle used to assign new threshold*

int zero = 0; // zero position micro SWITCH state normally low = 0

int Pm = 0; //Where you want antenna to point 
int Cycle = 0; //loop cycle count
int rot = 0; // 0 no rotation, -1 ccw, 1 cw
int Pa = 104; //Default target
int P = -5 ; //Current Antenna position 20 to 340 degrees 
                   //(20 degrees reserved for registration)
int Pd = 0 ;//Hole transitions to calculate rotation based on hardware                   
int countW = 0 ; // as Seen IR, above threashold IR
int countB = 0 ; // as Seen IR, below threashold IR
int page = 0 ; //Default html page out
int timeout = 0 ; //for wifi loop lockup avoidance
WiFiServer server(80);

//printWiFiStatus();

void setup(){
// Set up access point
WiFi.mode(WIFI_AP);
WiFi.softAP(SSIDh, PWDh, Channel, true); //(ssid, password, channel, hidden)

  Serial.begin(115200);//
  Serial.setDebugOutput(true);
  delay(10);
  
  pinMode(irLED, OUTPUT); // IR Emitor - IR LED
  pinMode(cwPin, OUTPUT); // Drives Motor
  pinMode(ccwPin, OUTPUT); // Reverse Drives Motor
  zero = digitalRead (5); //Switch to indicate zero position  
  pinMode(sensorPin,INPUT);//Analogue input of IR Sensitive LED voltage contribution

 // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
 //SERVER
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  
 
}

void loop() {
//=========================================================Antenna

//========================================================Antenna Ends  

chooseMap();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
    timeout++;
    if(timeout>10) {Serial.print("INFINITE LOOP BREAK!");  break;}    
  }
 timeout=0; 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

             
  // Match the BUTTON request page 0 - Preset Headings Page
  // Initiate relevant ESP8266 program variable changes
if (page==0){
    if (request.indexOf("/Watertown") != -1)  {
    Pm = 104;
    }
    if (request.indexOf("/Syracuse") != -1)  {
    Pm = 147;
    }
    if (request.indexOf("/Rochester") != -1)  {
    Pm = 189;
    }
    if (request.indexOf("/Buffalo") != -1)  {
    Pm = 212;
    }
    if (request.indexOf("/Brighton") != -1)  {
    Pm = 255;
    }
    if (request.indexOf("/Peterborough") != -1)  {
    Pm = 283;
    }
    if (request.indexOf("/FineTune") != -1)  {
    page = 1;
    }
}

// Match the BUTTON request page 1 - Finite headings adjustments
if (page==1){
    if (request.indexOf("/CounterClockwise") != -1)  {
    Serial.println("Counter Clockwise");
    //client.print ("Rotating CCW 4 Degrees.");
    //if ( P < 20) {client.print ("At Antenna limit 16."); }
   // else 
    { Pm = Pm - 4; }
 
    }
              
    if (request.indexOf("/Clockwise") != -1)  {
    Serial.println("Clockwise");
    //client.print ("Rotating CW 4 Degrees.");
    //if ( P > 340) { client.print ("At Antenna limit 344."); }
    //else  
    { Pm = Pm + 4;}
    }

    if (request.indexOf("/Reset") != -1)  {
    Serial.println("Reset");
    //client.print ("Antenna will reset to 104 Degrees."); 
    Pm = 104;
    P = -5;  
    } 
    
    if (request.indexOf("/Location") != -1)  {
    page = 0;
  }     
        
    if (request.indexOf("/CalibrateIR") != -1)  {
    NT = 1;
    page = 0;//calibration should be done with multiple IR inputs as in page 0 locations
  } 
     
}

 
  // Return the response - HTML Begins
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html><body style=\"background-color:PeachPuff;\"><font size=\"7\"><center>");
//For all HTML syntax, extra "quotes" must be preceded by \ to avoid syntax errors (per HTML code below)

//Adjust antenna position according to request 
if (page == 0) {   // begining of initial antenna control of HTML page 0
  
  client.print("Position ; "); 
  client.print(Pm);
  client.print(" Degrees for Channels "); 
  
  if (Pm == 104) {//Watertown
  client.print(" 7 21 35 41 "); 
  }
  if (Pm == 147) {//Syracuse
  client.print(" 17 24 25 44 45 47  "); 
  }
  if (Pm == 189) {//Rochester
  client.print("  10 28 45 16 "); 
  }
  if (Pm == 212) {//Buffalo
  client.print("  7 33 39  "); 
  }
  if (Pm == 255) {//Brighton
  client.print(" 27 30  "); 
  }
  if (Pm == 283) {//Peterborough
  client.print(" 12 35  "); 
  }          
  client.println("<br>"); 
  if (P==Pm){ //Antenna is at set position
   client.print("Antenna Set.<br>");
   Serial.println("Antenna Set.");
  }
  else {//Antenna is still rotating
   client.print("Please Wait, Antenna Rotating.<br>");
   Serial.println(" Antenna Rotating.");
  }

    
   client.print("   irT is ");
   client.print(irT);
   Serial.println(irT);


  
  
      client.println("<br><br>"); 

 //beginning of channel by location choices
client.println("<a href=\"/Watertown\"\"><button type=\"Button\" style=\" background-color:#fcc977; height: 150px; width: 300px; margin-right: 30px; font-size: 100%;\">Watertown </button></a>");
client.println("<a href=\"/Syracuse\"\"><button type=\"Button\" style=\" background-color:#fcc977; height: 150px; width: 300px; font-size: 100%\">Syracuse </button></a><br><br>");
client.println("<a href=\"/Rochester\"\"><button type=\"Button\" style=\" background-color:#fcc977; height: 150px; width: 300px; margin-right: 30px; font-size: 100%;\">Rochester </button></a>");
client.println("<a href=\"/Buffalo\"\"><button type=\"Button\" style=\" background-color:#fcc977; height: 150px; width: 300px; font-size: 100%\">Buffalo </button></a><br><br>");
client.println("<a href=\"/Brighton\"\"><button type=\"Button\" style=\" background-color:#fcc977; height: 150px; width: 300px; margin-right: 30px; font-size: 100%;\">Brighton</button></a>");
client.println("<a href=\"/Peterborough\"\"><button type=\"Button\" style=\" background-color:#fcc977; height: 150px; width: 300px; font-size: 100%\">Peterborough </button></a><br><br>");
client.println("<a href=\"/Refresh Status\"\"><button type=\"Button\" style=\" background-color:#b6d8ee; height: 150px; width: 300px; font-size: 100%;\">Refresh</button></a>");
client.println("<a href=\"/FineTune\"\"><button type=\"Button\" style=\" background-color:#fcfb84; height: 150px; width: 300px; font-size: 100%\">FineTune</button></a><br><br>");
client.println("<br>");

client.println("<br><br>"); 
}//end page 0
  
  
if (page == 1) {   // begining of control per HTML page 1
  
   client.print("Position ; "); 
  client.print(Pm);;
  client.print(" Degrees "); 

  if (NT ==1 ) {
    client.println(" ; ");
    client.print("Set to Calibrate irT ; ");
  }
  
  if (Pm < 104) {
  client.print(" North of Watertown "); 
  }
  if ((Pm > 104) && (Pm < 147)) {
  client.print(" Between Watertown & Syracuse"); 
  }
  if (( Pm > 147 ) && ( Pm <189 ))  {
  client.print(" Between Syracuse & Rochester "); 
  }
  if (( Pm > 189 ) && ( Pm <212 )) {
  client.print(" Betweem Rochester & Buffalo  "); 
  }
  if (( Pm > 212  ) && ( Pm < 283 )) {
  client.print(" Between Buffalo & Brighton  "); 
  }
  if (Pm > 283) {
  client.print(" North of Peterborough  "); 
  }          
  client.println("<br>"); 
  if (P==Pm){ //Antenna is at set position
   client.print("Antenna Set.<br>");
   Serial.println("Antenna Set.");
  }
  else {//Antenna is still rotating
   client.print("Please Wait, Antenna Rotating.<br>");
   Serial.println(" Antenna Rotating.");
  }
  client.println("<br>");


client.print("Rotate 4 Degrees <br>");
client.println("<a href=\"/CounterClockwise\"\"><button type=\"Button\" style=\" background-color:#bff4b0; height: 150px; width: 300px; margin-right: 30px; font-size: 100%;\">CCW</button></a>");
client.println("<a href=\"/Clockwise\"\"><button type=\"Button\" style=\" background-color:#bff4b0; height: 150px; width: 300px; font-size: 100%\">CW</button></a><br><br>");
client.println("<a href=\"/Reset\"\"><button type=\"Button\" style=\" background-color: #f08d77; height: 150px; width: 300px; margin-right: 30px; font-size: 100%;\">Reset</button></a>");
client.println("<a href=\"/Location\"\"><button type=\"Button\" style=\" background-color:#fcfb84; height: 150px; width: 300px; font-size: 100%\">Location</button></a><br><br>");
client.println("<a href=\"/Refresh Status\"\"><button type=\"Button\" style=\" background-color:#b6d8ee; height: 150px; width: 300px; font-size: 100%;\">Refresh</button></a><br><br>");
client.println("<a href=\"/CalibrateIR\"\"><button type=\"Button\" style=\" background-color:#f08d77; height: 150px; width: 300px; font-size: 100%;\">CalibrateIR</button></a>");
client.println("<br>");
if (P > 340) {client.print("Beyond upper limit, returned to Watertown, Reset Recommended<br>");}
if (P < 20) {client.print("Beyond Lower limit, returned to Watertown, Reset Recommended<br>");}

client.println("<br><br>"); 
}//end page1


client.println("</font></body></html>");
//HTML ENDS

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  }
 




//Subs
//    >======================================>
void Connect()
{
 // This loop is intended to re-open a connection but first we close the connection just in case:
    WiFi.disconnect();
    
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
//=============================================================
void printWiFiStatus() {
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}
//==============================================================
void inZero()//on boot or soft reset, ccw rotation to zero position

{
  Serial.print(" INZERO ");
  Serial.println("");
  zero = digitalRead (5);
  
  if (zero == HIGH) { //the switch is closed
    allOff(); // so stop 
    P = 0; //Set position to Zero sand triggers end of INZERO
    Pm = Pa; //set target to default
    Serial.print("Position P is now: ");
    Serial.print(P);
    Serial.print(" Change Position target: ");
    Serial.print(Pa);//antenna position default
    Serial.print('\n');
  }
  if (zero == LOW) { // the switch is open
  rotateCCW();//rotate until we hit zero position 
  delay (40); 
  Serial.print(" hope this 100 ms delay helps the soft reset: ");
  Serial.print('\n');    
    }
}

//============================================   

void allOff()
{
  Serial.println("No rotation");
  digitalWrite(cwPin, LOW);//Turn CCW off
  digitalWrite(ccwPin, LOW); //ccw  LED off
  digitalWrite(irLED, LOW);//IR emittor off
  Serial.println("irT,Pm,Cycle,rot,P,Pd,CountW,CountB,page");
  Serial.println(irT);Serial.println(Pm);Serial.println(Cycle);
  Serial.println(rot);Serial.println(P);Serial.println(Pd);Serial.println(countW);
  Serial.println(countB);Serial.println(page);
  rot = 0 ;
}

void rotateCCW()
{
    Serial.println("Rotating CCW");
    digitalWrite(irLED, HIGH);//IR emittor on
    digitalWrite(ccwPin, HIGH);//Turn CCW on
    digitalWrite(cwPin, LOW); //Turn CW of
    rot = -1 ;
}

void rotateCW()
{
    Serial.println("Rotating CW");
    digitalWrite(irLED, HIGH);//IR emittor on
    digitalWrite(ccwPin, LOW); //Turn CCW off
    digitalWrite(cwPin, HIGH);//Turn CCW on
    rot = 1 ;
}
//==================
void chooseMap()  //determines Pd or rotation amount
{        
         while (P == -5 ){//should only occur on boot or reset
               Serial.println("Finding Zero Position ");  
               inZero();
               }
         if (P > Pm){ //sets Pd with ccw rotation
                  if (P<20) { Pm = 104 ;
                   Serial.println("Beyond limits");
                   }
                  
               Pd = (P - Pm);//dark light transitions, one per degree
               rotateCCW(); 
               Serial.println("CCW to desired position ");         
               }
         if (P < Pm && P!=(-5)){//sets Pd with cw rotation
                if (P > 340) {Pm = 104 ;
                  Serial.println("Beyond limits");
                  }   
                Pd = (Pm - P);//dark light transitions, one per degree
                rotateCW();
                Serial.println("CW to desired Position "); 
                }      
         while (Pd > 0){//initiates Mapping X Pd & resets int Cycle
               Serial.println("Mapping "); 
               Cycle = 0;
               goMapped();         
               }
         allOff(); //Pd=0
         
}
//======================================
void goMapped()
       {            //rotate to mapped position Pm

  int reading = analogRead(sensorPin);    //Read irT strength
  Serial.print("Transition Pd = ");
  Serial.println(Pd);
  Serial.print("irt = ");
  Serial.println(reading);

  if (reading < irT){//Light     
    if (reading < irTd){irTd = reading;}
    if (reading > irTl){irTl = reading;}
    if (countW == 0) {
      Pd = Pd - 1 ;
      Serial.println("Light Cycle");
    countW = 1 ;//light cycle counted }
    countB = 0;//reset dark cycle
      } 
      delay (5);      
    }

  if (reading > irT){//Dark
    if (reading < irTd){irTd = reading;}
    if (reading > irTl){irTl = reading;}
    if (countB == 0) {
      Pd = Pd - 1 ;
      Serial.println("Dark Cycle");
    countB = 1 ;//Dark cycle counted }
    countW = 0;//reset Light cycle
      } 
      delay (5); 
    }
    if (Pd == 0) {
      P = Pm ;
      countB = 0;//reset dark cycle
      countW = 0;//reset Light cycle
      if (NT==1){
      irT = ((irTl + irTd)/2);
      Serial.print("irTl and irTd ");
      Serial.println(irTl, irTd);
      irTl = irT;
      irTd = irT;
      NT = 0;
      }
    }
    
  }

//================


