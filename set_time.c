#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B38400
/* POSIX compliant source */
#define _POSIX_SOURCE 1


int s_init(char *dev)
{
	int fd, c, res, i;
	struct termios tio;
	
	/* 
	 * Open modem device for reading and writing and not as controlling tty
	 * because we don't want to get killed if linenoise sends CTRL-C.
	 */
	fd = open(dev, O_RDWR | O_NOCTTY); 
	if (fd < 0)
	{
		perror(dev);
		return -1;
	}
	
	bzero(&tio, sizeof(tio)); /* clear struct for new port settings */
	
	/* 
	  BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	  CRTSCTS : output hardware flow control (only used if the cable has
				all necessary lines. See sect. 7 of Serial-HOWTO)
	  CS8     : 8n1 (8bit,no parity,1 stopbit)
	  CLOCAL  : local connection, no modem contol
	  CREAD   : enable receiving characters
	*/
	tio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	 
	/*
	  IGNPAR  : ignore bytes with parity errors
	  ICRNL   : map CR to NL (otherwise a CR input on the other computer
				will not terminate input)
	  otherwise make device raw (no other input processing)
	*/
	tio.c_iflag = IGNPAR | ICRNL;
	 
	/*
	 Raw output.
	*/
	tio.c_oflag = 0;
	 
	/*
	  ICANON  : enable canonical input
	  disable all echo functionality, and don't send signals to calling program
	*/
	tio.c_lflag = ICANON;
	 
	/* 
	  initialize all control characters 
	  default values can be found in /usr/include/termios.h, and are given
	  in the comments, but we don't need them here
	*/
	tio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	tio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	tio.c_cc[VERASE]   = 0;     /* del */
	tio.c_cc[VKILL]    = 0;     /* @ */
	tio.c_cc[VEOF]     = 4;     /* Ctrl-d */
	tio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	tio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	tio.c_cc[VSWTC]    = 0;     /* '\0' */
	tio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
	tio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	tio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	tio.c_cc[VEOL]     = 0;     /* '\0' */
	tio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	tio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	tio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	tio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	tio.c_cc[VEOL2]    = 0;     /* '\0' */
	
	/* 
	  now clean the modem line and activate the settings for the port
	*/
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &tio);
	
	/* Return fd to caller. */
	return fd;
}


int s_write(int fd, char *buf)
{
	int i, err;
	
	err = write(fd, buf, strlen(buf));
	if (err < 1) return -1;
	for (i = 0; i <= err; i++) read(fd, buf, 1);
	return err;
}


int s_write_line(int fd, char *buf)
{
	char buf_line[320];
	
	strncpy(buf_line, buf, 320);
	buf_line[sizeof(buf_line) - 3] = '\0';
	strcat(buf_line, "\n\r");
	return s_write(fd, buf_line);
}


int s_read_line(int fd, char *buf, int size)
{
	int err;
	
	err = read(fd, buf, size - 1);
	buf[err] = '\0';
	
	return err;
}


int main(int argc, char *argv[])
{
	int fd, err;
	char buf[255];

	if (argc < 2)
	{
		printf("Usage: set_date <tty> <date>\n");
		return 1;
	}
	
	fd = s_init(argv[1]);
	if (fd < 0) return 1;

	if (argc == 3)
	{
		sprintf(buf, "date %s\n\r", argv[2]);
		s_write_line(fd, buf);
	}
	else
	{
		time_t t = time(NULL);
		struct tm *ptm = localtime(&t);
		
		strftime(buf, 255, "date %H%M%S", ptm);
		printf("%s\n", buf);
		s_write_line(fd, buf);
		strftime(buf, 255, "date %Y%m%d", ptm);
		printf("%s\n", buf);
		s_write_line(fd, buf);
	}

	return 1;
}

