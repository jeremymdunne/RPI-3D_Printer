#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include <string>
#include <string.h>
#include <fstream>
#include <thread>
#include <exception>


#include "Heater.h"



using namespace std;

int main(){
    wiringPiSetup();
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
    pinMode(23,OUTPUT);
    pinMode(15, INPUT);
    Heater heater(23, 15);
    heater.setTargetTemp(200);
    heater.startHeating();
    while(heater.getTemp() < 200) cout << "Temp: " << heater.getTemp() << endl;
    heater.setCoolDownTemp(60);
    heater.startCoolDown();
    while(!heater.hasCooledDown()) cout << "Cooling Temp: " << heater.getTemp() << endl;
    cout << "Done!" << endl;
    digitalWrite(27, LOW);

}
