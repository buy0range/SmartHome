#include <Arduino_FreeRTOS.h> //Libraries we need to use
#include <Servo.h>
#include <semphr.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define MotionSensor_PIN 2 //define pins according to the pdf "Wiring the Smart Home (SH) File"
#define BUTTONL_PIN 4      //BUTTONL means Left button
#define BUTTONR_PIN 8
#define LEDyellow_PIN 5
#define Doorservo_PIN 9
#define PhotocellSensor_PIN A1
Servo doorservo;  // create servo object to control a servo
int pos = 0;
int val1 = 0;   //valtage of motion sensor
int val2 = 0;   //valtage of left button(emergency button)
int val3 = 1;   //current valtage of right button(toggle button)
bool Auto = 1;  // current status of auto mode


int Photoresistor = 0;           // the value of Photoresistor
int LEDBrightness = 0;         // LED intensity（0-255）
LiquidCrystal_I2C lcd(0x27, 20, 2);


//TaskHandle_t taskHandleLED;
TaskHandle_t taskHandleDoor;
TaskHandle_t taskHandleEmergency;
TaskHandle_t taskHandleToggle;


void setup(){
  pinMode(MotionSensor_PIN, INPUT); //define Motionsensor pin as input
  pinMode(LEDyellow_PIN, OUTPUT);   //define yellowlight pin as output
  pinMode(BUTTONL_PIN, INPUT_PULLUP); // use a pullup button
  pinMode(BUTTONR_PIN, INPUT_PULLUP); // use a pullup button
  pinMode(PhotocellSensor_PIN, INPUT); //define PhotocellSensor pin as input
  doorservo.attach(Doorservo_PIN); //define the pin of doorservo
  lcd.init(); // initialize the LCD
  lcd.backlight();
  Serial.begin(9600); //initialize the serial
  Serial.println("Setup complete");

  // set up tasks
  //xTaskCreate(LED, "Intensity", 128, NULL, 1, &taskHandleLED);// change the intensity of LED
  xTaskCreate(Door, "DoorControl", 128, NULL, 4, &taskHandleDoor); // Control the door task with handle
  xTaskCreate(EMBUTTON, "Stopdoor", 128, NULL, 3, &taskHandleEmergency);// Stop the door task with handle
  xTaskCreate(TGBUTTON, "ChangeModel", 128, NULL, 1, &taskHandleToggle);// change the auto or unauto with handle
  
  vTaskSuspend(taskHandleEmergency);
  vTaskSuspend(taskHandleDoor);
  
}

void loop() {}

// void LED(void *pvParameters){
//   while(1){
//       // read the value of photoresister
//     Photoresistor = analogRead(PhotocellSensor_PIN);

//     // Map the photoresistor to brightness range (0-255)
//     LEDBrightness = map(Photoresistor, 0, 1023, 255, 0); // the brighter outside, the weaker LED

//     // set the intensity
//     analogWrite(LEDyellow_PIN, LEDBrightness);

//     // print info
//     // Serial.print("Sensor Value: ");
//     // Serial.print(Photoresistor);
//     // Serial.print("  LED Brightness: ");
//     // Serial.println(LEDBrightness);

//     vTaskDelay(pdMS_TO_TICKS(1000));
//   }  
// }

void TGBUTTON(void *pvParameters){
  while(1){
    // read the value of photoresister
    Photoresistor = analogRead(PhotocellSensor_PIN);

    // Map the photoresistor to brightness range (0-255)
    LEDBrightness = map(Photoresistor, 0, 1023, 255, 0); // the brighter outside, the weaker LED

    // set the intensity
    analogWrite(LEDyellow_PIN, LEDBrightness);
    val3 = digitalRead(BUTTONR_PIN);            //read the valtage of right button
    
    if(val3 == LOW){
      Auto = !Auto;
    }else{}
    if(Auto){                       //auto on
      lcd.clear();
      lcd.print("Auto ON");
      vTaskResume(taskHandleEmergency);
      vTaskResume(taskHandleDoor);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }else{                         //auto off 
      lcd.clear();
      lcd.print("Auto OFF");
      vTaskSuspend(taskHandleEmergency);
      vTaskSuspend(taskHandleDoor);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }

     

    // if(val3 == LOW){                            //auto off
    //   //tgbutton = true;
    //   lcd.clear();
    //   lcd.print("Auto OFF");
    //   vTaskSuspend(taskHandleEmergency);
    //   vTaskSuspend(taskHandleDoor);
    //   vTaskDelay(pdMS_TO_TICKS(100));
    // }else{                                      //auto on
    //   //tgbutton = false;
    //   lcd.clear();
    //   lcd.print("Auto ON");
    //   vTaskResume(taskHandleEmergency);
    //   vTaskResume(taskHandleDoor);
    //   vTaskDelay(pdMS_TO_TICKS(1000));
    // }
  
  }
}

void EMBUTTON(void *pvParameters){
  
  
  while(1){
    // read the value of photoresister
    Photoresistor = analogRead(PhotocellSensor_PIN);

    // Map the photoresistor to brightness range (0-255)
    LEDBrightness = map(Photoresistor, 0, 1023, 255, 0); // the brighter outside, the weaker LED

    // set the intensity
    analogWrite(LEDyellow_PIN, LEDBrightness);
    val2 = digitalRead(BUTTONL_PIN);            //read the valtage of left button
    if(val2 == LOW){
      //embutton = true;
      vTaskSuspend(taskHandleDoor);
      vTaskSuspend(taskHandleToggle);
      lcd.clear();
      lcd.print("Enmergency Stop");
      vTaskDelay(pdMS_TO_TICKS(100));
    }else{
      //embutton = false;
      vTaskResume(taskHandleDoor);
      vTaskResume(taskHandleToggle);
      vTaskDelay(pdMS_TO_TICKS(1000)); 
    } 
  } 
}

void Door(void *pvParameters){
  
  while(1){
    // read the value of photoresister
    Photoresistor = analogRead(PhotocellSensor_PIN);

    // Map the photoresistor to brightness range (0-255)
    LEDBrightness = map(Photoresistor, 0, 1023, 255, 0); // the brighter outside, the weaker LED

    // set the intensity
    analogWrite(LEDyellow_PIN, LEDBrightness);
      val1 = digitalRead(MotionSensor_PIN);
      Serial.print("Sensor State: ");
      Serial.println(val1);
      if(val1 == HIGH){
        
        for (pos = 0; pos <= 180; pos += 1){      // goes from 0 degrees to 180 degrees, in steps of 1 degree
          doorservo.write(pos);                   // tell servo to go to position in variable 'pos'
          lcd.clear();
          lcd.print("Door is moving");
          vTaskDelay(pdMS_TO_TICKS(50));          // waits 50ms for the servo to reach the position
        }
        for (pos = 180; pos >= 0; pos -= 1){      // goes from 0 degrees to 180 degrees, in steps of 1 degree
          doorservo.write(pos);                   // tell servo to go to position in variable 'pos'
          lcd.clear();
          lcd.print("Door is moving");
          vTaskDelay(pdMS_TO_TICKS(50));          // waits 50ms for the servo to reach the position
        }
      }else{
        vTaskDelay(pdMS_TO_TICKS(100));
      }
      
  }
}


