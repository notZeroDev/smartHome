#include "https.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Original blocking function now private
void sendHTTPAsync(String url) {
  Serial.println("HTTP Request to: " + url);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    http.GET();
    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }
}

// Task wrapper to call the function in parallel
void httpTask(void *parameter) {
  String *url = static_cast<String *>(parameter);
  sendHTTPAsync(*url);
  delete url;  // Free memory

  vTaskDelete(NULL);  // Delete this task
}

// Call this to start the request without blocking
void makeHTTPRequest(String url) {
  String *urlCopy = new String(url);  // Create heap copy to pass into task
  xTaskCreate(
    httpTask,        // Task function
    "HTTP_Request",  // Task name
    8192,            // Stack size (bytes)
    urlCopy,         // Parameter to pass
    1,               // Priority
    NULL             // Task handle
  );
}
