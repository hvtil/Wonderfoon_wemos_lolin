///  Numpad type 1  4x3 numpad 7 wire and ground

void checkNumPad() {
  delay(20);                                     // check every 20 ms
  int czer = 0;                                  // reset column
  int rzer = 0;                                  // reset row

  for (int thisRow = 0; thisRow < (sizeof(row) / sizeof(row[0])); thisRow++) {
    row[thisRow] = digitalRead(rowPin[thisRow]);                                  // read if a row is pressed
    if (row[thisRow] == 0 && rzer == 0)
    {
      rzer = 1;
      break;
    }
  }
  //debug("");
  //debug1(" col ");
  for (int thisCol = 0; thisCol < (sizeof(col) / sizeof(col[0])) ; thisCol++) {
    col[thisCol] = digitalRead(colPin[thisCol]);                                   // read if a col is pressed
    if (col[thisCol] == 0 && czer == 0)
    {
      czer = 1;
      break;
    }
  }

  if (czer && rzer) {                                                             // if both are 1 ( to prevent the reading of the row and col half way between row and col a change was happening.
    PlayingContinuesly = false ;
    num = numPad[arrayFind("row")][arrayFind("col")] ;                            // find the number that belongs to this combination of row and col in the numPad array

    if (num > 10 ) {                                                              // 11 and 12 are * and #
      num = 0;
    }
    //debug(String (num));
    debug("Number = " + String(num));
    if (num != 0 && num != lastNum) {       // 0=no button pressed lastNum to check if number changed
      mp3Wake();                            // Wake up MP3 since hook is picked up
      playTrackInFolder(num, folderNumber);                      // Play song
      addLastNumber(num);                   // add last dailed number for pin array
      addLastTime(millis());                // add last time for pin
      lastNum = num;
    }
  }
  else lastNum = 0;                         // if button is
  // debug("");
  debug(String(lastNum));
}

int arrayFind(String type) {

  int wantedVal = 0;
  int wantedPos = 5 ;
  if ( type == "row") {

    for (int i = 0; i < 4; i++) {

      if (wantedVal == row[i]) {
        wantedPos = i;
        break;
      }
    }
  }
  else {
    for (int i = 0; i < 3; i++) {

      if (wantedVal == col[i]) {
        wantedPos = i;
        break;
      }
    }
  }

  return wantedPos;
}
