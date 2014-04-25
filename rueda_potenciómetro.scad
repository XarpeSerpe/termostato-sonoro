alto = 10;
radio_exterior = 15;
radio_interior = 4;
pi = 3.1415926;
perimetro = 2*pi*radio_exterior;
giro = 300; // grados de giro del potenciometro
dientes = 30; // NÃºmero de dientes
angulo_creciente = 10; // grados de la cuerda de cada diente
cima=radio_exterior/sen(angulo_creciente)*cos(angulo_creciente);

//rueda
//difference()
//{
//	cylinder(h=alto, r= radio_exterior);
//	cylinder(h=alto*1.1, r=radio_interior);
//}



polyhedron(
points = [ [0,0,0],
[radio_exterior*cos(giro/dientes/2),0,cima],
[radio_exterior*cos(giro/dientes/2), radio_exterior*sin(giro/dientes/2),0],
[radio_exterior*cos(giro/dientes/2),-radio_exterior*sin(giro/dientes/2),0] ],
faces = [ [0,1,2],[1,3,2],[0,3,1],[0,2,3] ]
);
