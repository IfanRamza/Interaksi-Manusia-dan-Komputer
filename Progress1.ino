int sensorPin = A0; // select the input pin for ldr
int sensorValue = 0; // variable to store the value coming from the sensor
int redLED = 13;
int yellowLED = 12;
int greenLED = 11;
int Saldo = 10000;

void setup() {
  Serial.begin(9600); //sets serial port for communication
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
}
void loop() { 
  //CASE LAMPU MERAH
  digitalWrite(13, HIGH);   // Lampu merah menyala
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  Serial.print("Jumlah saldo anda saat ini : "); //Penampilan Saldo
  Serial.println(Saldo);
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  if (sensorValue > 0 && sensorValue <= 100){
    Serial.println("Ada yang melanggar");
    Saldo = Saldo - 2000;
    Serial.print("Jumlah saldo anda saat ini : ");
    Serial.println(Saldo);
  } else {
    Serial.println("AMAN");
  }
  Serial.print("Nilainya pembacaan sensor : "); //prints the values coming from the sensor on the screen
  Serial.println(sensorValue);
  Serial.print(" ");
  delay(5000);

  
  //CASE LAMPU KUNING
  digitalWrite(13, LOW);   // Lampu kuning menyala
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  Serial.print("Jumlah saldo anda saat ini : "); //Penampilan Saldo
  Serial.println(Saldo);
  sensorValue = analogRead(sensorPin); // read the value from the sensor
    if (sensorValue > 0 && sensorValue <= 100){
    Serial.println("Ada yang melanggar");
    Saldo = Saldo - 1000;
    Serial.print("Jumlah saldo anda saat ini : ");
    Serial.println(Saldo);
  } else {
    Serial.println("AMAN");
  }
  Serial.print("Nilainya pembacaan sensor : "); //prints the values coming from the sensor on the screen
  Serial.println(sensorValue);
  Serial.print(" ");
  delay(2000);

  //CASE LAMPU HIJAU
  digitalWrite(13, LOW);   // Lampu hijau menyala
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  delay(4000);

}
