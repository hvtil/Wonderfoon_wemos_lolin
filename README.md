# Wonderfoon_wemos_lolin
Version #1001
A Wonderfoon is a rotary dial phone that is converted into a jukebox. The Wonderfoon is build for people with dementia or Alzheimer disease.

The longterm memory with persons with dementia is most of the time still present. Therefore they still remember the rotary dial telephone from years ago. When the dial a number a song is played, the can be songs they like or songs from years ago, again in most situations song they still remember.

This combination of the old phone and the old music does create miracles.

More info in Dutch on https://www.wonderfoon.eu

Wemos is connected to a DF-Player mp3-player via serial connection

On the SD-card 4 folders are required with names 01, 02, 03 and 04 
 
In folder 01 till 03 you can put 10 mp3-files (songs) with filenames: 001.mp3 till 010.mp3  
Folder 04 is the systemfolder these mp3-files are used for the voice feedback commands while starting the Wonderfoon or changing settings

When compiling put all ino files in a folder called wonderfoon and open the wonderfoon.ino in the arduino application.

if the MP3 module is from another brand the serial commands sometimes fail.
Is this case replace the following lines in function_mp3.ino (line 33 t/m 38):

void mp3Sleep()

{

execute_CMD(0x09, 0, 3); // Set MP3 player in sleep mode

execute_CMD(0x0A, 0, 0); // Set MP3 player in power loss

}


with this code:

void mp3Sleep()

{

execute_CMD(0x09, 0, 4); // Set MP3 player in sleep mode

//execute_CMD(0x0A, 0, 0); // Set MP3 player in power loss

}
