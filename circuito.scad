//include<ArduinoMp3Shield.scad>;

//Elementos a enlazar:
//LM35 5V, A3, GND
//BC107 Rele, A0, GND
//Rele Vin, BC107
//Potenciómetro 5V, resistencia, A5

//Nodo
module nodo()
{
cylinder(h=1, r=1.2, $fn=10);
}

//GND
color([0,0,1])union()
{
	translate([26, -49, 10]) nodo();
	translate([25.5, -49,10]) cube([1,7,1]);
	translate([11.5, -42.5,10]) cube([15,1,1]);
	translate([11.5, -42.5, 10]) cube([1,29,1]);
	translate([11.5,-14,10]) cube([8,1,1]);
	translate([20, -13.5, 10]) nodo();
	translate([13.5, 0, 10]) nodo();
	translate([13, -4, 10]) cube([1,4,1]);
	translate([13, -4, 10]) cube([4,1,1]);
	translate([17,-3.5,10]) nodo();
	translate([17,-4, 10]) cube([30,1,1]);
	translate([47,-20,10]) cube([1,17,1]);
	translate([47.5,-20,10]) nodo();
}

//5V
color([.8,0,0]) union()
{
	translate([23.5,-49,10]) nodo();
	translate([23,-49,10])cube([1,4,1]);
	translate([9,-45,10]) cube([15,1,1]);
	translate([9,-45,10]) cube([1,50,1]);
	translate([9,4,10]) cube([44,1,1]);
	translate([52,-20,10]) cube([1,25,1]);
	translate([50,-20,10]) nodo();	
	translate([17,-8.5,10]) nodo();
	translate([10,-9,10]) cube([7,1,1]);
	translate([52.5,-20,10]) nodo();
}

//Vin
	translate([31,-49,10]) nodo();
	translate([30.5, -49, 10]) cube([1,20,1]);
	translate([31, -29,10]) nodo();

	translate([25, -13.5 ,10]) nodo();
	translate([25,-14,10])  cube([6,1,1]);
	translate([30.5,-21,10]) cube([1,8,1]);
	translate([31, -21,10]) nodo();
//A0
	translate([36,-49,10]) nodo();
	translate([35.5,-49,10]) cube([1,40,1]);
	translate([22, -9, 10]) cube([14.5,1,1]);
	translate([22, -14, 10]) cube([1,5,1]);
	translate([22.5, -13.5 ,10]) nodo();


//A3
	translate([43.5,-49,10]) nodo();
	translate([43, -49, 10]) cube([1,43.5,1]);
	translate([17,-6.5,10]) cube([27,1,1]);
	translate([17,-6,10]) nodo();
	
//A5
	translate([48.5,-49,10]) nodo();
	translate([50,-20,10]) nodo();
	translate([48, -49.5, 10]) cube([2,1,1]);
	translate([49.5, -49.5, 10]) cube([1,29,1]);

//Pin3
translate([40.5,0,10]) nodo();
translate([40.5,3.5,10])color([1,0,0]) nodo();

//LM35
module LM35()
{
	for(i=[0:2])
	{
		translate([i*1.5,0,0]) cylinder(h=10, r=1/2, $fn=10);
	}
	translate([1.5,0,9])color([0,0,0]) cylinder(h=6, r=5/2);
}

//BC107
module BC107()
{
	for(i=[0:2])
	{
		translate([i*1.5,0,0]) cylinder(h=10, r=1/2, $fn=10);
	}
	translate([1.5,0,9])color([0.5,0.5,0.5]) cylinder(h=6, r=5/2);
}

//Rele
module rele()
{
	color([0,0,1]) cube([25,29,12]);
	translate([-5,2,3])rotate([0,90,0]) cylinder(h =5, r =0.5);
	translate([-5,2,9])rotate([0,90,0]) cylinder(h =5, r =0.5);
	translate([-5,23,9])rotate([0,90,0]) cylinder(h =5, r =0.5);
	translate([-5,26,3])rotate([0,90,0]) cylinder(h =5, r =0.5);
	translate([-5,20,3])rotate([0,90,0]) cylinder(h =5, r =0.5);
}

//Potenciometro
module pot()
{
	cylinder(h=12, r=24/2);
	translate([0,0,12]) cylinder(h=20, r=6/2);
}

//*********//
/*
translate([17,-7.5,10]) rotate([0,0,90]) LM35();
translate([21,-13.5,10]) BC107();
translate([33,-20,15]) rotate([0,-90,90])rele();
ArduinoMp3Shield(1,1,1);
*/
