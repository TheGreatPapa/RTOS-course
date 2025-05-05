#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char * get_uname(void){
  char buffer[256];
  FILE* uname_output = popen("uname -a", "r");
  fgets(buffer, sizeof(buffer), uname_output);
  pclose(uname_output);

  char * result = strdup(buffer);
  return result;
}


static void custom_log(const char * buffer){
  openlog("pthread", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:3]: %s", buffer);
  closelog();
}
