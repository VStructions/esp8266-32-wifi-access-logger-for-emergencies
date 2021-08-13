#include "networkCallbacks.hpp"

void onApIP() //This HTTP response serves the MAC archive 
{ 
  #ifdef ESP8266
  File macsFile = LittleFS.open(macsFilename, "r");
  #else
  File macsFile = LITTLEFS.open(macsFilename, "r");
  #endif
  uint16_t nMacs = 0;
  String macs, temp;

  if (!macsFile) {
    Serial.println("  ERROR OPENING FILE");
    server.send(200, "text/html", "<h1>You are connected,<br>    Error opening file</h1>");
  } else {
    while((temp = macsFile.readStringUntil('\n')) != "")  //Read a MAC
    {
      macs += temp.substring(0, 2) + ":" + temp.substring(2, 4) + ":";  //Format the MAC string with : for increased readablity
      macs += temp.substring(4, 6) + ":" + temp.substring(6, 8) + ":";
      macs += temp.substring(8, 10)+ ":" + temp.substring(10, 12)+"<br>"; //Terminate with HTML new line
      nMacs++;
    }
    macsFile.close();
    
    server.send(200, "text/html", "<h1><p>" + String(nMacs) + " MACs recorded:</p>" + macs + "</h1>");
  }

  String address = server.client().remoteIP().toString();
  Serial.println(address + " loaded root html");
}

void onClearArchive() //Be carefull accessing this, nothing is preventing deletion of archive in this program
{
  #ifdef ESP8266
  File macsFile = LittleFS.open(macsFilename, "w");
  #else
  File macsFile = LITTLEFS.open(macsFilename, "w");
  #endif

  if(!macsFile) 
  {
    server.send(200, "text/html", "<h1>Could not clear the archive,<br>    Error opening file</h1>");
    Serial.println("  Archive was not cleared,     ERROR OPENNING FILE");
  }
  else
  {
    macsFile.close();
    macCacheClear();
    server.send(200, "text/html", "<h1>Archive has been cleared</h1>");
    Serial.println("  Archive cleared");
  }
}

#ifdef ESP8266
void saveClient(const WiFiEventSoftAPModeStationConnected &event) 
#else
void saveClient(WiFiEvent_t event, WiFiEventInfo_t info)
#endif
{
  File macsFile;
  uint16_t numberOfClients;
  bool inCache=false, fWrite=true;
  char mac[13];
  
  #ifdef ESP8266
  numberOfClients = wifi_softap_get_station_num();  //Get the number of currently connected WiFi devices
  #else
  numberOfClients = WiFi.softAPgetStationNum();
  #endif

  Serial.println(" " + String(numberOfClients) + " connected client" + (numberOfClients > 1 ? "s" : ""));

  #ifdef ESP8266
  sprintf(mac, "%02X%02X%02X%02X%02X%02X", event.mac[0], event.mac[1],\   //Convert the client's MAC to string
  event.mac[2], event.mac[3], event.mac[4], event.mac[5]);
  #else
  sprintf(mac, "%02X%02X%02X%02X%02X%02X", info.sta_connected.mac[0], info.sta_connected.mac[1],\
  info.sta_connected.mac[2], info.sta_connected.mac[3], info.sta_connected.mac[4], info.sta_connected.mac[5]);
  #endif

  if( !(inCache = macCacheLookup(mac)) )  //If the newly connected device's MAC is not in cache,
  {
    #ifdef ESP8266
    macsFile = LittleFS.open(macsFilename, "a");
    #else
    macsFile = LITTLEFS.open(macsFilename, "a");
    #endif
    if(macsFile)
    {
      macsFile.println(mac);      //add it to the archive and the cache (This means that if cache has reached or
      fWrite = true;              //exceeded full capacity, archive could have duplicates. This limited memory coherency 
      macCacheInsert(mac);        //is a trade off for speed. What enforces this trunking method's memory coherency 
                                  //is the current use of this program (small in size and meant for single use by 
      macsFile.close();           //the "clients") in relation to the statistical probability of the same device 
    }                             //connecting again after another CACHE_SIZE-1 devices have connected).
    else {
      fWrite = false;
    }
  }

  Serial.printf("MAC: %.2s:%.2s:%.2s:%.2s:%.2s:%.2s %s%s\n", mac, mac+2, mac+4, mac+6, mac+8, mac+10,\
  (inCache ? "reconnected" : "connected"), (!fWrite ? ",   ERROR WHILE UPDATING ARCHIVE" : (!inCache ? ", archive updated" : "")));
}
