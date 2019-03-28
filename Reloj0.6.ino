

#include <FrequencyTimer2.h>


int timerRTC=100;
int d0=0;
int d1=0;
int d2=0;
int d3=0;
byte setadd=0;
byte datolisto=0;
int reg=0;

byte registros[3][8]={{0,0,0,0,0,0,0,0},//horas
                      {0,0,0,0,0,0,0,0},//minutos
                      {0,0,0,0,0,0,0,0}};//segundos

byte horasC[24][8]={{0,0,0,0,0,0,0,0},//00 lo necesito para setear PM
                    {0,0,0,0,0,0,0,1},//01
                    {0,0,0,0,0,0,1,0},//02
                    {0,0,0,0,0,0,1,1},//03
                    {0,0,0,0,0,1,0,0},//04
                    {0,0,0,0,0,1,0,1},//05
                    {0,0,0,0,0,1,1,0},//06
                    {0,0,0,0,0,1,1,1},//07
                    {0,0,0,0,1,0,0,0},//08
                    {0,0,0,0,1,0,0,1},//09
                    {0,0,0,1,0,0,0,0},//10
                    {0,0,0,1,0,0,0,1},//11
                    {0,0,0,1,0,0,1,0},//12 lo necesito para setear PM
                    {0,0,0,1,0,0,1,1},//13
                    {0,0,0,1,0,1,0,0},//14
                    {0,0,0,1,0,1,0,1},//15
                    {0,0,0,1,0,1,1,0},//16
                    {0,0,0,1,0,1,1,1},//17
                    {0,0,0,1,1,0,0,0},//18
                    {0,0,0,1,1,0,0,1},//19
                    {0,0,1,0,0,0,0,0},//20
                    {0,0,1,0,0,0,0,1},//21
                    {0,0,1,0,0,0,1,0},//22
                    {0,0,1,0,0,0,1,1}};//23


byte PM=0;
byte minutoCero=0;
byte puntitos=0;

byte regHoras[12]={0,0,0,0,0,0,0,0,0,0,0,0}; //a la hora de comparar las horas evalúo si la hora actual es disinta a los registros para corregirla
byte simbolo[4]={0,0,0,0};

int dir[3] = {12, 11}; //A1 A0
int datos[7] ={8,7,6,5,9,10,20}; //D6 D0

#define G 13

byte col = 0;
byte leds[4][4]; //en esta actualizar el dato a mostrar en display

// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[9]= {-1, 4, 3, 2, 19, 17, 15, 16, 18};//dummy dis1 - dis4/ ID-IA

// col[xx] of leds = pin yy on led matrix
int cols[4] = {pins[1], pins[2], pins[3], pins[4]};//dis1,dis2,dis3,dis4

// row[xx] of leds = pin yy on led matrix
int rows[4] = {pins[5], pins[6], pins[7], pins[8]};//ID,IC,IB,IA

int pattern[4] = {0,0,0,0};

void setup() {
  // sets the pins as output
  for (int i = 1; i <= 8; i++) {
    pinMode(pins[i], OUTPUT);
  }

  for (int i = 0; i < 2; i++) {//este
    pinMode(dir[i], OUTPUT);
  }

  for (int i = 0; i < 7; i++) {//y este
    pinMode(datos[i], INPUT);
  }

  // set up cols and rows
  for (int i = 1; i <= 4; i++) {
    digitalWrite(cols[i - 1], LOW);
  }

  for (int i = 1; i <= 4; i++) {
    digitalWrite(rows[i - 1], LOW);
  }

  clearLeds();

  // Turn off toggling of pin 11
  FrequencyTimer2::disable();
  // Set refresh rate (interrupt timeout period)
  FrequencyTimer2::setPeriod(2000);
  // Set interrupt routine to be called
  FrequencyTimer2::setOnOverflow(display);

 
  Serial.begin(9600);

  //------------Lectura inicial para cuando reinicia. Después sólo leo si min=00
  LeerMinutos();
  LeerHoras();
  CoregirHora();
  CargarMatrizLed();
}

void loop() {
    
    if(!timerRTC)
    {
      LeerMinutos();
      if(minutoCero)
        {
        LeerHoras();
        CoregirHora();  
        }
      LeerSegundos();
      if(registros[2][7]==1)
        puntitos=1;
      else
        puntitos=0;

      //tres variables que determinan el código: PM, puntito y registros[0][3]
      
      CorreccionSimbolos(); //a 1 / b=AM/PM c=puntitos
      
        
      CargarMatrizLed();

      timerRTC=100;
    }
    
}

void LeerMinutos()
{   
  digitalWrite(G,1);
  
  //dirección
  digitalWrite(dir[0],1);//A1 
  digitalWrite(dir[1],0);//A0 
  
 
  digitalWrite(G,0);
  minutoCero=1;
  for(int i=0;i<6;i++)
      {
      registros[1][i+1]=digitalRead(datos[i]);
      if(registros[1][i+1]==1) minutoCero=0;//si encuentro un 1 en el registro de minutos quiere decir que no es cero, y minutoCero pasa a 0, sino queda en 1;
      }
      if(analogRead(datos[6])>512)
        {
        registros[1][7]=1;
        minutoCero=0; //acá también xq el último bit lo leo analógicamente
        }
      else
        registros[1][7]=0;
}

void LeerHoras()
{
   digitalWrite(G,1);
  
  //dirección
  digitalWrite(dir[0],1);//A1 1FFBh Horas
  digitalWrite(dir[1],1);//A0 
  
  
  digitalWrite(G,0);
  
  for(int i=0;i<6;i++)
      registros[0][i+1]=digitalRead(datos[i]);

      if(analogRead(datos[6])>512)
        registros[0][7]=1;
      else
        registros[0][7]=0;
}

void LeerSegundos()
{
  digitalWrite(G,1);
  
  digitalWrite(dir[0],0);//A1 10 1FF9h segundos
  digitalWrite(dir[1],1);//A0 11
  
  
  digitalWrite(G,0);
   //sólo me interesa el bit menos significativo para titilar los puntitos
  
  if(analogRead(datos[6])>512)
    registros[2][7]=1;
  else
    registros[2][7]=0;
}

void CoregirHora()
{
  //verificar si son más de las 12
  for(int j=0; j<12; j++)
    {
    regHoras[j]=1; //lo pongo en 1. Si la comparación da que son distintos lo paso a cero. El que queda en 1 es el correcto
    
    for(int i=0; i<8; i++)
      {
        if(registros[0][i]!=horasC[j+12][i]) 
        {
          regHoras[j]=0; //registros[0] hora vs 12/23
        }
      }
    }
  //setear PM
  
  PM=0;
  
  for(int i=0;i<12;i++)
    if(regHoras[i]==1) PM=1;
    
  //corregir a la hora en formato 12hs
  for(int i=1;i<12;i++) //si es 12 no lo corrijo
    if(regHoras[i]) //si encontré una a corregir regHoras[1]=13 
      for(int j=0;j<8;j++) //actualizo todos los bits del registro hora
        registros[0][j]=horasC[i][j];
}

void CorreccionSimbolos()
{
  //cargar símbolos en primer digito
  if(!registros[0][3] && !PM && !puntitos) //1010
    {
    simbolo[0]=1;
    simbolo[1]=0;
    simbolo[2]=1;
    simbolo[3]=0;
    }
   if(!registros[0][3] && !PM && puntitos) //1011
    {
    simbolo[0]=1;
    simbolo[1]=0;
    simbolo[2]=1;
    simbolo[3]=1;
    }
     if(!registros[0][3] && PM && !puntitos) //1100
    {
    simbolo[0]=1;
    simbolo[1]=1;
    simbolo[2]=0;
    simbolo[3]=0;
    }
     if(!registros[0][3] && PM && puntitos) //0100
    {
    simbolo[0]=0;
    simbolo[1]=1;
    simbolo[2]=0;
    simbolo[3]=0;
    }
     if(registros[0][3] && !PM && !puntitos) //1101
    {
    simbolo[0]=1;
    simbolo[1]=1;
    simbolo[2]=0;
    simbolo[3]=1;
    }
     if(registros[0][3] && !PM && puntitos) //0101
    {
    simbolo[0]=0;
    simbolo[1]=1;
    simbolo[2]=0;
    simbolo[3]=1;
    }
     if(registros[0][3] && PM && !puntitos) //0010
    {
    simbolo[0]=0;
    simbolo[1]=0;
    simbolo[2]=1;
    simbolo[3]=0;
    }
     if(registros[0][3] && PM && puntitos) //0000
    {
    simbolo[0]=0;
    simbolo[1]=0;
    simbolo[2]=0;
    simbolo[3]=0;
    }
}

void CargarMatrizLed()
{
  for (int j = 0; j < 4; j++) {
      leds[j][0] = simbolo[j];
      leds[j][1] = registros[0][4+j];
      leds[j][2] = registros[1][j];
      leds[j][3] = registros[1][4+j];
      }
}

void clearLeds() {
  // Clear display array
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      leds[i][j] = 0; //leds es la variable en donde están almacenados los datos a sacar al momento de la interrupción
    }
  }
}


// Interrupt routine
void display() {
  digitalWrite(cols[col], LOW);  // Turn whole previous column off
  col++;
  if (col == 4) 
    {
    col = 0;
    }
  for (int row = 0; row < 4; row++) 
    {
    if (leds[row][col] == 1) 
      {
      digitalWrite(rows[row], HIGH);  // Turn on this led
      //Serial.print("0");
      }
    else 
    {
      digitalWrite(rows[row], LOW); // Turn off this led
      //Serial.print("1");
      }
    }
  digitalWrite(cols[col], HIGH); // Turn whole column on at once (for equal lighting times)
  
  //------contadores
  
  if(timerRTC) timerRTC--;
}
