# Notifier
ESP8266 calender door bell notifier

## Development prereqs
Use legacy USB-serial driver. E.g. https://github.com/rubengr/PL2303HXA-Phased-Out/blob/main/IO-Cable_PL-2303_Drivers-Generic_Windows_PL2303_Prolific.zip

Add a file called passwords.h to the project, containing the following fields
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

## TODO
- time
- manage daylight saving time
- web
- sleep
- wake from sleep by input