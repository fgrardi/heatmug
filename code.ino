#include <Adafruit_MLX90614.h>            // Library for the infrared sensor, to read the temp correctly
#include <ezButton.h>                     // Library that simplifies the button press
#include <ArduinoHttpClient.h>            // Library used for http request
#include <WiFiNINA.h>                     // Library to work with the Wi-Fi of the Arduino Nano 33 IoT

const char* ssid = "Studentenhuis MDC";   // your network SSID (name)
const char* password = "tmc2016dc";       // your network password (use for WPA, or use as key for WEP) 
const char* host = "heatmug.be";          // your server name
int port = 80;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, host, port);
             
int status = WL_IDLE_STATUS;              // The Wi-Fi radio's status
int ledState = LOW;                       // LedState used to set the LED
unsigned long previousMillisInfo = 0;     // Will store last time Wi-Fi information was updated
unsigned long previousMillisLED = 0;      // Will store the last time LED was updated
const int intervalInfo = 5000;            // Interval at which to update the board information

// Object with possible temperature scales
enum TemperatureScale {
  FAHRENHEIT,
  CELSIUS
};

const TemperatureScale tempScale = CELSIUS; // Change scale, code will calculate temperature dynamically
 
int minTemp = 40; // Set minimum temperature to 40° (when temp is CELSIUS) 
int maxTemp = 90; // Set maximum temperature to 90° (when temp is CELSIUS)

float intermediateSize = (maxTemp - minTemp) / 3; // Calculate intermediate temperatures to show on LED's
const int ledPins[5] = {6, 7, 8, 9, 10}; // Array of used pins for the LED's
ezButton limitSwitch(4);  // create ezButton object that attach to pin 4;

Adafruit_MLX90614 sensor = Adafruit_MLX90614(); // Library used to read temperature of MLX90614 sensor

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  
   // Set debounce time to 50 milliseconds
  limitSwitch.setDebounceTime(50);

  // Set the LED as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup for temperature sensor
  sensor.begin();

  // Looping over pins for LED's
  for(int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Attempt to connect to Wi-Fi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, password);

    // Wait 10 seconds for connection:
    delay(10000);
  }

  // You're connected now, so print out the data:
  Serial.println("You're connected to the network");
  Serial.println("---------------------------------------");
}

// Function to show the correct amount of LED's 
void controlLights(int numLights) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], i < numLights ? LOW : HIGH);
  }
}

// Read temperature depending on temperature scale
float readTemperature() {
  if (tempScale == FAHRENHEIT) {
    return sensor.readObjectTempF(); // Function that reads the temperature in Fahrenheit;
  }
  if (tempScale == CELSIUS) {
    return sensor.readObjectTempC(); // Function that reads the temperature in Celcius
  }
  return 0; // Error, something is wrong
}

// Calculate number of lights depending on temperature
int getNumberOfLights(float temp) {
  if (temp <= minTemp) {
    return 1;
  }
  if (temp <= minTemp + intermediateSize) {
    return 2;
  }
  if (temp <= minTemp + 2 * intermediateSize) {
    return 3;
  }
  if (temp <= maxTemp - intermediateSize) {
    return 4;
  } else {
    return 5;
  }
}

void loop() {
  limitSwitch.loop(); // MUST call the loop() function first

  // Get state of limit switch to see if button is pressed or not pressed
  int state = limitSwitch.getState();

  // If button is not pressed, post to http server and put light to 0
  if(state == HIGH) {
    Serial.println("Limit switch is not pressed");
    controlLights(0); // Put all lights off when button is not pressed
    postRequestWhenNotPressed(); // Post to http server with status not active

  // Else, button is pressed, loop over code to read temperature
  } else {
    Serial.println("Limit switch is pressed");

    float temperature = readTemperature(); // Read temperature and put it in variable temperature
    int numOfLights = getNumberOfLights(temperature); // Read number of lights (1, 2, 3, 4 or 5)
    controlLights(numOfLights); // Write HIGH or LOW depending of number of lights
  
    // Print temperature on serial monitor
    Serial.print("Temperature = ");
    Serial.print(temperature);
  
    // Print number of lights on serial monitor
    Serial.print(" (");
    Serial.print(numOfLights);
    
    if(numOfLights == 1){
      Serial.println(" light)");
    } else {
      Serial.println(" lights)");
    }
    postRequestWhenPressed(); // Post to http server with temperature data
  } 
  delay(200);
}

// Function to post request when the button is pressed 
void postRequestWhenPressed() {
  float temperature = readTemperature(); // Read the temperature from infrared sensor
  String contentType = "application/x-www-form-urlencoded"; // Send formdata in a single block in the http body
  String postData = "temp=" + String(temperature); // Put data in String var

  client.post("/save.php", contentType, postData); // Post request

  // Read the status code and body of the response
  int statusCode = client.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  String response = client.responseBody();
  Serial.print("Response: ");
  Serial.println(response);
}

// Function to post request when the button is not pressed 
void postRequestWhenNotPressed() {
  float temperature = readTemperature(); // Read the temperature from infrared sensor
  String contentType = "application/x-www-form-urlencoded"; // Send formdata in a single block in the http body
  String postData = "temp=0"; // Put data equal to 0 when button not pressed

  client.post("/save.php", contentType, postData); // Post request

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  String response = client.responseBody();
  Serial.print("Response: ");
  Serial.println(response);
}
