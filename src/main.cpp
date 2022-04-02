#include <ESP8266WiFi.h>
#include <DNSServer.h>
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

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // reply to all requests with same HTML
  webServer.onNotFound([](AsyncWebServerRequest *request) {
    request->send(200, "text/html", pageSource);
  });

  // the POST message response
  webServer.on("/message/", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "The new message is:\n" + request->arg("plain"));
  });

  webServer.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
}
