const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 66;        // value output to the PWM (analog out) 66 to set 20%
int lastOut ;

void setup() {
  Serial.begin(115200);
   // Configure Timer 1 for PWM @ 25 kHz.
    TCCR1A = 0;           // undo the configuration done by...
    TCCR1B = 0;           // ...the Arduino core library
    TCNT1  = 0;           // reset timer
    TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
           | _BV(COM1B1)  // same on ch; B
           | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
    TCCR1B = _BV(WGM13)   // ditto
           | _BV(CS10);   // prescaler = 1
    ICR1   = 320;         // TOP = 320

    // Set the PWM pins as output.
    pinMode( 9, OUTPUT);
    pinMode(10, OUTPUT);
 
}

// PWM output @ 25 kHz, only on pins 9 and 10.
// Output value should be between 0 and 320, inclusive.
void analogWrite25k(int pin, int value)
{
    switch (pin) {
        case 9:
            OCR1A = value;
            break;
        case 10:
            OCR1B = value;
            break;
        default:
            // no other pin will work
            break;
    }
}

void loop() {
    // read the analog in value:
    //sensorValue = analogRead(analogInPin);
    if (Serial.available() > 0) {
    String teststr = Serial.readString();  //read until timeout
    teststr.trim();
    sensorValue = teststr.toInt();
    outputValue = map(constrain(sensorValue, 20, 100), 20, 100, 66, 320);
    
    if (outputValue != lastOut) {
      Serial.print("Out: ");
      Serial.print(sensorValue);
      Serial.print(" - ");
      Serial.println(outputValue);
      //analogWrite25k(10, outputValue);
    }
     }
    // map it to the range of the analog out:
    analogWrite25k(10, outputValue);
    lastOut = outputValue ;
}