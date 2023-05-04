#include "arduino_secrets.h"
// Arduino_JSON - Version: Latest 
#include <Arduino_JSON.h>

#define USE_THREAD  1
#ifdef USE_THREAD
  // ArduinoThread - Version: 2.1.1
  // Using the following library for simple blink thread: 
  // https://github.com/ivanseidel/ArduinoThread
# include <StaticThreadController.h>
# include <Thread.h>
# include <ThreadController.h>
#endif

/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/2c89ead6-14b0-4e53-8f61-953a0424fa83 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  String message;
  String response;
  String text;
  bool send_message;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include "ArduinoHttpClient.h"

// Replace with your ChatGPT API endpoint and access token
const char* apiServer   = "api.openai.com";
int port = 443;
const char* apiEndpoint = "/v1/completions";
// Get your OpenAI API token from here: https://platform.openai.com/account/api-keys
//const char* accessToken = "your_OpenAI_access_token";
const char* accessToken = SECRET_OPENAI_API_TOKEN;
int maxTokens = 30;
// Other models: gpt-3.5-turbo
// Check the full list here: https://platform.openai.com/account/rate-limits
String OpenAImodel = "text-davinci-003";

// Some alternative endpoints if you want to play
//const char* apiEndpoint = "/v1/engines/davinci-codex/completions";
//const char* apiFullURL = "https://api.openai.com:443/v1/engines/davinci-codex/completions";
//const char* apiFullURL = "https://api.openai.com:443/v1/completions";

// Replace with your question
String question = "What is the meaning of life?";

// Initialize the WiFi client and HTTP client
WiFiSSLClient wifiClient;
//WiFiClient wifiClient;  // Not needed for OpenAI as it uses HTTPs
HttpClient httpClient = HttpClient(wifiClient, apiServer, port);

bool message_available = false;
bool message_sent = false;

#ifdef USE_THREAD
Thread thread_blink = Thread();
#endif

JSONVar jsonData;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  //ArduinoIoTPreferredConnection.cleanCloudMessage();
  message = PropertyActions::CLEAR;
  message  = "";
  response = "";

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();

#ifdef USE_THREAD
  // Use the Thread library to configure a blink loop
	thread_blink.onRun(blink_loop);
	thread_blink.setInterval(2000);
#endif
  Serial.println("Start!");
}

int http_send(String question)
{
  question.trim();  // Remove leading and trailing spaces that might not be accepted by JSON parsers
  question += ". With max " + String(maxTokens) + " chars.";
  String requestBody = "{\"model\":\"" + OpenAImodel +
      "\",\"prompt\":\"" + question + 
      "\",\"max_tokens\":" + String(maxTokens) + "}";

  Serial.print("Sending question to ChatGPT: ");
  Serial.println(question);

  Serial.print("wifiClient.status(): ");
  Serial.println(wifiClient.status());

  Serial.print("WiFi.status(): ");
  Serial.println(WiFi.status());
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI Not connnected");
    return -1;
  }

  // Prepare the HTTP request
  Serial.println("Begin");
  httpClient.beginRequest();
  Serial.println("post: " + String(apiEndpoint));
  int ret = httpClient.post(apiEndpoint);
  if (ret != 0) {
    Serial.println("Error " + String(ret) + " sending frame to ChatGPT");
    return -1;
  }
  Serial.println(ret);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Authorization", "Bearer " + String(accessToken));
  httpClient.sendHeader("Content-Length", requestBody.length());
  // Not needed
  //httpClient.sendHeader("Accept", "application/json");
  //Serial.println("User-agent");
  //httpClient.sendHeader("User-Agent", "ArduinoWiFi/1.0");
  //Serial.println("Host");
  //httpClient.sendHeader("Host", "api.openai.com");
  
  // Send the question to ChatGPT
  Serial.print("requestBody: ");
  Serial.println(requestBody);
  httpClient.beginBody();
  httpClient.print(requestBody);
  httpClient.endRequest();
  Serial.println("End");

  return 0;
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
  
#if 0
  static unsigned long last = 0;
  if (millis() - last > 5000) {
    Serial.println("Tick:" + String(millis()) + " connected:" + String(ArduinoCloud.connected()));
    last = millis();
  }
#endif

  if (message_available) {
    message_available = false;
    message  = "";
    response = "";
    int ret = http_send(question);
    if (ret == 0)
      message_sent = true;
  }

  // Check if the response is available
  if (message_sent == true) {
    message_sent = false;
    // Read the response and print it to the Serial Monitor
    int statusCode = httpClient.responseStatusCode();
    String responseBody = httpClient.responseBody();
    Serial.println("responseStatusCode: " + String(statusCode));
    Serial.println("responseBody: " + responseBody);

    // If status == 200 ==> Update the variables so that they are sent to the IoT Cloud
    if (statusCode == 200) {
      jsonData = JSON.parse(responseBody);
      String parsedText = jsonData["choices"][0]["text"];
      parsedText.trim();
      response = parsedText;
      message  = parsedText;
      Serial.println("Parsed text: " + response);
    }
    else {
      response = "ERROR " + String(statusCode);
      message  = response;
    }
  }

#ifdef USE_THREAD
	if(thread_blink.shouldRun())
		thread_blink.run();
#endif
}

#ifdef USE_THREAD
void blink_loop()
{
  int led_blink = LED_BUILTIN;
  static int led_status = LOW;
  
  led_status = (led_status == LOW ? HIGH : LOW);
  digitalWrite(led_blink, led_status);

  //Serial.println("blink_loop");
}
#endif

/*
  Since Message is READ_WRITE variable, onMessageChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onMessageChange()  {
  // Add your code here to act upon Message change
  static bool first_time = true;
  // Ignore the first message
  Serial.print("Message: ");
  Serial.println(message);
  if (first_time) {
     // Ignoring the first message just in case the IoT Cloud wants to send us the latest one on init 
    first_time = false;
    Serial.println("Ignoring firt message");
  }
  else {
    message_available = true;
    question = message;
  }
}

/*
  Since Text is READ_WRITE variable, onTextChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTextChange()  {
  // Add your code here to act upon Text change
  Serial.println("New text: " + text);
}

/*
  Since SendMessage is READ_WRITE variable, onSendMessageChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSendMessageChange()  {
  // Add your code here to act upon SendMessage change
  if (send_message) {
    send_message      = false;
    message_available = true;
    question = text;
    Serial.print("New message button triggered. Question: ");
    Serial.println(question);
  }
}

/*
  Since Response is READ_WRITE variable, onResponseChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onResponseChange()  {
 // Add your code here to act upon Response change
}







