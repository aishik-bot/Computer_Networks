/*
by Aishik Sikdar
Server side for recieving/sending message from/to the client side
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>



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
<filename> <port number>

main() function parameters:
argc: total number of parameters(in this case 2)
argv: original parameters(filename, port number)
*/
int main(int argc, char *argv[])
{
    //check if the user has given port number as input, if not terminate the program
    if (argc<2)
    {
        fprintf(stderr,"PORT number not provided! program terminated\n");
        exit(1);
    }
    
    int sockfd, newsockfd, portno, n;
    char buf[200];

    struct sockaddr_in server_addr,client_addr;//sockaddr_in is included in header netinet/in.h . Points to the internet addresss. 
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket descriptor = socket(domain{IPV4 protocol}, communication type{TCP}, Protocol value for IP=0)

    //check if socket function has resulted in a failure, if so it shows an error message following program termination
    if(sockfd<0)
    {
        error("Error opening socket!");
    }

    bzero((char *)&server_addr, sizeof(server_addr));//erases data in which the function references to. 

    portno = atoi(argv[1]);//extracting the port no. from the command line argument after converting from string to integer using atoi()

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0)
    {
        error("Binding failed!");
    }

    listen(sockfd, 4);//listen to which client is trying to connect. Maximum 4 clients are allowed in this case

    clilen = sizeof(client_addr);

    //accept the connection in new file descriptor newsockfd
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen);
    
    //check if there is error on accepting the connection
    if(newsockfd<0)
    {
        error("Error on accept!");
    }

    while(1)
    {
        bzero(buf,200);
        n = read(newsockfd, buf, 200); //read message from the client and store it to 'buf'

        //check if read() fails
        if(n<0)
        {
            error("Error on reading!");
        }

        printf("CLIENT: %s\n",buf);// show the message sent by the client

        bzero(buf, 200);
        printf("SERVER: ");
        fgets(buf, 200, stdin);

        n = write(newsockfd, buf, strlen(buf)); //write message for the client's corresponding read function and store in 'buf'

        //check if write fails
        if(n<0)
        {
            error("Error in writing!");
        }

        //whenever server says "Exit", loop terminates
        if(strncmp("Exit", buf, 4)==0)
            break;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}