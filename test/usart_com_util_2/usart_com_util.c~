

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>

void ascom_transmit(int r, int w){

  unsigned char buf[1024];
  int nb;
  while(1){
    nb = read(r, buf, 1024);
    write(w, buf, nb);
  }
}


int main(int argc, char **argv){

  if(argc != 2){
    printf("usage : %s <serialcomfile>\n", *argv);
    return 1;
  }

  struct stat sb;
  if(stat(argv[1], &sb) == -1){
    printf(strerror(errno));
    return 1;
  }

  if(!S_ISCHR(sb.st_mode)){
    printf("invalid file type (%s)\n", argv[1]);
    return 1;
  }

  int fd = open(argv[1], O_RDWR);

  if(fd == -1){
    printf(strerror(errno));
    return 1;
  }

  struct termios t, backup;
  tcgetattr(fd, &t);
  backup = t;
  cfmakeraw(&t);
  cfsetspeed(&t, B9600);
  tcsetattr(fd, TCSANOW, &t);

  int pid;
  if((pid = fork()) == -1){
    printf(strerror(errno));
    return 1;
  }

  int status;
  if(pid){
    ascom_transmit(STDIN_FILENO, fd);
    wait(&status);
    tcsetattr(fd, TCSANOW, &backup);
  }
  else{
    ascom_transmit(fd, STDOUT_FILENO);
  }

  return 0;
}
