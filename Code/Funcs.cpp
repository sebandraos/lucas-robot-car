/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2021-01-05 09:30:14
 * @LastEditors: Changhua
 * @Description: Smart Robot Car V4.0
 * @FilePath:
 */
#include <avr/wdt.h>
// #include <hardwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "Funcs.h"
#include "Drivers.h"

#include "ArduinoJson-v6.11.1.h" //ArduinoJson
#include "MPU6050_getdata.h"

#define _is_print 1
#define _Test_print 0

Funcs Functions;

/*Hardware device object list*/
MPU6050_getdata AppMPU6050;
Drivers_RBGLED AppRBG_LED;
Drivers_Key AppKey;
Drivers_ITR20001 AppITR20001;
Drivers_Voltage AppVoltage;

Drivers_Motor AppMotor;
Drivers_ULTRASONIC AppULTRASONIC;
Drivers_Servo AppServo;
Drivers_IRrecv AppIRrecv;
/*f(x) int */
static boolean
function_xxx(long x, long s, long e) // f(x)
{
    if (s <= x && x <= e)
        return true;
    else
        return false;
}
static void
delay_xxx(uint16_t _ms)
{
    wdt_reset();
    for (unsigned long i = 0; i < _ms; i++)
    {
        delay(1);
    }
}

/*Movement Direction Control List*/
enum SmartRobotCarMotionControl
{
    Forward,       //(1)
    Backward,      //(2)
    Left,          //(3)
    Right,         //(4)
    LeftForward,   //(5)
    LeftBackward,  //(6)
    RightForward,  //(7)
    RightBackward, //(8)
    stop_it        //(9)
}; // direction方向:（1）、（2）、 （3）、（4）、（5）、（6）

/*Mode Control List*/
enum SmartRobotCarFunctionalModel
{
    Standby_mode,           /*Standby Mode*/
    TraceBased_mode,        /*Line Tracking Mode*/
    ObstacleAvoidance_mode, /*Obstacle Avoidance Mode*/
    Follow_mode,            /*Following Mode*/
    Rocker_mode,            /*Rocker Control Mode*/
    Mode_inspect,
    Mode_Programming_mode,                   /*Programming Mode*/
    Mode_ClearAllFunctions_Standby_mode,     /*Clear All Functions And Enter Standby Mode*/
    Mode_ClearAllFunctions_Programming_mode, /*Clear All Functions And Enter Programming Mode*/
    Mode_MotorControl,                       /*Motor Control Mode*/
    Mode_CarControl_TimeLimit,               /*Car Movement Direction Control With Time Limit*/
    Mode_CarControl_NoTimeLimit,             /*Car Movement Direction Control Without Time Limit*/
    Mode_MotorControl_Speed,                 /*Motor Speed Control*/
    Mode_ServoControl,                       /*Servo Motor Control*/
    Mode_LightingControl_TimeLimit,          /*RGB Lighting Control With Time Limit*/
    Mode_LightingControl_NoTimeLimit,        /*RGB Lighting Control Without Time Limit*/

};

/*Application Management list*/
struct RobotApplication
{
    SmartRobotCarMotionControl Motion_Control;
    SmartRobotCarFunctionalModel Functional_Mode;
    unsigned long CMD_CarControl_Millis;
    unsigned long CMD_LightingControl_Millis;
};
RobotApplication Application_SmartRobotCar;

void Funcs::Init(void)
{
    bool res_error = true;
    Serial.begin(9600);
    AppVoltage.Drivers_Voltage_Init();
    AppMotor.Drivers_Motor_Init();
    AppServo.Drivers_Servo_Init(90);
    AppKey.Drivers_Key_Init();
    AppRBG_LED.Drivers_RBGLED_Init(20);
    AppIRrecv.Drivers_IRrecv_Init();
    AppULTRASONIC.Drivers_ULTRASONIC_Init();
    AppITR20001.Drivers_ITR20001_Init();
    res_error = AppMPU6050.MPU6050_dveInit();
    AppMPU6050.MPU6050_calibration();

    // while (Serial.read() >= 0)
    // {
    //   /*Clear serial port buffer...*/
    // }
    Application_SmartRobotCar.Functional_Mode = Standby_mode;
}

/*
  Startup operation requirement：
*/
void Funcs::Bootup(void)
{
    Application_SmartRobotCar.Functional_Mode = Standby_mode;
}

/*
 Robot car update sensors' data:Partial update (selective update)
*/
void Funcs::SensorDataUpdate(void)
{

    // AppMotor.Drivers_Motor_Test();
    { /*Battery voltage status update*/
        static unsigned long VoltageData_time = 0;
        static int VoltageData_number = 1;
        if (millis() - VoltageData_time > 10) // read and update the data per 10ms
        {
            VoltageData_time = millis();
            VoltageData_V = AppVoltage.Drivers_Voltage_getAnalogue();
            if (VoltageData_V < VoltageDetection)
            {
                VoltageData_number++;
                if (VoltageData_number == 500) // Continuity to judge the latest voltage value multiple
                {
                    VoltageDetectionStatus = true;
                    VoltageData_number = 0;
                }
            }
            else
            {
                VoltageDetectionStatus = false;
            }
        }
    }

    { /*value updation for the IR sensors on the line tracking module：for the line tracking mode*/
        TrackingData_R = AppITR20001.Drivers_ITR20001_getAnaloguexxx_R();
        TrackingDetectionStatus_R = function_xxx(TrackingData_R, TrackingDetection_S, TrackingDetection_E);
        TrackingData_M = AppITR20001.Drivers_ITR20001_getAnaloguexxx_M();
        TrackingDetectionStatus_M = function_xxx(TrackingData_M, TrackingDetection_S, TrackingDetection_E);
        TrackingData_L = AppITR20001.Drivers_ITR20001_getAnaloguexxx_L();
        TrackingDetectionStatus_L = function_xxx(TrackingData_L, TrackingDetection_S, TrackingDetection_E);
    }
}

void Funcs::Wait(float time_s)
{
    delay((uint32_t)(time_s * 1000));
}

void Funcs::SetServo(int angle)
{
    AppServo.Drivers_Servo_control((uint32_t)angle);
}

float Funcs::HowFarIsTheUltrasoundReading(void)
{
    uint16_t distance;
    AppULTRASONIC.Drivers_ULTRASONIC_Get(&distance);
    return (float)distance;
}

boolean Funcs::IsItFarEnoughToDriveForwards(float distance)
{
    if (distance >= 30)
        return true;
    else
        return false;
}

void Funcs::Move(float angle, float time, float speed)
{
    Rotate(angle, speed);

    uint8_t sp = (uint8_t)speed;
    AppMotor.Drivers_Motor_control(true, sp, true, sp, true);
    Wait(time);
    StopMoving();
}

void Funcs::SetMotorSpeeds(float left, float right)
{
    uint8_t l = (uint8_t)abs(left);
    uint8_t r = (uint8_t)abs(right);
    AppMotor.Drivers_Motor_control(left > 0, l, right > 0, r, l > 0 || r > 0);
}

void Funcs::StopMoving(void)
{
    AppMotor.Drivers_Motor_control(true, 0, true, 0, false);
}

void Funcs::Rotate(float angle, float speed)
{
    float yaw = GetYaw();

    float targetYaw = Normalize(yaw + angle);

    uint8_t sp = (uint8_t)speed;

    if (angle < 0)
    {
        AppMotor.Drivers_Motor_control(true, sp, false, sp, true);
    }
    else
    {
        AppMotor.Drivers_Motor_control(false, sp, true, sp, true);
    }

    float tolerance = 2;
    float minimumYaw = Normalize(targetYaw - tolerance);
    float maximumYaw = Normalize(targetYaw + tolerance);

    int attemptCount = 0;
    while (true)
    {
        yaw = GetYaw();

        if (yaw > minimumYaw && yaw < maximumYaw)
        {
            break;
        }
        else
        {
            Wait(0.001);
            attemptCount++;
            if (attemptCount > 1000)
            {
                sp *= 0.75;
                if(sp < 60){
                    sp = 60;
                }
                if (angle < 0)
                {
                    AppMotor.Drivers_Motor_control(true, sp, false, sp, true);
                }
                else
                {
                    AppMotor.Drivers_Motor_control(false, sp, true, sp, true);
                }
                attemptCount = 0;
            }
        }
    }

    StopMoving();
}

float Funcs::GetYaw(void)
{
    float yaw;
    AppMPU6050.MPU6050_dveGetEulerAngles(&yaw);
    return Normalize(yaw);
}

void Funcs::RotateRight(float angle, float speed)
{
    Rotate(angle, speed);
}

void Funcs::RotateLeft(float angle, float speed)
{
    Rotate(-angle, speed);
}

int Funcs::GetKeyValue(void)
{
    uint8_t keyValue;
    AppKey.Drivers_key_Get(&keyValue);
    AppKey.keyValue = 0;
    return (int)keyValue;
}

float Funcs::Normalize(float angle)
{
    while (angle > 180)
    {
        angle = angle - 360;
    }

    while (angle < -180)
    {
        angle = angle + 360;
    }
    return angle;
    // return angle - (360 * floor((angle + 180) / 360));
}

void Funcs::SetLEDColor(int r, int g, int b, int p)
{
    AppRBG_LED.Drivers_RBGLED_Color(1, r, g, b, p);
}

void Funcs::ParseSerialInput(void)
{
    static String SerialPortData = "";
    uint8_t c = '\n';
    boolean isjson = false;
    boolean isComplete = false;
    if (Serial.available() > 0)
    {
        while (c != '}' && Serial.available() > 0)
        {
            c = Serial.read();
            if (c == '{')
            {
                isjson = true;
            }

            SerialPortData += (char)c;

            if (c == '}')
            {
                isComplete = true;
                break;
            }
        }
    }
    else
    {
        return;
    }

    if (c == '}') // Data frame tail check
    {
        if (SerialPortData.equals("{Factory}") || SerialPortData.equals("{WA_NO}") || SerialPortData.equals("{WA_OK}") || SerialPortData.equals("{\"N\":100}"))
        {
            SerialPortData = "";
            return;
        }

        Serial.println("Parsing...");
        Serial.println(SerialPortData);
        StaticJsonDocument<200> doc;                                       // Declare a JsonDocument object
        DeserializationError error = deserializeJson(doc, SerialPortData); // Deserialize JSON data from the serial data buffer
        SerialPortData = "";
        if (error)
        {
            Serial.println("error:deserializeJson");
        }
        else if (!error) // Check if the deserialization is successful
        {
            if (doc.containsKey("N"))
            {
                return;
            }

            String command = doc["cmd"];

            Serial.println("Command:");
            Serial.println(command);

            float speed = 0;
            if (doc.containsKey("speed"))
            {
                speed = doc["speed"];
            }

            float angle = 0;
            if (doc.containsKey("angle"))
            {
                angle = doc["angle"];
            }

            if (command == "move")
            {
                float time = doc["time"];
                Move(angle, time, speed);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "rotate")
            {
                Rotate(angle, speed);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "set_motor_speeds")
            {
                float left = doc["left"];
                float right = doc["right"];
                SetMotorSpeeds(left, right);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "wait")
            {
                float time = doc["time"];
                Wait(time);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "servo")
            {
                SetServo((int)angle);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "get_ultrasound")
            {
                float distance = HowFarIsTheUltrasoundReading();
                Serial.println("{\"ok\":true,\"result\":" + String(distance, 4) + "}");
            }
            else if (command == "led")
            {
                float r = doc["r"];
                float g = doc["g"];
                float b = doc["b"];
                float p = doc["p"];
                SetLEDColor(r, g, b, p);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "stop")
            {
                Move(0, 0, 0);
                Serial.println("{\"ok\":true}");
            }
            else if (command == "get_image")
            {
            }
            else if (command == "get_yaw")
            {
                float yaw = GetYaw();
                Serial.println("{\"ok\":true,\"result\":" + String(yaw, 4) + "}");
            }
        }
    }
}