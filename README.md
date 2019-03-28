# M48T08_Clock
Reloj con RTC M48T08 y ATMEGA328

Reloj diseñado en torno a un display 5-LT-02/02m, un RTC paralelo M48T08 y un ATMEGA328.

Procedimiento para setear la hora:

1) completar los registros con la fecha actual y la hora con algunos minutos a futuro

byte registros[8][8]={{0,0,0,1,1,0,0,1},//año 
                      {0,0,0,0,0,0,1,1},//mes
                      {0,0,1,0,0,0,1,1},//dia 
                      {0,0,0,0,0,1,1,0},//dia semana 
                      {0,0,0,1,0,0,0,1},//horas
                      {0,0,1,1,0,1,0,1},//minutos 
                      {1,0,0,0,0,0,0,0},//segundos
                      {0,0,0,0,0,0,0,0}};//cal
                      
Según hoja de datos el bit más significativo del registro segundos debe ser puesto en 1 primero y en 0 después para asegurar que el RTC comience a correr. Así que se puede cargar eso así sin configurar la hora, y después pasarlo a 0.

2) cargar código
3) Esperar a que sea la hora seteada en los registros y resetear el micro

Nota: No es nada elegante, podría hacerse mediante un menú por serie, o lo que sea, pero funciona y es rápido. Ver pines de conexión en el código. Lo hice con un Mega por disponibilidad de pines, pero podría hacerse también con un Nano
