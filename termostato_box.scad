include <arduino.scad>
include <arduino_shield_mp3.scad>

largo	= 69;
ancho	= 57; 
alto		= 30;
pared	=  3;

module caja()
{
	difference()
	{
		cube([largo+pared*2, ancho+pared*2, alto]);
		translate([pared, pared, pared])
		cube([largo, ancho, alto+pared]);
	}
}

module cerebro()
{
	union()
	{
		Arduino(1,1,1);
		translate([0, 0, 12])
		ArduinoMp3Shield(1,1,1);
	}
}

difference()
{
	translate([-18,-57,-3]) caja();
	#cerebro();
}
