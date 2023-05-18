#include <ArduinoBLE.h>
#include <Servo.h>

Servo servo1, servo2, servo3, servo4;  // crea el objeto servo

BLEService ledService("FFE0"); // Servicio
BLEByteCharacteristic switchCharacteristic("FFE1", BLERead | BLEWrite | BLENotify);

const int BLE_LED_PIN = LED_BUILTIN;

void setup() {
  Serial.begin(9600);

  servo1.attach(2);  // vincula el servo al pin digital 2
  servo2.attach(3);  // vincula el servo al pin digital 3
  servo3.attach(4);  // vincula el servo al pin digital 4
  servo4.attach(5);  // vincula el servo al pin digital 5

  digitalWrite( BLE_LED_PIN, LOW );

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Failed");
    while (1);
  }

  //************************************************************************
  //************************************************************************
  
  BLE.setLocalName("BrazoRobotico");
  
  //************************************************************************
  //************************************************************************
  
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();

  Serial.println("BLE Periférico");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    int motorseleccionado = 0;
    int angulomovimiento = 0;
    while (central.connected()) {
      digitalWrite( BLE_LED_PIN, HIGH );
      
      byte perValue = (byte)0x00;    
      switchCharacteristic.readValue(perValue);

      // Datos recibidos mayores de 180, se resta y da el servo a mover
      // Datos recibidos por debajo o igual a 180, son los grados a mover
      if (perValue > 180) {
        motorseleccionado = perValue - 180;
      } else {
        angulomovimiento = perValue;
        if (motorseleccionado > 0) {

          switch (motorseleccionado) {
            case 1:
              servo1.write(angulomovimiento);
              break;
            case 2:
              servo2.write(angulomovimiento);
              break;
            case 3:
              servo3.write(angulomovimiento);
              break;
            case 4:
              servo4.write(angulomovimiento);
              break;
          }
          motorseleccionado = 0;
        }
      }    
      delay(500);
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite( BLE_LED_PIN, LOW );
  }  
}
