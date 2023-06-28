#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

#define LED_REGISTRADA 2
#define LED_NO_REGISTRADA 3

char tarjetaRegistrada[] = "2a657129";

Servo servoMotor;
int servoPin = 4;  // Cambia el número de pin según la configuración de tu servo

void setup() {
  Serial.begin(9600);
  pinMode(LED_REGISTRADA, OUTPUT);
  pinMode(LED_NO_REGISTRADA, OUTPUT);
  digitalWrite(LED_REGISTRADA, LOW);
  digitalWrite(LED_NO_REGISTRADA, LOW);
  SPI.begin();
  mfrc522.PCD_Init();

  servoMotor.attach(servoPin);  // Inicializa el servo en el pin correspondiente
  servoMotor.write(0);          // Gira el servo a 0 grados al inicio
  delay(500);                   // Espera 0.5 segundos para que el servo llegue a la posición inicial

  Serial.println("Aproxima tu tarjeta RFID...");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tarjetaID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      tarjetaID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      tarjetaID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println("Tarjeta RFID detectada: " + tarjetaID);

    if (tarjetaID == tarjetaRegistrada) {
      Serial.println("Tarjeta registrada.");
      for (int i = 0; i < 10; i++) {
        digitalWrite(LED_REGISTRADA, HIGH);
        delay(500);  // Enciende el LED durante 0.5 segundos
        servoMotor.write(180);
        digitalWrite(LED_REGISTRADA, LOW);
        delay(500);  // Apaga el LED durante 0.5 segundo
        servoMotor.write(0);

      }


    } else {
      for (int i = 0; i < 30; i++) {
        digitalWrite(LED_NO_REGISTRADA, HIGH);
        delay(40);  // Enciende el LED durante 0.5 segundos
        digitalWrite(LED_NO_REGISTRADA, LOW);
        delay(40);  // Apaga el LED durante 0.5 segundos
      }
      Serial.println("Tarjeta no registrada.");
    }

    mfrc522.PICC_HaltA();
  }
}
