#include <FrequencyTimer2.h>

int timerRTC=100;

byte registros[8][8]={{0,0,0,1,1,0,0,1},//año 19  D7....D0
                      {0,0,0,0,0,0,1,1},//mes 2
                      {0,0,1,0,0,0,1,1},//dia 27
                      {0,0,0,0,0,1,1,0},//dia semana 4
                      {0,0,0,1,0,0,0,1},//horas 23
                      {0,0,1,1,0,1,0,1},//minutos 
                      {1,0,0,0,0,0,0,0},//segundos
                      {0,0,0,0,0,0,0,0}};//cal

int dir[3] = {10, 11, 12}; //A2 A1 A0

int datos[8] ={30,31,32,33,34,35,36,37}; //D7 D6... D0
//int datos[8] ={37,36,35,34,33,32,31,30}; //D7 D6... D0

#define G 2
#define W 3

void setup() {
//------------------------------------------PINES
for (int i = 0; i < 3; i++) {
    pinMode(dir[0], OUTPUT);
    pinMode(dir[1], OUTPUT);
    pinMode(dir[2], OUTPUT);
  }

  for (int i = 0; i < 8; i++) {
    pinMode(datos[i], OUTPUT);
    }

  pinMode(G,OUTPUT); //pin 2
  pinMode(W,OUTPUT); //pin 3

//--------------------------------------------------------------INICIO CONFIGURACIÓN
  //poner en modo de escritura
  digitalWrite(G,1);
  digitalWrite(W,1);
  //dirección
  digitalWrite(dir[0],0);//A2 10 1FF8h control
  digitalWrite(dir[1],0);//A1 11
  digitalWrite(dir[2],0);//A0 12
  //delay(1);
  digitalWrite(W,0); //habilito escritura
  
  //poner en 1 bit W de registro de control
  registros[7][0]=1; //si esto no está en uno no debería actualizar los registros del RTC. Puedo probar apagar entre medio
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[7][i]); //paso a los pines de datos el valor del registro de control
      
  delay(1);//espero un toque a que tDVWH (150ns?)
  digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],1);//10 1FFFh año
  digitalWrite(dir[1],1);//11
  digitalWrite(dir[2],1);//12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[0][i]); //año

  delay(1);//espero un toque a que tDVWH (150ns?)

    digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],1);//10 1FFEh mes
  digitalWrite(dir[1],1);//11
  digitalWrite(dir[2],0);//12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[1][i]); //año

  delay(1);//espero un toque a que tDVWH (150ns?)

    digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],1);//10 1FFDh dia
  digitalWrite(dir[1],0);//11
  digitalWrite(dir[2],1);//12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[2][i]); //año --------------------

  delay(1);//espero un toque a que tDVWH (150ns?)

    digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],1);//10 1FFCh dia semana
  digitalWrite(dir[1],0);//11
  digitalWrite(dir[2],0);//12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[3][i]); //año

  delay(1);//espero un toque a que tDVWH (150ns?)

    digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],0);//10 1FFBh horas
  digitalWrite(dir[1],1);//11
  digitalWrite(dir[2],1);//12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[4][i]); //horas

  delay(1);//espero un toque a que tDVWH (150ns?)

    digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],0);//10 1FFAh minutos
  digitalWrite(dir[1],1);//11
  digitalWrite(dir[2],0);//12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[5][i]); //minutos

  delay(1);//espero un toque a que tDVWH (150ns?)
  //pasar de nuvo a cero el bit W

    digitalWrite(W,1); //deshabilito escritura
  delay(1);//espero un toque a que tWHDH (150ns?)
  //actualizar todos los registros menos estado
  //dirección
  digitalWrite(dir[0],0);//A2 //10 1FF9h segundos
  digitalWrite(dir[1],0);//A1 //11
  digitalWrite(dir[2],1);//A0 //12

  digitalWrite(W,0); //habilito escritura
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[6][i]); //segundos

  delay(1);//espero un toque a que tDVWH (150ns?)

  //poner en 0 bit W de registro de control
  registros[7][0]=0;

 digitalWrite(W,1);
  //dirección
  digitalWrite(dir[0],0);//10 1FF8h control
  digitalWrite(dir[1],0);//11
  digitalWrite(dir[2],0);//12
  //delay(1);
  digitalWrite(W,0); //habilito escritura
  
  //poner en 1 bit W de registro de control
  
  for(int i=0;i<8;i++)
      digitalWrite(datos[i],registros[7][i]); //paso a los pines de datos el valor del registro de control
      
  delay(1);//espero un toque a que tDVWH (150ns?)
  digitalWrite(W,1); //deshabilito escritura

  
}

void loop() {
  // put your main code here, to run repeatedly:
  //poner en modo de escritura
  //poner en 1 bit W de registro de control
  //actualizar todos los registros menos estado
  //pasar de nuvo a cero el bit W
  
}



