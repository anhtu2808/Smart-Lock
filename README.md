
# Smart Lock Project

## Introduction

This project is a comprehensive Smart Lock system that integrates IoT and embedded systems to provide advanced home security solutions. The system replaces traditional key-based locks with a modern, secure, and convenient system, incorporating fingerprint authentication, numeric passcodes, and remote monitoring via a smartphone app.

## Features

- **Fingerprint Authentication**: Secure access using biometric fingerprint recognition.
- **Numeric Passcode Entry**: Backup access method using a 4x4 keypad for numeric code entry.
- **Remote Monitoring and Control**: Real-time monitoring and control via the Blynk app on a smartphone.
- **Access Logging**: Logs all access attempts with timestamps using Google Sheets API.
- **LCD Display**: Provides user feedback and system status updates.
- **Automatic Locking**: Servo motor locks and unlocks the door based on authentication.

## Hardware Components

- **Arduino Uno**: Microcontroller for central processing.
- **ESP8266 Wi-Fi Module**: Enables Wi-Fi connectivity for IoT functionality.
- **Fingerprint Sensor (AS608XD65)**: Captures and authenticates fingerprints.
- **4x4 Keypad**: Allows numeric code input.
- **Servo Motor (SG90)**: Controls the physical locking mechanism.
- **16x2 LCD Display**: Displays system messages and prompts.
- **Miscellaneous**: Breadboard, jumper wires, resistors, power supply.

## Software Components

- **Programming Language**: C
- **Development Environment**: Arduino IDE
- **Mobile Application**: Blynk App (for iOS and Android)
- **Cloud Services**: Google Sheets API (for access logging)
- **Communication Protocols**: UART, I2C, Wi-Fi

## Setup Instructions

### Hardware Setup

1. **Assemble the Components**: Connect the Arduino Uno, ESP8266, fingerprint sensor, keypad, servo motor, and LCD display according to the circuit diagram.
2. **Power Supply**: Ensure all components are powered appropriately (most components use 5V DC).
3. **Wiring**: Double-check all connections to match the pin configurations.

### Software Setup

1. **Install Arduino IDE**: Download and install the latest version of the [Arduino IDE](https://www.arduino.cc/en/software).
2. **Install Required Libraries**:
   - **Adafruit Fingerprint Sensor Library**
   - **Servo.h**
   - **LiquidCrystal_I2C.h**
   - **Keypad.h**
   - **ESP8266WiFi.h**
   - **BlynkSimpleEsp8266.h**
3. **Blynk App Setup**:
   - Download the Blynk app on your smartphone.
   - Create a new project and select **ESP8266** as the device.
   - Obtain the Auth Token sent to your email.
4. **Google Sheets API Setup**:
   - Create a new Google Sheet to log access events.
   - Open the Script Editor in Google Sheets (`Extensions > Apps Script`).
   - Write a script to handle data logging and deploy it as a web app.
   - Obtain the web app URL for use in the Arduino code.

### Programming the Arduino

1. **Configure Wi-Fi Credentials**: In the Arduino code, set your Wi-Fi SSID and password.
   ```cpp
   char ssid[] = "Your_WiFi_SSID";
   char pass[] = "Your_WiFi_Password";
   ```
2. **Set Blynk Auth Token**: Insert the Auth Token from the Blynk app into the code.
   ```cpp
   char auth[] = "Your_Blynk_Auth_Token";
   ```
3. **Set Google Sheets API URL**: Insert the URL of your deployed Google Apps Script.
   ```cpp
   const char *GScriptId = "Your_Google_Script_Web_App_URL";
   ```
4. **Upload the Code**: Connect the Arduino Uno to your computer and upload the code.
5. **ESP8266 Configuration**: Ensure the ESP8266 module is properly connected and configured in the code.

## Usage Instructions

1. **Power On**: Turn on the power supply to the system.
2. **Fingerprint Enrollment**:
   - On first use, enroll fingerprints by following prompts on the LCD display.
   - Use the fingerprint sensor to scan and store authorized fingerprints.
3. **Numeric Passcode Setup**:
   - Set a numeric passcode using the 4x4 keypad as a backup authentication method.
4. **Lock/Unlock Operation**:
   - **Unlocking**:
     - Place a registered finger on the fingerprint sensor.
     - Or enter the numeric passcode on the keypad.
     - Upon successful authentication, the servo motor will unlock the door.
   - **Locking**:
     - The system automatically locks after a predefined timeout.
     - Can also be locked remotely via the Blynk app.
5. **Remote Monitoring and Control**:
   - Open the Blynk app to monitor access attempts and lock status.
   - Use the app to remotely lock or unlock the door.
6. **Access Logs**:
   - All access events are logged in the Google Sheet, including date, time, user, and authentication method.

## Demonstration

- **Video Demo**: [Watch Here](https://demo.anhtudev.works/smart-lock)

## Project Documentation

For detailed information about the project, refer to the project report:

- **Project Report**: [Watch Here](https://anhtudev.works.pdf)

## Troubleshooting

- **Fingerprint Sensor Issues**:
  - Ensure the sensor is correctly wired to the Arduino.
  - Check the power supply to the sensor.
  - Verify that the fingerprint library is properly installed.
- **Wi-Fi Connection Problems**:
  - Double-check Wi-Fi credentials.
  - Ensure the ESP8266 module is properly connected and configured.
  - Check for interference or weak Wi-Fi signal.
- **Blynk App Not Responding**:
  - Verify the Auth Token is correct.
  - Ensure the Arduino is connected to the internet.
  - Restart the Blynk app and the Arduino system.
- **Access Logs Not Updating**:
  - Check the Google Apps Script deployment and permissions.
  - Ensure the Google Sheets API URL is correctly set in the code.

## Acknowledgments

- **Mentors and Instructors**: Thank you to all who provided guidance throughout this project.
- **Open-Source Libraries**: Gratitude to the developers of libraries and tools used.

## Contact

For any questions or suggestions, please contact:

- **Name**: Đặng Mai Anh Tú
- **Email**: [dangmaianhtu@gmail.com](mailto:dangmaianhtu@gmail.com)
- **Website**: [www.anhtudev.works](http://www.anhtudev.works)
- **GitHub**: [github.com/anhtu2808](https://github.com/anhtu2808)

---
