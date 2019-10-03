void waitForDial()
{
  int dialReading = digitalRead(dialPin);
  if (dialReading != lastDialState) {
    // reset the debouncing timer
    lastDialDebounceTime = millis();
  }

  if ((millis() - lastDialDebounceTime) > dialDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (dialReading != dialState) {
      dialState = dialReading;

      // If dialstate goes from High to Low  , Dial starts
      if (dialState == LOW) {
        debug("");
        Serial.write(" dial start");
        PlayingContinuesly = false ;  //reset play continu
        debug("" );
        // resetMP3();
      }

      // if dialState goes from Low to High . Dial Ends
      if (dialState == HIGH)
      
      {
        countedPulses = pulseCount;
        if (countedPulses <= 1 )
        {
          countedPulses = 1;
        }
        if (countedPulses >= 10 )
        {
          countedPulses = 10;
        }
        pulseCount = 0;
        debug("");
        Serial.write(" dial stop ");
        debug("");
        Serial.write(" dialed ");
        debug1(String(countedPulses));
        debug("");
        //ledState = HIGH;
        //digitalWrite(ledPin, ledState);
        addLastNumber(countedPulses);  // add last dailed number for pin array
        addLastTime(millis());         // add last time for pin
        checkAll();
        playTrackInFolder(countedPulses, folderNumber);
        folderNumber = EEPROM_getFolder();
      }
    }

    if (dialState == LOW) {
      countPulse();

    }
  }
  lastDialState = dialReading;
}


void countPulse()
{
  int pulseReading = digitalRead(pulsePin);
  // debug(pulseReading);
  if (pulseReading != lastPulseState) {
    // reset the debouncing timer
    lastPulseDebounceTime = millis();
  }
  if ((millis() - lastPulseDebounceTime) > pulseDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (pulseReading != pulseState) {
      pulseState = pulseReading;


      // only toggle the LED if the new button state is LOW
      if (pulseState == HIGH) {
        //ledState = LOW;

        pulseCount =  pulseCount + 1;
        Serial.write("write pulse ");
        debug1(String(pulseCount));
        debug("");
      }
    }
  }

  // set the LED:
  // digitalWrite(ledPin, ledState);
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastPulseState = pulseReading;
}
