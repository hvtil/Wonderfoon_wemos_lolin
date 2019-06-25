//easteregg

boolean ee() {
  debug("egg");
  if (lastDialed[1] == lastDialed[2] && lastDialed[3] == lastDialed[2])
  {
    debug("egg =1");
    playegg();
    return true;
  }
  return false;
}



void clearLastDialed()
{
  lastDialed[0] = 20;
  lastDialed[1] = 21;
  lastDialed[2] = 22;
  lastDialed[3] = 23;
}
//other common functions

int playRand() {
  return random(1, 11);   // Randomizer for track
}
int foldRand() {          // Randomizer for folder
  return random(1, 4);  // (3)
}


void debug( String  text) {
  if (DEB)  Serial.println(text);   // debug with CRLF
}

void debug1( String  text) {
  if (DEB)  Serial.print(text);     // debug without CRLF
}


void addLastNumber(int newNumber) {
  debug("add " + String(newNumber));
  lastDialed[0] = lastDialed[1];
  lastDialed[1] = lastDialed[2];
  lastDialed[2] = lastDialed[3];
  lastDialed[3] = newNumber;

}

void addLastTime(long newTime) {
  lastTime[0] = lastTime[1];
  lastTime[1] = lastTime[2];
  lastTime[2] = lastTime[3];
  lastTime[3] = newTime;


}

void checkLastFour() {
  debug("Check Last four ");
  int i;
  for (i = 0; i < 4; i = i + 1) {
    debug(String(lastDialed[i]));
    debug(String(lastTime[i]));
  }

  String pin = String(lastDialed[0]) + String(lastDialed[1]) + String(lastDialed[2]) + String(lastDialed[3]);
  if (pin == String(pinCode)) {
    debug(pin + " Yes");
    if (lastTime[3] - lastTime[0] < 10000) {

      debug("wifi on");
      // ws = true;
      switchWebServer(true, true);        //turn wifi on
      clearLastDialed();
    }
  }

}

void checkChangeVolume()
{
  if (lastDialed[1] == 2 && lastDialed[2] == 1 && checkChangeTime())
  {
    debug("volume change");
    updateVolume(String (lastDialed[3]));
    playVolume();
    clearLastDialed();
  }
}

void checkChangeFolder()
{
  if (lastDialed[1] == 3 && lastDialed[2] == 1 && checkChangeTime())
  {
    debug("folder change");
    if ( lastDialed[3] < 4 )
    {
      EEPROM_storeFolder(lastDialed[3]);
      playFolder(lastDialed[3]);
    }
    clearLastDialed();
  }
}

void checkChangeWillekeurig()
{
  if (lastDialed[1] == 4 && lastDialed[2] == 1 && checkChangeTime())
  {
    debug("Speel willekeurig");
    if ( lastDialed[3] < 3 )
    {
      EEPROM_storePlayMode(lastDialed[3] - 1);
      playWillekeurig(lastDialed[3] - 1);
    }
    clearLastDialed();
  }
}

void  checkChangeWifi() {
  if (lastDialed[1] == 9 && lastDialed[2] == 1 && checkChangeTime())
  {
    debug("wifi change");
    if ( lastDialed[3] < 3 )
    {
      switchWebServer( lastDialed[3] - 1, true);
    }
    clearLastDialed();
  }

}

void checkReset()
{
  if (lastDialed[1] == 9 && lastDialed[2] == 9 && checkChangeTime())
  {
    //willekeurig
    if ( lastDialed[3] == 9 )
    {
      playReset();
      EEPROM_init(true);
      audioVolume = EEPROM_getVolume();
      folderNumber = EEPROM_getFolder();
      playMode = EEPROM_getPlayMode();
      mp3Wake();
      playVolume();                                           // play vulume status
      playFolder(folderNumber);                               // play folder number status
      playWillekeurig(playMode);
    }
    clearLastDialed();
  }
}






  void checkChangeContinues() {
    if (lastDialed[1] == 5 && lastDialed[2] == 1 && checkChangeTime())
    {
      debug("continu change");
      if ( lastDialed[3] == 1 )
      {
        debug("PlayRandom");
        PlayingRandom = true ;
        playContinu();
        playRandom();
      }
      clearLastDialed();
    }
  }






  bool checkChangeTime() {
    if (lastTime[3] - lastTime[1] < 10000)
      return true;
    else
      return false;
  }


  void checkAll() {
    checkLastFour();
    checkChangeVolume();
    checkChangeFolder();
    checkChangeWillekeurig();
    checkChangeWifi();
    checkChangeContinues();
    checkReset();
    ee();
  }
