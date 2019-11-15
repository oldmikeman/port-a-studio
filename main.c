// CC-by-www.Electrosmash.com
// Based on OpenMusicLabs previous works.
// pedalshield_uno_booster.ino: pressing the pushbutton_1 or 2 turns the volume up or down.

//defining harware resources.
#include <wiringPi.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

//#define LED 13
#define FOOTSWITCH 1
#define AUDIO_IN 4
//#define TOGGLE 2
//#define PUSHBUTTON_1 A5
//#define PUSHBUTTON_2 A4

//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

//other variables
int input, vol_variable=512;
int counter=0;
unsigned int ADC_low, ADC_high;

void setup() { //setup IO - inputs/outputs pins configurations and pull-ups
    printf("Setting up your Port-A-Studio...\n");
    if (wiringPiSetup () < 0) {
        fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
        return 1;
    } 
    pinMode(FOOTSWITCH, INPUT);
    pinMode(TOGGLE, INPUT);
    pinMode(PUSHBUTTON_1, INPUT);
    pinMode(PUSHBUTTON_2, INPUT);
    pinMode(LED, OUTPUT);
    /*
    // setup ADC- configured to be reading automatically the hole time.
    ADMUX = 0x60; // left adjust, adc0, internal vcc
    ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
    ADCSRB = 0x07; // t1 capture for trigger
    DIDR0 = 0x01; // turn off digital inputs for adc0

    // setup PWM - for more info about this config check the forum.
    TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
    TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1 TIMSK1 = 0x20; // interrupt on capture interrupt ICR1H = (PWM_FREQ >> 8);
    ICR1L = (PWM_FREQ & 0xff);
    DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs
    sei(); // turn on interrupts - not really necessary with arduino
     */
}
/*
void loop()
{
    //Turn on the LED if the effect is ON.
    if (digitalRead(FOOTSWITCH)) digitalWrite(LED, HIGH);
    else  digitalWrite(LED, LOW);
    //nothing more here, all happens in the Timer 1 interruption.
}
*/
void timerInterrupt(void)
{
    printf("audio in #: %d", ++counter);
}
/*
ISR(TIMER1_CAPT_vect) //Timer 1 interruption.
{
    // read the ADC input signal data: 2 bytes Low and High.
    ADC_low = ADCL; // Low byte need to be fetched first
    ADC_high = ADCH;
    //construct the input sumple summing the ADC low and high byte.
    input = ((ADC_high << 8) | ADC_low) + 0x8000; // make a signed 16b value

    // The push-buttons are checked now:
    counter++; //to save resources, the push-buttons are checked every 100 times.
    if(counter==100)
    {
        counter=0;
        if (!digitalRead(PUSHBUTTON_1)) {
            if (vol_variable<1024) 
                vol_variable=vol_variable+1; //increase the vol } if (!digitalRead(PUSHBUTTON_2)) { if (vol_variable>0) vol_variable=vol_variable-1; //decrease vol
        }
    }

    //the amplitude of the signal is modified following the vol_variableusing the Arduino map fucntion
    input = map(input, 0, 1024, 0, vol_variable);

    //write the PWM output signal
    OCR1AL = ((input + 0x8000) >> 8); // convert to unsigned, send out high byte
    OCR1BL = input; // send out low byte
}
*/
int main(void)
{
    setup();
    
    if ( wiringPiISR (AUDIO_IN, INT_EDGE_RISING, &myInterrupt) < 0 ) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
        return 1;
    }
    
    while(1){
        //Turn on the LED if the effect is ON.
        if (digitalRead(FOOTSWITCH)) digitalWrite(LED, HIGH);
        else  digitalWrite(LED, LOW);
        //nothing more here, all happens in the Timer 1 interruption.
    }
    
    return 0;
}