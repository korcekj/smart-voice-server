// Properties of remote
#define R_PIN "pin"
#define R_NAME "name"
#define R_TYPE "type"
#define R_FREQUENCY "frequency"
#define R_COMMAND "command"

// ESP8266 GIO PIN
#define MIN_ESP8266_PIN 0
#define MAX_ESP8266_PIN 16
#define DEFAULT_ESP8266_PIN 0

// REMOTE TYPE
#define MIN_NUM_TYPE 0
#define MAX_NUM_TYPE 0
#define SONY_REMOTE 0

// REMOTE FREQUENCY
#define DEFAULT_REMOTE_FREQUENCY 38

// SONY COMMANDS
#define SONY_NUM_COMM 5
#define SONY_POWER 2704
#define SONY_VOLUME_UP 1168
#define SONY_VOLUME_DOWN 3216
#define SONY_CHANNEL_UP 144
#define SONY_CHANNEL_DOWN 2192

// ALL COMMANDS
extern const int SONY_COMMANDS[SONY_NUM_COMM];
