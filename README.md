termostato-sonoro
=================

Se usa un Arduino Uno R3 para controlar la temperatura de una habitacion.
Se usa una VS1053 MP3 Shield de Geeetech para informar verbalmente de la temperatura seleccionada.
La media de temperatura se realiza con un divisor de tension hecho una termoresistencia y una resistencia.
La termoresistencia debe ser calibrada.
La temperatura por defecto es 21.0 ºC.
La temperatura deseada se selecciona con dos sensores capacitivos que la subiran o bajaran a tramos de medio grado.
La temperatura deseada se anuncia con la reproduccion del fichero mp3 adecuado.
Evita conexiones y desconexiones rapidas del rele espaciandolas 30 segundos.
Para el acceso al fichero mp3 se usa la libreria  SdFat y SdFatUtil desarrolladas por XXX.
Para reproducir el fichero mp3 se usa la libreria SFEMP3Shield desarrollada por Bill Porter y Michael P. Flaga.
