#ifndef _Funcs_H_
#define _Funcs_H_

#include <Arduino.h>

class Funcs
{
public:
  void Init(void);
  void Bootup(void);
  void SensorDataUpdate(void);   //Sensor Data Update
  void Wait(float);              // Makes the robot wait for a time
  void SetServo(int);            // Sets the position of the servo to a given angle
  float HowFarIsTheUltrasoundReading(void); // Gets the distance from the ultrasound sensor
  boolean IsItFarEnoughToDriveForwards(float);  // Determines whether the given distance is far enough to drive towards
  void Move(float, float, float);  // Moves the robot at a given angle from straight, for a given time, and at a given speed
  void SetMotorSpeeds(float, float); // Sets the speeds of the left and right motors. Negative values make the go backwards.
  void Rotate(float, float);  // Rotates the robot to a given angle from straight, at a given speed
  void RotateRight(float, float);  // Rotates the robot to a given angle to the right (clock-wise), at a given speed
  void RotateLeft(float, float);  // Rotates the robot to a given angle to the left (counter clock-wise), at a given speed
  int GetKeyValue(void); // Gets how many times the button has been pressed
  float GetYaw(void); // Gets the current Yaw of the robot
  void SetLEDColor(int,int,int,int); // Sets the color of the LED to the given RGB values
  float Normalize(float); // Normalizes an angle to between -180 and +180
  void ParseSerialInput(); // Interprets incoming commands from python

private:
  void StopMoving(void); // Stops the motors

private:
  /*Sensor Raw Value*/
  volatile float VoltageData_V;        //Battery Voltage Value
  volatile uint16_t UltrasoundData_mm; //Ultrasonic Sensor Value (mm)
  volatile uint16_t UltrasoundData_cm; //Ultrasonic Sensor Value (cm)
  volatile int TrackingData_L;         //Line Tracking Module Value (Left)
  volatile int TrackingData_M;         //Line Tracking Module Value (Middle)
  volatile int TrackingData_R;         //Line Tracking Module Value (Right)
  /*Sensor Status*/
  boolean VoltageDetectionStatus = false;
  boolean UltrasoundDetectionStatus = false;
  boolean TrackingDetectionStatus_R = false;
  boolean TrackingDetectionStatus_M = false;
  boolean TrackingDetectionStatus_L = false;

public:
  /*Sensor Threshold Setting*/
  const float VoltageDetection = 7.00;
  const uint8_t ObstacleDetection = 20;

  String CommandSerialNumber;
  uint8_t Rocker_CarSpeed = 250;
  uint8_t Rocker_temp;

public:
  uint8_t TrackingDetection_S = 250;
  uint16_t TrackingDetection_E = 850;
  uint16_t TrackingDetection_V = 950;
};
extern Funcs Functions;
#endif
