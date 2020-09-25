# esp8266-ap-test
`ESP8266` template of `WIFI Access point` project based on `ESP8266_RTOS_SDK`  
Configured to compile with `VSCode` on `Windows` using [Toolchain for Windows (Legacy GNU Make)](https://docs.espressif.com/projects/esp-idf/en/stable/get-started-legacy/windows-setup.html)  

## Toolchain setup guide
* Clone git repository `https://github.com/espressif/ESP8266_RTOS_SDK.git` into `C:\Espressif\ESP8266_RTOS_SDK` folder. Alternative path can be used.  
Checkout lastest release branch. In my case `origin/release/v3.3` worked fine.  
Add system environment variable `IDF_PATH` with `C:\Espressif\ESP8266_RTOS_SDK` value.  

* Download [esp32_win32_msys2_environment_and_gcc8_toolchain-20191220.zip](https://dl.espressif.com/dl/esp32_win32_msys2_environment_and_gcc8_toolchain-20191220.zip)  
Unzip into `C:\` (Toolchain should be located at `C:\msys32`, otherwise you will need to correct the `"terminal.integrated.shell.windows"` path in `.vscode\settings.json`)  

* To provide **xtensa-lx106 toolchain** and **python** paths for environment,  
create `esp8266_toolchain.sh` file in the `C:\msys32\etc\profile.d` folder with:

      export PATH="$PATH:/opt/xtensa-lx106-elf/bin"  
      export PATH="$PATH:/mingw32/bin"

# Build and Flash
Check for `CONFIG_ESPTOOLPY_PORT` value in `sdkconfig`.  

`VSCode` -> `Ctrl + Shift + B`:  
* **Build** - to build project  
* **Flash** - to flash project

# Test AP
* Connect to **Test Station** via WI-FI using **12345678** pass
* Open `http://192.168.4.1/test`
* Send `http://192.168.4.1/test?led=on` to light the embedded LED
* Watch for the logs at **115200** baud
