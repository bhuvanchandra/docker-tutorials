#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include "led_trigger.h"

typedef struct {
  char led[8];
  char trigger[16];
  char default_trigger[16];
  char f_path[32];
} led_trigger;

led_trigger trigger;
static int done = 0;

void sig_handler(int signal) {
  if (!set_trigger(trigger.led, "mmc0", trigger.f_path))
    printf("%s is set as trigger for %s\n", "mmc0", trigger.led);
  done = 1;
}

int set_trigger(char* led, char* trigger, char* f_path)
{
  int fd = 0, ret = 0;
  fd = open(f_path, O_WRONLY);
	if(fd < 0) {
		perror("led trigger: Open failed!");
		close(fd);
		return -1;
	}

	ret = write(fd, trigger, strlen(trigger));
	if(ret < 0) {
		perror("led trigger: Write failed!");
		close(fd);
		return ret;
	}
	close(fd);
  return 0;
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("Usage: %s <led-name> <trigger>\n", argv[0]);
    return -1;
  }

  strncpy(trigger.led, argv[1], strlen(argv[1]));
  strncpy(trigger.trigger, argv[2], strlen(argv[2]));
  printf("LED:%s, trigger: %s\n", trigger.led, trigger.trigger);

  signal(SIGINT, sig_handler);
  signal(SIGKILL, sig_handler);
  signal(SIGTERM, sig_handler);

  char f_path[64];
  sprintf(f_path, TRIGGER, trigger.led);
  printf("sysfs led trigger path: %s\n", f_path);
  strncpy(trigger.f_path, f_path, strlen(f_path));

  if (!set_trigger(trigger.led, trigger.trigger, trigger.f_path))
    printf("%s is set as trigger for led: %s\n", trigger.trigger, trigger.led);
  else
    exit(-1);

  while(!done)
    sleep(1);

  return 0;
}
