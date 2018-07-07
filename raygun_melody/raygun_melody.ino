/* RAYGUN!!! Example Code
   https://learn.sparkfun.com/tutorials/protosnap-lilypad-development-simple-hookup-guide/uploading-an-arduino-sketch
   By: Nick Poole and Dia Campbell
       SparkFun Electronics
   Date: September 12, 2012
   License: This code is released into the open domain. Please
   use, re-use, and modify this code in any which way you
   require.
   
   This example code was written specifically for the ProtoSnap
   LilyPad Simple Development Board. After initializing the pins
   in setup(), this code jumps straight into making awesome,
   super-pleasant-sounding Raygun sounds. From time to time, the
   LEDs will sequentially light up (5->6->10->11).

   All sounds are labeled with an approximate onomatopoeia. You 
   can move around the for() loops to make the Raygun that best 
   suits you.
*/

const int buzzer = 6; //Buzzer pin
int freq; //frequency out (don't freak out)

void setup()
{
  // Set buzzer pin as OUTPUT:
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  // BOOOOOOOOoooooooooo sound (descending) >>>>>>>>>>>>>>>>>>>>>
  // Goes from 1000 Hz to 340 Hz with 2ms delays in between
  for (int b=1000; b>340; b--)
  { 
    tone(buzzer, b);
    delay(2);   
  }
  // End of BOOOOOOOOoooooooooo <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  // Turn the buzzer off, this must be called if the tone() 
  // function was not given a duration (the 3rd paramater).
  noTone(buzzer);

  // DSIFGSIVOESRGJIOFDSJGFSD sound (white noise) >>>>>>>>>>>>>>>
  // This for loop plays 5000 random frequenices to create what
  // sounds like white noise.
  for(int i =0;i<5000;i++)
  { 
    freq = random(240,1080);
    tone(buzzer, freq);
  }
  // End of DSIFGSIVOESRGJIOFDSJGFSD <<<<<<<<<<<<<<<<<<<<<<<<<<<<

  noTone(buzzer);  // Turn off the buzzer

  // WAWAWAWAWAWAWAWA sound (pacman waka) >>>>>>>>>>>>>>>>>>>>>>>
  // This set of for loops very quickly sweeps frequencies
  // down then up then down then up (10 iterations of down/up)
  for(int r=0;r<10;r++)
  { 
    for(int c=1000;c>340;c--)
    {
      tone(buzzer, c);
    }
    for(int d=340;d<1000;d++)
    {
      tone(buzzer, d);
    }
  }
  // End of WAWAWAWAWAWAWAWA <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  noTone(buzzer);

  

  // DSIFGSIVOESRGJIOFDSJGFSD sound (white noise) >>>>>>>>>>>>>>>
  // Same as the last DSIFGSIVOESRGJIOFDSJGFSD
  for(int i =0;i<5000;i++)
  { 
    freq = random(240,1080);
    tone(buzzer, freq);
  }
  // End of DSIFGSIVOESRGJIOFDSJGFSD <<<<<<<<<<<<<<<<<<<<<<<<<<<<

  noTone(buzzer);

  // ooooOOOOBooooOOOOB sound (ascending) >>>>>>>>>>>>>>>>>>>>>>>
  // This set of for loops will twice sweep frequencies UP
  // from 340 Hz to 1000 Hz, with a small delay in between
  // each tone.
  for(int s=0;s<2;s++)
  { 
    for(int g=340;g<1000;g++)
    {
      tone(buzzer,g);
      delay(2);
    }
  }
  // end of ooooOOOOBooooOOOOB <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  noTone(buzzer);

 
  // DSIFGSIVOESRGJIOFDSJGFSD sound (white noise) >>>>>>>>>>>>>>>
  // same white noise as usual.
  for(int i =0;i<5000;i++)
  { 

    freq = random(240,1080);
    tone(buzzer, freq);
  }
  // End of DSIFGSIVOESRGJIOFDSJGFSD <<<<<<<<<<<<<<<<<<<<<<<<<<<<

  noTone(buzzer);

  // Now that we've reached the end of loop(), jump back up
  // to the top and do it all over again! Yay!
}
