//////////////////////////////
// Settings defined below

// WIFI
#define WIFI_SSID      "<SET_YOUR_SSID>"
#define WIFI_PASSWORD  "<SET_YOUR_PASS>"

// TIMEZONES
#define TIMEZONE euCET // See NTP_Time.h tab for other "Zone references", UK, usMT etc

// Update every 15 minutes, up to 1000 request per day are free (viz average of ~40 per hour)
const int UPDATE_INTERVAL_SECS = 1UL * 60UL; // 1 minute

// Pins for the TFT interface are defined in the User_Config.h file

// For units use "metric" or "imperial"
const String units = "metric";

// Sign up for a FREE key and read API configuration info here:
// https://openweathermap.org/, change x's to your API key
const String api_key = "<SET_YOUR_API_KEY>";

// Set the forecast longitude and latitude to at least 4 decimal places
const String latitude =  "xx.xxxx";
const String longitude = "xx.xxxx";

// For language codes see https://openweathermap.org/current#multi
const String language = "eu"; // Default language = en = English

// Short day of week abbreviations used in 4 day forecast (change to your language)
const String shortDOW [8] = {"???", "AL", "AR", "AZ", "OG", "OR", "LR", "IG"};

// Change the labels to your language here:
const char sunStr[]        = "Egunsenti";
const char cloudStr[]      = "Hezetasuna";
const String moonPhase[8]  = {"Ilberria", "Ilgora", "Ilgora", "Ilgora", "Ilbetea", "Ilbehera", "Ilbehera", "Ilbehera"};

// End of user settings
//////////////////////////////
