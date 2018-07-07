/*QTRSensors.c modificado para proyecto Pinguino*/
/*
 * Written by Ben Schmidel et al., October 4, 2010
 * Copyright (c) 2008-2012 Pololu Corporation. For more information, see
 *
 *   http://www.pololu.com
 *   http://forum.pololu.com
 *   http://www.pololu.com/docs/0J19
 *
 * You may freely modify and share this code, as long as you keep this
 * notice intact (including the two links above).  Licensed under the
 * Creative Commons BY-SA 3.0 license:
 *
 *   http://creativecommons.org/licenses/by-sa/3.0/
 *
 * Disclaimer: To the extent permitted by law, Pololu provides this work
 * without any warranty.  It might be defective, in which case you agree
 * to be responsible for all resulting costs and damages.
 */

 /* Modificado por Leonardo Javier Alvarez 14/04/2016*/

#include <p32xxxx.h>        // PIC32 registers
#include <typedef.h>        // Pinguino's type (u8, u16, u32, ...)
#include <const.h>
#include <pin.h>            // Pinguino's pin definitions
#include <QTRSensors.h>
#include <delay.c>
#include <millis.c>
#include <digitalw.c>
#include <system.c>

  // Base class data member initialization (called by derived class init())
void QTRSensors_init(unsigned char *pins, unsigned char numSensors, unsigned char emitterPin, unsigned int timeout)
{
    unsigned char i;

    g_maxValue = timeout;  // Fix the timeout limit
    g_lastValue=0; // assume initially that the line is left.

    // Clear the calibrated data
    for(i=0;i<g_numSensors;i++)
    {
        calibratedMinimumOn[i] = g_maxValue;
        calibratedMinimumOff[i] = g_maxValue;
        calibratedMaximumOn[i] = 0;
        calibratedMaximumOff[i] = 0;
    }



    if (numSensors > QTR_MAX_SENSORS)
        g_numSensors = QTR_MAX_SENSORS;
    else
        g_numSensors = numSensors;

    for (i = 0; i < g_numSensors; i++)
    {
         g_pins[i] = *pins;
         // Update the pointer
         pins++;

    }
    g_emitterPin = emitterPin;
}

// Reads the sensor values into an array. There *MUST* be space
// for as many values as there were sensors specified in the constructor.
// Example usage:
// unsigned int sensor_values[8];
// sensors.read(sensor_values);
// The values returned are a measure of the reflectance in abstract units,
// with higher values corresponding to lower reflectance (e.g. a black
// surface or a void).
void QTRSensors_read(unsigned int *sensor_values, unsigned char readMode)
{
    unsigned int off_values[QTR_MAX_SENSORS];
    unsigned char i;
    unsigned int *aux;

    aux = sensor_values; //Guardo el puntero
    if(readMode == QTR_EMITTERS_ON || readMode == QTR_EMITTERS_ON_AND_OFF)
        emittersOn();
    else
        emittersOff();

    readPrivate(sensor_values);
    emittersOff();

    if(readMode == QTR_EMITTERS_ON_AND_OFF)
    {
        readPrivate(off_values);
        sensor_values = aux;    // recupero el puntero
        for(i=0;i<g_numSensors;i++)
        {
            *sensor_values += g_maxValue - off_values[i];
            sensor_values++;
        }
    }
}

// Turn the IR LEDs off and on.  This is mainly for use by the
// read method, and calling these functions before or
// after the reading the sensors will have no effect on the
// readings, but you may wish to use these for testing purposes.
void emittersOff()
{
    if (g_emitterPin == QTR_NO_EMITTER_PIN)
        return;
    pinmode(g_emitterPin, OUTPUT);
    digitalwrite(g_emitterPin, LOW);
    Delayus(200);
}

void emittersOn()
{
    if (g_emitterPin == QTR_NO_EMITTER_PIN)
        return;
    pinmode(g_emitterPin, OUTPUT);
    digitalwrite(g_emitterPin, HIGH);
    Delayus(200);
}

// Resets the calibration.
void QTRSensors_resetCalibration()
{
    unsigned char i;
    for(i=0;i<g_numSensors;i++)
    {
        calibratedMinimumOn[i] = g_maxValue;
        calibratedMinimumOff[i] = g_maxValue;
        calibratedMaximumOn[i] = 0;
        calibratedMaximumOff[i] = 0;
    }
}

// Reads the sensors 10 times and uses the results for
// calibration.  The sensor values are not returned; instead, the
// maximum and minimum values found over time are stored internally
// and used for the readCalibrated() method.
void QTRSensors_calibrate(unsigned char readMode)
{
    if(readMode == QTR_EMITTERS_ON_AND_OFF || readMode == QTR_EMITTERS_ON)
    {
        calibrateOnOrOff(calibratedMinimumOn,
                         calibratedMaximumOn,
                         QTR_EMITTERS_ON);
    }


    if(readMode == QTR_EMITTERS_ON_AND_OFF || readMode == QTR_EMITTERS_OFF)
    {
        calibrateOnOrOff(calibratedMinimumOff,
                         calibratedMaximumOff,
                         QTR_EMITTERS_OFF);
    }
}

void calibrateOnOrOff(unsigned int *calibratedMinimum, unsigned int *calibratedMaximum, unsigned char readMode)
{
    int i;
    int j;
    unsigned int sensor_values[16];
    unsigned int max_sensor_values[16];
    unsigned int min_sensor_values[16];

    /*    // Initialize the max and min calibrated values to values that
        // will cause the first reading to update them.

    for(i=0;i<g_numSensors;i++){

        *calibratedMaximum = 0;
        calibratedMaximum++;
    }

    for(i=0;i<g_numSensors;i++){
        *calibratedMinimum = g_maxValue;
        calibratedMinimum++;
    }
    */

    for(j=0;j<10;j++)
    {
        QTRSensors_read(sensor_values,readMode);
        for(i=0;i<g_numSensors;i++)
        {
            // set the max we found THIS time
            if(j == 0 || max_sensor_values[i] < sensor_values[i])
                max_sensor_values[i] = sensor_values[i];

            // set the min we found THIS time
            if(j == 0 || min_sensor_values[i] > sensor_values[i])
                min_sensor_values[i] = sensor_values[i];
        }
    }

    // record the min and max calibration values
    for(i=0;i<g_numSensors;i++)
    {
        if(min_sensor_values[i] > (*calibratedMaximum))
            (*calibratedMaximum) = min_sensor_values[i];
        if(max_sensor_values[i] < (*calibratedMinimum))
            (*calibratedMinimum) = max_sensor_values[i];
        // update the address pointing to
        calibratedMaximum++;
        calibratedMinimum++;

    }
}


// Returns values calibrated to a value between 0 and 1000, where
// 0 corresponds to the minimum value read by calibrate() and 1000
// corresponds to the maximum value.  Calibration values are
// stored separately for each sensor, so that differences in the
// sensors are accounted for automatically.
void QTRSensors_readCalibrated(unsigned int *sensor_values, unsigned char readMode)
{
    int i;
    int x = 0;
    unsigned int calmin,calmax;
    unsigned int denominator;
    /*
    // if not calibrated, do nothing
    if(readMode == QTR_EMITTERS_ON_AND_OFF || readMode == QTR_EMITTERS_OFF)
        if(!calibratedMinimumOff || !calibratedMaximumOff)
            return;
    if(readMode == QTR_EMITTERS_ON_AND_OFF || readMode == QTR_EMITTERS_ON)
        if(!calibratedMinimumOn || !calibratedMaximumOn)
            return;
    */
    // read the needed values
    QTRSensors_read(sensor_values,readMode);

    for(i=0;i<g_numSensors;i++)
    {

        // find the correct calibration
        if(readMode == QTR_EMITTERS_ON)
        {
            calmax = calibratedMaximumOn[i];
            calmin = calibratedMinimumOn[i];
        }
        else if(readMode == QTR_EMITTERS_OFF)
        {
            calmax = calibratedMaximumOff[i];
            calmin = calibratedMinimumOff[i];
        }
        else // QTR_EMITTERS_ON_AND_OFF
        {

            if(calibratedMinimumOff[i] < calibratedMinimumOn[i]) // no meaningful signal
                calmin = g_maxValue;
            else
                calmin = calibratedMinimumOn[i] + g_maxValue - calibratedMinimumOff[i]; // this won't go past _maxValue

            if(calibratedMaximumOff[i] < calibratedMaximumOn[i]) // no meaningful signal
                calmax = g_maxValue;
            else
                calmax = calibratedMaximumOn[i] + g_maxValue - calibratedMaximumOff[i]; // this won't go past _maxValue
        }

        denominator = calmax - calmin;

        if(denominator != 0)
            x = (((long)(*sensor_values)) - calmin)*1000/denominator;
        if(x < 0)
            x = 0;
        else if(x > 1000)
            x = 1000;
        *sensor_values = x;
        sensor_values++;
    }

}


// Operates the same as read calibrated, but also returns an
// estimated position of the robot with respect to a line. The
// estimate is made using a weighted average of the sensor indices
// multiplied by 1000, so that a return value of 0 indicates that
// the line is directly below sensor 0, a return value of 1000
// indicates that the line is directly below sensor 1, 2000
// indicates that it's below sensor 2000, etc.  Intermediate
// values indicate that the line is between two sensors.  The
// formula is:
//
//    0*value0 + 1000*value1 + 2000*value2 + ...
//   --------------------------------------------
//         value0  +  value1  +  value2 + ...
//
// By default, this function assumes a dark line (high values)
// surrounded by white (low values).  If your line is light on
// black, set the optional second argument white_line to true.  In
// this case, each sensor value will be replaced by (1000-value)
// before the averaging.
int QTRSensors_readLine(unsigned int *sensor_values, unsigned char readMode, unsigned char white_line)
{
    unsigned char i, on_line = 0;
    unsigned long avg; // this is for the weighted total, which is long
                       // before division
    unsigned int sum; // this is for the denominator which is <= 64000
    unsigned int *aux;
    int value;
    aux = sensor_values;
    QTRSensors_readCalibrated(sensor_values, readMode);

    avg = 0;
    sum = 0;
    sensor_values = aux; //recover pointer address
    for(i=0;i<g_numSensors;i++) {
        value = *sensor_values;
        if(white_line)
            value = 1000-value;

        // keep track of whether we see the line at all
        if(value > 200) {
            on_line = 1;
        }

        // only average in values that are above a noise threshold
        if(value > 50) {
            avg += (long)(value) * (i * 1000);
            sum += value;
        }
        sensor_values++; // update the pointer
    }

    if(!on_line)
    {
        // If it last read to the left of center, return 0.
        if(g_lastValue < (g_numSensors-1)*1000/2)
            return 0;

        // If it last read to the right of center, return the max.
        else
            return (g_numSensors-1)*1000;

    }

    g_lastValue = avg/sum;

    return g_lastValue;
}

// Reads the sensor values into an array. There *MUST* be space
// for as many values as there were sensors specified in the constructor.
// Example usage:
// unsigned int sensor_values[8];
// sensors.read(sensor_values);
// ...
// The values returned are in microseconds and range from 0 to
// timeout (as specified in the constructor).
void readPrivate(unsigned int *sensor_values)
{
    unsigned char i;
    unsigned int *aux;
    unsigned long startTime = 0;
    unsigned int time = 0;

    aux = sensor_values;
    for(i = 0; i < g_numSensors; i++)
    {
        *sensor_values = g_maxValue;
        // Update the pointer
        sensor_values++;
        digitalwrite(g_pins[i], HIGH);   // make sensor line an output
        pinmode(g_pins[i], OUTPUT);      // drive sensor line high
    }

    Delayus(10);              // charge lines for 10 us

    for(i = 0; i < g_numSensors; i++)
    {
        pinmode(g_pins[i], INPUT);       // make sensor line an input
        //digitalwrite(g_pins[i], LOW);        // important: disable internal pull-up!
    }
    // Recover the starting point address

    startTime = micros();
    while (micros() - startTime < g_maxValue)
    {
        time = micros() - startTime;
        sensor_values = aux;
        for (i = 0; i < g_numSensors; i++)
        {
            if (digitalread(g_pins[i]) == LOW && time < *sensor_values)
                *sensor_values = time;
            sensor_values++;   // increment the pointer
        }
    }
}
