# smartHome
![Blynk Dashboard](https://github.com/user-attachments/assets/344e83ac-e2ed-468c-8265-09db51ce6b53)
A cloud based smart home system based on ESP32, Arduino, Blynk, IFTTT and Google Home  
**Watch our [Demo Video](https://drive.google.com/file/d/1qNk_YSzM0B3jiyF9gBpYPDaZySVHHBRv/view) Of the Project**
**and [Linkedin Post](https://www.linkedin.com/feed/update/urn:li:activity:7332065979402715136/)**
## Features
### 1. Smart Lock
The smart lock system provides three secure methods for unlocking the door:

#### Passkey Entry
Users can input a predefined passkey through the Blynk app or a connected interface.

#### One-Time Password (OTP)
An 8-digit OTP can be generated via the Blynk app or Google Assistant voice command. This updates virtual pin `V1`, triggering OTP generation and sending it to the user through Telegram. Once used, the OTP is automatically invalidated.

#### NFC Tag (Optional)
NFC-based access is disabled by default for security reasons, as NFC tags can be cloned. This feature can be toggled using either the Blynk app or Google Assistant, which modifies virtual pin `V16` responsible for enabling/disabling the `nfcEnabled` boolean.

#### Additional Security Measures:
- Upon successful entry, a new log entry is added to a connected Google Sheet using IFTTT Webhooks (if message notifications are enabled).
- If an intruder fails to enter the correct passkey three times, the system enters a **Locked Mode**, during which all unlock attempts are disabled. A warning message is sent to the user via Telegram, along with a link to the live camera stream from the ESP32-CAM.
- Locked Mode can be disabled remotely through the Blynk app or Google Assistant.

---

### 2. House Safety
The system integrates environmental sensors for real-time safety monitoring:

#### Gas Leak Detection
Uses an MQ-4 Carbon Monoxide Sensor. If gas is detected:
- Immediately shuts off AC, water heaters, and light/power socket circuits.
- Sends an emergency Telegram alert to the user.

#### Water Leak Detection
Monitors for leaks using a Water Level Sensor. If water is detected:
- Disconnects light and power socket circuits.
- Sends a Telegram alert to notify the user.

These safety mechanisms ensure rapid response to potential hazards, preventing damage or risk to occupants.

---

### 3. Demand Side Management (DSM)
The Demand Side Management feature is designed to improve electrical safety and optimize energy usage in the home:

The system continuously monitors the total current drawn by three major load groups:
1. Air Conditioners (AC)
2. Water Heaters
3. Lighting and Power Sockets

When the total current consumption reaches **40A**, the system:
1. Automatically disconnects the AC load to reduce the demand.
2. If the load remains at or exceeds 40A after this action, it proceeds to disconnect the Water Heater load.

This staged load-shedding strategy prevents electrical overloading and potential hazards, ensuring both system safety and energy efficiency.

---

### 4. Google Home & Google Sheets Integration via IFTTT
The system supports seamless integration with Google Assistant through IFTTT:

#### Voice Commands
Users can control devices and modes (e.g., lock/unlock door, enable NFC mode) via simple voice commands using Google Assistant.

#### Data Logging  
<table>
<tr>
<td style="width:80%">

 System events such as successful entries and safety incidents are automatically logged to Google Sheets using IFTTT Webhooks, allowing for easy access and history tracking.

</td>
<td style="width:20%">

<img src="https://github.com/user-attachments/assets/1f096a85-ff29-4e6d-9e14-59158bee6944" width="100%"/>

</td>
</tr>
</table>


## Libraries
### ESP32
| Library Name            | Purpose                                           |
|-------------------------|---------------------------------------------------|
| Blynk                   | Cloud and mobile app communication with ESP32     |
| UniversalTelegramBot    | Send and receive Telegram messages                |
| MFRC522 / MFRC522v2     | RFID/NFC tag reading                              |
| WiFi                    | Wi-Fi communication (ESP32 built-in)              |
| HTTPClient              | HTTP GET/POST requests (ESP32 built-in)           |
### Arduino
| Library Name         | Purpose                                         | Required Installation |
|----------------------|-------------------------------------------------|------------------------|
| SoftwareSerial       | Serial communication on other digital pins     | ✅ (built-in)          |
| Keypad               | Read input from matrix keypads                  | ✅ (Library Manager)   |
| LiquidCrystal_I2C    | Control I2C-based LCDs (1602, 2004)             | ✅ (Library Manager)   |
| Servo                | Control servo motors                            | ✅ (built-in)          |
| Wire                 | I2C communication interface                     | ✅ (built-in)          |

## Configuration
### Code Setup
**on Linux**
``` bash
# Copy and rename env.example.h to env.h in both directories
cp env.example.h Arduino/env.h
cp env.example.h ESP-A/env.h

# Copy and rename env.example.cpp to env.cpp in both directories
cp env.example.cpp Arduino/env.cpp
cp env.example.cpp ESP-A/env.cpp
 ```
**or manully on any other OS**
1. Create `env.h` file
2. Copy `env.example.h` content to `env.h`
3. Modify `env.h` with your actual environment-specific settings.

> ⚠️ Do not commit `env.h` — it is ignored via `.gitignore` to protect sensitive or local configuration.
---
### Wifi setup
Update the SSID and Password for the wifi which would connected to ESP32 board using this file on `env.cpp`
   ``` cpp
   // Wi-Fi credentials
   char ssid[] = "Your_WiFi_SSID";
   char pass[] = "Your_WiFi_Password";
   ```
### Telegram Setup
 1. **Important!:** You need to authorize **CallMeBot** to contact you using [this link](https://api2.callmebot.com/txt/login.php). Or alternatively, you can start the bot sending /start to [@CallMeBot_txtbot](https://t.me/CallMeBot_txtbot).
 2. update the follwing line with you own Telegram User ID  
      ``` cpp
      #define TELEGRAM_USER "Your_Telegram_User"
      ``` 
 3. you make your own bot using [@botfather](https://t.me/BotFather) and change the following line with your BOT Token
      ``` cpp
      #define BOT_TOKEN "Your_Bot_Token"
      ```
4. Update the following line with you own Chat ID you can ge it from [@raw_data_bot](https://t.me/raw_data_bot)
      ``` cpp
      #define CHAT_ID "Your_Chat_ID"
      ```
### IFTTT
make an account to [IFTTT](https://ifttt.com/) and update the `IFTTT_AUTH` 
``` cpp
#define IFTTT_AUTH "Your_IFTTT_AUTH"
```
### Blynk
1. update the following lines with you Workspace Credentials
``` cpp
// Blynk configuration
#define BLYNK_TEMPLATE_ID "Your_Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Template_Name"
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"
```
2. on your temeplate's date streams make the following configurations
   
   | Pin | Name               | Data Type |
   |-----|--------------------|-----------|
   | V0  | Total Load         | Double    |
   | V1  | OTP               | Integer   |
   | V2  | ACLoadActive      | Integer   |
   | V7  | HeaterLoadActive  | Integer   |
   | V8  | LightLoadActive   | Integer   |
   | V9  | stream local ip   | String    |
   | V10 | ACLoad            | Double    |
   | V11 | HeaterLoad        | Double    |
   | V12 | LightLoad         | Double    |
   | V13 | enableMessage     | Integer   |
   | V14 | enableCalls       | Integer   |
   | V16 | NFC switch        | Integer   |
   | V17 | Door              | Integer   |
   | V18 | Locked            | Integer   |
   | V19 | enableLogs        | Integer   |
## Credits
The ESP32-CAM code is based on a tutorial from  [RandomNerdTutorials](https://RandomNerdTutorials.com/esp32-cam-video-streaming-web-server-camera-home-assistant/
  )
