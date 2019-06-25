

//webServer.b

void switchWebServer( boolean ws,boolean pla)
{
  webserverOn = ws;
  if (ws)
  {
    WiFi.forceSleepWake();
    WiFi.mode(WIFI_AP);
    webServer.begin();
    debug("Wifi on ");
    wsUpTime = millis();
    playWifi(1);
  }
  else
  {
    webServer.stop();
    webServer.close();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    debug("Wifi off");
    if (pla) playWifi(0);
  }
}

void handleRoot()
{
  debug("HandleRoot");
  sendPage(1);
}

void handleHome() {
  debug("HandleHome");
  sendPage(1);
}

void handleHarder()
{ //Define the handling function for the path harder
  debug("HandleHarder");
  String volume = updateVolume("+");
  HTMLVolume = String((audioVolume - 9) * 10);
  sendPage(2);
}

void handleZachter() {
  debug("HandleZachter");
  //Define the handling function for the path zachter
  String volume = updateVolume("-");
  HTMLVolume = String((audioVolume - 9) * 10);
  sendPage(2);
}

void handleVolume() {
  debug("HandleVolume");
  HTMLVolume = String((audioVolume - 9) * 10);
  sendPage(2);
}


void handleVolumeEnd()
{
    sendPage(1);
  playVolume();
}

void handleManual() {
  sendPage(3);
}


void handleAbout() {
  sendPage(4);
}


void handleRandom() {
  debug("PlayRandom");
   PlayingRandom = true ;          
    playRandom();
    handleManual();
}


void handleTrack() {
  debug("HandleTrack " + webServer.arg("track"));
  PlayingRandom = false ; 
  if (webServer.arg("track") == "999")    //# = 9999 = stop
  {
    MP3stop();
  }
  else {
   playTrackInFolder(webServer.arg("track").toInt(),folderNumber);
  }
  handleManual();

}

void handleConfig() {
  String nameArg = webServer.arg("name");
  String pmArg = webServer.arg("pm");
  String folArg = webServer.arg("fol");
  debug("Name varialble: " + nameArg);
  debug("Playmode varialble: " + pmArg);
  debug("Folder varialble: " + folArg);
  if (!nameArg.equals("")) {
    debug(nameArg);
    EEPROM_storeName(nameArg);
  }

  if (pmArg == "on") {
    EEPROM_storePlayMode(1);
    playWillekeurig(1);
  }
  else {
    if (nameArg != "") {  // check for first time the page is loaded / then do not store Playmode
      EEPROM_storePlayMode(0);
      playWillekeurig(0);
    }
  }
  if (EEPROM_getPlayMode() == 1)  {
    HTMLPlaymodeCheckbox = "<br/>Speelstand willekeurig </td><td> &nbsp; &nbsp;<input type='checkbox'  class='double' name='pm' checked ";
  }
  else  {
    HTMLPlaymodeCheckbox = "<br/>Speelstand willekeurig </td><td> &nbsp; &nbsp;<input type='checkbox'  class='double' name='pm' >";
  }

  if (folArg == "")
  {
    folArg = String(folderNumber);
  }
  else {
    EEPROM_storeFolder(folArg.toInt());
    playFolder(folArg.toInt());
  }
  HTMLPlayFolderRadioButtons = ""
                               "<p>MP3 files afspelen uit folder:</p>"
                               "<div>";

  HTMLRadioButton1 = "<input type='radio' id='f1' name='fol' value='1' class='double'";
  HTMLRadioButton2 = "><label for='f1'>&nbsp;&nbsp;&nbsp;Folder 01</label><br/><br/><input type='radio' id='f2' name='fol' value='2' class='double'";
  HTMLRadioButton3 = "><label for='f2'>&nbsp;&nbsp;&nbsp;Folder 02</label><br/><br/><input type='radio' id='f3' name='fol' value='3' class='double'";
  HTMLRadioButtonEnd = "><label for='f3'>&nbsp;&nbsp;&nbsp;Folder 03</label></div>";
  HTMLPlayFolderRadioButtons.concat(HTMLRadioButton1);
  if (folArg == "1")HTMLPlayFolderRadioButtons.concat("checked");
  HTMLPlayFolderRadioButtons.concat(HTMLRadioButton2);
  if (folArg == "2")HTMLPlayFolderRadioButtons.concat("checked");
  HTMLPlayFolderRadioButtons.concat(HTMLRadioButton3);
  if (folArg == "3")HTMLPlayFolderRadioButtons.concat("checked");
  HTMLPlayFolderRadioButtons.concat(HTMLRadioButtonEnd);

  if (nameArg == "") {
    sendPage(5);
  }
  else sendPage(1);
}



void handleClose() {
  switchWebServer(false, true);
}

void handleNotFound() {
  sendPage(1);
}

void sendPage(int page) {
  responseHTML = HTMLPage(page);
  webServer.send(200, "text/html", responseHTML);
}


void checkWifiTime() {
  //    debug("Wifi is on and time = "+ String(millis() - wsUpTime));
  if  (millis() - wsUpTime > wifiTime)
  {
    debug("Wifi is on and time = " + String(millis() - wsUpTime));
    switchWebServer(false, true);
  }
}


String HTMLHeadBegin = ""
                       "<!DOCTYPE html><html><head><title>Wonderfoon</title>"
                       "<style> body { background-color: #f0f0f0; font-family: Arial, Helvetica, Sans-Serif; font-size: 16px; "
                       "Color: #000088; margin: 15px; padding: 15px;}"
                       "h1 { font-size: 30px; font-weight: bold; }h2 { font-size: 50px; font-weight: bold; } "
                       "a:link {color: #000088; text-decoration: none;} a:visited {color: #000088; text-decoration: none;} a:hover {color: #000088; text-decoration: none;} a:active {color: #000088; text-decoration: none;}";//#000088


String HTMLHeadEnd = ""
                     "</style></head>";





String HTMLPhoneImage = ""
                        "<center><img src = "
                        "'data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEAeAB4AAD/4QAiRXhpZgAATU0AKgAAAAgAAQESAAMAAAABAAEAAAAAAAD/2wBDAAIBAQIBAQICAgICAgICAwUDAwMDAwYEBAMFBwYHBwcGBw"
                        "cICQsJCAgKCAcHCg0KCgsMDAwMBwkODw0MDgsMDAz/2wBDAQICAgMDAwYDAwYMCAcIDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAz/wAARCAAyAFcDAS"
                        "IAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKS"
                        "o0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+P"
                        "n6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nz"
                        "g5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADA"
                        "MBAAIRAxEAPwD9B9U1K8uPEF9JJd3Tu9zIzM0rEklzkk5q7p95OB/r7j/v4adeaG51u864M8h6f7RrQsPDzuRwfyrSTKHW1zcMMefN/wB9mrM+rjS41a6vmhVuRvlILfQdT+FeI/F/9s7w/wCD/E"
                        "F54Z8N3UGqeIbO5+w3UqqJILSYFhIg/vyR7fmJGwMyr85LiOx4TjuNUummvrhr66LHzJfM8zew44buPccenGK4XiIy+F3Dn97kS1PYbbxtFPKsdvJe3Tdtmef6/pVxPFMqH94y27DjbPdrG35MQa"
                        "8a8LeLLjx98UdW8P20k8Gj+Howl/cREL5t0efITIOdgxvbn5iVGDG2drWfg1o15eO03iDxKvHzQC5QxpwemyMOCcHqx9hXNPESteKKUk9L/cenp4shkOG1KzVvQXf+RVy1vzesPLvoZD2CXkZz/w"
                        "CPZrzN/wBieKG285pvGT8GRj/amwwpknJJATAAwACWIxnJOTX8O/s26VoWsJdNr3jiT9z8tldX0aQFW5WTCxCQn0Ifbg9CeRlHEVG+n3/8AqVl3PVr83VmwWZrqAtyBJuQn88VUe4mcf6+Rvo5rg"
                        "db8Ual8IfF+n2Ud1dXOg60jCBpyJFjnTloZONrbl+dCQchXBwQpbobjVl1Gza6hH2cqQC8edqsc469M4P5HjFbwxN3ZhyvdHUeC9Qu7bxfZyR3FxG6l8FZSp+41FV/hBfL4svYLgcXFpNJBcIOzh"
                        "Ccj2KsD9cjtRXfF3VzNxsYvibx7p+kazeRQRNdTLM4OOFB3HjNeefEH9qa48F6uunW8McepSRQyxhDFmESvJGsmJXXzAjRkske6QryqN0rfvLSN9avWA3RrcSDcoyv3j37V4noVnb+Ov2jL7V7"
                        "5tP+x6TrE1jFL5huJY4rQFdqqqkRh54QWOAWwF3sAANswl7KEVT+KTS1/Fnn4epOpUfNsk9jnf2nfgtJ4I+Hnw8/sSG10/QPBM9ylxCqkFZLoQbp2Kgl3d4MM7HOSgzjAFofHrSPhj8OlvP7Q+"
                        "1apcIRaQR/Jt45dsEkgZ4xty2Owau28Vft4/BvQtc1vwxqfim0XxFpqsk+gXWm3DX17ltqRwW5iJumk4ZI4Q7MvO0ANt+bvEXxe0X4v/tDeD9as/hv8WPAOn2cVzp13d6t4fi0jRdVglt51gWa"
                        "FWLLtnmBRyFB3kOGxGY/n8xwalU5sLWSi7Ky1tsr3W3zNsPiHCLdSN33PoT4Ca2ngj4Dapr003ktcXMjyzAf6sAF3Ye4JlOSfvHJPWva/g54gj0r41y+HYVs7DUEsZDbRXN95qahKJLdg7iLGJ"
                        "hB5rLGcugbnqc+O/BXwrp/jH9nq78N3HnLY3Ut7YPhyJNonkXO4YOSOuMde1O8Qyafr+mR2vxFd9G17R7ad7TVIfs1hpurXzBNl2LxkCQyyMgBgnKOC8wQXKpFNU4inJUlFdjXC9z7a03T9Wvn"
                        "UL5LMDz9nmK/+hQtXJ+JwviyHxHZ2d1p95feH3QL5TjMN0EBuIyQeUyV3EBQJC4IDITXlPhn4feK4NPtftPxU8c6t4Nbw6ILmaS9uzaPdBl5F6w8xpGG5Wk3CHYN6xB2LLzfxT03UvHnhLwzb/"
                        "DvWo9Du7e5txq2p2d158FxYRsGNt9phfypJVcYDRm4YKJY5Gj3hmxw9FR73/M9GUm1fSx1PiG4tfGXgOyvGjk8q4jhvoULFWXcquoJUgjtnHXp0NYWmaxp+r6SG0/VG8u4CyMpZH28ZHC7ex75"
                        "PPXmus1vRUi8HGRYVh09ZorSGMDapXgbV9lGB+I966rRviDp9zpdray7JZLZNgV13BDk9MjH5dK7p4OVSSkp8v6k4etGEbOPMU/2XfCMugzX19cXRuI9WuFaEGLZhY43UsPmbO4tjP8AsUV1nh"
                        "nxCt94gtcbRkt0P+waK9elH2cFC5yzlzSueAfGTxovhXxHdR6HeS2uoeY7ySeXuhtU3HMhHfjOEP3uuMDB+UNS+Nlr4v8AjhN4Lj1abQZVie4OpPCtxJcXsjZBkTcpZdxyxPJZht2Fia9A+K/h"
                        "j43aT8YPEFvb+Gby88O6j4gFxFfWf2O/RLeRpY2JieQS5iaS1mCmMgpZSx8iZQXeFPhn8YviZqmj6lq3g6Szv7WOF4rvVnsYBpjNawGVAqFpeLiC4hLR/M8F+xLOyRSReTmMqlepyuMrReiS39"
                        "X5k4WPLDmTV3vfp6GD8Nv2Jrjwxdz+Ide15r7xvqkRi1bVrMNaG74A8tfLKt5SqqKqk9FBPOTVP4pfs8QW9nG02pNMt2HtXV4DllZGyCxcnkDHPXNfZ/w++Ei6D4CtNL1S6bUr5f3s97jazylV"
                        "VmX2+UcYA/2VGFHBfHD9nLVte8M3kOmstw7ASW0qj54ZVO5GKH7wDAZAJyMivXp4Gk6HK42dtv66nm4hVIzbi7o82/ZC1gWvgm60u+urddQs75zIkkoUsGVW35Y4wzF+55B9s+42NoJHH+kaax"
                        "xjjUbfv7+ZXxjafF/Q/hjr97a+ModW8H6gXzNb6rpd1HGrcA7ZvK8p1z0IblcHAzW9Y/tffCFCN/xI8CwN6T6xbwt+TsDXjxxUYLkl08z1MNh5ygmfVNh8EfCY1UX66B4Agvlbf9rkWwEwb1Dg"
                        "ls+9a/iy2nWyhm0vxRpNnq1ncxXMTMwuIZghyYpOG+RuhwCcDAKkh0+VdP8A2tvhDcNtj+Jnw+mPTCa9auf0c1sWv7Tfw8mC/ZfEtjqRPQadbzXxP4Qo9DxNK1lb7zujgZt3d/uPpTxz4wXXPD"
                        "Wkaf8A2lBqFwLt7idoE2RRsTu2IvBCDnGQAc8AfdXS8G/CVL+w+0zX8oR3JVVQA8Hnk+9eK/CTxlD8T9at4PD2m+J7tkcO8lz4d1HT4FXI5MlzBErD/cLEDJOBX1ZpWlrouiQ2iuGeJApbH3j3"
                        "P4nJ/GuzCWqq/QitGVJ6dSHwX4WtdJ1m3EfmPtLDLn/ZNFUdLuNRi8V2ql2VcsSNvUbGxRXpeyR5ntlfZnSeI9Fs4vE2oKtpaqoupQAIlAHzfSoU0q1x/wAe1v1/55iiiunoZx3ZZh0u1AX/AE"
                        "eDr/zzFWotMtgP+PeD/v2KKKzluaxLtvCkDYRFQeijFXoUBj6D8qKKwkdNImMKE/dX8qRolVCQqg+oFFFY0zokQSQrORvVX5/iGaiexh/54xf98CiitjlkW/C+l2s/iG38y3t32lyN0YOPlNFF"
                        "FVIyluf/2Q==' height = '208' width = '360'> </center >";




String HTMLPage(int page) {
  String HTML;
  switch (page) {

    case 1: //home
      HTML = "" + HTMLHeadBegin + HTMLHeadEnd + ""
             "<body><h2 align='center'>Wonderfoon " + AfdelingNaam + "<br/><br/>Dit de webpagina van de Wonderfoon<br/></h2>" + HTMLPhoneImage + "<br/>"
             "<h2>Maak een keuze uit de volgende opties</h1><br><br>"
             "<br/><br/><br/><center>"
             "<h2><a href=\"/volume\">Volume aanpassen.</a></h2>"
             "<h2><a href=\"/manual\">Mp3 bedienen.</a></h2>"
             "<h2><a href=\"/config\">Instellingen.</a></h2>"
             "<h2><a href=\"/about\">Over de wonderfoon.</a></h2>"
             "<h2><a href=\"/close\">Wifi afsluiten.</a></h2></center>"
             "</body></html>";
      return HTML;
      break;

    case 2: //volume

      HTML = "" + HTMLHeadBegin + HTMLHeadEnd + ""
             "<body><h2 align='center'>WonderFoon " + AfdelingNaam + "<br/><br/>Volume instellingen<br/></h2>"
             "<br><br><br><center><table><tr>"
             "<td width='120px' align='left'><a href='/zachter'><p style='font-size:110px;'><<<</p>" +  "</a></td>" //HTMLSpeakerMinusImage +
             "<td width='80px' align='center'><h2>  " + HTMLVolume + "  </h2></td>"
             "<td width='120px' align='right'><a href='/harder'><p style='font-size:110px;'>>>></p>" +   "</a></td>" //HTMLSpeakerPlusImage +
             "</tr></table><h2><center><br/><a href='/volumeend'>Terug</a></center></h2>"
             "</center></body><HTML>";
      return HTML;
      break;



    case 3:
      // Bediennen
      HTML = "" + HTMLHeadBegin + ""
             "h1 { font-size: 30px; font-weight: bold; }h2 { font-size: 50px; font-weight: bold; }"
             ".myButton {"
             " -moz-box-shadow:inset 0px 0px 0px 0px #ffffff;"
             " -webkit-box-shadow:inset 0px 0px 0px 0px #ffffff;"
             " box-shadow:inset 0px 0px 0px 0px #ffffff;"
             " background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #f9f9f9), color-stop(1, #e9e9e9));"
             " background:-moz-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             " background:-webkit-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             " background:-o-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             " background:-ms-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             " background:linear-gradient(to bottom, #f9f9f9 5%, #e9e9e9 100%);"
             "   filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#f9f9f9', endColorstr='#e9e9e9',GradientType=0);"
             " background-color:#f9f9f9;"
             " border:4px solid #dcdcdc;"
             " display:inline-block;"
             " cursor:pointer;"
             " color:#666666;"
             " font-family:Arial, Helvetica, Sans-Serif;"
             " font-size:50px;"
             " font-weight:bold;"
             " padding:8px 63px;"
             " text-decoration:none;"
             " text-shadow:0px 1px 50px #ffffff;"
             "}"
             ".myButton:active {"
             " position:relative;"
             " top:1px;"
             "}" + HTMLHeadEnd + ""
             "<body><h2 align='center'>WonderFoon " + AfdelingNaam + " <br/><br/>MP3 bedienen<br/>Kies een nummer om het af te spelen <br/> ( # = stop )<br/></h2>"
             "<center><table><tr><td><a href = '/start?track=1' class = 'myButton'>1</a></td><td><a href = '/start?track=2' class = 'myButton'>2</a></td><td><a href = '/start?track=3' class = 'myButton'>3 </a></td></tr>"
             "<tr><td><a href = '/start?track=4' class = 'myButton'>4</a> </td> <td> <a href = '/start?track=5' class = 'myButton'>5</a></td> <td> <a href = '/start?track=6' class = 'myButton'>6 </a></td> </tr> "
             "<tr><td> <a href = '/start?track=7' class = 'myButton'>7</a></td> <td><a href = '/start?track=8' class = 'myButton'>8</a> </td> <td><a href = '/start?track=9' class = 'myButton'>9 </a> </td> </tr> "
             "<tr><td> <a href = '/rand' class = 'myButton'>*</a></td> <td><a href = '/start?track=10' class = 'myButton'>0</a> </td> <td><a href = '/start?track=999' class = 'myButton'># </a> </td> </tr></table></center> "
             "<br><h2><center><a href = '/home'>Terug </a> </center> </h2>"
             " </center></body><HTML>";
      return HTML;
      break;

    case 4:
      // About
      HTML = "" + HTMLHeadBegin + HTMLHeadEnd + ""
             "<body><h2 align = 'center'>WonderFoon " + AfdelingNaam + " <br/> <br/> Over de Wonderfoon <br/> </h2> "
             "<h1 >Deze wonderfoon is een doorontwikkeling van de originele Wonderfoon.<br/>"
             "De Wonderfoon is een oude PTT telefoon die is omgebouwd tot mini - jukebox met behulp van een kleine computer. "
             "Als je de hoorn van de haak neemt hoor je de kiestoon en als je daarna een nummer kiest tussen de 0 en de 9 hoor je vervolgens een muziekje.<br><br>" + HTMLPhoneImage + " <br/> "
             "De naam 'Wonderfoon' is bedacht door verzorgers die ouderen met dementie begeleiden. In die telefoon zitten 10 muziekjes uit hun verleden, "
             "van Wim Sonneveld als Nikkelen Nelis tot Tom Manders als Dorus. Die muziek, samen met een apparaat dat ook uit hun verleden komt, "
             "blijkt een mooi middel om deze ouderen te activeren, rust te geven en ook soms te emotioneren. Een wonderbaarlijke telefoon zogezegd."
             "<br/><br/>Voor meer informatie over de originele: www.wonderfoon.nl </h1> "
             "<br><h2><center><a href = '/home'>Terug </a> <center> </h2> "
             " </center> </body > <HTML>";
      return HTML;
      break;


    case 5:
      // Config
      HTML = "" + HTMLHeadBegin + ""
             "input[type = text] {"
             "padding: 12px 20px;"
             "margin: 8px 0;"
             "box - sizing: border - box;"
             "border: none;"
             "border-bottom: 1px solid red;"
             "color: #666666;"
             "font-family: Arial, Helvetica, Sans-Serif;"
             "font-size: 50px;"
             "font-weight: bold;"
             "}"
             ".double {"
             "zoom: 2;"
             "transform: scale(2);"
             "-ms-transform: scale(2);"
             "-webkit-transform: scale(2);"
             "-o-transform: scale(2);"
             "-moz-transform: scale(2);"
             "transform-origin: 0 0;"
             "-ms-transform-origin: 0 0;"
             "-webkit-transform-origin: 0 0;"
             "-o-transform-origin: 0 0;"
             "-moz-transform-origin: 0 0;"
             "-webkit-transform-origin: 0 0;"
             "}"
             ".myButton {"
             "-moz-box-shadow:inset 0px 0px 0px 0px #ffffff;"
             "-webkit-box-shadow:inset 0px 0px 0px 0px #ffffff;"
             "box-shadow:inset 0px 0px 0px 0px #ffffff;"
             "background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #f9f9f9), color-stop(1, #e9e9e9));"
             "background:-moz-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             "background:-webkit-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             "background:-o-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             "background:-ms-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
             "background:linear-gradient(to bottom, #f9f9f9 5%, #e9e9e9 100%);"
             "background-color:#f9f9f9;"
             "border:4px solid #dcdcdc;"
             "display:inline-block;"
             "cursor:pointer;"
             "color:#666666;"
             "font-family:Arial, Helvetica, Sans-Serif;"
             "font-size:50px;"
             "font-weight:bold;"
             "padding:8px 63px;"
             "text-decoration:none;"
             "text-shadow:0px 1px 50px #ffffff;}"
             "" + HTMLHeadEnd + ""
             ""
             "<body><h2 align = 'center'>WonderFoon " + AfdelingNaam + " <br/> <br/> Instellingen </h2> "
             "<h2><center>"
             "<table>"
             "<form action='/config' method='post'>"
             "<tr><td> Afdeling: </td><td>"
             "<input type='text' name='name' value='" + AfdelingNaam + "' required "
             "minlength='4' maxlength='15' size='15'><br></td></tr><tr><td>"
             "" + HTMLPlaymodeCheckbox + ""
             "</td></tr><tr><td colspan='2'>" + HTMLPlayFolderRadioButtons + "</td></tr></table><br/><br/><br/><input type='submit' class='myButton'></form>"
             "<br/><br/><a href = '/home'>Terug </a> <center> </h2>"
             "</center> </body> <HTML>";
      return HTML;
      break;

    default:
      return  HTMLPage(1);
      break;
  }
  //webServer.e
}
