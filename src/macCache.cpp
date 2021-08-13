#include "macCache.hpp"

void macCacheClear()
{
  uint16_t i;

  for(i=0; i<CACHE_SIZE; i++){  //Zero out the MAC strings
    macCache.array[i][0] = '\0';
  }

  macCache.lastMac = (uint16_t)-1;  //Set last MAC pointer to -1 (MAXIMUM lastMac == CACHE_SIZE)
}

void macCacheInit(File macsFile) //Caller opens and closes the file
{
  uint16_t i, readChars=12;

  macCacheClear();

  i=0; 
  cacheLoop:
  readChars = macsFile.readBytes(macCache.array[i], 12U);   //Try to read a whole MAC
  if(readChars == 12){              //If MAC is whole, terminate the string and continue
    macCache.array[i][12] = '\0';
    i++;
    goto cacheLoop;
  } else {                          //If MAC is not whole, terminate the string for safety,
    macCache.array[i][readChars] = '\0';  //(This probably won't happen)
    i--;                            //set lastMac to the previous whole MAC
  }
  macCache.lastMac = i;
}

void macCacheInsert(char mac[13])   //Make a new MAC entry to the cache 
{
  if(macCache.lastMac == CACHE_SIZE-1) {  //If cache is full, start overlapping the first entries
    macCache.lastMac = 0;
  }
  else {
    macCache.lastMac++;
  }
  //This cache keeps the last CACHE_SIZE MACs, order doesn't matter

  memcpy(macCache.array[macCache.lastMac], mac, sizeof(char) * 12);   //Finally copy over the new MAC to the cache
  macCache.array[macCache.lastMac][12] = '\0';
}

bool macCacheLookup(char mac[13]) //Searches for the mac given as an arg, in the cache
{
  uint16_t i, matches = 0;

  for(i=0; i < CACHE_SIZE; i++) //Search the whole cache
  {
    if(macCache.array[i][0] == '\0')  //This exists when the cache is not full (CACHE_SIZE)
      break;                          //and symbolizes END OF CACHE (marked on init)

    for(matches=0; matches < 13; matches++)           //For each character in this MAC string from the cache,
    {
      if(macCache.array[i][matches] == mac[matches])  //check each character from the MAC in the arg, matches. 
        continue;                                    //If so, remember it matched
      else                          
        break;                         //If not, MACs are different and continue to the next MAC in cache
    }
    if(matches == 13)                 //If all characters matched, MACs are the same 
      return true;
  }
  return false;                       //The loop ended and no MACs were matched
}