
#define TRIGGER	"/sys/class/leds/%s/trigger"

int set_trigger(char* led, char* trigger, char* f_path);
