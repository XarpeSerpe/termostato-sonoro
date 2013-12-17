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

module boton()
{

translate([0,0,6]) cylinder(h = 4, r = 13/2); //pulsador
cylinder(h = 6, r= 19/2, $fn=6);
translate([0,0,-8]) cylinder( h = 8 , r1 = 12/2, r2 = 16/2);
translate([-2, 2, -26]) cube([4, 2, 18]);
translate([-2, -4, -26]) cube([4, 2, 18]);
}


// The box
difference()
{
	translate([-18,-57,-3]) caja();
	#cerebro();
}

difference()
{

translate([-18, 6, -pared]) cube([largo+pared*2 ,20+pared, alto ]);
union()
{
translate([(ancho)*2/3, 14 ,alto - 9]) rotate([0,0,90]) boton();
translate([(ancho)/3-18, 14, alto - 9]) rotate([0,0,90]) boton();

}

}
