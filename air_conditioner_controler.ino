#include <dht.h>
#include <Servo.h>
dht DHT;
Servo myservo;
#define DHT11_PIN 5
#define Constant_1SEC 1000ul
#define Constant_1MIN Constant_1SEC * 60
#define Constant_1HR Constant_1MIN * 60
#define max_open_duration Constant_1HR
#define min_open_duration Constant_1MIN * 30
#define max_close_duration Constant_1HR * 24
#define min_close_duration Constant_1MIN * 30
#define critical_max_temperature 28.00
#define critical_min_temperature 26.50
#define array_len 60
#define min_angle 120
#define max_angle 150
#define DHT_GND_pin 6
#define DHT_VCC_pin 4
#define LED_pin 13
#define SERVO_pin 9
struct DHT11_datatype {
    double humidity;
    double temperature;
};
DHT11_datatype DHT11_data = {0.0, 0.0};
unsigned long start = 0; //time
int testing_count = 0; // < 4 means "testing"
bool air_conditioner_state = true;

void setup(){
    pinMode(DHT_GND_pin, OUTPUT);
    digitalWrite(DHT_GND_pin,false);
    pinMode(DHT_VCC_pin, OUTPUT);
    digitalWrite(DHT_VCC_pin,true);
    pinMode(LED_pin,OUTPUT); //設定D13為輸出
    digitalWrite(LED_pin, air_conditioner_state); //設定D13輸出低電位，True:開LED燈.
    Serial.begin(9600); //串列埠通訊鮑率9600
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    myservo.attach(SERVO_pin); // 咖啡色 = GND，紅色 = V5，黃色 = SERVO_pin
    myservo.write(max_angle);
    reset_time();
}
void loop(){
    if(testing_count < 4){
        testing_count++;
        do_myservo();
        delay(Constant_1SEC * 5);
        return;
    }else{
        unsigned long duration = 0; //time
        duration = get_duration();
        if ((duration >= max_open_duration && air_conditioner_state) || (duration >= max_close_duration && !air_conditioner_state)){
            Serial.println("over_max");
            do_myservo();
            delay(Constant_1MIN * 10 + Constant_1SEC);
            return;
        }
        if ((duration >= min_open_duration && air_conditioner_state) || (duration >= min_close_duration && !air_conditioner_state)){
            DHT11_data = get_average_DHT11_data(); // need array_len sec
            Serial.print("1 min average of humidity and temperature: \t");
            Serial.print(DHT11_data.humidity, 1);
            Serial.print("\t");
            Serial.println(DHT11_data.temperature, 1);
            if ((DHT11_data.temperature > critical_max_temperature && !air_conditioner_state) || (DHT11_data.temperature < critical_min_temperature && air_conditioner_state)){
                do_myservo();
                delay(Constant_1MIN * 10 + Constant_1SEC);
                return;
            }
        }
        delay(Constant_1MIN * 10 + Constant_1SEC);
    }
}

void reset_time(){
    start = millis();
    // millis() function itself
    // Unsigned long 32bit variable. Meaning 2^32-1 milliseconds range.
    // This equates to: (2^32-1)ms = 49.71 days.
}

unsigned long get_duration(){
    return millis() - start;
}

void do_myservo(){
    reset_time();
    myservo.write(min_angle);
    delay(Constant_1SEC);
    myservo.write(max_angle);
    air_conditioner_state = !air_conditioner_state;
    digitalWrite(LED_pin, air_conditioner_state);
}

double average(double *arr, int len){
    double sum =0.0;
    for(int i = 0; i < len; i++){
        sum += arr[i];
    }
    return sum/len;
}

DHT11_datatype get_average_DHT11_data(){
    DHT11_datatype dht_data = {0.0, 0.0};
    double humidity_array[array_len] = {0};
    double temperature_array[array_len] = {0};
    for(int i = 0; i < array_len; i++){
        check_DHT11();
        if(DHT.humidity < 10.0 || DHT.temperature < 10.0){
            i=-1;
            Serial.println();
            delay(Constant_1SEC);
            continue;
        }
        humidity_array[i] = DHT.humidity;
        temperature_array[i] = DHT.temperature;
        Serial.print("i=");
        Serial.print(i);
        Serial.print(",\t");
        Serial.print(DHT.humidity, 1); //印出小數點後1位濕度值
        Serial.print(",\t");
        Serial.println(DHT.temperature, 1); //印出小數點後1位溫度值
        delay(Constant_1SEC);
    }
    dht_data.humidity = average(humidity_array, array_len);
    dht_data.temperature = average(temperature_array, array_len);
    return dht_data;
}

void check_DHT11(){
    Serial.print("DHT11, \t");
    //檢查連線狀態
    int chk = DHT.read11(DHT11_PIN);
    switch (chk)
    {
        case DHTLIB_OK:
            Serial.print("OK            ,\t");
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Serial.print("Checksum error,\t");
            break;
        case DHTLIB_ERROR_TIMEOUT:
            Serial.print("Time out error,\t");
            break;
        case DHTLIB_ERROR_CONNECT:
            Serial.print("Connect error,\t");
            break;
        case DHTLIB_ERROR_ACK_L:
            Serial.print("Ack Low error,\t");
            break;
        case DHTLIB_ERROR_ACK_H:
            Serial.print("Ack High error,\t");
            break;
        default:
            Serial.print("Unknown error,\t");
            break;
    }
}

/*
  DHT11 溫濕度感測 與 電器控制
  Youtube channel:https://www.youtube.com/user/m0923678421
  Video:https://youtu.be/EOFC16Zlkvs
  Blog:http://blog.xuite.net/m0923678421/development
  Article:http://blog.xuite.net/m0923678421/development/540376849
*/
