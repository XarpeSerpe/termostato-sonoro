/*
Usar un Arduino Uno para controlar la temperatura de una habitacion
La media de temperatura se realiza con una termoresistencia que debe ser calibrada
La temperatura deseada se selecciona con dos botones que la subiran o bajaran a tramos de medio grado
La temperatura deseada se anuncia bien por pitidos (posteriores versione slo haran con mp3)
La subida se anuncia con tonos agudos, un pitido por cada tramo que se separe de 20 ºC
La bajada se anuncia con tonos graves, un pitido por cada tramos que se separe de 20 ºC
La temperatura por defecto es 20.0 ºC
/**
 * \file MP3Shield_Library_Demo.ino
 *
 * \brief Example sketch of using the MP3Shield Arduino driver
 * \remarks comments are implemented with Doxygen Markdown format
 *
 * \author Bill Porter
 * \author Michael P. Flaga
 *
 * This sketch listens for commands from a serial terminal (like the Serial
 * Monitor in the Arduino IDE). If it sees 1-9 it will try to play an MP3 file
 * named track00x.mp3 where x is a number from 1 to 9. For eaxmple, pressing
 * 2 will play 'track002.mp3'. A lowe case 's' will stop playing the mp3.
 * 'f' will play an MP3 by calling it by it's filename as opposed to a track
 * number.
 *
 * Sketch assumes you have MP3 files with filenames like "track001.mp3",
 * "track002.mp3", etc on an SD card loaded into the shield.
 */

#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

/* Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif*/
#include <CapacitiveSensor.h>

/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;
CapacitiveSensor sube = CapacitiveSensor(2,4);
CapacitiveSensor baja = CapacitiveSensor(2,0);

/**
 * \brief Object instancing the SFEMP3Shield library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
SFEMP3Shield MP3player;
const int sensor_out   = 17;// A3 se enciende para poder medir
const int sensor_input = A0;// lee el la caida de voltaje que sale de 4
const int rele         = 19;// A5 antes era el 8
const int sensor_subir = 4;//Derisensor_outvacion de la interrupcion para subida de temperatura
const int sensor_bajar = 0;//Derivacion de la interrupcion para bajada de temperatura
const int sensor_emite = 2;
int temp; //temperatura actual
volatile int filtro = 2000; //Tiempo humbral para activar una interrupcion, testa tiempo idoneo ya que el rebote depende del interruptor usado
// Ojo al uso de millis(), a los 70 dias se reinicia, hay que capturar esta excepcion
volatile int lastInt = 0;
volatile int temp_obj = 250.0; //temperatura objetivo aun sin mapear
float orquilla = 5; //temperatura objetivo aun sin mapear

void setup()
{
  Serial.begin(115200);
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
  //Initialize the MP3 Player Shield
  
  pinMode(sensor_out, OUTPUT);
  pinMode(sensor_input, INPUT);
  pinMode(rele,    OUTPUT);
 
  digitalWrite(3, HIGH); //pull-up Esto elimina los rebotes?
  digitalWrite(sensor_out, LOW);
  digitalWrite(rele, LOW);
  
  MP3player.begin();
  suena("inicio.mp3");
  delay(2000); // Hay que dar tiempo a termnie la reproduccion.
  
}


void loop()
{
  medir_temp();  
  sensor_capacitivo();  
}

void medir_temp() // Compara temperatura actual con la temperatura deseada. Posiblemente se pueda mejorar para no repetir estados.
{// esta funcion lee el voltaje, no la intensidad. Por esto se precisa que la termistor este en un divisor de corriente
  digitalWrite(sensor_out, HIGH);
  temp = analogRead(sensor_input); Serial.print(temp);
  Serial.print("   ");
  Serial.println(temp_obj);
  if(temp < temp_obj)
  {
    digitalWrite(rele, HIGH);
  }
  else
  {
    digitalWrite(rele, LOW);
  }
}

void suena(char trackName[])
{
  MP3player.playMP3(trackName, 0);
}

void sensor_capacitivo() //interrupcion
{
  long tiempo_sube;
  long tiempo_baja;
    
  tiempo_sube = sube.capacitiveSensor(30);
  tiempo_baja = baja.capacitiveSensor(30);
  
  if(tiempo_sube > 500 && tiempo_baja < 500) // estos valores deben ser testados
  {
    temp_obj = temp_obj + 5; // esta sin mapear
    if (temp_obj > 255) temp_obj = 250;

    switch(temp_obj)
    {
      case 150:
        suena("150.mp3");  
      break;
      case 155:
        suena("155.mp3" );  
      break;
      case 160:
        suena("160.mp3" );  
      break;
      case 165:
        suena("165.mp3" );  
      break;
      case 170:
        suena("170.mp3" );  
      break;
      case 175:
        suena("175.mp3" );  
      break;
      case 180:
        suena("180.mp3" );  
      break;
      case 185:
        suena("185.mp3" );  
      break;
      case 190:
        suena("190.mp3" );  
      break;
      case 195:
        suena("195.mp3" );  
      break;
      case 200:
        suena("200.mp3" );  
      break;
      case 205:
        suena("205.mp3" );  
      break;
      case 210:
        suena("210.mp3" );  
      break;
      case 215:
        suena("215.mp3" );  
      break;
      case 220:
        suena("220.mp3" );  
      break;
      case 225:
        suena("225.mp3" );  
      break;
      case 230:
        suena("230.mp3" );  
      break;
      case 235:
        suena("235.mp3" );  
      break;
      case 240:
        suena("240.mp3" );  
      break;
      case 245:
        suena("245.mp3" );  
      break;
      case 250:
        suena("250.mp3" );  
      break;
    }
  }
  else if(tiempo_sube < 500 && tiempo_baja > 500)
  {
    temp_obj = temp_obj - 5; //esta sin mapear
    if (temp_obj < 150) temp_obj = 150;
    switch(temp_obj)
    {
      case 150:
        suena("150.mp3" );  
      break;
      case 155:
        suena("155.mp3" );  
      break;
      case 160:
        suena("160.mp3" );  
      break;
      case 165:
        suena("165.mp3" );  
      break;
      case 170:
        suena("170.mp3" );  
      break;
      case 175:
        suena("175.mp3" );  
      break;
      case 180:
        suena("180.mp3" );  
      break;
      case 185:
        suena("185.mp3" );  
      break;
      case 190:
        suena("190.mp3" );  
      break;
      case 195:
        suena("195.mp3" );  
      break;
      case 200:
        suena("200.mp3" );  
      break;
      case 205:
        suena("205.mp3" );  
      break;
      case 210:
        suena("210.mp3" );  
      break;
      case 215:
        suena("215.mp3" );  
      break;
      case 220:
        suena("220.mp3" );  
      break;
      case 225:
        suena("225.mp3" );  
      break;
      case 230:
        suena("230.mp3" );  
      break;
      case 235:
        suena("235.mp3" );  
      break;
      case 240:
        suena("240.mp3" );  
      break;
      case 245:
        suena("245.mp3" );  
      break;
      case 250:
        suena("250.mp3" );  
      break;
    }
  }
  else if(tiempo_sube > 500 && tiempo_baja > 500)
  {
    suena("error.mp3");
  }
  else
  {
    delay(1);
  }
}
  
