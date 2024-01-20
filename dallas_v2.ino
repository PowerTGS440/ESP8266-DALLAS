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
// Dallas DS18B20 demo file v2 (dual sensor)
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
// WYKORZYSTANA PAMIĘĆ RAM i FLASH z włączonym SERIAL MONITOR
// 
// Variables and constants in RAM (global, static), used 28260 / 80192 bytes (35%) 
// Code in flash (default, ICACHE_FLASH_ATTR), used 246476 / 1048576 bytes (23%)   
//
// WYKORZYSTANA PAMIĘĆ RAM i FLASH z wyłączonym SERIAL MONITOR
//
// Variables and constants in RAM (global, static), used 28176 / 80192 bytes (35%)
// Code in flash (default, ICACHE_FLASH_ATTR), used 235540 / 1048576 bytes (22%)
//
// Jak widać, pamięć RAM jest zoptymalizowana pod SERIAL MONITOR, ale największą różnicę
// widać w pamięci FLASH, przestawienie SERIAL na FALSE daje nam 10936 bajtów (10KBajta)
// oszczędności pamięci. W przypadku małych projektów nie przejmujemy się tymi wartościami
// ale gdy tworzymy coś wielkiego warto korzystać z MAKR i wyłączać funkcje które są
// nam potem nie potrzebne !!! W trakcie pisania kodu potrzebujemy sprawdzić pewne zmienne
// ale sprytne tworzenie kodu (dzielenie na funkcje) i poźniejsze ich wyłączenie jednym
// logicznym warunkiem owocuje ogromnymi zyskami pamięci :-)
//
// █████████████████████████████████████████████████████
// M A K R O
// █████████████████████████████████████████████████████
                                    
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#define F(string_literal) (FPSTR(PSTR(string_literal)))

#define SERIAL        true                        // SERIAL włączony
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

int ilosc_czujnikow;                      // liczba podłączonych czujników (Dallas v2)
                                          // zmienna musi być typu integer !!!

// █████████████████████████████████████████████████████
// O B I E K T   K L A S Y   B M E 2 8 0   O R A Z   A H T 2 0
// █████████████████████████████████████████████████████

OneWire oneWire(oneWireBus);
DallasTemperature sensor(&oneWire);
DeviceAddress tempDeviceAddress; 

// █████████████████████████████████████████████████████
// D E K L A R A C J E   F U N K C J I
// █████████████████████████████████████████████████████

#if SERIAL
    void Show_DALLAS_Sensor (float &temp_C, float &temp_F, int &sensor_nr, DeviceAddress deviceAddress); 
    void Print_DALLAS_Address(DeviceAddress deviceAddress); 
#endif

// █████████████████████████████████████████████████████
// S E T U P
// █████████████████████████████████████████████████████

void setup() 
{
    #if SERIAL
        Serial.begin(115200);
        Serial.printf_P( PSTR ("\n\nInicjalizacja sensora DALLAS DS18B20") ); 
        Serial.printf_P( PSTR ("\nLokalizuje podłączone sensory...") );
    #endif

    sensor.begin();                // inicjalizacja sensora DS18B20    
    ilosc_czujnikow = sensor.getDeviceCount();     // wykryj ilosc podłączonych czujników

    #if SERIAL
        Serial.printf_P( PSTR ("\nWykryto : ") );
        Serial.print(ilosc_czujnikow, DEC);
        Serial.printf_P( PSTR (" czujniki DS18B20.") );
    #endif

    //-------------------------------------------------------------------------------//
    // Pętla FOR
    // wykonuje się tyle razy ile wykryto czujników, liczona od wartości 0 do
    // wartości "ilosc czujnikow", wartość ta po każdym kroku jest zwiększana
    // o jeden za pomocą iteracji "i++"
    //-------------------------------------------------------------------------------//

    for(int i = 0; i < ilosc_czujnikow; i++)
    {    
        if(sensor.getAddress(tempDeviceAddress, i))
        {
            #if SERIAL
                Serial.printf_P( PSTR ("\nZnaleziono sensor DALLAS nr. ") );
                Serial.print(i, DEC);
                Serial.printf_P( PSTR (" pod adresem: ") );
                Print_DALLAS_Address(tempDeviceAddress);            
            #endif
        } 
        else 
        {
            #if SERIAL
                Serial.printf_P( PSTR ("\nZnaleziono DUCHA sensor DALLAS  ") );
                Serial.print(i, DEC);
                Serial.printf_P( PSTR (" Nie można pobrać adresu, sprawdź okablowanie!") );
            #endif
        }                           // koniec warunku IF
    }                               // koniec pętli FOR    
}

// █████████████████████████████████████████████████████
// L O O P
// █████████████████████████████████████████████████████

void loop() 
{
    for(int j = 0; j < ilosc_czujnikow; j++)
    {    
        if(sensor.getAddress(tempDeviceAddress, j))
        {      
            Read_DALLAS_Sensor(dallas_sensor.temperatura_C, dallas_sensor.temperatura_F, tempDeviceAddress);

            #if SERIAL
                Show_DALLAS_Sensor(dallas_sensor.temperatura_C, dallas_sensor.temperatura_F, j, tempDeviceAddress); 
            #endif
        }
    }  
    delay(LOOP_DELAY);
}

// █████████████████████████████████████████████████████
// R E A D   D A L L A S   S E N S O R
// █████████████████████████████████████████████████████
// funkcja odczytuje temperaturę z czujnika DALLAS
// na wejsciu otrzymuje adresy struktury dallas_sensor gdzie zapisuje odczytane dane
// █████████████████████████████████████████████████████

void Read_DALLAS_Sensor (float &temp_C, float &temp_F, DeviceAddress &SensorAddress)
{
    #if SERIAL
        Serial.printf_P( PSTR ("\nRozpoczynam pobieranie wartości z czujnika DALLAS DS18B20") );
    #endif
    
    sensor.requestTemperatures();                     // wysyłamy żądanie do urządzenia
    temp_C = sensor.getTempC(SensorAddress);          // zapisujemy odczyt pod adresem dallas_sensor.temperatura_C
    temp_F = DallasTemperature::toFahrenheit(temp_C); // konwersja temperatury w Celcius do Fahrenheit
}

// █████████████████████████████████████████████████████
// S H O W  D A L L A S   S E N S O R
// █████████████████████████████████████████████████████
// funkcja wyświetla temperaturę pobraną przez czujnik DALLAS DS18B20
// na wejsciu otrzymuje adresy struktury dallas_sensor z wartościami
// kolejna lekcja, MAKRO też dotyczy funkcji, jeśli SERIAL MONITOR jest false
// jest ona pomijana i nie dołączana do kodu programu 
// █████████████████████████████████████████████████████

#if SERIAL
void Show_DALLAS_Sensor (float &temp_C, float &temp_F, int &sensor_nr, DeviceAddress deviceAddress)
{
        Serial.printf_P( PSTR ("\n[DALLAS DEVICE] ") );
        Print_DALLAS_Address(deviceAddress);
        Serial.printf_P( PSTR (" [Sensor] %d Temperatura = %.02f ºC"), sensor_nr, temp_C);  
        Serial.printf_P( PSTR ("\n[DALLAS DEVICE] ") );
        Print_DALLAS_Address(deviceAddress);       
        Serial.printf_P( PSTR (" [Sensor] %d Temperatura = %.02f ºF\n"), sensor_nr, temp_F);  
}
#endif

// █████████████████████████████████████████████████████
// P R I N T   D A L L A S   A D D R E S S
// █████████████████████████████████████████████████████
// funkcja wyświetla adresy wykrytych urządzeń DALLAS DS18B20
// na wejsciu otrzymuje adres urządzeń
// kolejna lekcja, MAKRO też dotyczy funkcji, jeśli SERIAL MONITOR jest false
// jest ona pomijana i nie dołączana do kodu programu 
// █████████████████████████████████████████████████████

#if SERIAL
void Print_DALLAS_Address(DeviceAddress deviceAddress) 
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16) Serial.printf_P ( PSTR ("0") );
        Serial.print(deviceAddress[i], HEX);
    }
}
#endif

// █████████████████████████████████████████████████████
// END OF FILE : src/sensor/dallas/dallas.ino
// █████████████████████████████████████████████████████
