//Library yang digunakan
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

/*** VARIABEL ***/
//ESP8266
#define DEBUG true
SoftwareSerial ser(6, 7); // RX, TX //Kanan atas ke 6, kiri bawah ke 7

//RFID Reader
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
byte nuidPICC[3]; // Init array that will store new NUID/
int sucessRead;
char setID[50];

//Sistem Tilang
int sensorPin = A0; // select the input pin for ldr
int sensorValue = 0; // variable to store the value coming from the sensor
int redLED = 3;  //LED Merah
int yellowLED = 4;  //LED Kuning
int greenLED = 5; //LED Hijau
int Saldo = 10000; //Saldo Pengendara
String thisString;
void setup() {
  // put your setup code here, to run once:
  delay(5000);               
  Serial.begin(115200);
  ser.begin(115200);
  
  //RFID Setup
  SPI.begin();        // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 card
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  Serial.println(F("RFID Ready!"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
    
  //Sistem Tilang Setup
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  
  //ESP8266 Setup
  connectWifi();
  Serial.print("TCP/UDP Connection...\n");
  sendCommand("AT+CIPMUX=0\r\n",2000,DEBUG); // reset module
  delay(5000);
}

void loop() {
 
 digitalWrite(redLED, HIGH);   // Lampu merah menyala
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
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
  digitalWrite(redLED, LOW);   // Lampu kuning menyala
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, LOW);
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
  digitalWrite(redLED, LOW);   // Lampu hijau menyala
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH);
  delay(4000);
  
  
//  do {
//  Serial.print("Input Kartu");  
//  sucessRead = getID();
//  } while (!sucessRead);// put your main code here, to run repeatedly:

//  sucessRead = getID();
//  if (sucessRead) {
//    Serial.print("BERHASIL");
//  }
  
  Serial.print("Input Kartu");  
  sucessRead = getID();
  sprintf(setID, "%s%02x%02x%02x%02x.%s", nuidPICC[0]);
  sendDataID(setID);
  delay(5000);

  thisString = String(Saldo);
  sendDataID(thisString);
  delay(10000);
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

int getID() { 
   // Getting ready for Reading PICCs 
   if ( ! rfid.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue 
     return 0; 
   } 
   if ( ! rfid.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue 
     return 0; 
   } 
   // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC 
   // I think we should assume every PICC as they have 4 byte UID 
   // Until we support 7 byte PICCs 
   Serial.println(F("Scanned PICC's UID:")); 
   for (int i = 0; i < 4; i++) {  // 
     nuidPICC[i] = rfid.uid.uidByte[i]; 
     Serial.print(nuidPICC[i], HEX); 
   } 
   Serial.println(""); 
   rfid.PICC_HaltA(); // Stop reading
   return 1; 
}

void sendDataID(String id) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  
  cmd += "192.168.43.70";
  cmd += "\",80\r\n";
  sendCommand(cmd,1000,DEBUG);
  delay(5000);
  
  String cmd2 = "GET /IMKA/receiver.php?apples="; // Link ke skrip PHP                    
  cmd2 += id;  
  cmd2 += " HTTP/1.1\r\n";
  cmd2 += "Host: 192.168.43.70\r\n\r\n\r\n"; // Host
  String pjg="AT+CIPSEND=";
  pjg += cmd2.length();
  pjg += "\r\n";
    
  sendCommand(pjg,1000,DEBUG);
  delay(500);
  sendCommand(cmd2,1000,DEBUG);
  delay(5000);
}

String sendCommand(String command, const int timeout, boolean debug) {
  String response = "";
      
  ser.print(command); // send the read character to the esp8266
      
  long int time = millis();
      
  while( (time+timeout) > millis()) {
    while(ser.available()) {
      // The esp has data so display its output to the serial window 
      char c = ser.read(); // read the next character.
      response+=c;
    }  
  }
      
  if(debug) {
    Serial.print(response);
  }
      
  return response;
}

void connectWifi() {
  //Set-wifi
  Serial.print("Restart Module...\n");
  sendCommand("AT+RST\r\n",2000,DEBUG); // reset module
  delay(5000);
  Serial.print("Set wifi mode : STA...\n");
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  delay(5000);
  Serial.print("Connect to access point...\n");
  sendCommand("AT+CWJAP=\"Hura\",\"buatimka\"\r\n",3000,DEBUG);
  delay(5000);
  Serial.print("Check IP Address...\n");
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  delay(5000);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
