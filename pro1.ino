/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>
#include "dht.h"
const int trigPin = 3; 
const int echoPin = 2; 
int duration;
float distance;
const int fan=6;
int temperature;
int sensorPin = A1; // select the input pin for LDR 
int sensorValue = 0; // variable to store the value coming from the sensor 
#define dht_apin A0                                                       // Analog Pin sensor is connected to
dht DHT;
                                                                          // Enter a MAC address and IP address for your controller below.
                                                                          // The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 137 , 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
   
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  pinMode(trigPin, OUTPUT);                                   ///// Ultrasonic pins
  pinMode(echoPin, INPUT);                                    ///// ultrasonic pins
  pinMode(fan,OUTPUT);//client.print(" The ");
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin ///

           ////////////////////////////////////////////////////////LASER

            client.print("1. The Patient Saline Status-");

           //////////////////////////////////////////////////////////end LASER
          
     /////////////////////////////////////////////////////////////// swap DH11
     DHT.read11(dht_apin);
     client.println("<br />");
    client.println("<br />");
    client.println("<br />");
    client.print("2.Humidity & Temperature -");
    //Serial.print("Current humidity = ");
     client.println("<br />");
     client.println("<br />");
    client.print("Current humidity = ");
    //Serial.print(DHT.humidity);
    client.print(DHT.humidity);
    //Serial.print("%  ");
    client.print("%  ");
    //client.println("<br />");
    client.println("<br />");
    //Serial.print("temperature = ");
    client.print("Temperature = ");
    //Serial.print(DHT.temperature); 
    client.print(DHT.temperature);
    //Serial.println("C  ");
    client.print("C  ");
    delay(5000);//Wait 5 seconds before accessing sensor again.
    /////////////////////////////////////////////////////////////end dht11      

    /////////////////////////////////////////////////////////////// swap ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    client.println("<br />");
    client.println("<br />");
    client.println("<br />");
    client.print("3.The Status of Dustbin -" );
    if(distance<=13)
      { 
         client.print(" Full ");
      }
      else
      {
         client.print(" Not Full Yet ");
        }
      client.print("\n");
    //Serial.print("Distance: ");
    //Serial.println(distance);
    //client.print("Distance: ");
    //client.print(distance);
    delay(500); // waiting for .5 sec
    ////////////////////////////////////////////////////////////////end ultrasonic

    ////////////////////////////////////////////////////////////LDR 
        //sensorValue = analogRead(sensorPin); // read the value from the sensor 
       //Serial.println(sensorValue); //prints the values coming from the sensor on the screen 
    client.println("<br />");
    client.println("<br />");
    client.println("<br />");
       client.print("4.Electricity Status -");
       temperature=DHT.temperature;
        client.println("<br />");
if(temperature>=30) 
{analogWrite(fan,255);
    client.println("The Fan speed - Full");                  }   // High Temp full speed
if(temperature>=22&&temperature<=29)
{analogWrite(fan,220);
client.println("The Fan speed - Modrate");} // Normal Temp Modrate speed
if(temperature>=15&&temperature<=21)
{analogWrite(fan,175);
client.println("The Fan speed - Low");} // Low Temp Low Speed
if(temperature<=15)
{analogWrite(fan,0);
client.println("The Fan is off");}  // Extreme Low Fan off
    
        //delay(100); 
    /////////////////////////////////////////////////////////////end LDR




          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

