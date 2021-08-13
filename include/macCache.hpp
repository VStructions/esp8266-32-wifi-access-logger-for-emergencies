#ifndef MACCACHE_HPP
#define MACCACHE_HPP

#include "string.h"
#include "LittleFS.h"

#ifndef CACHE_SIZE
#define CACHE_SIZE 1000
#endif

struct macAddressCache
{
  char array[CACHE_SIZE][13]; //Used for the last CACHE_SIZE macs
  uint16_t lastMac;
};

extern struct macAddressCache macCache;

void macCacheClear();
void macCacheInit(File macsFile);
void macCacheInsert(char mac[13]);
bool macCacheLookup(char mac[13]);

#endif