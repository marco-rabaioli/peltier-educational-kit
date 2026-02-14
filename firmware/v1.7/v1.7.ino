#include <OneWire.h>
#include <DallasTemperature.h>


#define pinFANL           2     // digital pin fan load
#define pinPowerPELTIER   3     // digital pin mosfet peltier (PWM)
#define pinHeatPELTIER    4     // digital pin relè peltier
#define pinRESL           5     // digital pin resistor load
#define pinSENSD          7     // digital pin temperature sensor digital (DS18B20)
#define pinSENSA          A0    // analog pin temperature sensors analog LM35
#define pinLEDblink       13    // led blink on board

#define timeout_temp      8000  // timeout massimo tra una lettura e l'altra del sensore per controllo sovratemperatura
#define timeout           6000  // timeout for serial commands for actuator [ms]
int t_blink =             300;  // led blink while serial commands are ok [ms]

#define peltierPowerLimit  50    // max limit for perlier actuator [0-255] 
#define resistorPowerLimit 50    // max limit for resistor actuator [0-255]
#define tmax               60    // temperature max 



OneWire oneWire(pinSENSD);            // one wire istance for digital sensor DS18B20
DallasTemperature sensors(&oneWire);  // dallas istance for digital sensor DS18B20
DeviceAddress sensorAdress;           


const int MAX_LEN = 20;     // max length array for serial communication
char received[MAX_LEN];     // array for serial communication

int setPoint = 25;

int i=0;

int error = 0;          // 0 -> all is ok
                        // 1 -> fault on digital sensor

int pwmPELTIER = 0;
int pwmFAN = 0;
int pwmRES = 0;

char mode = 'M';    // A -> STAND-ALONE CONTROLLER:  stand alone controller, in this mode Arduino closes the loop automatically, the set-point can set by serial command
                    // M -> INTERFACE MODE:     in this mode the loop is open, the actuator values comes from serial command. The loop is closed in Matlab/Simulink

bool controller_state = 0;
bool stream_state = 1;
float y1=0, y2=0, y=0, u=0, temp=0;


unsigned long int timer_timeout = 0;
unsigned long int timer_blink = 0;
unsigned long int timer_temperature = 0;
unsigned long int time = 0;
unsigned long int diff = 0;

unsigned long int dt = 0;
unsigned long int time_prev = 0;


//###########################################################################################################################################################################################
//                                                           USER VARIABLES
// use name starting with underscore like_   int _derivativeAction

float _error = 0;
float _sumError = 0;





//###########################################################################################################################################################################################


void setup() {
  
  Serial.begin(115200);

  pinMode(pinFANL, OUTPUT);
  pinMode(pinPowerPELTIER, OUTPUT);
  pinMode(pinHeatPELTIER, OUTPUT);
  pinMode(pinRESL, OUTPUT);
  pinMode(pinLEDblink, OUTPUT);


 // setPwmFrequency(pinPowerPELTIER, 8);


  actuator(255,0,0);
  setupDS18B20();
  delay(1000);
  float t=readTemperature(sensorAdress);
  
  

}

void loop() {
  
  
  if (Serial.available() > 0) SerialHandle();

  

  switch(mode){

      case 'A':
                dt = millis()-time_prev;
                time_prev=millis();
                if(controller_state) 
                {
                  y1=readTemperature(sensorAdress);
                  y2=readAnalogSensor();
                  stand_alone_controller();
                  actuator(pwmFAN, u, pwmRES);
                  timer_timeout = millis();
                  if(stream_state) stream();

                } 
                else 
                {
                  actuator(100,0,0);
                  u=0;
                  pwmFAN=0;
                  pwmPELTIER=0;
                  pwmRES=0;
                }

                delay(200);

                
                break;
                
      case 'M': 
                if(millis()-timer_timeout > timeout) 
                {
                  actuator(100,0,0);
                  pwmFAN=0;
                  pwmPELTIER=0;
                  pwmRES=0;
                }
                else{
                          actuator(pwmFAN, pwmPELTIER, pwmRES);
                }


                break;


  }


blink();
check_temperature();
delay(10);

}




void SerialHandle()
{
  int i = 0;
        while (i < MAX_LEN - 1) {             
            if (Serial.available()) {  
                char c = Serial.read(); 
                if (c == '\n') {         
                    break;
                }
                received[i] = c;          
                i++;
            }
        }
        received[i] = '\0';

    if (received[0] != '%') {
                     Serial.println("La stringa non inizia con '%'");
                    received[0] = "*";  
                    received[1] = "*"; 

    } else removeFIRST(received);     // remove "%"


  if (received[0] == '\0') {
      Serial.println("BAD STRING! (Vuota dopo aver rimosso '%')");
      return;
  }

      char CTR = received[0];
      bool ck = false;

      /** SET STAND-ALONE CONTROLLER MODE **/
      if(CTR == 'A'){
        t_blink=1000;
        mode='A';   
        Serial.println("Stand-alone controller mode - Waiting for start command");
        controller_state = 0;
        ck = true;
      }

      /** SET INTERFACE MODE **/
      if(CTR == 'M'){
        t_blink=300;
        mode='M';
        Serial.println("Interface mode - Waiting for actuator commands");
        ck = true;
      }

      /** START STAND-ALONE CONTROLLER **/
      if(CTR == 'T'){
        controller_state=1;
        Serial.println("Stand-alone controller started");
        ck = true;
      }

      if(CTR == 'K'){
        stream_state=1;
        ck = true;
      }

      if(CTR == 'H'){
        stream_state=0;
        ck = true;
      }

      if(CTR == 'S'){
        controller_state=0;
        Serial.println("Stand-alone controller stopped");
        ck = true;
      }

      /** SEND DIGITAL TEMPERATURE **/
      if(CTR == 'b'){
        float t=readTemperature(sensorAdress);
        Serial.println(t);
        ck = true;
      }

      /** SEND ANALOG TEMPERATURE **/
      if(CTR == 'a'){
        Serial.println(readAnalogSensor());
        ck = true;
      }

      /** SET PWM RESISTOR LOAD DISTURBANCE **/
      if(CTR == 'r'){
        removeFIRST(received);
        pwmRES = atoi(received);
        Serial.println(pwmRES);
        timer_timeout = millis();
        ck = true;
      }

      /** SET PWM FAN LOAD DISTURBANCE **/
      if(CTR == 'f'){
        removeFIRST(received);
        pwmFAN = atoi(received);
        Serial.println(pwmFAN);
        timer_timeout = millis();
        ck = true;
      }

      /** SET PWM PELTIER, FAN & RESISTOR AND SEND TEMPERATURE (DIGITAL OR ANALOG)**/
      if(CTR == 'c'){
        removeFIRST(received);
        pwmPELTIER = charToInt(received, 'r');
        pwmRES = charToInt(received, 'f');
        char c = lastChar(received);
        pwmFAN = charToInt(received, c);

        if(c == 'a') Serial.println(readAnalogSensor());
        else         Serial.println(readTemperature(sensorAdress));    //ANALOG
        timer_timeout = millis();
        ck = true;
      }

      /** SET PWM PELTIER CELL **/
      if(CTR == 'p'){
        removeFIRST(received); 
        pwmPELTIER = charToInt(received, '\0');
        Serial.println(pwmPELTIER);     //response
        timer_timeout = millis();
        ck = true;
      }

      /** SET SETPOINT STAND-ALONE CONTROLLER**/
      if(CTR == 's'){
        removeFIRST(received); 
        setPoint = charToInt(received, '\0');
        Serial.println(setPoint);     //response
        timer_timeout = millis();
        ck = true;
      }



      /** DISPLAY OVERALL VARIABLES**/
      if(CTR == 'd'){
        Serial.println();
        Serial.print("Mode: ");
        if(mode=='A') {
          Serial.print("stand alone controller ");
          if(controller_state) Serial.println("active");
          else Serial.println("disactive");
          Serial.print("Setpoint: ");
          Serial.print(setPoint);
          Serial.println("°C");
          Serial.println();
        }else Serial.println("interface");
        
        Serial.print("Peltier PWM: ");
        Serial.println(pwmPELTIER);
        Serial.print("Fan PWM: ");
        Serial.println(pwmFAN);
        Serial.print("Resistor PWM: ");
        Serial.println(pwmRES);


        ck = true;
      }

      if(ck == false){
        Serial.println("BAD STRING !");
        ck = true;
      }


      
}



void removeFIRST(char *array) {
    if (array[0] == '\0') return;  

    for (int i = 0; array[i] != '\0'; i++) {
        array[i] = array[i + 1];
    }
}


int charToInt(char *array, char c) {
    char temp[10];
    if (array[0] == '\0') return 0;  

    short int n = 0;

    for (int i = 0; array[i] != c; i++) {
        temp[i] = array[i];
        n++;
    }

    for(int j = 0; j<=n; j++ ) removeFIRST(array);

    temp[n]='\0';


    int pwm = atoi(temp);

    return pwm;


}


char lastChar(char *array){
    if (array[0] == '\0') return '\0';  
    short int n=0;
    while (array[n] != '\0') { 
        n++;
    }
    return array[n - 1];


}


void actuator(int fan, int peltier, int resistor){


  if(temp > tmax)
  {
    digitalWrite(pinFANL, HIGH);
    digitalWrite(pinHeatPELTIER, LOW);
    analogWrite(pinRESL, 0);
  
  for (int i = 0; i < 4; i++) {
          digitalWrite(pinLEDblink, HIGH);
          digitalWrite(pinHeatPELTIER, HIGH);
          delay(175); 
          digitalWrite(pinLEDblink, LOW);
          digitalWrite(pinHeatPELTIER, LOW);
          delay(175);
         }   
    return;
  }






  float pelt = (peltier/100.0)*peltierPowerLimit;
  float res = (resistor/100.0)*resistorPowerLimit;

  if(pelt>peltierPowerLimit)  pelt = peltierPowerLimit;
  if(pelt<-peltierPowerLimit) pelt = -peltierPowerLimit;
  if(res>resistorPowerLimit) res = resistorPowerLimit;

  if(fan == 0)  digitalWrite(pinFANL, LOW);
  else          digitalWrite(pinFANL, HIGH);

  analogWrite(pinPowerPELTIER, abs(pelt));
  analogWrite(pinRESL, res);

  if(pelt > 0)     digitalWrite(pinHeatPELTIER, HIGH);
  else                digitalWrite(pinHeatPELTIER, LOW);

}



void setPwmFrequency(int pin, int divisor) 
{
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) 
  {
    switch (divisor) 
    {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) 
    {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else               
    {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) 
  {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


void setupDS18B20()
{
    sensors.begin();
    Serial.print(sensors.getDeviceCount(), DEC);
    if (!sensors.getAddress(sensorAdress, 0)) Serial.println("Unable to find sensor DS18B20"); 
    Serial.print("Device 0 Address: ");
    printAddress(sensorAdress);
    sensors.setResolution(sensorAdress, 12);   // set the resolution 12 BIT
    
    delay(100);
}

float readTemperature(DeviceAddress deviceAddress)
{   
    timer_temperature = millis();
    float tempC=0;
    do{
      sensors.requestTemperatures();
      tempC = sensors.getTempC(deviceAddress);
    }while(tempC==0 || tempC == DEVICE_DISCONNECTED_C);
    if(tempC == DEVICE_DISCONNECTED_C) 
    {
      error = 1;
      return 88;
    }
    temp=tempC;
    return tempC;
}

float readAnalogSensor()
{
  float analogR=0;
  for(int q=0;q<4;q++) 
  {
    analogR=analogR+analogRead(pinSENSA);
    delay(15);
  }
  analogR = (analogR/1023/4)*500;
  
  // analogR=analogRead(pinSENSA);
  // analogR=(analogR/1023)*500;

  return analogR;
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


void blink()
{
  time = millis();
  diff = time-timer_blink;

  if(time-timer_timeout > timeout)digitalWrite(pinLEDblink, LOW);
  else
  { 
    
    if(diff < t_blink/2)    digitalWrite(pinLEDblink, LOW);
    else if(diff < t_blink) digitalWrite(pinLEDblink, HIGH);
    else timer_blink = time;
  }

}

void check_temperature()
{
  if((millis() - timer_temperature) > timeout_temp) readTemperature(sensorAdress);

}




void stream()
{
  Serial.print("Setpoint:");
  Serial.print(setPoint);
  Serial.print(",y:");
  Serial.print(y);
  Serial.print(",u:");
  Serial.print(u);
  Serial.print(",FAN:");
  if(pwmFAN>0) Serial.println(10);
  else Serial.println(0);

}



//#######################################################################################################################################################################################################
/* THIS FUNCTION IS DEDICATED FOR USER THAT WHANTS TO IMPLEMENT A STAND ALONE CONTEOLLER 
   There are available this variables:


   setPoint -> the setpoint setted through serial command  DON'T ASSIGN ANY VALUE READ ONLY
   dt -> it's the time between each iteration, it's useful if you have to compute integral or derivative actions  DON'T ASSIGN ANY VALUE READ ONLY [ms]
   pwmRES -> it's the resistor load disturbance action. DON'T ASSIGN ANY VALUE READ ONLY
   pwmFAN -> it's the fan load disturbance. DON'T ASSIGN ANY VALUE READ ONLY
   y1 -> digital reading
   y2 -> analog reading

   u -> it's the Peltier control action u. You can assign value between -100% to 100%




  if you need to declare new global variable you can do at the top of the program. In order to avoid any conflicts start all names with underscore sign _

  An iteration take about 210ms. If you need you can increase the delay adding directly into the function



 */




void stand_alone_controller()
{
  y=y1;    // digital reading

  float Kp=1.0;
  float Ti=2.0;
  
  _error = setPoint-y;
  _sumError += _error*dt/1000;

  u = Kp*(_error + _sumError/Ti);  // PI controller with positional algorithm


}



//#######################################################################################################################################################################################################
