/*
by Aishik Sikdar
Client side for sending/recieving message to/from the server side
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

//ERROR HANDLER
//Interprets and outputs the error message passed as parameter. Whenever error occurs this error is invoked and the program is terminated
void error(const char *msg)
{
    perror(msg);//error function which interprets the error number and outputs the error description using STDERR stream
    exit(1);//terminates the program 
}

//MAIN METHOD
/*Take input from the user as command line arguement from the terminal

Command line input should be given as:
<filename> <host name/server IP> <port number>

main() function parameters:
argc: total number of parameters(in this case 3)
argv: original parameters(filename, hostname, port number)
*/
int main(int argc, char *argv[])
{
    //check if the user has given port number as input, if not terminate the program
    if (argc<2)
    {
        fprintf(stderr,"Hostname or PORT number not provided! program terminated\n");
        exit(1);
    }

    int sockfd, portno, n;
    char buf[200];

    struct sockaddr_in server_addr; //sockaddr_in is included in header netinet/in.h . Points to the internet addresss. 
    struct hostent *server; //used to store hostname and ip address

    //socket creation 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  //socket descriptor = socket(domain{IPV4 protocol}, communication type{TCP}, Protocol value for IP=0)
    
    //check if socket function has resulted in a failure, if so it shows an error message following program termination
    if(sockfd<0)
    {
        error("Error opening socket!");
    }

    bzero((char *)&server_addr, sizeof(server_addr));//erases data in which the function references to. 

    portno = atoi(argv[2]);//extracting the port no. from the command line argument after converting from string to integer using atoi()

    server = gethostbyname(argv[1]);

    //check if the given hostname exists
    if(server == NULL)
    {
        fprintf(stderr, "ERROR! No such host");
    }

    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length); //copying data from server to server_addr
    server_addr.sin_port = htons(portno);

    //connect to server and check if the connection was succesfull
    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0)
    {
        error("Connection failed!");
    }

    while(1)
    {
        bzero(buf, 200);
        printf("CLIENT: ");
        fgets(buf, 200, stdin);

        n = write(sockfd, buf, strlen(buf)); //write message for the server's corresponding read function and store in 'buf'

        //check if write fails
        if(n<0)
        {
            error("Error in writing!");
        }

        bzero(buf, 200);
        n = read(sockfd, buf, 200); //read message from the server and store it to 'buf'

        //check if read() fails
        if(n<0)
        {
            error("Error on reading!");
        }
        printf("SERVER: %s\n",buf);// show the message sent by the server

         //whenever client says "Exit", loop terminates
        if(strncmp("Exit", buf, 4)==0)
            break;
    }

    close(sockfd);
    return 0;
}