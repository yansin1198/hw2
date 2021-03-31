// uLCD-144-G2 basic text demo program for uLCD-4GL LCD driver library
//
#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);
AnalogIn ain(A0);

DigitalIn buttonUp(D10);
DigitalIn buttonDown(D11);
DigitalIn buttonConfirm(D12);

int currentRow = 2;
int confirm = 0;
int sample;
int i;
float ADCdata[1000];
Thread thread;

void getSample();
void waveForm(int frequency);
int uLCDControl();

int main() {
    uLCD.reset();
    uLCD.background_color(0xFFFFFF);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.cls();
    uLCD.color(BLUE);
    uLCD.text_width(2); //4X size text
    uLCD.text_height(2);
    uLCD.locate(3, 2);
    uLCD.printf("230 Hz\n");
    uLCD.locate(3, 4);
    uLCD.printf("678 Hz\n");
    while(1) {
        uLCD.locate(1, currentRow);
        uLCD.color(RED);
        uLCD.printf(">");
        if (buttonUp) {
            uLCD.locate(1, currentRow);
            uLCD.printf(" ");
            currentRow = currentRow - 2;
            if (currentRow <= 2) {
                currentRow = 2;
                uLCD.locate(1, currentRow);
                uLCD.printf(">");
            } else {
                uLCD.locate(1, currentRow);
                uLCD.printf(">");
            }
        } 
        else if (buttonDown) {
            uLCD.locate(1, currentRow);
            uLCD.printf(" ");
            currentRow = currentRow + 2;
            if (currentRow >= 4) {
                currentRow = 4;
                uLCD.locate(1, currentRow);
                uLCD.printf(">");
            } else {
                uLCD.locate(1, currentRow);
                uLCD.printf(">");
            }
        }

        confirm = buttonConfirm.read();
        while (confirm) {
            thread.start(get_point);
            uLCD.locate(1, currentRow);
            uLCD.printf(">");
            if (currentRow == 2) {
                //while(!buttonUp.read() && !buttonDown.read() && !buttonConfirm.read()) {
                    waveForm(230);
                    /*for (i = 0; i < sample; i++){
                        ADCdata[i] = ain;
                        ThisThread::sleep_for(1ms/sample);
                    }
                    for (i = 0; i < sample; i++){
                        printf("%f\r\n", ADCdata[i]*3.3);
                        ThisThread::sleep_for(300ms);
                    }*/
                //}
                uLCD.locate(3, 6);
                uLCD.printf("230 Hz\n");
            }
            else if (currentRow == 4) {
                //while(!buttonUp.read() && !buttonDown.read() && !buttonConfirm.read()) {
                    waveForm(678);
                //}
                uLCD.locate(3, 6);
                uLCD.printf("678 Hz\n");
            }

            if (buttonUp.read() || buttonDown.read()) {
                confirm = 0;
            }
        }
    }


    //int selection = uLCDControl();
    /*if (selection == 0) {
        //waveForm(230);
        uLCD.locate(3, 6);
        uLCD.printf("230 Hz\n");
    } else if (selection == 1) {
        //waveForm(678);
        uLCD.locate(3, 6);
        uLCD.printf("678 Hz\n");
    }*/
}

void getSample(){
   // while(1){
        for (int i = 0; i < 1000; i++){
            sample = ain;
            ADCdata[i] = sample;
            //ThisThread::sleep_for(1);
            wait_us(1000);
        }
        for (int i = 0; i < 1000; i++){
            printf("%f\r\n", ADCdata[i]);
        }
   // }
}

void waveForm(int frequency) {
    float increment = (0.00001f * frequency);
    float sample = 0;
    float S = 0.9f; // define the X-axis value when the waveform peaks at 3V
    //confirm = buttonConfirm.read();
    while(!buttonUp.read() && !buttonDown.read() && !buttonConfirm.read()) { 
        // make 'aout' go from 0 to 1, in increment steps
        for(float i = 0.0f; i < 0.9f; i += increment)
        {
            // we want to achieve i * 3.3 = 3V
            if (i < 0.82f) {
                sample =  i / 0.9f;
            }
            else {
                sample = (0.9f - i) / 0.08f * S;
            }
            aout = sample;

            /*while (buttonUp.read() || buttonDown.read()) {
                confirm = 0;
            }*/
        }

        
    }
}






