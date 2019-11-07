int inByte = 0;         // incoming serial byte

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   //Configurar el puerto serial a 9600 8N1
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("UPC");
  Serial.println("Ingenieria Electronica y Mecatronica");  
  Serial.println("Autor: Kalun Jose Lau Gan");  
  Serial.println("Menu, ingrese opcion:");  
  Serial.println("(1) Encender LED");  
  Serial.println("(2) Apagar LED");  
  delay(2000);
PREGUNTA:  
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (inByte == '1'){
      Serial.println("Led encendido");
      digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)      
    }
    else if (inByte == '2'){
      Serial.println("Led apagado");
      digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
    }
    else{
      Serial.println("Tecla incorrecta");
    }
  }
  goto PREGUNTA;
}
