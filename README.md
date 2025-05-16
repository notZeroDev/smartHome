# smartHome
## Configuration
**on Linux**
``` bash
   cp env.example.h env.h
 ```
**or manully on any other OS**
1. Create `env.h` file
2. Copy `env.example.h` content to `env.h`
3. Modify `env.h` with your actual environment-specific settings.

> ⚠️ Do not commit `env.h` — it is ignored via `.gitignore` to protect sensitive or local configuration.
---
 **Important!:** You need to authorize **CallMeBot** to contact you using [this link](https://api2.callmebot.com/txt/login.php). Or alternatively, you can start the bot sending /start to [@CallMeBot_txtbot](https://t.me/CallMeBot_txtbot).
