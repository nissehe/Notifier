# Notifier
ESP8266 calender door bell notifier

## Development prereqs
Use legacy USB-serial driver. E.g. https://github.com/rubengr/PL2303HXA-Phased-Out/blob/main/IO-Cable_PL-2303_Drivers-Generic_Windows_PL2303_Prolific.zip

Register an account at timezonedb.com to get an API key

Add a file called passwords.h to the project, containing the following fields
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";
const char* TIMEZONEDB_API_KEY = "";

## Notes
IP is hard coded to 192.168.0.202

Device must be restarted to apply DST/non-DST time

## TODO