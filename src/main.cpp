#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include "ESPAsyncWebServer.h"

#include <PageSource.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
AsyncWebServer webServer(80);

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("waygate");
  LittleFS.begin();

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // reply to all requests with same HTML
  webServer.onNotFound([](AsyncWebServerRequest *request) {
    request->send(200, "text/html", pageSource);
  });

  // the POST message response
  webServer.on("/message/", HTTP_POST, [](AsyncWebServerRequest *request){
    const String from = request->getParam("from", true)->value();
    const String body = request->getParam("body", true)->value();
    const String timestamp = request->getParam("timestamp", true)->value();

    // write this to storagae
    File f = LittleFS.open("/messages.csv", "a");
    if (f) {
      f.println(from + "\037" + body + "\037" + timestamp);
    } else {
      Serial.println("file open failed");
    }

    request->redirect("/");
  });

  webServer.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
}
