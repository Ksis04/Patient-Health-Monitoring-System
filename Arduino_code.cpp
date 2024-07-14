#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define DHTPIN D4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

// DS18B20 sensor configuration
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
PulseOximeter pox;

/* 1. Define the WiFi credentials */
// #define WIFI_SSID "TP-Link_F9CB"
// #define WIFI_PASSWORD "1234567890"
#define WIFI_SSID "Surya3551"
#define WIFI_PASSWORD "28301205"


// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBxIkX3AI-kcVgTyIrFQRxE0VSc900z_U0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://health-report1-default-rtdb.firebaseio.com/"

/* 4. Define the user Email and password that already registered or added in your project */
#define USER_EMAIL "naharsha46@gmail.com"
#define USER_PASSWORD "@#Harsha1209#@"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Initialize DS18B20 sensor
    sensors.begin();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the API key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign-in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // See addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    // Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);

    Firebase.setDoubleDigits(5);
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    sensors.requestTemperatures();
    float bt = sensors.getTempCByIndex(0);
    // Serial.print("Temperature: ");
    // Serial.println(t);
    // Serial.print("Humidity: ");
    // Serial.println(h);
    // Serial.print("Body Temperature: ");
    // Serial.println(bt);

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
        sendDataPrevMillis = millis();

        // Create a unique key for each set of data
        String tempKey = Firebase.pushInt(fbdo, "/test/temperature", t) ? fbdo.pushName() : "";
        Serial.printf("Push Temperature... %s\n", tempKey.c_str());

        String humidityKey = Firebase.pushInt(fbdo, "/test/humidity", h) ? fbdo.pushName() : "";
        Serial.printf("Push Humidity... %s\n", humidityKey.c_str());

        String btKey = Firebase.pushInt(fbdo, "/test/temperatureC_DS18B20", bt) ? fbdo.pushName() : "";
        Serial.printf("Push Body Temperature... %s\n", btKey.c_str());

        // You can also retrieve the data to verify
        Firebase.getFloat(fbdo, "/test/temperature/" + tempKey);
        Serial.printf("Get Pushed Temperature... %s\n", String(fbdo.to<float>()).c_str());

        Firebase.getFloat(fbdo, "/test/humidity/" + humidityKey);
        Serial.printf("Get Pushed Humidity... %s\n", String(fbdo.to<float>()).c_str());

        Firebase.getFloat(fbdo, "/test/temperatureC_DS18B20/" + btKey);
        Serial.printf("Get Pushed Body Temperature... %s\n", String(fbdo.to<float>()).c_str());

        Serial.println();
    }
}
