#include <Keypad.h>// importa libreria Keypad
#include <ESP32Servo.h>//  importa libreria Servo
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//CONFIGURACION LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//CONFIGURACION ULTRASONICO
const int trigPin = 36; 
const int echoPin = 39;
long duration;

// CONFIGURACION TECLADO MATRICIAL
const uint8_t ROWS = 4;// define numero de filas
const uint8_t COLS = 4;// define numero de columnas
char keys[ROWS][COLS] = {  // define la distribucion de teclas 
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t colPins[COLS] = { 16, 4, 2, 15 };// pines correspondientes a las filas
uint8_t rowPins[ROWS] = { 19, 18, 5, 17 };// pines correspondientes a las columnas
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);// crea objeto con los prametros creados previamente
char pass[7]="ABCD45"; //Declaramos la variable que almacena el password
char ingresaPass[7];//Varible que alamacena la clave que se va ingresando
int indice=0;//Variable que define el numero de letra que se va ingresando

//CONFIGURACION SENSORES
const int sensorPin = 3;  // the number of the pushbutton pin
int sensorState = 0;

const int sensorPin2 = 23;  // the number of the pushbutton pin
int sensorState2 = 0;


//CONFIGURACION SERVOMOTOR
int pinServo=14;//Inicializamos la variable del pin del servo
Servo servo_1; //Instaciamos e servo

//CONFIGURACIÓN BOTÓN CLAVE
const int Interrup = 36;


int contadorP=0, contadorG=0;

void presencia1();
void presencia2();
void contra();
int distanciaVaso();


int var; 


void setup() {
  
  servo_1.attach(pinServo, 500, 2500); 
  delay(1500);

  
  pinMode(Interrup, INPUT);
  attachInterrupt(Interrup,contra,CHANGE);
  
  pinMode(sensorPin, INPUT);
  pinMode(sensorPin2, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(pinServo,OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Abarca - Peralta");
  lcd.setCursor(0,1);
  lcd.print("Control Embebido");
  delay(3000);
  lcd.clear();

}

void loop() {
lcd.setCursor(0,0);
lcd.print("Sel.Bebida:");
lcd.setCursor(0,1);
lcd.print("Tam.Vaso:");
lcd.setCursor(12,0);
lcd.print("G:");
lcd.setCursor(12,1);
lcd.print("P:");
  if(Serial.available())
  {
    lcd.clear();
    var=Serial.read();
    if(var=='a'){
    digitalWrite(pinServo,HIGH);
    servo_1.write(45); //Movemos el servo_1 a 90 grados
    lcd.setCursor(11,0);
    lcd.print("1");      
    }
    if(var=='b'){
    digitalWrite(pinServo,HIGH);
    servo_1.write(90); 
    lcd.setCursor(11,0);
    lcd.print("2");      
    }
    if(var=='c'){  
    digitalWrite(pinServo,HIGH);
    servo_1.write(135);   
    lcd.setCursor(11,0);
    lcd.print("3"); 
    }
    if(var=='d'){     
    lcd.setCursor(9,1);
    lcd.print("G");
  
    }
    if(var=='e'){  
    lcd.setCursor(9,1);
    lcd.print("P");
    }

  }

  /*sensorState=digitalRead(sensorPin);
  lcd.setCursor(3,1);
  lcd.print(sensorState);

  sensorState2=digitalRead(sensorPin2);
  lcd.setCursor(5,1);
  lcd.print(sensorState2);*/
  //contra();

}

void contra(){
  lcd.setCursor(0,0);
  lcd.print("Ingrese su clave:");
  int i=1;
  char key = keypad.getKey();
  // comprueba que se haya presionado una tecla
  if (key) {
    //Alamacenamos la letra ingresada en arreglo de letras ingresaPass
    ingresaPass[indice]=key;
    //Aumentamos la letra que se va a alamacenar
    indice++;
    i++;
    // envia a monitor serial la tecla presionada
    Serial.println(key);
    lcd.setCursor(0,i);
    lcd.print("*");

  }
  //Si ya se ingresaron 6 letras o mas
  if(indice==6){
    //Si el password coincide con la clave ingresada
    if(strcmp(pass,ingresaPass)==0){
      // Mostramos el mensaje de acceso permitido
      Serial.println("Acceso permitido");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Acceso permitido");
     // digitalWrite(pinLedVerde, HIGH);
      //Movemos el servo_1 a 0 grados
      servo_1.write(179);
      //Esperamos 1.5 segundos
      delay(2500);
      //Movemos el servo_1 a 179 grados
      servo_1.write(0);
      //apagamos el led verde
      //digitalWrite(pinLedVerde, LOW);
      //Esperamos 1.5 segundos
      delay(1500);

    }
    //Si el password NO coincide con la clave ingresada
    else{
      // Mostramos el mensaje de acceso denegado
      Serial.println("Acceso denegado");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Acceso denegado");
      //digitalWrite(pinLedRojo, HIGH);
    }
    //Regresamos el indice a 0 para ingresar nuevas letras al password
    indice=0;
  }
  
  
}
