const int Trigger1 = 10;   //Pin digital 2 para el Trigger del sensor
const int Echo1 = 5;   //Pin digital 3 para el Echo del sensor
const int led1 = 2;
const int Trigger2 = 11;   
const int Echo2 = 6;
const int led2 = 3;
const int Trigger3 = 13; //se invirtio Trigger3 con Trigger4 por un error en la soldadura
const int Echo3 = 7; 
const int led3 = 4;
const int Trigger4 = 12;   
const int Echo4 = 8;
const int led4 = 9;
//const int buzzer = 2; variable en desuso 
const int leds[4] = {led1,led2,led3,led4};
const int DistanciaDeSensado = 3000;
long t=1000;// sonido estandar
long t1; //tiempo que demora en llegar el eco 
long t2;
long t3;
long t4;
int tiempoTono = 300;
int tono1=450;
int tono2=1400;
int tono3=2300;
int tono4=3500;
int tonos[4] = {tono1,tono2,tono3,tono4};
int RandomNum;
int MaxNivel=9;
int Dificultad[30];
int contador=0;
int fin=0;
int opcion=0;

void setup() {
  //Serial.begin(9600);//iniciailzamos la comunicación (comunicacion cerrada debido a la necesidad del uso del pin 0
  pinMode(Trigger1, OUTPUT); //pin como salida
  pinMode(Trigger2, OUTPUT);
  pinMode(Trigger3, OUTPUT);
  pinMode(Trigger4, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Echo2, INPUT);
  pinMode(Echo3, INPUT);
  pinMode(Echo4, INPUT);//pin como entrada
  digitalWrite(Trigger1, LOW);
  digitalWrite(Trigger2, LOW);
  digitalWrite(Trigger3, LOW);
  digitalWrite(Trigger4, LOW);//Inicializamos el pin con 0
  Sensores();
  Inicializador();
  randomSeed(analogRead(0)); //iniciamos la semilla con la lectura del pin analogico para que el random sea correcto
}

//comienza la ejecucion loopeada
void loop()
{

     Winner(MaxNivel, contador); //comprueba si el jugador ya gano
     Serial.println("inicio del juego");
     Serial.print("Nivel ");
     Serial.println(contador);
     Dificultad[contador]=random(1, 5); // elige un numero random de 1 a 4 (incluyentes)
     Serial.print("Random " );
     Serial.println(Dificultad[contador]);
     delay(1000);
     for(int i=0;i<=contador;i++) //reproduce la secuencia de principio a fin
     {
       switch(Dificultad[i])
       {
        case 1: Sonar(t, tono1, led1);
                break;
        case 2: Sonar(t, tono2, led2);
                break;              
        case 3: Sonar(t, tono3, led3);
                break;
        case 4: Sonar(t, tono4, led4);
                break;                    
       }
       delay(tiempoTono);      
     }
     contador++;
     fin=0;
     opcion=Dificultad[contador-1]; //solo para Debug luego eliminar
     Serial.print("Numero a elegir "); //solo para Debug luego eliminar
     Serial.println(opcion);
   do
   {
     //opcion = 0;
     opcion = Sensores(); //toma la opcion elegida por el usuario
     
     if (opcion!=0) //la opcion 0 es == null
     {
       if( opcion == Dificultad[fin]) // comprueba si la opcion elegida es igual al numero random alojado en el array en la posicion "fin"
       {
         Serial.print("Eleccion correcta ");
         Serial.println(opcion);
         //tone(2, 4000, 50);
         fin++;
         opcion=Dificultad[fin-1]; //solo para Debug luego eliminar
         Serial.print("Numero a elegir "); //solo para Debug luego eliminar
         Serial.println(opcion);
         delay(300);
       }
       else{
        {
         GameOver();
         break;      
        }
       }      
     }

   }while(fin<contador);
   delay(1000);
}



/*inicio 
 *de 
 *las 
 *funciones 
 *y 
 *procedimientos*/

void Inicializador()//inicia el juego
{
  contador=0;
  memset(Dificultad, 0, sizeof(Dificultad)); //reset del array
  tiempoTono = 300;
  Sonar(t, tono1, led1);
  Sonar(t, tono2, led2);
  Sonar(t, tono3, led3);
  Sonar(t, tono4, led4);
  switch (ElegirDificultad())
  {
    case 1:
          MaxNivel = 9;
          tiempoTono = 300;
          break;
    case 2:
          MaxNivel = 14;
          tiempoTono = 250;
          break;
    case 3:
          MaxNivel = 19;
          tiempoTono = 200;
          break;
     case 4:
          MaxNivel = 29;
          tiempoTono = 100;
          break;                             
  }
  delay(200);
  for(int i=0;i<4;i++)
  {
    Sonar(t,tonos[i],leds[i]);
    delay(tiempoTono);
  }
  delay(1500);  
}

int ElegirDificultad()
{
  int dif;
  do{
    dif = Sensores();
  }while(dif==0);
  
  return dif;
}


int Sensores()
{
  t1 = Leer(Trigger1,Echo1); //lee lo obtenido por el sensor de ultrasonido 1
  t2 = Leer(Trigger2,Echo2);
  t3 = Leer(Trigger3,Echo3);
  t4 = Leer(Trigger4,Echo4);
  
  if(Sonar(t1, tono1, led1)) // suena y marca la opcion elegida
  {
    return 1;
  }
  if(Sonar(t2, tono2, led2))
  {
    return 2;
  }
  if(Sonar(t3, tono3, led3))
  {
    return 3;
  }
  if(Sonar(t4, tono4, led4))
  {
    return 4;
  }
  
  return 0;
}

long Leer(int Trigger, int Echo)
{
  long t;
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  return t;
}

bool Sonar(long t,int tono, int led)
{
  if(t<DistanciaDeSensado && t>0)
  {
    tone(led, tono, tiempoTono);
    digitalWrite(led , HIGH);   
    delay(tiempoTono);
    digitalWrite(led , LOW);
    return true;
  }
  else
  {
    return false;
  }  
}

void Winner(int MaxNivel, int contador)
{
    int pos=0;
      if(contador>MaxNivel)
     {
         for(int i=10;i<3500;i=i+250)
         {
          tone(leds[pos], i, 200);
          delay(200);
          pos++;
          if(pos>3)
          {
            pos=0;
          }
         }
         Serial.println("Winner");
         Inicializador();
     }
}

void GameOver()
{
    Serial.print("Eleccion Incorrecta "); //solo para debug
    Serial.println(opcion); //solo para debug
    
    digitalWrite(led1 , HIGH);   
    digitalWrite(led2 , HIGH); 
    digitalWrite(led3 , HIGH); 
    digitalWrite(led4 , HIGH); 
    tone(led1, 300, 1500);
    tone(led2, 300, 1500);
    tone(led3, 300, 1500);
    tone(led4, 300, 1500);
    delay(1500);
    digitalWrite(led1 , LOW);
    digitalWrite(led2 , LOW);
    digitalWrite(led3 , LOW);
    digitalWrite(led4 , LOW);
    
    Serial.println("GAME OVER");
    delay(5000);
    Inicializador();
}
