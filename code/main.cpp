// uLCD-144-G2 basic text demo program for uLCD-4GL LCD driver library
//
#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4); //D7
AnalogIn ain(A0);
DigitalIn buttonUp(D10);
DigitalIn buttonDown(D11);
DigitalIn buttonConfirm(D12);

void getSample();
void waveGenerate(int frequency1);

float ADCdata[100];
int p; //samplePoint
int sampleIndex; //ADCdata Index
int countTimes;
float T = 100;
int frequency = 10;
int start = 0;

int main() {

    uLCD.reset();
    uLCD.background_color(0xFFFFFF);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.cls();
    uLCD.color(BLUE);
    uLCD.locate(2, 6);
    uLCD.printf("%5d Hz", frequency);
    
    while(1) {
        if (buttonUp == 1) {
            frequency += 10;  
            if (frequency >= 80) {
                frequency = 80;
            }
            uLCD.locate(2, 6);
            uLCD.printf("%5d", frequency);
            uLCD.locate(2, 9);
            uLCD.printf("       \n");
            uLCD.locate(2, 10);
            uLCD.printf("         \n");
            start = 0;
        }

        if (buttonDown == 1) {
            frequency -= 10;
            if (frequency <= 10) {
                frequency = 10;
            }
            uLCD.locate(2, 6);
            uLCD.printf("%5d", frequency);
            uLCD.locate(2, 9);
            uLCD.printf("       \n");
            uLCD.locate(2, 10);
            uLCD.printf("         \n");
            start = 0;
        }

        if (buttonConfirm == 1) {
            uLCD.locate(2, 6);
            uLCD.printf("%5d\n", frequency);
            uLCD.locate(2, 9);
            uLCD.printf("Confirm\n");
            uLCD.locate(2, 10);
            uLCD.printf("Frequency\n");
            T = 10000/frequency; // 0.1ms unit
            start = 1; 
            getSample();
        }
        
        if (start == 1) {
            waveGenerate(frequency);
            
        }

        wait_us(T);
    }
}

void getSample() {
    for (int i = 0; i < 100; i++) {
        printf("%f\r\n", ADCdata[i]);
    }
}

void waveGenerate(int frequency1) {

    float peakHeight = 3.3f / 3.0f; 

    // divide the waveform into 100 parts.
    // consider ID % 10 = 9 , we increase wave before 90 parts and decrease after. 
    if (p >= 100) {
        p = 0;
        /*if (sampleIndex < 500) {
            if (countTimes % (frequency1 / 5) == 0)
                ADCdata[sampleIndex] = ain;
        }*/
    }
    else if (p <= 90 && p >= 1) { // 0~90
        aout = p / 90.0f / peakHeight; // aout * 3.3 = 3
        /*if (sampleIndex < 500) {
            if (countTimes % (frequency1 / 5) == 0)
                ADCdata[sampleIndex] = ain;
        }*/
    }
    else if (p > 90 && p < 100) { // 91~99
        aout = (100 - p) / 10.0f / peakHeight; // aout * 3.3 = 3
        /*if (sampleIndex < 500) {
            if (countTimes % (frequency1 / 5) == 0)
                ADCdata[sampleIndex] = ain;
        }*/
    }
    p++;

    if (sampleIndex >= 100) {
        sampleIndex = 0;
        countTimes = 0;
    } else if (sampleIndex < 100 && (countTimes % (frequency/2) == 0)) {
        ADCdata[sampleIndex] = ain;
        sampleIndex++;
    }
    countTimes++;
}
