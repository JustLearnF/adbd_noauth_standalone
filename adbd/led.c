#include "led.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void led_light(int type, int brightness) {
  char str[8];
  sprintf(str, "%d", brightness);
  int fd;
  switch (type) {
    case LED_R:
      fd = open("/sys/class/leds/red/brightness",O_WRONLY);
      if(fd >= 0)
      {
        write(fd,str,strlen(str));
      }
      break;
    case LED_G:
      fd = open("/sys/class/leds/green/brightness",O_WRONLY);
      if(fd >= 0)
      {
        write(fd,str,strlen(str));
      }
      break;
    case LED_B:
      fd = open("/sys/class/leds/blue/brightness",O_WRONLY);
      if(fd >= 0)
      {
        write(fd,str,strlen(str));
      }
      break;

    default:
      break;
  }
  close(fd);
}
void led_light_one(int type, int brightness) {
  led_light(LED_R, 0);
  led_light(LED_G, 0);
  led_light(LED_R, 0);
  led_light(type, brightness);
}