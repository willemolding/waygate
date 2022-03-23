#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String siteHTML = ""
"<!DOCTYPE html><html lang='en'><head>"
"<meta name='viewport' content='width=device-width'>"
"<title>WAYGATE</title></head><body>"
"<h1>You've found something interesting!</h1>"
"<p>This is a Waygate. Its a little place where you can leave your mark. Leave messages for people around you and read what other people have wrote.</p></body></html>"
"";

void handlePostMessage() {
  if (webServer.method() != HTTP_POST) {
    webServer.send(405, "text/plain", "Method Not Allowed");
  } else {
    // arg("plain") gets the POST body
    webServer.send(200, "text/plain", "The new message is:\n" + webServer.arg("plain"));
  }
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("waygate");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // reply to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", siteHTML);
  });

  // the POST message response
  webServer.on("/message/", handlePostMessage);

  webServer.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}