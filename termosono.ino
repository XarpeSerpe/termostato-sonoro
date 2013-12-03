/*
Se usa un Arduino Uno R3 para controlar la temperatura de una habitacion.
Se usa una VS1053 MP3 Shield de Geeetech para informar verbalmente de la temperatura seleccionada.
La media de temperatura se realiza con un divisor de tension hecho una termoresistencia y una resistencia.
La termoresistencia debe ser calibrada.
La temperatura por defecto es 20.0 ºC.
La temperatura deseada se selecciona con dos sensores capacitivos que la subiran o bajaran a tramos de medio grado.
La temperatura deseada se anuncia con la reproduccion del fichero mp3 adecuado.
Evita conexiones y desconexiones rapidas del rele espaciandolas 30 segundos.
Para el acceso al fichero mp3 se usa la libreria  SdFat y SdFatUtil desarrolladas por XXX.
Para reproducir el fichero mp3 se usa la libreria SFEMP3Shield desarrollada por Bill Porter y Michael P. Flaga.
 */

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
/* Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif*/
#include <CapacitiveSensor.h>

//Inicializa la tarjeta SD
SdFat sd;
//Inicializa la Shield MP3
SFEMP3Shield MP3player;

CapacitiveSensor sube = CapacitiveSensor(2,4);
CapacitiveSensor baja = CapacitiveSensor(2,0);

const int sensor = A0;// lee el la caida de voltaje en la termoresistencia.
const int rele   = 19;// A5 control del rele que activa o desactiva la caldera.
/*
Cambiar por interrupciones.
En la Shield MP3 el pin 2 controla el DREG, la derivo al pin 0 de Arduino.
Esto precisa cambiar el uso del pin 2 de Arduino al pin 0 de Arduino bien en la libreria SDFAT bien en la de MP3, estudiar esto
const int sensor_subir = 3;//Derivacion de la interrupcion para subida de temperatura.
const int sensor_bajar = 2;//Derivacion de la interrupcion para bajada de temperatura.

*/
int temp_v; //la "temperatura" medida por el lm35 en el A0
float temp; //temperatura actual
volatile int temp_obj = 21; //temperatura objetivo aun sin mapear
float orquilla = 5; //temperatura objetivo aun sin mapear

void setup()
{
  Serial.begin(115200);
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
  //Initialize the MP3 Player Shield
  
  pinMode(sensor, INPUT);
  pinMode(rele,   OUTPUT);
  
  digitalWrite(rele, LOW);
  
  MP3player.begin();
  suena("inicio.mp3");
  delay(2000); // Hay que dar tiempo a termine la reproduccion.
  
}


void loop()
{
  medir_temp();  
  sensor_capacitivo();  
}

void medir_temp() // Compara temperatura actual con la temperatura deseada. Posiblemente se pueda mejorar para no repetir estados.
{// esta funcion lee el voltaje, no la intensidad. Por esto se precisa que la termistor este en un divisor de corriente
  
  temp_v = analogRead(sensor);
  temp = 20.0 / 500.0 * temp_v; //recalibrar esta transformacion, usar regresion lineal
  Serial.print(temp);
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
  
