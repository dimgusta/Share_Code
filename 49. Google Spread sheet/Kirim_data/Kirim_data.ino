#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

//----------------------------------------SSID dan Password wifi mu gan.
const char* ssid = "iPhone 13"; //--> Nama Wifi / SSID.
const char* password = "resapannya"; //-->  Password wifi .
//----------------------------------------

//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

String GAS_ID = "AKfycbzwDkuEwkNFbU6M8U4T5a0_uqgKRbxev0xtd7l_3A"; //--> spreadsheet script ID

//============================================================================== void setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);

  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");

  pinMode(ON_Board_LED, OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //-->

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();
}
void loop() {
  float nilai1 = 11;
  float nilai2 = 20;
  float nilai3 = 30;
  float nilai4 = 40;
  float nilai5 = 50;

  sendData(nilai1, nilai2, nilai3, nilai4, nilai5);
  delay(5000);
}

//==============================================================================
//============================================================================== void sendData
// Fungsi  untuki Kirim data to Google Sheets
void sendData(float teg, float arus, float daya, float energi, float biaya) {
  Serial.println("=========="); Serial.print("connecting to "); Serial.println(host);

  //----------------------------------------Connect ke Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Proses dan kirim data
  String string_teg =  String(teg, 2); // fungsi DEC mengakhiri value terakhir
  String string_arus =  String(arus, 2); // fungsi DEC mengakhiri value terakhir
  String string_daya =  String(daya, 2); // fungsi DEC mengakhiri value terakhir
  String string_energi =  String(energi, 2); // fungsi DEC mengakhiri value terakhir
  String string_biaya =  String(biaya, 2); // fungsi DEC mengakhiri value terakhir

  String url = "/macros/s/" + GAS_ID +
               "/exec?Volt=" + string_teg +
               "&Current=" + string_arus +
               "&Power=" +  string_daya  +
               "&Energy=" + string_energi +
               "&Cost=" + string_biaya
               ;
  //metode HTTP yang digunakan
  //https://script.google.com/macros/s/AKfycbzwDkuEwkNFbU6M8U4T5a0_uqgKRbxev0xtd7l_3A/exec?Volt=1&Current=2&Power=3&Energy=4&Cost=5


  Serial.print("requesting URL: "); Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Check data terkirim atau tidak
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}
