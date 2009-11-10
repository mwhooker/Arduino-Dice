#include <Wire.h>
#include <string.h>
#undef int
#include <stdio.h>

#include "WProgram.h"
void setup();
void loop();
void waitForRoll();
void setDisplay(const int die[3], int displayMe);
void
nunchuck_init ();
void
send_zero ();
int *get_accel_xyz(int *dest);
char
nunchuk_decode_byte (char x);
uint8_t outbuf[6];		// array to store arduino output
int cnt = 0;
int ledPin = 13;

const int DIE_A[3] = {2, 3, 4};
const int DIE_B[3] = {5, 6, 7};

void waitForRoll(); 
void setDisplay(const int die[3], int displayMe);

void setup() {
  Serial.begin(19200);
  Serial.print ("Finished setup\n");
  //Wire.begin ();		// join i2c bus with address 0x52
  //nunchuck_init (); // send the initilization handshake

  for (int i = 0; i < 3; ++i) {
    pinMode(DIE_A[i], OUTPUT);
    pinMode(DIE_B[i], OUTPUT);
  }

}


void loop()
{
  int reading[3] = {0};
  //get_accel_xyz(reading);
  
    
  waitForRoll();

  setDisplay(DIE_A, random(1,7));
  setDisplay(DIE_B, random(1,7));  
}


/***********************
dice
************************/

//@todo wait for the person to actually roll
void waitForRoll() {
  delay(10000);
}

void setDisplay(const int die[3], int displayMe) {
  if (displayMe > 6 || displayMe < 0) {
    return;
  }
  
  Serial.print("displaying: ");
  Serial.print(displayMe, DEC);
  Serial.print("\n");
  
  Serial.print((displayMe & 1) ? HIGH : LOW, DEC);
  Serial.print(" ");
  Serial.print((displayMe & 2) ? HIGH : LOW, DEC);
  Serial.print(" ");
  Serial.print((displayMe & 4) ? HIGH : LOW, DEC);
  Serial.print("\n");
  Serial.print("Pins: ");
  Serial.print(die[0], DEC);
  Serial.print(" ");
  Serial.print(die[1], DEC);
  Serial.print(" ");
  Serial.print(die[2], DEC);
  Serial.print("\n");

    
  digitalWrite(die[0], (displayMe & 1) ? HIGH : LOW);
  digitalWrite(die[1], (displayMe & 2) ? HIGH : LOW);
  digitalWrite(die[2], (displayMe & 4) ? HIGH : LOW);

}

/*********************
nunchuck
*********************/


void
nunchuck_init ()
{
  Wire.beginTransmission (0x52);	// transmit to device 0x52
  Wire.send (0x40);		// sends memory address
  Wire.send (0x00);		// sends sent a zero.  
  Wire.endTransmission ();	// stop transmitting
}

void
send_zero ()
{
  Wire.beginTransmission (0x52);	// transmit to device 0x52
  Wire.send (0x00);		// sends one byte
  Wire.endTransmission ();	// stop transmitting
}



int *get_accel_xyz(int *dest) {
  cnt = 0;
  Wire.requestFrom (0x52, 6);	// request data from nunchuck
  while (Wire.available ())
    {
      outbuf[cnt] = nunchuk_decode_byte (Wire.receive ());	// receive byte as an integer
      digitalWrite (ledPin, HIGH);	// sets the LED on
      cnt++;
    }
    digitalWrite (ledPin, LOW);	// sets the LED off

    send_zero (); // send the request for next bytes
    delay(100);
  
  // If we recieved the 6 bytes, then go print them
  if (cnt < 5)
    {
      return dest;
    }

  
  int accel_x_axis = outbuf[2] * 2 * 2; 
  int accel_y_axis = outbuf[3] * 2 * 2;
  int accel_z_axis = outbuf[4] * 2 * 2;

 // byte outbuf[5] contains bits for z and c buttons
 // it also contains the least significant bits for the accelerometer data
 // so we have to check each bit of byte outbuf[5]

  if ((outbuf[5] >> 2) & 1)
    {
      accel_x_axis += 2;
    }
  if ((outbuf[5] >> 3) & 1)
    {
      accel_x_axis += 1;
    }

  if ((outbuf[5] >> 4) & 1)
    {
      accel_y_axis += 2;
    }
  if ((outbuf[5] >> 5) & 1)
    {
      accel_y_axis += 1;
    }

  if ((outbuf[5] >> 6) & 1)
    {
      accel_z_axis += 2;
    }
  if ((outbuf[5] >> 7) & 1)
    {
      accel_z_axis += 1;
    }
    
    dest[0] = accel_x_axis;
    dest[1] = accel_y_axis;
    dest[2] = accel_z_axis;
  
    return dest;

}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char
nunchuk_decode_byte (char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

