#include <dht.h>
#include <Servo.h>
dht DHT;
Servo myservo;
#define DHT11_PIN 5
#define Constant_1SEC 1000
#define Constant_1MIN Constant_1SEC * 60
#define Constant_1HR Constant_1MIN * 60
#define max_open_duration Constant_1HR
#define min_open_duration Constant_1MIN * 30
#define max_close_duration Constant_1HR * 24
#define min_close_duration Constant_1MIN * 30
#define critical_max_temperature 28.00
#define critical_min_temperature 24.00
#define array_len 60
#define min_angle 0
#define max_angle 180
// double humidity = 0.0; //存放讀取到的濕度變數
// double temperature = 0.0; //存放讀取到的溫度變數
struct DHT11_datatype {
    double humidity;
    double temperature;
};
DHT11_datatype DHT11_data = {0.0, 0.0};
unsigned long start = 0; //time
void setup(){
    pinMode(6, OUTPUT); //設定D6為輸出
    digitalWrite(6,false); //設定D6為HDT11模組的GND
    pinMode(4, OUTPUT); //設定D4為輸入
    digitalWrite(4,true); //設定D6為HDT11模組的VCC
    pinMode(13,OUTPUT); //設定D13為輸出
    digitalWrite(13,false); //設定D13輸出低電位，關閉LED燈
    Serial.begin(9600); //串列埠通訊鮑率9600
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    // Serial.println();
    // Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
    myservo.attach(9);
    reset_time();

    noInterrupts (); //reset mills()
    timer0_millis = 0; //reset mills()
    interrupts (); //reset mills()
}

void loop(){
    unsigned long duration = get_duration(); //time
    if ((duration >= max_open_duration) || (duration >= max_close_duration)){
        do_myservo();
        return;
    }
    if ((duration >= min_open_duration) || (duration >= min_close_duration)){
        DHT11_data = get_average_DHT11_data();
        Serial.print("1 min average of humidity and temperature: \t%d\t%d", DHT11_data.humidity, DHT11_data.temperature);
        if ((DHT11_data.temperature > critical_max_temperature) || (DHT11_data.temperature < critical_min_temperature)){
            do_myservo();
            return;
        }
    }
    delay(Constant_1MIN * 10 + Constant_1SEC);
}

void reset_time(){
    start = millis();
    Millis() function itself
    // Unsigned long 32bit variable. Meaning 2^32-1 milliseconds range (no negative numbers possible).
    // This equates to: (2^32-1) / 1000ms / 60sec / 60min / 24hr = 49.71 days.
}

unsigned long get_duration(){
    return millis() - start;
}

void do_myservo(){
    reset_time();
    myservo.write(max_angle);
    delay(Constant_1SEC);
    myservo.write(min_angle);
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
        humidity_array[i] = DHT.humidity;
        temperature_array[i] = DHT.temperature;
        // print_DHT11();
        delay(Constant_1SEC);
    }
    dht_data.humidity = average(humidity_array, array_len);
    dht_data.temperature = average(temperature_array, array_len);
    return dht_data;
}

void print_DHT11(){
    Serial.print(humidity, 1); //印出小數點後1位濕度值
    Serial.print(",\t");
    Serial.println(temperature, 1); //印出小數點後1位溫度值
}

void check_DHT11(){
    Serial.print("DHT11, \t");
    //檢查連線狀態
    int chk = DHT.read11(DHT11_PIN);
    switch (chk)
    {
        case DHTLIB_OK:
            Serial.print("OK,\t");
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