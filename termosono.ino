/*
Usar un Arduino Uno para controlar la temperatura de una habitacion
La media de temperatura se realiza con una termoresistencia que debe ser calibrada
La temperatura deseada se selecciona con dos botones que la subiran o bajaran a tramos de medio grado
La temperatura deseada se anuncia bien por pitidos (posteriores versione slo haran con mp3)
La subida se anuncia con tonos agudos, un pitido por cada tramo que se separe de 20 ºC
La bajada se anuncia con tonos graves, un pitido por cada tramos que se separe de 20 ºC
La temperatura por defecto es 20.0 ºC

Hay que eliminar los "rebotes" de las interrupciones, bien por hardware bien por software.
Por software es haciendo que no se active la interrupcion si no es superior a un tiempo dado
*/
//pins interrupt pins 2, 3
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
 * Sketch assumes you have MP3 files with filenames like "track001 .mp3"  ,
 * "track002 .mp3"  , etc on an SD card loaded into the shield.
 */
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif
SdFat sd;
SFEMP3Shield MP3player;

const int sensor_out   = 17;// A3 se enciende para poder medir
const int sensor_input   = A0;// lee el la caida de voltaje que sale de 4
const int rele      = 19;// A5 antes era el 8
const int sube      = 4;//Derisensor_outvacion de la interrupcion para subida de temperatura
const int baja      = 0;//Derivacion de la interrupcion para bajada de temperatura

int i; // para el contador dentro de las interrupciones
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
  
  
  #if (0)
    // Typically not used by most shields, hence commented out.
    Serial.println(F("Applying ADMixer patch."));
    if(result = MP3player.ADMixerLoad("admxster.053") == 0)
    {
      Serial.println(F("Setting ADMixer Volume."));
      MP3player.ADMixerVol(-3);
    }
  #endif
        
  pinMode(sensor_out, OUTPUT);
  pinMode(sensor_input, INPUT);
  pinMode(rele,    OUTPUT);
 
  digitalWrite(3, HIGH); //pull-up Esto elimina los rebotes?
  digitalWrite(sensor_out, LOW);
  digitalWrite(rele, LOW);
  
  //attachInterrupt(0, aumentar_temp, RISING); // no la puedo usar pues la ocupa la shield mp3 para DREQ
  attachInterrupt(1, cambiar_temp, LOW); // uso una sola interrupcion para subida o bajada, esto implica usar finales de carrera.
}

void medir() // Compara temperatura actual con la temperatura deseada. Posiblemente se pueda mejorar para no repetir estados.
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

void cambiar_temp() //interrupcion
{
  uint8_t result;
  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.
  if(digitalRead(sube) && !digitalRead(baja))
  {
    temp_obj = temp_obj + 5; // esta sin mapear
    for(i=0; i < 2000; i++);
    if (temp_obj > 255) temp_obj = 250;

    switch(temp_obj)
    {
      case 150:
        result = MP3player.playMP3("150.mp3");  
      break;
      case 155:
        result = MP3player.playMP3("155.mp3" );  
      break;
      case 160:
        result = MP3player.playMP3("160.mp3" );  
      break;
      case 165:
        result = MP3player.playMP3("165.mp3" );  
      break;
      case 170:
        result = MP3player.playMP3("170.mp3" );  
      break;
      case 175:
        result = MP3player.playMP3("175.mp3" );  
      break;
      case 180:
        result = MP3player.playMP3("180.mp3" );  
      break;
      case 185:
        result = MP3player.playMP3("185.mp3" );  
      break;
      case 190:
        result = MP3player.playMP3("190.mp3" );  
      break;
      case 195:
        result = MP3player.playMP3("195.mp3" );  
      break;
      case 200:
        result = MP3player.playMP3("200.mp3" );  
      break;
      case 205:
        result = MP3player.playMP3("205.mp3" );  
      break;
      case 210:
        result = MP3player.playMP3("210.mp3" );  
      break;
      case 215:
        result = MP3player.playMP3("215.mp3" );  
      break;
      case 220:
        result = MP3player.playMP3("220.mp3" );  
      break;
      case 225:
        result = MP3player.playMP3("225.mp3" );  
      break;
      case 230:
        result = MP3player.playMP3("230.mp3" );  
      break;
      case 235:
        result = MP3player.playMP3("235.mp3" );  
      break;
      case 240:
        result = MP3player.playMP3("240.mp3" );  
      break;
      case 245:
        result = MP3player.playMP3("245.mp3" );  
      break;
      case 250:
        result = MP3player.playMP3("250.mp3" );  
      break;
    }
  }
  else if(!digitalRead(sube) && digitalRead(baja))
  {
    temp_obj = temp_obj - 5; //esta sin mapear
    for(i=0 ; i < 2000; i++);
    if (temp_obj < 150) temp_obj = 150;
    switch(temp_obj)
    {
      case 150:
        result = MP3player.playMP3("150.mp3" );  
      break;
      case 155:
        result = MP3player.playMP3("155.mp3" );  
      break;
      case 160:
        result = MP3player.playMP3("160.mp3" );  
      break;
      case 165:
        result = MP3player.playMP3("165.mp3" );  
      break;
      case 170:
        result = MP3player.playMP3("170.mp3" );  
      break;
      case 175:
        result = MP3player.playMP3("175.mp3" );  
      break;
      case 180:
        result = MP3player.playMP3("180.mp3" );  
      break;
      case 185:
        result = MP3player.playMP3("185.mp3" );  
      break;
      case 190:
        result = MP3player.playMP3("190.mp3" );  
      break;
      case 195:
        result = MP3player.playMP3("195.mp3" );  
      break;
      case 200:
        result = MP3player.playMP3("200.mp3" );  
      break;
      case 205:
        result = MP3player.playMP3("205.mp3" );  
      break;
      case 210:
        result = MP3player.playMP3("210.mp3" );  
      break;
      case 215:
        result = MP3player.playMP3("215.mp3" );  
      break;
      case 220:
        result = MP3player.playMP3("220.mp3" );  
      break;
      case 225:
        result = MP3player.playMP3("225.mp3" );  
      break;
      case 230:
        result = MP3player.playMP3("230.mp3" );  
      break;
      case 235:
        result = MP3player.playMP3("235.mp3" );  
      break;
      case 240:
        result = MP3player.playMP3("240.mp3" );  
      break;
      case 245:
        result = MP3player.playMP3("245.mp3" );  
      break;
      case 250:
        result = MP3player.playMP3("250.mp3" );  
      break;
    }
  }
  else
  {
        Serial.println("Error en las interrupciones");
  }
}
  
void loop()
{
  medir();
  delay(1000);
}
  

