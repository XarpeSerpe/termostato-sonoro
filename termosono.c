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

const int medir_1   = 7;
const int termo_1   = A0;
const int speaker   = 11;
const int rele      = 8;

int i; // para el contador dentro de las interrupciones
float temp; //temperatura actual
volatile int filtro = 2000; //Tiempo humbral para activar una interrupcion, testa tiempo idoneo ya que el rebote depende del interruptor usado
// Ojo al uso de millis(), a los 70 dias se reinicia, hay que capturar esta excepcion
volatile int lastInt = 0;

volatile float temp_obj = 250.0; //temperatura objetivo aun sin mapear
float orquilla = 0.25;

void setup()
  {
    Serial.begin(9600);
    
    pinMode(medir_1, OUTPUT);
    pinMode(termo_1, INPUT);
    pinMode(speaker, OUTPUT);
    pinMode(rele,    OUTPUT);
    pinMode(2,       INPUT);
    pinMode(3,       INPUT);
    digitalWrite(2, HIGH); //pull-up Esto elimina los rebotes?
    digitalWrite(3, HIGH); //pull-up
    
    digitalWrite(medir_1, LOW);
    digitalWrite(speaker, LOW);
    digitalWrite(rele, LOW);
    
    attachInterrupt(0, aumentar_temp, RISING);
    attachInterrupt(1, disminuir_temp, RISING);
  }

void medir() // Compara temperatura actual con la temperatura deseada. Posiblemente se pueda mejorar para no repetir estados.
  {
    digitalWrite(medir_1, HIGH);
    temp = analogRead(termo_1); // esta funcion lee el voltaje, no la intensidad. Por esto se precisa que la termistor este en un divisor de corriente
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
  
void aumentar_temp()//interrupcion
  {
    
      temp_obj = temp_obj + 5; // esta sin mapear
      for(i=0 ; i < 16000000; i++)
      tone(speaker, 40, 1000);
      
      
  }
  
void disminuir_temp() //interrupcion
  {
    
      temp_obj = temp_obj - 5; //esta sin mapear
      for(i=0 ; i < 16000000; i++)
      tone(speaker, 2000, 1000);
     
  }
  
void loop()
  {
    medir();
    delay(1000);
  }
