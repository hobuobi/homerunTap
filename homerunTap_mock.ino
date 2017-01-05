// -----------------------------------------
// Publish and Dashboard with Photoresistors
// -----------------------------------------
// This app will publish an event when the beam of light between the LED and the photoresistor is broken.
// It will publish a different event when the light is intact again.

// Just like before, we're going to start by declaring which pins everything is plugged into.

int led = D0; // This is where your LED is plugged in. The other side goes to a resistor connected to GND.

int force = A0; // This is where your photoresistor is plugged in. The other side goes to the "power" pin (below).
int p = D6;
// The following values get set up when your device boots up and calibrates:
int intactValue; 
int brokenValue; 
int threshold; // This is a value halfway between ledOnValue and ledOffValue, above which we will assume the led is on and below which we will assume it is off.

bool done = false; 
bool ready = false;
int startTime;
int endTime;
String totalTime;
// We start with the setup function.

void setup() {
  Particle.function("activate",activate);
  pinMode(led,OUTPUT); // Our LED pin is output (lighting up the LED)
  pinMode(force,INPUT);  // Our photoresistor pin is input (reading the photoresistor)
  pinMode(p,OUTPUT);
  digitalWrite(p,HIGH);
  digitalWrite(led,LOW);
 threshold = 3500; 
 startTime = 0;
 endTime = 0;
 
}


// Now for the loop.

void loop() {
  /* In this loop function, we're going to check to see if the beam has been broken.
  When the status of the beam changes, we'll send a Particle.publish() to the cloud
  so that if we want to, we can check from other devices when the LED is on or off.

  We'll also turn the D7 LED on when the Photoresistor detects a beam breakagse.
  */

  if (analogRead(force)>threshold) {
      digitalWrite(D7,HIGH);
      if(!done && ready){
          digitalWrite(led,HIGH);
          endTime = millis();
          totalTime = String(float(endTime-startTime)/60000);
          Particle.publish("TIME",totalTime);
          done = true;
      }
      
  }
  else{
      digitalWrite(D7,LOW);
      if(done){
          digitalWrite(led,LOW);
          done = false;
          ready = false;
      }
      
  }

}

int activate(String command) {
    /* Particle.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */

    if (command=="start") {
        startTime = millis();
        digitalWrite(led,HIGH);
        ready = true;
        return 1;
    }
    else {
        return -1;
    }
}