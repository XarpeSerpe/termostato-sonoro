/*
Se usa un Arduino Uno R3 para controlar la temperatura de una habitacion.
Se usa una VS1053 MP3 Shield de Geeetech para informar verbalmente de la temperatura ambiente y de la temperatura objetivo seleccionada.
La tamperatura ambiente es medida por una termoresistencia previamente calibrada con un LM35.
La temperatura objetivo por defecto es 5.0 ºC.
La temperatura deseada se selecciona con un potenciometro.
La temperatura deseada se anuncia con la reproduccion del fichero mp3 adecuado.
Evita conexiones y desconexiones rapidas del rele espaciandolas 120 segundos.
Para el acceso al fichero mp3 se usa la libreria  SdFat y SdFatUtil desarrolladas por William Greiman
Para reproducir el fichero mp3 se usa la libreria SFEMP3Shield desarrollada por Bill Porter y Michael P. Flaga.
Se le dara corriente al sensor termico solo 3 segundos para estar estacionario, se promedia la medida.
 */

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>

//Inicializa el acceso a tarjeta SD
SdFat sd;
//Inicializa el acceso a Shield MP3
SFEMP3Shield MP3player;

const int sensor = A0;// lee el la caida de voltaje en la termoresistencia
const int power  = A5;// alimenta a la termoresistencia.
const int rele   = 10;// control del rele que activa o desactiva la caldera.
const int potenciometro = A2; // regulador de temperatura.
const int boton = 3; // si el boton esta pulsado entonces informa de la temperatura actual.

int temp_v; // el voltaje que cae en la termoresistencia
int temp; // temperatura actual
int temp_obj = 5.0; // temperatura objetivo al iniciar el sistema
int pot_old; // viejo valor del potenciometro
int pot_now; // valor actual del potenciometro.
unsigned long time;
unsigned long time_old;

void setup()
{
        attachInterrupt(1, info, LOW);//Boton	
  
  	//Initialize the SdCard.
  	if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
  	if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  	//Initialize the circuit.
       	pinMode(sensor, INPUT);
  	pinMode(rele,   OUTPUT);
        digitalWrite(rele, LOW);	
        // pinMode(boton, INPUT);
  	pinMode(potenciometro, INPUT);
        pinMode(power, OUTPUT);
        
        pot_old = read_pot(); // Toma un potencial inicial de referencia.
        time_old = millis();
    		
        //Initialize the MP3 Player Shield
  	MP3player.begin();
        //union twobyte mp3_vol;
        //mp3_vol.word = MP3player.GetVolume();
        //MP3player.SetVolume(160,160);
  	MP3player.playMP3("inicio.mp3");
  	delay(2500); // Hay que dar tiempo a termine la reproduccion.
/*
        Serial.begin(115200);// debug
        Serial.println("*******");//debug
        Serial.println("* 0.6 *");//debug
        Serial.println("*******");//debug
*/  
}

void loop()
{
	
        time = millis();
  	if(time-time_old >= 60000) //cambiar a 60000 tras el debug
	{
		temp = medir_temp();
		time_old = time;
/*
                Serial.print("Temperatura ambiente "); //debug
		Serial.println(temp);//debug
*/
	}
	temp_obj = seleccion();
}

int medir_temp() // Compara temperatura actual con la temperatura deseada.
{
 digitalWrite(power, HIGH);
 delay(200);
 long sum_temp = 0.0; 
 for (int i = 0; i < 200; i++)
  { 
    temp_v = 0.2346979*analogRead(sensor)-30.90486; // transforma termoresistencia en escala de LM35 R-squared: 0.98027
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
digitalWrite(power, LOW);
return temp;
}

int seleccion()
{
  int time_wait; // tiempo de cada fichero.
  String name;
  char fichero[10];
  pot_now = read_pot();  
  if (pot_old != pot_now)
  {
    do
    {
     pot_old = pot_now; 
     pot_now = read_pot();
    }
    while(pot_old != pot_now);
/*    
    Serial.print("Temperatura objetivo "); // debug
    Serial.println(pot_now); // debug
*/    
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
    MP3player.playMP3(fichero);
    delay(time_wait);
    medir_temp();
   }
  return pot_old;
}

int read_pot()
{
 long sum_pot = 0;
 for(int i = 0; i < 200 ; i++)
  { 
    pot_now = map(analogRead(potenciometro), 470, 1023, 1, 30);// ajustar
    sum_pot += pot_now;
  }
  
  return sum_pot/200;
} 
void info()
{
        int time_wait; 
	String name;
	char fichero[10];
	temp = medir_temp();
	name = String(temp);
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
	MP3player.playMP3(fichero);
        delay(time_wait);
        //debug Serial.println("Boton pulsado");
}
