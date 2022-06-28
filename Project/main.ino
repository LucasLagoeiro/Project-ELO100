// DEFINIÇÃO DOS PINOS
#define pinSENSOR_E A0
#define pinSENSOR_D A1

#define enable1 7
#define enable2 9


// Pinos do L293D
#define Entrada1 6
#define Entrada2 8
#define Entrada3 10
#define Entrada4 26

//Define os pinnos de velocidade do L293D
#define PinoVelocidade12 3
#define PinoVelocidade34 10

// Led Vermelho (Junto com LDR)
#define pinLed 0

// DEFINIÇÃO DAS VELOCIDADES
#define frente      30 // PORCENTAGEM DA VELOCIDADE USADA PARA FRENTE
#define curva       60 // PROCENTAGEM DA VELOCIDADE USADA PARA CURVA
#define paradoD     95
#define paradoE     94

//DECLARAÇÃO DE FUNÇÕES

void mover_Frente();
void curva_Esquerda();
void curva_Direita();
void ficar_Parado();
void algo_na_Frente();
void debug_Valores();

// Define a porta para acionamento do LED que sinaliza o disparo da foto
int led = 11;


// Variáveis do CD4511B (BCD-to-7 Segment Latch/Decoder/Driver):
int const cd_a = 2;
int const cd_b = 3;
int const cd_c = 4;
int const cd_d = 5;

// Variavel para Sensor Ultrassonico
int y = 0;

// Função  do Sensor Ultrassonico

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


void setup() {
  // Inicializa a comunicação serial:
  Serial.begin(9600);

  pinMode(enable1,OUTPUT);
  pinMode(enable2,OUTPUT);

  digitalWrite(enable1,HIGH);
  digitalWrite(enable2,HIGH);


  // Pinos de Entrada do Sensores da Direita e Esquerda

  pinMode(pinSENSOR_D,INPUT);
  pinMode(pinSENSOR_E,INPUT);

  
  
  //Define os pinos como saida  
  pinMode(PinoVelocidade12, OUTPUT);  
  pinMode(PinoVelocidade34, OUTPUT);  
  pinMode(Entrada1, OUTPUT);  
  pinMode(Entrada2, OUTPUT);
  pinMode(Entrada3, OUTPUT);  
  pinMode(Entrada4, OUTPUT);  
  
 
  // MANTEM OS MOTORES PARADOS
  algo_na_Frente();
  
  delay(12000); //DELAY PARA DAR TEMPO DE POSICIONAR O ROBO NA LINHA (12 segundos)
  
  // Define as portas dos LEDs como saida

  pinMode(led, OUTPUT);


  // Pino de saída para o CD4511B:
  pinMode(cd_a,OUTPUT);       // Bit menos significativo (LSB)
  pinMode(cd_b,OUTPUT);
  pinMode(cd_c,OUTPUT);
  pinMode(cd_d,OUTPUT);       // Bit mais significativo (MSB)


}


void loop() {

 Serial.println(digitalRead(pinSENSOR_D));
 Serial.println(digitalRead(pinSENSOR_E));
  
  //Defina a velocidade de rotação
  int velocidade = 500;
   
  // Defina a distancia
  y = 0.006783 * readUltrasonicDistance(13, 12);
  Serial.println(y);
  
  // Presenca detectada, dispara a camera
  if(y < 10){
    digitalWrite(led,HIGH);
    Serial.print("PERIGO! ALGO NA FRENTE DETECTADO!!");
    algo_na_Frente();

   }
  else if(y>10){

    // APAGAR O LED CASO NADA SEJA DETECTADO A FRENTE
    
    digitalWrite(led,LOW);

    // COMPARA OS VALORES LIDOS PARA VER QUAL MOVIMENTO FAZER

    if (!digitalRead(pinSENSOR_D)==0 && !digitalRead(pinSENSOR_E)==0) {
      ficar_Parado();
    } else if (digitalRead(pinSENSOR_D)==0 && !digitalRead(pinSENSOR_E)==0) {
      curva_Esquerda();
    } else if (!digitalRead(pinSENSOR_D)==0 && digitalRead(pinSENSOR_E)==0) {
      curva_Direita();
    } else if(digitalRead(pinSENSOR_D)==0 && digitalRead(pinSENSOR_E)==0){
      mover_Frente();
    }

}
}

// ESCOPO DAS FUNÇÕES DOS MOTORES

void mover_Frente()
{
  Serial.println("Andando para frente");
  digitalWrite(Entrada1, LOW);  
  digitalWrite(Entrada2, HIGH);
  digitalWrite(Entrada3, LOW);  
  digitalWrite(Entrada4, HIGH);
}
void curva_Direita()
{
  Serial.println("Curva direita");
  digitalWrite(Entrada1, LOW);  
  digitalWrite(Entrada2, HIGH);
  digitalWrite(Entrada3, LOW);  
  digitalWrite(Entrada4, LOW);
}
void curva_Esquerda()
{
  Serial.println("Curva esquerda");
  digitalWrite(Entrada1, LOW);
  digitalWrite(Entrada2, LOW);
  digitalWrite(Entrada3, LOW);  
  digitalWrite(Entrada4, HIGH);
}
void ficar_Parado()
{  
  Serial.println("Parado");
  digitalWrite(Entrada1, LOW);  
  digitalWrite(Entrada2, LOW);
  digitalWrite(Entrada3, LOW);  
  digitalWrite(Entrada4, LOW);
   for (int x=8; x>0 ; x--){
     if (y > 10){
      digitalWrite(led,LOW);
      y = 0.006783 * readUltrasonicDistance(13, 12);
      Serial.print("Contagem: ");
      Serial.println(x);
      display(x,y);
      delay(1000);
     }
   }
  mover_Frente(); // para sair da linha preta
  delay(1000);
   
 }

// Caso detecte algo na frente do carrinho
void algo_na_Frente()
{
  Serial.println("Algo na frente");
  digitalWrite(Entrada1,LOW);
  digitalWrite(Entrada2,LOW);
  digitalWrite(Entrada3,LOW);
  digitalWrite(Entrada4,LOW);
}                       

// Funções de controle do display:

void display(int numero, int distancia){
  Serial.println(y);
  if(numero == 1 && y > 10) {                   // Apresenta o número 1 (código 0001)
    digitalWrite(cd_a, HIGH);                 // Define cd_a como nível lógico alto
    digitalWrite(cd_b, LOW);                  // Define cd_b como nível lógico baixo
    digitalWrite(cd_c, LOW);                  // Define cd_c como nível lógico baixo
    digitalWrite(cd_d, LOW);                  // Define cd_d como nível lógico baixo
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
  else if(numero == 2 && y > 10) {                // Apresenta o número 2 (código 0010)
    digitalWrite(cd_a, LOW);
    digitalWrite(cd_b, HIGH);
    digitalWrite(cd_c, LOW);
    digitalWrite(cd_d, LOW);
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
  else if(numero == 3 && y > 10) {                // Apresenta o número 3 (código 0011)
    digitalWrite(cd_a, HIGH);
    digitalWrite(cd_b, HIGH);
    digitalWrite(cd_c, LOW);
    digitalWrite(cd_d, LOW);
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
   else if(numero == 4 && y > 10) {                // Apresenta o número 4 (código 0100)
    digitalWrite(cd_a, LOW);
    digitalWrite(cd_b, LOW);
    digitalWrite(cd_c, HIGH);
    digitalWrite(cd_d, LOW);
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
  else if(numero == 5 && y > 10) {                // Apresenta o número 5 (código 0101)
    digitalWrite(cd_a, HIGH);
    digitalWrite(cd_b, LOW);
    digitalWrite(cd_c, HIGH);
    digitalWrite(cd_d, LOW);
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
  else if(numero == 6 && y > 10) {                // Apresenta o número 6 (código 0110)
    digitalWrite(cd_a, LOW);
    digitalWrite(cd_b, HIGH);
    digitalWrite(cd_c, HIGH);
    digitalWrite(cd_d, LOW);
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
  else if(numero == 7 && y > 10) {                // Apresenta o número 7 (código 0111)
    digitalWrite(cd_a, HIGH);
    digitalWrite(cd_b, HIGH);
    digitalWrite(cd_c, HIGH);
    digitalWrite(cd_d, LOW);
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
  else if(numero == 8 && y > 10) {                // Apresenta o número 8 (código 1000)
    digitalWrite(cd_a, LOW);
    digitalWrite(cd_b, LOW);
    digitalWrite(cd_c, LOW);
    digitalWrite(cd_d, HIGH);
    y = 0.006783 * readUltrasonicDistance(13, 12);
 }
  else if (numero == 0 && y >10) {           // Apresenta o número 0 (código 0000)
    digitalWrite(cd_a, LOW);                   // Define cd_a como nível lógico baixo
    digitalWrite(cd_b, LOW);                   // Define cd_b como nível lógico baixo
    digitalWrite(cd_c, LOW);                   // Define cd_c como nível lógico baixo
    digitalWrite(cd_d, LOW);                   // Define cd_d como nível lógico baixo
    y = 0.006783 * readUltrasonicDistance(13, 12);
  }
}