//EEPROM

//   Stores values read from analog input 0 into the EEPROM.
//   These values will stay in the EEPROM when the board is
//   turned off and may be retrieved later by another sketch.

void EEPROM_init(bool force) {
  debug("EEPROM_init");
  
  byte val = EEPROM.read(100);          // initialize first boot, and write 77 in adress 100 to make sure we know next time the phone starts it has already be started before.
  int initRead = val;
  if (initRead != 77|| force)
  {                                     // if started the first time

    debug ("init 14 - 1- 0 - 77");
    EEPROM.write(0, 14);                // volume set to 10
    delay(500);
    EEPROM.write(1, 1);                 //  folder set to 1
    delay(500);
    EEPROM.write(2, 0);                 //  folder set to 1
    delay(500);
    EEPROM.write(100, 77);              // set initialized to 77    
    delay(500);
  }
  EEPROM.commit();
}

void EEPROM_clear(){
 for (int L = 0; L < 7; ++L) {
    EEPROM.write(0 + L, 254);
  }
  EEPROM.write(100, 254);
  EEPROM.commit();
  }

void EEPROM_storeVolume(int volume) {  // store volume setting is EEPROM address 0
  int EEADRESS = 0;
  if (volume > 19) {                      
    volume = 19;
  }

  EEPROM.write(EEADRESS, volume);
  debug("Volume store =" + String(volume));
    delay(500);  // delay to prevent crashes during storing data

  EEPROM.commit();
}

unsigned int EEPROM_getVolume() {
  int EEADRESS = 0;                               //volume adress = 0
  byte val = EEPROM.read(EEADRESS);               //read volume from EEPROM
  int volumeRead = val;
  debug("GetVolume =" + String(volumeRead));
  return (volumeRead);

}

void EEPROM_storeFolder(int folder) {   //store music folder selection setting is EEPROM address 1
  int EEADRESS = 1;
  EEPROM.write(EEADRESS, folder);
  debug("Store Folder =" + String(folder));
  EEPROM.commit();
  folderNumber = folder;
  delay(500);
}

unsigned int EEPROM_getFolder() {       //retrieve music folder selection setting is EEPROM address 1
  int EEADRESS = 1;
  byte val = EEPROM.read(EEADRESS);
  // delay(200);
  int folder = val;
  debug("Getfolder =" + String(folder));
  return (folder);
}


void EEPROM_storePlayMode(int pmode) {     //store random mode setting is EEPROM address 2
  int EEADRESS = 2;
  EEPROM.write(EEADRESS, pmode);
  debug("Store Playmode =" + String(pmode));
  EEPROM.commit();
  playMode = pmode;
  delay(500);
}

unsigned int EEPROM_getPlayMode() {       //retrieve random mode setting is EEPROM address 2
  int EEADRESS = 2;
  byte val = EEPROM.read(EEADRESS);
  //delay(200);
  int pmode = val;
  debug("GetPlaymode =" + String(pmode));
  return (pmode);
}


void EEPROM_storeName(String buffer) {     //store name setting is EEPROM address 3-18
  int EEADRESS = 3; //2-16
  while (buffer.length() < 15)
  {
    debug(String (buffer.length()));       // fill unused positions with space
    buffer.concat(" ");
  }
  for (int L = 0; L < 15; ++L) {
    EEPROM.write(EEADRESS + L, buffer[L]);
  }
  EEPROM.commit();
  delay(500);
  AfdelingNaam = buffer;
  debug(buffer);
}


String EEPROM_getName() {                   //retieve name setting is EEPROM address 3-18
  int EEADRESS = 3; //3-18
  //delay(200);
  String buffer;
  for (int L = 0; L < 15; ++L) {
    if (isAlphaNumeric(EEPROM.read(L + EEADRESS)))
      buffer += char(EEPROM.read(L + EEADRESS));
  }
  return buffer;
}
