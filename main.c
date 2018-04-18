/*
 *
 */

#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "Inits.h"
#include "Defines.h"
#include "USART_Header_File.h"
#include "MPU6050.h"
#include "I2Cdev.h"

#pragma config FOSC = INTOSC_EC //internal oscillator 
#pragma config PLLDIV = 2       //divide by 2 to give 4MHz to PLL 
#pragma config FCMEN = OFF      //no fail safe clock monitor 
#pragma config IESO = OFF       //oscillator switchover disabled 
#pragma config PWRT = ON        //oscillator power up timer enabled (release version only) 
#pragma config BOR = OFF        //hardware brown out reset 
#pragma config WDT = OFF        //watchdog timer disabled 
#pragma config MCLRE = ON       //MCLR pin enabled 
#pragma config LPT1OSC = ON     //timer1 low power operation 
#pragma config PBADEN = OFF     //portB 0to 4 digital - not analogue 
#pragma config LVP = OFF        //low voltage programming disabled 
#pragma config CCP2MX = OFF     //portc1 = CCP2 
#pragma config XINST = OFF      //do not allow PIC18 extended instructions 
#pragma config  STVREN = ON     //stack overflow will cause reset 

int x_accel;
int y_accel;
int z_accel;
int temperature;
int x_gyro;
int y_gyro;
int z_gyro;

unsigned long last_read_time;
float         last_x_angle;  // These are the filtered angles
float         last_y_angle;
float         last_z_angle;  
float         last_gyro_x_angle;  // Store the gyro angles to compare drift
float         last_gyro_y_angle;
float         last_gyro_z_angle;

//  Use the following global variables and access functions
//  to calibrate the acceleration sensor
float    base_x_accel;
float    base_y_accel;
float    base_z_accel;

float    base_x_gyro;
float    base_y_gyro;
float    base_z_gyro;

int16_t ax, ay, az, gx, gy, gz;

inline unsigned long get_last_time() {return last_read_time;}
inline float get_last_x_angle() {return last_x_angle;}
inline float get_last_y_angle() {return last_y_angle;}
inline float get_last_z_angle() {return last_z_angle;}
inline float get_last_gyro_x_angle() {return last_gyro_x_angle;}
inline float get_last_gyro_y_angle() {return last_gyro_y_angle;}
inline float get_last_gyro_z_angle() {return last_gyro_z_angle;}

void read_gyro_accel_vals(void) {
  // Read the raw values.
  // Read 14 bytes at once, 
  // containing acceleration, temperature and gyro.
  // With the default settings of the MPU-6050,
  // there is no filter enabled, and the values
  // are not very stable.  Returns the error value
  
  //accel_t_gyro_union* accel_t_gyro = (accel_t_gyro_union *) accel_t_gyro_ptr;
   
  MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  x_accel = ax;
  y_accel = ay;
  z_accel = az;
  x_gyro = gx;
  y_gyro = gy;
  z_gyro = gz;
}

void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}

void calibrate_sensors() {
  int                   num_readings = 20;
  float                 _x_accel = 0;
  float                 _y_accel = 0;
  float                 _z_accel = 0;
  float                 _x_gyro = 0;
  float                 _y_gyro = 0;
  float                 _z_gyro = 0;
  
  //Serial.println("Starting Calibration");

  // Discard the first set of values read from the IMU
  read_gyro_accel_vals();
  
  // Read and average the raw values from the IMU
  for (int i = 0; i < num_readings; i++) {
    read_gyro_accel_vals();
    _x_accel += x_accel;
    _y_accel += y_accel;
    _z_accel += z_accel;
    _x_gyro += x_gyro;
    _y_gyro += y_gyro;
    _z_gyro += z_gyro;
    __delay_ms(100);
  }
  _x_accel /= num_readings;
  _y_accel /= num_readings;
  _z_accel /= num_readings;
  _x_gyro /= num_readings;
  _y_gyro /= num_readings;
  _z_gyro /= num_readings;
  
  // Store the raw calibration values globally
  base_x_accel = _x_accel;
  base_y_accel = _y_accel;
  base_z_accel = _z_accel;
  base_x_gyro = _x_gyro;
  base_y_gyro = _y_gyro;
  base_z_gyro = _z_gyro;
  
  //Serial.println("Finishing Calibration");
}

int main(void) {
    // Get angle values from accelerometer
    float RADIANS_TO_DEGREES = 180/3.14159;
    float alpha = 0.96;
    float alphaInv = (1.0-alpha);
    float dt = 0.02;
    float FS_SEL = 131;
    
    OSCCON = 0x72; // internal oscillator, 8MHz
    
    initIO();
    initBT();
    //initSTEER();
    
    __delay_ms(30);
    // I2C (400 kHz)
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD = 14;
    // MPU6050
    MPU6050(MPU6050_ADDRESS_AD0_LOW);
    printf("Init MPU... \r\n");
    MPU6050_initialize();
    __delay_ms(30);
    if(MPU6050_testConnection())
        printf("MPU enabled!\r\n");
    else printf("MPU failure\r\n");
    
    __delay_ms(1000);
    calibrate_sensors();
    
    while (1) {
        LATCbits.LC2=1;
        
        // Read the raw values.
        read_gyro_accel_vals();
        
        //printf("a/g:\t%d\t%d\t%d\t%d\t%d\t%d\r\n", ax, ay, az, gx, gy, gz);
        
        float gyro_x = (x_gyro - base_x_gyro)/FS_SEL;
        float gyro_y = (y_gyro - base_y_gyro)/FS_SEL;
        float gyro_z = (z_gyro - base_z_gyro)/FS_SEL;
        
        
        // Get raw acceleration values
        //float G_CONVERT = 16384;
        float accel_x = x_accel;
        float accel_y = y_accel;
        float accel_z = z_accel;

        //  float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
        float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
        float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
        float accel_angle_z = 0;

        // Compute the (filtered) gyro angles
        //##float dt =(t_now - get_last_time())/1000.0;
        float gyro_angle_x = gyro_x*dt + get_last_x_angle();
        float gyro_angle_y = gyro_y*dt + get_last_y_angle();
        float gyro_angle_z = gyro_z*dt + get_last_z_angle();

        // Compute the drifting gyro angles
        float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
        float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
        float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();

        // Apply the complementary filter to figure out the change in angle - choice of alpha is
        // estimated now.  Alpha depends on the sampling rate...
        float angle_x = alpha*gyro_angle_x + alphaInv*accel_angle_x;
        float angle_y = alpha*gyro_angle_y + alphaInv*accel_angle_y;
        float angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle

        // Update the saved data with the latest values
        //##set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
        set_last_read_angle_data(0, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
        
        printf("%f, %f, %f\r\n", angle_x, angle_y, angle_z);
        
        __delay_ms(20);
        LATCbits.LC2=0;
        
    }
}


