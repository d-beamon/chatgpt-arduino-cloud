// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)

void onMessageChange();
void onResponseChange();
void onTextChange();
void onSendMessageChange();

String message;
String response;
String text;
bool send_message;

void initProperties(){

  ArduinoCloud.addProperty(message, READWRITE, ON_CHANGE, onMessageChange);
  ArduinoCloud.addProperty(response, READWRITE, ON_CHANGE, onResponseChange, 1);
  ArduinoCloud.addProperty(text, READWRITE, ON_CHANGE, onTextChange);
  ArduinoCloud.addProperty(send_message, READWRITE, ON_CHANGE, onSendMessageChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);