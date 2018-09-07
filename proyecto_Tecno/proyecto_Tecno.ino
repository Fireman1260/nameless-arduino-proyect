
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <SPI.h>
#include <Process.h>


String deviceId = "B4:21:8A:F0:1E:5E";

float humidity = 0;
float tempf = 0;
  
// Select the pin used on LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
byte mac[6];
int power = A3;
int GND = A2;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;

void setup(){    
    pinMode(power, OUTPUT);
    pinMode(GND, OUTPUT);

    digitalWrite(power, HIGH);
    digitalWrite(GND, LOW);

     lcd.begin(16, 2);  

    //Initialize the I2C sensors and ping them
    sensor.begin();
  Bridge.begin();  // Initialize the Bridge
  Console.begin();  // Initialize the Serial
}

void loop()
{

  Serial.println("Inicia");

   //Get readings from all sensors
    getWeather();
    printInfo();
    postData();
    delay(3000);
    
  //}
    
}

void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTempF();
  
}

void printInfo(){
 /*LECTURA DE TEMPERATURA*/
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(6,0);
  lcd.print(tempf);
  lcd.setCursor(11,0);
  lcd.print("F");

  /*LECTURA DE HUMEDAD*/
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.setCursor(10,1);
  lcd.print(humidity);
  lcd.setCursor(15,1);
  lcd.print("%");
}

void postData()
{
   String var = "";
  String value1 = String(tempf);
  String value2 = String(humidity);
  String value3 = deviceId;

  var = "{\"Temp\":" + value1 + ",\"Hum\":" + value2 + ",\"Dispositivo\":\"" + value3 + "\"}";
  Serial.println(var);

  HttpClient client;
  client.setHeader("Content-Type: application/json");
  String httpBody = var;
  String httpDestination = "http://tecproyectoapi.azurewebsites.net/api/postData"; //URL Sever 
  client.post(httpDestination, httpBody);

}
