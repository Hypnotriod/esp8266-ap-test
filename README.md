# esp8266-ap-test
`ESP8266` template `WIFI Access point` project based on `ESP8266_RTOS_SDK`  
Configured to compile with `VSCode` under `Windows` using [Toolchain for Windows (Legacy GNU Make)](https://docs.espressif.com/projects/esp-idf/en/stable/get-started-legacy/windows-setup.html)  

## Setup guide and known issues
* Clone git repository `https://github.com/espressif/ESP8266_RTOS_SDK.git` into `C:\Espressif\ESP8266_RTOS_SDK` folder. Alternative path can be used.  
Checkout lastest release branch. In my case `origin/release/v3.3` worked fine.  
Add system environment variable `IDF_PATH` with `C:\Espressif\ESP8266_RTOS_SDK` value.  

* Download [esp32_win32_msys2_environment_and_gcc8_toolchain-20191220.zip](https://dl.espressif.com/dl/esp32_win32_msys2_environment_and_gcc8_toolchain-20191220.zip)  
Unzip into `C:\` (Toolchain should be located in `C:\msys32`, otherwise you will need to correct the `"terminal.integrated.shell.windows"` path in `.vscode\settings.json`)  

* To provide **xtensa-lx106 toolchain** and **python** paths:  
Create `esp8266_toolchain.sh` file in the `C:\msys32\etc\profile.d` folder with:

      export PATH="$PATH:/opt/xtensa-lx106-elf/bin"  
      export PATH="$PATH:/mingw32/bin"

# Build and Flash
Check for `CONFIG_ESPTOOLPY_PORT` value in `sdkconfig`.  

`VSCode` -> `Ctrl` + `Shift` + `B`:  
* **Build** - to build project  
* **Flash** - to flash project
