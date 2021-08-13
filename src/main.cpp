#ifdef ESP8266
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#else //ESP32
#include "FS.h"
#include "../.pio/libdeps/nodemcu-32s/LittleFS_esp32/src/LITTLEFS.h"
#define CONFIG_LITTLEFS_CACHE_SIZE 512 //Improves read speed
#include "WiFi.h"
#include "WebServer.h"
#endif
/* includes by this project */
#include "macCache.hpp"
#include "networkCallbacks.hpp"

/* configuration  wifi */
const char *ssid = "EMERGENCY WiFi access logger";
#ifdef ESP8266
const char *macsFilename = "MACs.txt";
#else
const char *macsFilename = "/MACs.txt";
#endif

struct macAddressCache macCache;

#ifdef ESP8266
static WiFiEventHandler clientConnectedHandler; //stationConnectedHandler
ESP8266WebServer server(80);
#else
WebServer server(80);
#endif

void setup() {
    File macsFile;
    IPAddress apIP;
    #ifdef ESP8266
    FSInfo fs_info;
    #endif

    delay(1000);
    Serial.begin(115200);
    Serial.println("Initializing FS...");

    #ifdef ESP8266
    if (LittleFS.begin())
    #else
    if (LITTLEFS.begin())
    #endif
    {
      Serial.println("FS ready");

      #ifdef ESP8266
      LittleFS.info(fs_info);
      Serial.println("  Total space: " + String(fs_info.totalBytes) + " B");
      Serial.println("  Used space:  " + String(fs_info.usedBytes) + " B");
      Serial.println("  Free space:  " + String(fs_info.totalBytes - fs_info.usedBytes) + " B");
      #else
      Serial.println("  Total space: " + String(LITTLEFS.totalBytes()) + " B");
      Serial.println("  Used space:  " + String(LITTLEFS.usedBytes()) + " B");
      Serial.println("  Free space:  " + String(LITTLEFS.totalBytes() - LITTLEFS.usedBytes()) + " B");
      #endif

      #ifdef ESP8266
      macsFile = LittleFS.open(macsFilename, "r");    //Check file's existence
      #else
      macsFile = LITTLEFS.open(macsFilename, "r");
      #endif

      if(!macsFile){  //If file does NOT exist
        Serial.println("Creating the MAC archive file ");

        #ifdef ESP8266
        macsFile = LittleFS.open(macsFilename, "w");
        #else
        macsFile = LITTLEFS.open(macsFilename, "w");
        #endif
        if(!macsFile){
          Serial.println("  ERROR WHILE CREATING MAC ARCHIVE FILE");
          while(1){}
        } else {
          macsFile.close();
        }
      } else {                    //If file does exist
        macCacheInit(macsFile);   //Initiate the MAC cache
        macsFile.close();
      }
    }else{  //If filesystem begin() failed
      Serial.println("FS failed to init");
      while(1){}
    }

    Serial.print("Configuring AP,");
    #ifdef ESP8266     //Hook callback to whenever a device connects to the WiFi access point
    clientConnectedHandler = WiFi.onSoftAPModeStationConnected(saveClient);
    #else
    WiFi.onEvent(saveClient, WiFiEvent_t::SYSTEM_EVENT_AP_STACONNECTED);
    #endif
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    apIP = WiFi.softAPIP();
    Serial.println(" IP address: " + apIP.toString());
    Serial.println("WiFi handler hooked");
    server.on("/", onApIP);   //Hook callbacks to whenever there is an HTTP request on apIP and apIP/clearArchive 
    server.on("/clearArchive", onClearArchive);
    server.begin();
    Serial.println("HTTP server started");  
}

void loop() {
    server.handleClient();    //Operate the server
    delay(500);
}
