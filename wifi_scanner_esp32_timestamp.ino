//* ESP32 WiFi Scanning example with timestamps and WiFi connection */
// Based on this Wokwi code: "https://wokwi.com/projects/305569599398609473"
// Includes time and date stamp and wifi login details. 
// You must connect to a WiFi for the timestamp to work.
// Set for ESP32 WROOM board. Cheers, SteamWolfAU.

#include "WiFi.h"
#include "time.h"

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;  // Change to your timezone offset in seconds
const int daylightOffset_sec = 0;  // Change if using daylight saving

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  
  // Connect to WiFi
  connectToWiFi();
  
  // Initialize time (requires WiFi connection)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Setup done!");
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Time not available";
  }
  char timeString[64];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeString);
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectToWiFi();
  }
  
  Serial.print("[" + getTimestamp() + "] ");
  Serial.println("Scanning...");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.print("[" + getTimestamp() + "] ");
  Serial.println("Scan done!");
  
  if (n == 0) {
    Serial.print("[" + getTimestamp() + "] ");
    Serial.println("No networks found.");
  } else {
    Serial.println();
    Serial.print("[" + getTimestamp() + "] ");
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print("[" + getTimestamp() + "] ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}
