--- Servo heading stabilization system; for use in RC cars, or other such systems with PWM servo steering. ---  

            *Targeted for Pic18F4550 with MPU6050.  
            
            *Servo control is accomplished via PWM at <=50Hz.  
            
            *Uses UART (HC-05 Bluetooth) for system adjustment.
            

This program uses a modified version of Jeff Rowberg's I2Cdev library to obtain heading information from the MPU6050's
DMP. The I2Cdev library only supports raw data reads from the MPU6050 on PIC18f devices, so I have ported the code from
Arduino which makes use of the MPU6050's DMP to process accelerometer/gyro readouts, and provide euler angles to the 
PIC18f. The purpose of this device is to interpret steering info from the receiver and act as a man-in-the-middle between 
the receiver and steering servo, allowing the user to gain heading stability control without replacement of the receiver
and/or radio systems in the vehicle.

To Summarize:   
      -If drift is detected, or, if change in heading angle is detected without a corresponding steering in put from the   
        receiver, the device will take over control of the servo steering system, and attempt to correct the drift.  
      -If manual steering input is received from the receiver, the device will pass through the PWM signal to the steering    
        servo, unchanged.   
      -Correction strength can be adjusted through Bluetooth-UART devices such as HC-05, or via direct UART connection.    
    
Further information on adjustent and tuning will be added as the project matures.
