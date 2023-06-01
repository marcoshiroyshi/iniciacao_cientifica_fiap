#include <Servo.h>

#define numOfValsRec 5     // definimos 5 pois vamos receber os valores de cada dedo
#define digitsPerValRec 1  // Quantidade de dígitos

// Definindo cada servo
Servo servoThumb;
Servo servoIndex;
Servo servoMiddle;
Servo servoRing;
Servo servoPinky;

int valsRec[numOfValsRec];                              // aqui estamos armazenando os números inteiros recebidos
int stringLength = numOfValsRec * digitsPerValRec + 1;  // $00000 tamanho da palavra
int counter = 0;                                        // contador
bool counterStart = false;                              // flag - Sempre que recebemos um "$" o contador começa
String receivedString;                                  // recebemos a string inteira para depois quebrá-la em substrings e colocar no valsRec

void setup() {
  Serial.begin(9600);
  servoThumb.attach(7);    // Polegar no pino 7
  servoIndex.attach(6);    // Indicador no pino 9
  servoMiddle.attach(5);  // Médio no pino 11
  servoRing.attach(8);     // Anelar no pino 8
  servoPinky.attach(10);   // Mindinho no pino 10
}

void receiveData() {
  while (Serial.available()) {
    char c = Serial.read();  // capturamos as Strings enviadas pelo Python

    if (c == '$') {         // verificamos se a string é "$", pois $ será o início da palavra
      counterStart = true;  // Definimos o início do contador como verdadeiro
    }
    if (counterStart) {  // Se o contador for verdadeiro
      if (counter < stringLength) {
        receivedString = String(receivedString + c);
        counter++;
      }
      if (counter >= stringLength) {
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1;
          valsRec[i] = convertValue(receivedString.substring(num, num + digitsPerValRec));
        }
        receivedString = "";
        counter = 0;
        counterStart = false;
      }
    }
  }
}

int convertValue(String valueStr) {
  int value = valueStr.toInt();
  if (value == 2) {
    return 0;    // Desligado
  } else if (value == 1) {
    return 90;   // 100%
  } else {
    return 180;  // 50%
  }
}

void loop() {
  receiveData(); // recebemos os dados do Python
  servoThumb.write(valsRec[0]);
  servoIndex.write(valsRec[1]);
  servoMiddle.write(valsRec[2]);
  servoRing.write(valsRec[3]);
  servoPinky.write(valsRec[4]);
}
