#ifndef NETWORKCALLABCKS_HPP
#define NETWORKCALLABCKS_HPP

#include "string.h"
#include "LittleFS.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
extern "C" {
  #include<user_interface.h>
}
#else
#include "WiFi.h"
#include "WebServer.h"
#endif
#include "macCache.hpp"

#ifdef ESP8266
extern ESP8266WebServer server;
extern const char *macsFilename; //There were problems with a single definition...
#else
extern WebServer server;
extern const char *macsFilename;
#endif

#ifdef ESP8266
void saveClient(const WiFiEventSoftAPModeStationConnected &event); 
#else
void saveClient(WiFiEvent_t event, WiFiEventInfo_t info);
#endif
void onApIP();
void onClearArchive();

#endif