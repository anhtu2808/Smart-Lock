#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Keypad.h>
#include <EEPROM.h>

#define RX_PIN A2  // RX of SoftwareSerial
#define TX_PIN A3  // TX of SoftwareSerial

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(2, 3);  // TX, RX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo myServo;
SoftwareSerial espSerial(RX_PIN, TX_PIN);

String userPasscode;
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 4, 5, 6, 7 };
byte colPins[COLS] = { 10, 11, 12, 13 };

const byte servoPin = 9;
const unsigned long lockTime = 7 * 1000;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setStringToRom(int startAddress, const String &data) {
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(startAddress + i, data[i]);
  }
  EEPROM.write(startAddress + data.length(), '\0');
}
void sendDataToESP(String col1, String col2, String col3, String col4) {
  String data = col1 + ";" + col2 + ";" + col3 + ";" + col4 +";";
  espSerial.println(data);
}
String getStringFromRom(int startAddress) {
  String data = "";
  char c;
  int i = 0;

  while (true) {
    c = EEPROM.read(startAddress + i);
    if (c == '\0') {
      break;
    }
    data += c;
    i++;
  }
  return data;
}

void setup() {
  lcd.init();
  lcd.backlight();
  myServo.attach(servoPin);
  myServo.write(0);
  Serial.begin(9600);
  espSerial.begin(9600);
  finger.begin(57600);
  sendDataToESP("1", "2", "2", "2");
  if (!finger.verifyPassword()) { 
    Serial.println("Did not find fingerprint sensor :(");
    lcd.print("Check sensor");
    while (true) delay(1000);
  }


  userPasscode = getStringFromRom(0);
  Serial.print("userPasscode: ");
  Serial.print(userPasscode);
}

void loop() {
  char key;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A: PASSCODE");
  lcd.setCursor(0, 1);
  lcd.print("B: FINGERPRINT");

  while (!(key = keypad.getKey()))
    ;

  if (key == 'A') handlePasscode();
  if (key == 'B') handleFingerprint();
}

void handlePasscode() {
  int countP = 0;
  do {
    lcd.clear();
    lcd.print("Pass:");
    lcd.setCursor(0, 1);
    lcd.print("Press # to enter!");
    String passcode = getNumFromKeypad();
    Serial.print(passcode);

    if (passcode == userPasscode) {
      sendDataToESP("User", "sucesss", passcode, "passcode");
      lcd.clear();
      unlock();
      break;
    } else if (passcode == "1111") {
      sendDataToESP("Admin", "sucesss", passcode, "passcode");
      adminMenu();
      break;
    } else {
      countP++;
      sendDataToESP("User", "fail", passcode, "passcode");
      showFailedAttempts(countP);
      if (countP == 5) {
        waitForRetry();
        break;
      }
    }
  } while (true);
}

void handleFingerprint() {
  int countF = 0;
  do {
    lcd.clear();
    lcd.print("Enter finger....");
    delay(3000);
    int result = verifyFingerprint();
    if (result >= 0) {
      countF = 0;
      Serial.print("Fingerprint ID matched: ");
      Serial.println(result);
      String ID =  "ID:" + String(result);
      sendDataToESP("User", "sucesss", ID , "Fingerprint");
      lcd.clear();
      lcd.print("Welcome user ");
      lcd.setCursor(0,1);
      lcd.print(ID);
      delay(1500);
      unlock();
      break;
    } else if(result==-5) {
      sendDataToESP("User", "fail", "Invalid", "Fingerprint");
      countF++;
      showFailedAttempts(countF);
      if (countF == 5) {
        waitForRetry();
        break;
      }
    }
  } while (true);
}

void adminMenu() {
  lcd.clear();
  lcd.print("A:Add   B:Delete");
  lcd.setCursor(0, 1);
  lcd.print("C:Change pass");

  char key2;
  while (!(key2 = keypad.getKey()))
    ;

  switch (key2) {
    case 'A': enrollFingerprint(); break;
    case 'B': deleteFingerprint(); break;
    case 'C': changePasscode(); break;
    default: break;
  }
}

void enrollFingerprint() {
  lcd.clear();
  lcd.print("FPID:");
  String enrollID = getNumFromKeypad();
  lcd.setCursor(0, 0);
  lcd.print("Enrolling ID: ");
  lcd.print(enrollID);
  lcd.setCursor(0, 1);
  lcd.print("Place finger.....");
  while (getFingerprintEnroll(enrollID.toInt()) != FINGERPRINT_OK) delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Add successfully!");
  delay(2000);
}

void deleteFingerprint() {
  lcd.clear();
  lcd.print("FPID:");
  String deleteID = getNumFromKeypad();
  lcd.setCursor(0, 0);
  lcd.print("Deleting ID: ");
  lcd.print(deleteID);
  finger.deleteModel(deleteID.toInt());
  lcd.clear();
  lcd.print("Deleted");
  delay(2000);
}

void changePasscode() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Press # to save");
  lcd.setCursor(0, 0);
  lcd.print("Pass:");
  String newUserPasscode = getNumFromKeypad();
  setStringToRom(0, newUserPasscode);
  userPasscode = getStringFromRom(0);
  lcd.clear();
  lcd.print("Pass Changed");
  delay(2000);
}

void showFailedAttempts(int count) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Failed time: ");
  lcd.print(count);
  lcd.setCursor(0, 1);
  lcd.print("Trial left: ");
  lcd.print(5 - count);
  delay(2000);
}

void waitForRetry() {
  lcd.clear();
  lcd.print("Wait 15 seconds....");
  delay(15000);
}

String getNumFromKeypad() {
  char myKey;
  String numberString = "";
  lcd.setCursor(5, 0);
  while (true) {
    myKey = keypad.getKey();
    if (myKey == '#') return numberString;
    if (myKey == '*' && numberString.length() > 0) {
      lcd.setCursor(5 + numberString.length() - 1, 0);
      lcd.print(" ");
      lcd.setCursor(5 + numberString.length() - 1, 0);
      numberString.remove(numberString.length() - 1);
      Serial.print("\b \b");
    } else if (myKey >= '0' && myKey <= '9' && numberString.length() < 5) {
      numberString += myKey;
      lcd.print(myKey);
      Serial.println(numberString);
    }
  }
}

uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.print("Finger taken..");
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER: Serial.println("No finger detected"); break;
      case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Communication error"); break;
      case FINGERPRINT_IMAGEFAIL: Serial.println("Imaging error"); break;
      default: Serial.println("Unknown error"); break;
    }
    delay(500);
  }

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) return p;

  Serial.println("Remove finger");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER)
    ;

  Serial.print("ID ");
  Serial.println(id);
  Serial.println("Place same finger again");
  lcd.setCursor(0, 0);
  lcd.print("One more time!!!!!!");

  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    switch (p) {
      case FINGERPRINT_OK: Serial.println("Image taken"); break;
      case FINGERPRINT_NOFINGER: Serial.println("No finger detected"); break;
      case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Communication error"); break;
      case FINGERPRINT_IMAGEFAIL: Serial.println("Imaging error"); break;
      default: Serial.println("Unknown error"); break;
    }
  }

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) return p;

  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p != FINGERPRINT_OK) return p;

  Serial.print("Storing model for #");
  Serial.println(id);

  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK) return p;

  return FINGERPRINT_OK;
}

int verifyFingerprint() {
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) return -5;

  return finger.fingerID;
}

void unlock() {
  lcd.clear();
  lcd.print("Unlock....!");
  myServo.write(180);
  delay(lockTime);
  myServo.write(0);
}
