#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const int buzzer = 9;
const int PIR = 8;
const int Lazzer = 7;

int LDR = 0;
int chk;
int val = 0;
unsigned long previoustime = 0;
unsigned long Currenttime = 0;
unsigned long seconds;


  void setup() {
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
  Serial.println("Found fingerprint sensor!");
  } else {
  Serial.println("Did not find fingerprint sensor :(");
  while (1) { delay(1); }

}

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  // Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  // Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  // Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  // Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  // Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  // Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  // Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
      Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  
  pinMode(buzzer, OUTPUT);
  pinMode(Lazzer, OUTPUT); 
  pinMode(PIR, INPUT); 

}
 
   
 


void loop() {
  getFingerprintID();
  delay(50);  
  digitalWrite(Lazzer, HIGH);
  LDR = analogRead(A0);
  Serial.println(LDR);
  int val = digitalRead(PIR);
  Serial.println(val); 
  delay(500);

}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  val = digitalRead(PIR);
  if (p==FINGERPRINT_OK){
    Serial.println("Image taken");
  }
  else if(p == FINGERPRINT_NOFINGER){      
                                             Serial.println("No finger detected");
   
       if(LDR >= 400) 
       {Currenttime = millis();
      if ((Currenttime - previoustime > 3000 ) && val == 1)
      {
          Serial.println("i m in milis");
        jump:
          while(!(p == FINGERPRINT_OK)){
          Serial.println("i am in while loop");
          tone(buzzer, 980,1000); // Send 1KHz sound signal..
          delay(500);
          tone(buzzer, 980,1000);
          break;
          }
     
          p = finger.getImage();
          p = finger.image2Tz();
          p = finger.fingerSearch();
          if (p == FINGERPRINT_OK){
           
            noTone(buzzer);   
        }
        else{
          goto jump;
        }
     }
    return p;
  }
  }
  else if(p == FINGERPRINT_PACKETRECIEVEERR){
  Serial.println("Communication error");
  return p;
  }
  else if(p == FINGERPRINT_IMAGEFAIL){
  Serial.println("Imaging error");
  return p;
  }
  else{
  Serial.println("Unknown error");
  return p;
  }
  //  switch (p) {
  //   case FINGERPRINT_OK:
  //     Serial.println("Image taken");
  //     break;
  //   case FINGERPRINT_NOFINGER:
  //     Serial.println("No finger detected");
  //     return p;
  //   case FINGERPRINT_PACKETRECIEVEERR:
  //     Serial.println("Communication error");
  //     return p;
  //   case FINGERPRINT_IMAGEFAIL:
  //     Serial.println("Imaging error");
  //     return p;
  //   default:
  //     Serial.println("Unknown error");
  //     return p;
  

  // // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
                                       //Serial.println("Found a print match!");
       tone(buzzer, 1000); // Send 1KHz sound signal...
       digitalWrite(Lazzer, LOW);
       delay(1500);        // ...for 1 sec
       noTone(buzzer);     // Stop sound...   
     } 
  //else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  //   Serial.println("Communication error");
  //   return p;
  //} 
  else if (p == FINGERPRINT_NOTFOUND ) {
  Serial.println("Did not find a match");
  return p;

   }
   // else {
  //   Serial.println("Unknown error");
  //   return p;
  // }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
  delay(2000);
}


// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  //found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
  }
