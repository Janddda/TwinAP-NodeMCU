#include <Arduino.h>

#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  server(80);          // HTTP server

String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
                      "be redirected here.</p></body></html>";


void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("FreeWifi");
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Wifi..........OK");
  dnsServer.start(DNS_PORT, "*", apIP);
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("DNS...........OK");
  server.onNotFound([]() {
    server.send(200, "text/html", responseHTML);
  });
  server.on("/submit", handleSubmit);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

void handleSubmit(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Submeteu");
  lcd.setCursor(0, 0);
  lcd.print(server.arg("id"));
  server.send(200, "text/html", server.arg("id"));
  }
