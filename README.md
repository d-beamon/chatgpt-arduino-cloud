# chatgpt-arduino-cloud

# Overview
The project involves using an Arduino IoT Cloud compatible device, such as an Arduino Nano RP2040 Connect or any ESP32/ESP8266 device, as a middleware between the IoT Cloud and OpenAI's GPT-3.5 language model. The device receives prompts (questions) from the IoT Cloud, sends them to the OpenAI API, receives and parses the replies, and sends them back to the IoT Cloud.

To get started, you need to create an OpenAI account, create an API key, and have enough credits. Then, you can create your device on the IoT Cloud, program it, and set up the dashboard on the IoT Cloud. The dashboard allows you to write questions (prompts) and receive ChatGPT's replies.

In testing, there are two ways to interact with ChatGPT that can be used indistinctly: through the chat window or the question, response, send question mechanism. If an error is received, the error code and a description are provided.

The project offers some final considerations, such as variable tweaking, defining the maximum number of tokens that ChatGPT will use in generating a response, and keeping in mind the limits on OpenAI API usage.

# Pre-requisites
Before getting started you need
* to have an OpenAI account
* to create an API key
* have enough credits

## Create an OpenAI account
To create an OpenAI account, you have to go to https://platform.openai.com/ and follow the steps.

## Create an API key
An API key is a necessary step to use the API. 
Go to the following URL https://platform.openai.com/account/api-keys, click on "Create new Secret key" and follow the instructions.
Note down the key, as it will not be possible to see it afterwards.

## Check the number of credits
You can check your usage here: https://platform.openai.com/account/usage
You will see a breakdown list of the number of tokens you have used.

# The setup
The setup is really simple.

You just need an Arduino IoT Cloud compatible device such as an Arduino Nano RP2040 Connect, an Arduino MKR WiFi 1010 or any ESP32/ESP8266 device.
This device will be configured as an Arduino IoT Cloud device and the attached sketch will be running on it. It will act as a middleware between the Arduino IoT Cloud and ChatGPT. It will receive the prompts (questions) from the IoT Cloud, send them to OpenAI API, receive and parse the replies and send them back to the IoT Cloud.
*Note: From now on, we will assume we are using a Nano RP2040 Connect device, although any other IoT Cloud compatible device could be used*

On the IoT Cloud we will have a dashboard that we will use to write the questions (prompts) and receive ChatGPT's replies. You can use your mobile phone and the IoT Remote app compatible with both [Android](https://play.google.com/store/apps/details?id=cc.arduino.cloudiot) and [iOS](https://apps.apple.com/us/app/arduino-iot-cloud-remote/id1514358431).

# The process
This process is really simple
1. Get an OpenAPI key (check Pre-requisistes)
2. Create your device on the IoT Cloud and program it
3. Set up the dashboard on the IoT Cloud
4. Testing

# Create your device on the IoT Cloud and program it
The Nano RP2040 Connect is the device where the software will run.

You just need to follow the following steps: 
1. Create the Device 
Connect the Nano RP2040 Connect board to your PC using the USB cable. Go to the [Devices](https://create.arduino.cc/iot/devices) section of the Arduino IoT Cloud and click on ADD. 
Just follow the instructions on the wizard and your device will be detected, configured to be used by the Arduino Cloud and added to the list of devices. 

2. Create the Thing 
Go to the [Things](https://create.arduino.cc/iot/things) section of the Arduino IoT Cloud and click on CREATE. Give it a name and your device will be created. 

3. Create the Variables 
Add the variables clicking on the ADD button. 
At the end of the process, your list of variables should look like this 

|Name|Type|
|---|---|
|message|String|
|response|String|
|send_message|bool|
|text|String|

4. Associate the Thing with your Device
Go to the Associated Device section and click on “Select Device”. Choose your Nano RP2040 Connect from the list. 

5. Configure the network 
Go to the Network section and click on “Change”. Set the credentials of your WIFI access point. 

6. Edit the sketch 
Paste the sketch on the “Sketch” subsection of the Thing section. You can find the sketch on the Code section at the end of the project. 

7. Add your API Key
There are two ways to include you OpenAI API key. You can choose the one which is more convenient for you.
a) Modify directly the `accessToken` variable on the sketch replacing `SECRET_OPENAI_API_TOKEN` with your API key.
b) Open the full web editor and go to the `Secret` tab. Introduce the API key in the box identified as `SECRET_OPENAI_API_TOKEN`

8. Program the device
Build the sketch and program the device as usual. 
*Note: After the first programming, your device can be updated over-the-air (OTA)*

# Create a dashboard
Go to the Dashboards section and click on CREATE. Assign a name and start adding the widgets according to the following table: 

|Widget type   |Widget name   |Variable name   |   
|---|---|---|
|Messenger|Chat window|message|
|Value|Question|text|
|Value|Response|response|
|Push Button|Send Question|send_message|

The final result can be something like this:

# Let's test the system
The first important thing is that we have implemented two ways to interact with ChatGPT that can be used indistinctly:
1. The Chat Window
Write a question on the box and wait for the answer. It will take a couple of seconds and will give you a parsed answer. That's it.
2. The Question, Response, Send Question mechanism
Write a question on the Question box, push the "Send Question" button and wait for the answer. It will take a couple of seconds and will give you a parsed answer. That's it.

In both cases, if an error is received, you will get the error code and a description.

In order to chat, you can access your dashboard using either your browser or the IoT Remote mobile app. 

# Final considerations
## Variable tweaking
In the sketch, the following variables can be tweaked:
* `maxTokens`: Define the maximum number of tokens (characters) that ChatGPT will use in its response
* `apiEndpoint`: OpenAI has many different endpoints. You can investigate and play with them
* `OpenAImodel`: You can use a different language model. Check the full list [here](https://platform.openai.com/account/rate-limits).

Remember that OpenAI tokens are not free. So remember to limit the maximum number of tokens with the `maxTokens` variable so that your billing doesn't grow without control. Bear that also in mind when you ask your questions.
## LED Blink
The sketch makes the builtin LED blink every 2 seconds. This way, we know that the board is alive. It uses the ArduinoThread library that you can find[here](https://github.com/ivanseidel/ArduinoThread). If you want to disable the blink, you just have to comment out the line `#define USE_THREAD 1` 

## Import the Thing and Dashboard with the Arduino Cloud CLI
In the github project, you can find the YAML files describing the Thing and Dashboard: https://github.com/d-beamon/chatgpt-arduino-cloud/tree/master/IoT-Cloud

You can use the following tutorial to create your Thing and Dashboard using the Arduino Cloud CLI easily and those templates.
* [Create a Thing with Cloud CLI](https://docs.arduino.cc/arduino-cloud/getting-started/arduino-cloud-cli#create-things)
* [Create a Dashboard with Cloud CLI](https://docs.arduino.cc/arduino-cloud/getting-started/arduino-cloud-cli#create-dashboard)

# Future use cases and expansions
There are many follow-up projects to this one. These are the ones that I have in mind:
* Chat with ChatGPT with your voice using a NanoRP2040 Connect. The idea is to use the mic of the board, process the speech, send it to OpenAI, wait for the reply and show the answer. We can use a display, a widget on the dashboard or use a TTS synth with a speaker.
* Use natural language to perform daily actions using a NanoRP2040 Connect board. Use ChatGPT as a parser of natural language receiving the list of actions in an actionable way

