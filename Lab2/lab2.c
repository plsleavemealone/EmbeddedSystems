/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Name/UNI: Jino Haro (jmh2289), Tvisha Gangwani (trg2128)
 */
#include "fbputchar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "usbkeyboard.h"
#include <pthread.h>

/* Update SERVER_HOST to be the IP address of
 * the chat server you are connecting to
 */
/* micro36.ee.columbia.edu */
#define SERVER_HOST "160.39.202.255"
#define SERVER_PORT 42000

#define BUFFER_SIZE 128
int netRow=1;
int netCol=0;
/*
 * References:
 *
 * http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 * 
 */

int sockfd; /* Socket file descriptor */

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);

int main()
{
  int err, col,row,row1,col1,sendRes,m;
  m=0;
  row1 = 14;
  col1 = 1;
  char stringSend [128];
  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;
  char keystate[12];

  if ((err = fbopen()) != 0) {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }
	for (col = 0 ; col < 64 ; col++) {
	for (row =0; row <24;row++)
    
    fbputchar(' ', row, col);
  }
  /* Draw rows of asterisks across the top and bottom of the screen 
  for (col = 0 ; col < 64 ; col++) {
    fbputchar('*', 0, col);
    fbputchar('*', 23, col);
  }

	for (col = 0 ; col < 64 ; col++) {
    //fbputchar('*', 0, col);
    fbputchar('*', 12, col);
  }
  fbputs("Hello CSEE 4840 World!", 4, 10);*/

  /* Open the keyboard */
  if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }
    
  /* Create a TCP communications socket */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error: Could not create socket\n");
    exit(1);
  }

  /* Get the server address */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);
  if ( inet_pton(AF_INET, SERVER_HOST, &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "Error: Could not convert host IP \"%s\"\n", SERVER_HOST);
    exit(1);
  }

  /* Connect the socket to the server */
  if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error: connect() failed.  Is the server running?\n");
    exit(1);
  }

  /* Start the network thread */
  pthread_create(&network_thread, NULL, network_thread_f, NULL);

  /* Look for and handle keypresses */
  for (;;) {
    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    if (transferred == sizeof(packet)) {
      sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0],
	      packet.keycode[1]);
      printf("%s\n", keystate);
      fbputs(keystate, 6, 0);
      
      
      
      if (packet.keycode[0] == 0x2c) { /* space pressed? */
		//why tho

		for (col = 0 ; col < 64 ; col++) {
    	//fbputchar('*', 0, col);
    		fbputchar('*', 12, col);
  		}
  		
  		fbputs("Hello CSEE 4840 World!", 0, 0);

      }
      
      if (packet.keycode[0]) { /* Trying to print a char */
      	if ((packet.keycode[0] > 0x1d )& (packet.keycode[0] < 0x28) )
      	
      	{
      		
      		
      		char c  = packet.keycode[0] +19;
      		if (packet.keycode[0] == 0x27){
      			c = '0';
      		}
      		//this prints number 0 
      		stringSend[m] = packet.keycode[0];
      		m = m +1;
      		
			fbputchar(c,row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;
      	}
      	}
      	//else{
	else if{
		//this has to be limited to the normal characters
      		char c  = packet.keycode[0] + 93;
      		
			fbputchar(c,row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;
			}
			
	    }
	      //im adding this
	 else{
	 ///this part should be for shifts or whatever
	 }
		
      }
      
      //MOVING THE CURSOR 
      fbputchar('_',row1,col1);
      if (packet.keycode[0] == 0x4f){
      	fbputchar(' ',row1, col1);
      	col1 = col1 +1 ;
      	
      	}
      if (packet.keycode[0] == 0x50){
      	fbputchar(' ',row1, col1);
      	col1 = col1 - 2 ;
      	
      	}
      if (packet.keycode[0] == 0x52){
      	fbputchar(' ',row1, col1);
      	row1 = row1 -1 ;
      	
      	}
      if (packet.keycode[0] == 0x51){
      	fbputchar(' ',row1, col1);
      	row1 = row1 +1 ;
      	
      	}
      	// IF ENTER 
      	if (packet.keycode[0] == 0x28){
      	fbputchar('S',row1, col1);
      	//
      	//stringSend[m] = '\0';
      	printf("%s",stringSend);
      	
      //	sendRes = send(sockfd,"trg2128", strlen("trg2128"),0);
	sendRes = send(sockfd, /*here should go the pointer to the stringSend head */, )
      	m =0;
      	if (sendRes < 0){
      	printf("Error");
		}      	
		else{
		printf("sent");
		}
      	
      	}
      	
      if (packet.keycode[0] == 0x29) { /* ESC pressed? */
	break;
      }
    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  return 0;
}

void *network_thread_f(void *ignored)
{
  char recvBuf[BUFFER_SIZE];
  int n;
  /* Receive data */
	//we need to count the length here
	int stringlength = 0;
	//this needs to increment with each character we receive, i dont th
	///ink we need it now, but itss good for formatting
	//since we need to break the screen at some point
  
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("%s", recvBuf);
    fbputs(recvBuf, netRow, 0);
    netRow = netRow +1 ;
    
    
    
  }
  

  return NULL;
}

