#include "http-server.h"
#include <string.h>

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
int num = 0;

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator
    // TODO: send response back to client?
	write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
	write(client_sock, response_buff, strlen(response_buff));
}

void handle_shownum(int client_sock)
{
	char response_buff[BUFFER_SIZE];
	snprintf(response_buff, BUFFER_SIZE, "num = %d\r\n", num);
	write(client_sock, response_buff, strlen(response_buff));
}

void handle_increment(int client_sock)
{
	char response_buff[BUFFER_SIZE];
	snprintf(response_buff, BUFFER_SIZE, "incrementing num...\r\n");
	num++;
	write(client_sock, response_buff, strlen(response_buff)); 
}

void handle_add(int client_sock, char* path)
{
	char response_buff[BUFFER_SIZE];
	int add_val = 0;
	sscanf(path, "/add?value=%d", &add_val);//collects add_val from path, if not number, add_val will be 0
	snprintf(response_buff, BUFFER_SIZE, "Adding %d to num...\r\n", add_val);
	num += add_val;//actually adding to the num!
	write(client_sock, response_buff, strlen(response_buff));
}

void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {//the path will now be hopefully stored in path
        printf("Invalid request line\n");
        return;
	}
	//need to see if path is shownum
	if(!strcmp(path, "/shownum"))//shownum is the path
	{
		handle_shownum(client_sock);
		return;
	}
	if(!strcmp(path, "/increment"))
	{
		handle_increment(client_sock);
		return;
	}
	//need to check if the path is correct for add-> /add?value=, will use strncmp
	if(!strncmp(path, "/add?value=", strlen("/add?value=")))
	{
		handle_add(client_sock, path);//will test the legitmacy of the value after "value="	
		return;
	}
	handle_404(client_sock, path); 
} 

int main(int argc, char *argv[]) 
{ 
	int port = 0; 
	if(argc >= 2)
	{ // if called with a port number, use that 
		port = atoi(argv[1]);
    }

    start_server(&handle_response, port);

}
