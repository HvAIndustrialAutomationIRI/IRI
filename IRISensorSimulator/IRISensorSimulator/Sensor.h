//
//  Sensor.h
//  IRISensorSimulator
//
//  Created by Hva Amsterdam on 3/19/13.
//  Copyright (c) 2013 Hva Amsterdam. All rights reserved.
//

#ifndef __IRISensorSimulator__Sensor__
#define __IRISensorSimulator__Sensor__

#include <iostream>

class Sensor{
public:
    void setSensorId(int theSensorId);
    void setMinValue(double theMinValue);
    void setMaxValue(double theMaxValue);
    
private:
    int sensorId;
    double minValue;
    double maxValue;
    double currentValue;
};


#endif /* defined(__IRISensorSimulator__Sensor__) */

