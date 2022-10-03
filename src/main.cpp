#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h> 

/*
I, Mychaylo Tatarynov, student number 000800271, certify that all code submitted is my own work; 
that I have not copied it from any other source. 
I also certify that I have not allowed my work to be copied by others.

2022/10/03
*/

//Setting up the interface for the sensor.
const int OneWireBus = D3;  
OneWire oneWire(OneWireBus);
DallasTemperature DS18B20(&oneWire);
//Place to save the 8-byte sensor address.
DeviceAddress TempAddress;

//Whether or not the device was connected last iteration.
bool LastConnected = false;

//Converts a device address into an uppercase hex string.
String addressToString(DeviceAddress address) {
  String output = "";
  for (int i = 0; i < 8; i++) {
    output += String(address[i], HEX);
  }
  output.toUpperCase();

  return output;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  DS18B20.begin();

  Serial.println("\n\nName: Mychaylo Tatarynov (000800271)");
  Serial.println("ESP8266 Chip Id: " + String(ESP.getChipId()));
  Serial.println("Flash Chip Id: " + String(ESP.getFlashChipId()) + "\n");

  //Whether or not the device is currently connected.
  bool isConnected = DS18B20.getAddress(TempAddress, 0);

  if (isConnected) {
    Serial.println("Found DS18B20 sensor with address " + addressToString(TempAddress));
  } else {
    Serial.println("No DS18B20 temperature sensors are installed!");
  }

  LastConnected = isConnected;
}

void loop() {
  // put your main code here, to run repeatedly:

  //Whether or not the device is currently connected. Despite being the same thing as in setup, the value has no reason to be global.
  bool isConnected = DS18B20.isConnected(TempAddress);
  //Whether or not the sensor's connection status has changed since last iteration.
  bool connectionChanged = isConnected != LastConnected;

  //If the sensor is disconnected live, the program will wait until it is reconnected. If it was never connected to begin with,
  //then there is no address and the program must be restarted to work with the sensor once it is connected.
  if (connectionChanged) {
    if (isConnected) {
      Serial.println("DS18B20 sensor reconnected!");
    } else {
      Serial.println("DS18B20 sensor disconnected!");
    }
  }

  LastConnected = isConnected;
  if (!isConnected) {
    return;
  }
  
  //Telling the sensor to read temperature and grabbing. Since only 1 sensor is present, we grab the first index.
  DS18B20.requestTemperatures(); 
  double temperature = DS18B20.getTempCByIndex(0);
  
  String judgement;
  if (temperature < 10) {
    judgement = "cold!";
  } else if (temperature < 15) {
    judgement = "cool";
  } else if (temperature < 25) {
    judgement = "perfect";
  } else if (temperature < 30) {
    judgement = "warm";
  } else if (temperature < 35 ) {
    judgement = "hot";
  } else {
    judgement = "too Hot!";
  }

  Serial.println("Current temperature is: " + String(temperature) + "°C or " + judgement);

  delay(5000);
}