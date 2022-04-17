#include <Arduino.h>
#include "WiFi.h"
#include <string.h>
#include <HTTPClient.h>
#include "../lib/ArduinoJson/ArduinoJson-v6.19.4.h"

// #define SERIAL_RX_BUFFER_SIZE 2048
// #define _SS_MAX_RX_BUFF 2048 // RX buffer size

#define DEBUG 0
#define DEBUG2 0

const char* date = NULL;
// char* hour = ""; 
// char* min = "";
// char* sec = "";

const char *weather_data_URL = "https://devapi.qweather.com/v7/weather/now?lang=en&key=7f64706b4a41490580c9372aa2546ea2&location=108.94,34.22&gzip=n";
// const char* bili_follower_data_URL = "http://api.bilibili.com/x/relation/stat?vmid=520265599";

#if !DEBUG2

bool WiFiPswdCheck(char *wifissid, char *wifipswd)
{

#if DEBUG
  Serial.print(wifipswd);
#endif

  int timewaiting = 0;

  WiFi.begin(wifissid, wifipswd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    timewaiting++;
    //等待时间超过30*500ms = 15s
    if (timewaiting >= 20)
    {
      return false;
    }
  }

  return true;
}

void getWeatherData()
{
  DynamicJsonDocument doc(1024);

  int temp = 0;
  int humidity = 0;
  int date = 0;

  HTTPClient httpclint;
  httpclint.begin(weather_data_URL);
  // httpclint.begin(bili_follower_data_URL);

  // Serial.println("http start2");

  int httpcode = httpclint.GET(); //获得请求码：200 OK

  Serial.printf("Http code = %d \n", httpcode);

  if (httpcode < 0)
  {
    Serial.println("web error");
  }
  else if (200 == HTTP_CODE_OK)
  {
    String recBuffer = httpclint.getString();
    // Serial.println(recBuffer);

    deserializeJson(doc, recBuffer);
    temp = doc["now"]["temp"];
    humidity = doc["now"]["humidity"];
    // date = doc["now"]["obsTime"];
    // Serial.println(date);

    Serial.printf("Temp: %2d, Humidity: %2d%%",temp, humidity);
    Serial.println("");
  }
}

void WiFiSelect()
{

  Serial.println("Select one to connect:");

  int WiFiNumber = 0;
  // int count = 0;

  while (1)
  {

    //等待串口接收数据
    while (!Serial.available())
      ;
    //从串口接收选择的wifi序号
    WiFiNumber = Serial.parseInt() - 1;

    //检测wifi序号是否正确
    if (WiFiNumber < 0 || WiFiNumber >= 10)
    {
      Serial.println("Wrong Number, Select one again");
    }

    // wifi正确：则输出所选wifi的SSID 并要求输入密码
    else
    {
      Serial.println("You have selected: " + WiFi.SSID(WiFiNumber));
      Serial.print("Type in pswd: ");

      char ssid[30] = "";
      char pswd[30] = "";

      //等待输入wifi密码，将参数传给WiFiPswdCheck()
      while (!Serial.available())
        ;
      strcpy(ssid, WiFi.SSID(WiFiNumber).c_str());
      Serial.readString().toCharArray(pswd, sizeof(pswd));

      if (WiFiPswdCheck(ssid, pswd))
      {
        Serial.println("");
        Serial.print("WiFi connected, IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Gateway is: ");
        Serial.println(WiFi.gatewayIP());
        break;
      }
      else
      {
        Serial.println("WiFi connecting error!");
        break;
      }
    }
    // count++;
    // if (count>=5) break;
  }
}

void setup()
{
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
  Serial.println("scan start");
  

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println("networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
      if (i > 8)
      { //只显示前五个wifi接入点
        break;
      }
    }
  }
  // Wait a bit before scanning again
  // delay(5000);
  WiFiSelect();

  getWeatherData();
}


void loop()
{

  delay(5000);
  getWeatherData();
  
}

#endif

#if DEBUG2

String mystring = "{" code ":" 200 "," updateTime ":" 2022 - 04 - 11T17 : 17 + 08 : 00 "," fxLink ":" http : // hfx.link/1tpv1","now":{"obsTime":"2022-04-11T17:13+08:00","temp":"23","feelsLike":"16","icon":"104","text":"阴","wind360":"315","windDir":"西北风","windScale":"5","windSpeed":"37","humidity":"29","precip":"0.0","pressure":"961","vis":"11","cloud":"100","dew":"6"},"refer":{"sources":["QWeather","NMC","ECMWF"],"license":["no commercial use"]}}";
                                                                                                             char mychararray[50] = "what!";
int i = 0;
char c = 'a';

void setup()
{
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
}

void loop()
{
  // // Serial.println(mystring);
  // Serial.println(sizeof(mystring));
  // mystring.toCharArray(mychararray, sizeof(mychararray));
  // Serial.println(sizeof(mychararray));
  // Serial.println(mystring);
  // delay(500);

  Serial.println(mystring);
  Serial.println(mychararray);
  Serial.println(i);
  Serial.println(c);
  // Serial.print());
  delay(1000);
}

#endif