# smartHome
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
update the following lines with you Workspace Credentials
``` cpp
// Blynk configuration
#define BLYNK_TEMPLATE_ID "Your_Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Template_Name"
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"
```
