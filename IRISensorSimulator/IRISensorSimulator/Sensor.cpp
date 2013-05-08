//
//  Sensor.cpp
//  IRISensorSimulator
//
//  Created by Hva Amsterdam on 3/19/13.
//  Copyright (c) 2013 Hva Amsterdam. All rights reserved.
//



#include "Sensor.h"
using namespace std;
class Sensor
{
public:
    void setSensorId(int theSensorId);
    void setMinValue(double theMinValue);
    void setMaxValue(double theMsaxValue);
    
private:
    int sensorId;
    double minValue;
    double maxValue;
    double currentValue;
};

void Sensor::setSensorId(int theSensorId){
    sensorId = theSensorId;
}

void Sensor::setMinValue(<#double theMinValue#>){
    minValue = theMinValue;
}