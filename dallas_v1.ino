// █████████████████████████████████████████████████████
//
// ESP32/ESP8266 BlackBook by PowerTGS (2024)
//
// author : Tomasz Majcher 
// e-mail : powerTGS440@gmail.com
// phone  : +48 668 082121
//
// █████████████████████████████████████████████████████
//
// Dallas DS18B20 demo file
//
// source file      : sourcce/sensor/dallas/dallas.ino
// device           : tested on esp32/esp2866
// code platform    : ArduinoIDE
// 
// █████████████████████████████████████████████████████
// M E M O R Y   I N F O
// █████████████████████████████████████████████████████
// PROGMEM to funkcja Arduino AVR, która została przeniesiona do ESP8266, aby zapewnić 
// kompatybilność z istniejącymi bibliotekami Arduino, a także zaoszczędzić pamięć RAM. 
//
// 
// Variables and constants in RAM (global, static), used 28440 / 80192 bytes (35%) bez PROGMEM 
// Variables and constants in RAM (global, static), used 28252 / 80192 bytes (35%) z PROGMEM
//
// █████████████████████████████████████████████████████
// M A K R O
// █████████████████████████████████████████████████████
                                    
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#define F(string_literal) (FPSTR(PSTR(string_literal)))

#define SERIAL        true                         // SERIAL włączony
#define SERIAL_SPEED  115200                       // prędkość SERIAL
#define LOOP_DELAY    5000                         // opóźnienie pętli LOOP 

// █████████████████████████████████████████████████████
// B I B L I O T E K I
// █████████████████████████████████████████████████████                

#include <OneWire.h>
#include <DallasTemperature.h>

// █████████████████████████████████████████████████████
// Z M I E N N E   G L O B A L N E
// █████████████████████████████████████████████████████           

const int oneWireBus = 4;                 // GPIO PINS (ESP8266) D2    

struct DALLAS_SENSOR                      // struktura dla czujnika DALLAS
{                                         // można prościej, ale nauka nie idzie w las
    float   temperatura_C = 0;            // temperatura w st. Celcius  
    float   temperatura_F = 0;            // temperatura w st. Fanrenheit     
}   dallas_sensor;                        // tworzymy nazwę dla naszej struktury

// █████████████████████████████████████████████████████
// O B I E K T   K L A S Y   B M E 2 8 0   O R A Z   A H T 2 0
// █████████████████████████████████████████████████████

OneWire oneWire(oneWireBus);
DallasTemperature sensor(&oneWire);

// █████████████████████████████████████████████████████
// D E K L A R A C J E   F U N K C J I
// █████████████████████████████████████████████████████

void Read_DALLAS_Sensor (float &temp_C, float &temp_F); // uczymy się przysyłać adres zmiennej
void Show_DALLAS_Sensor (float &temp_C, float &temp_F); // uczymy się przesyłać adres zmiennej

// █████████████████████████████████████████████████████
// S E T U P
// █████████████████████████████████████████████████████

void setup() 
{
    #if SERIAL
        Serial.begin(115200);
        Serial.printf_P( PSTR ("\n\nInicjalizacja sensora DALLAS DS18B20") ); 
    #endif
  
    sensor.begin();                // inicjalizacja sensora DS18B20
}

// █████████████████████████████████████████████████████
// L O O P
// █████████████████████████████████████████████████████

void loop() 
{
  Read_DALLAS_Sensor(dallas_sensor.temperatura_C, dallas_sensor.temperatura_F);
  Show_DALLAS_Sensor(dallas_sensor.temperatura_C, dallas_sensor.temperatura_F); 
  
  delay(LOOP_DELAY);
}

// █████████████████████████████████████████████████████
// R E A D   D A L L A S   S E N S O R
// █████████████████████████████████████████████████████
// funkcja odczytuje temperaturę z czujnika DALLAS
// na wejsciu otrzymuje adresy struktury dallas_sensor gdzie zapisuje odczytane dane
// █████████████████████████████████████████████████████

void Read_DALLAS_Sensor (float &temp_C, float &temp_F)
{
    #if SERIAL
        Serial.printf_P( PSTR ("\n\nRozpoczynam pobieranie wartości z czujnika DALLAS DS18B20") );
    #endif
    
    sensor.requestTemperatures();              // wysyłamy żądanie do urządzenia
    temp_C = sensor.getTempCByIndex(0);        // zapisujemy odczyt pod adresem dallas_sensor.temperatura_C
    temp_F = sensor.getTempFByIndex(0);        // zapisujemy odczyt pod adresem dallas_sensor.temperatura_F
}

// █████████████████████████████████████████████████████
// S H O W  D A L L A S   S E N S O R
// █████████████████████████████████████████████████████
// funkcja wyświetla temperaturę pobraną przez czujnik DALLAS DS18B20
// na wejsciu otrzymuje adresy struktury dallas_sensor z wartościami
// █████████████████████████████████████████████████████

void Show_DALLAS_Sensor (float &temp_C, float &temp_F)
{
    #if SERIAL
        Serial.printf_P( PSTR ("\n[DALLAS_DS18B20_Sensor] Temperatura = %.02f ºC"), temp_C);  
        Serial.printf_P( PSTR ("\n[DALLAS_DS18B20_Sensor] Temperatura = %.02f ºF\n"), temp_F);  
    #endif
}

// █████████████████████████████████████████████████████
// END OF FILE : src/sensor/dallas/dallas.ino
// █████████████████████████████████████████████████████
