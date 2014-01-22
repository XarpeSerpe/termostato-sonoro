/*
Se usa un Arduino Uno R3 para controlar la temperatura de una habitacion.
Se usa una VS1053 MP3 Shield de Geeetech para informar verbalmente de la temperatura seleccionada.
Un LM35 mide la temperatura ambiente.
La temperatura objetivo por defecto es 5.0 ºC.
La temperatura deseada se selecciona con un potenciometro.
La temperatura deseada se anuncia con la reproduccion del fichero mp3 adecuado.
Evita conexiones y desconexiones rapidas del rele espaciandolas 30 segundos.
Para el acceso al fichero mp3 se usa la libreria  SdFat y SdFatUtil desarrolladas por William Greiman
Para reproducir el fichero mp3 se usa la libreria SFEMP3Shield desarrollada por Bill Porter y Michael P. Flaga.
 */
/*To Do
Botón de consulta de temperatura
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

//Inicializa el acceso a tarjeta SD
SdFat sd;
//Inicializa el acceso a Shield MP3
SFEMP3Shield MP3player;

const int sensor = A0;// lee el la caida de voltaje en el lm35
const int rele   = 10;// control del rele que activa o desactiva la caldera.
const int potenciometro = A3; // regulador de temperatura.
const int boton = 3; //Si el boton esta pulsado entonces informa de la temperatura actual.
/* problemas de alimentación del potenciometro, cuando cae la resistencia, cae el potencial del LM35*/

int temp_v; //la "temperatura" medida por el lm35 en el A0
int temp; //temperatura actual
int temp_obj = 5.0; // Temperatura objetivo al iniciar el sistema
int pot_old;
int pot_now;
unsigned long time;
unsigned long time_old;

void setup()
{
  	
  	Serial.begin(9600);//debug

  	//Initialize the SdCard.
  	if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
  	if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  	//Initialize the circuit.
        pinMode(boton, INPUT); //asignar entrada para la interrupcion
  	pinMode(sensor, INPUT);
  	pinMode(rele,   OUTPUT);
  	pinMode(boton, INPUT);
  	pinMode(potenciometro, INPUT);
        digitalWrite(rele, LOW);
        pot_old = read_pot(); // Toma un potencial inicial de referencia.
        time_old = millis();
    		
    //Initialize the MP3 Player Shield
  	MP3player.begin();
  	MP3player.playMP3("inicio.mp3");
  	delay(2000); // Hay que dar tiempo a termine la reproduccion.
        Serial.println("*******");//debug
        Serial.println("* 0.4 *");//debug
        Serial.println("*******");//debug
        attachInterrupt(1, info, LOW);//Boton
}

void loop()
{
	time = millis();
  	if(time-time_old >= 2000) //cambiar a 30000 tras el debug
	{
		temp = medir_temp();
		time_old = time;
                Serial.print("Temperatura ambiente "); //debug
		Serial.println(temp);//debug
	}
	temp_obj = seleccion();
}

int medir_temp() // Compara temperatura actual con la temperatura deseada. Posiblemente se pueda mejorar para no repetir estados.
{
 long sum_temp = 0.0; 
 for (int i = 0; i < 200; i++)
  { 
    temp_v = analogRead(sensor);
    temp = 100.0 * (5.0 / 1024.0 * temp_v + 5.0 / 1024.0); // ajuste de LM35 10mV/Cº
    sum_temp += temp;
  }
  temp = sum_temp/200;
  if(temp < temp_obj)
  {
    digitalWrite(rele, HIGH);
  }
  else
  {
    digitalWrite(rele, LOW);
  }
return temp;
}

int seleccion()//ahora puede cambiar sin avisar
{
  int time_wait; // tiempo de espera de cada fichero, asi cuando cae la potencia en el arduino no medimos nada
  String name;
  char fichero[10];
  pot_now = read_pot();  
  if (pot_old != pot_now)//ojo a la inducción del termostato, medir cuando se apaga e impedir que se ejecute el seleccion
  {
    do
    {
     pot_old = pot_now; 
     pot_now = read_pot();
    }
    while(pot_old != pot_now);
    
    Serial.print("Temperatura objetivo ");
    Serial.println(pot_now);
    name = String(pot_now);
    if (name == "11" || name == "13")
    { time_wait = 4000; }
    else if (name == "8" || name == "15")
    { time_wait = 3000; }
    else if (name == "22" || name == "30")
    { time_wait = 5000; }
    else
    { time_wait = 1000; }
    name += ".mp3";
    strcpy(fichero, name.c_str());
    MP3player.playMP3(fichero);// hay que añadir la interrupcion de pista.
    delay(time_wait);
   }
  return pot_old;
}

int read_pot()
{
 long sum_pot = 0;
 for(int i = 0; i < 200 ; i++)
  { 
    pot_now = map(analogRead(potenciometro), 0, 1023, 1, 30);
    sum_pot += pot_now;
  }
  
  return sum_pot/200;
} 
void info()
{
	String name;
	char fichero[10];
	temp = medir_temp();
	name = String(temp);
	name += ".mp3";
	strcpy(fichero, name.c_str());
	MP3player.playMP3(fichero);
        delay(2000);
        //debug Serial.println("Boton pulsado");
}
