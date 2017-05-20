#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <fcntl.h>      // for open
#include <unistd.h>     // for close
#include <list.h>
#include <jansson.h>

void parseJson(List *vector, char *str)
{
    int counter = 0;
    json_error_t err;
    json_t *obj = json_loads(str, 0, &err);
    int index = 0;
    json_t *value = NULL;
    json_t *arr = json_object_get(obj, "data");
    counter = json_array_size(arr);

    index = 0;
    int array[counter];

    json_array_foreach(arr, index, value)
    {
        array[index] = (int)json_integer_value(value);
        List_add(vector, (void *)&array[index]);
    }
}


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while (1)
    {
        printf("Enter message : ");
        scanf("%s", message);

        //Send some data
        if (send(sock, message, strlen(message), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if (recv(sock, server_reply, 2000, 0) < 0)
        {
            puts("recv failed");
            break;
        }
        List *list = List_new();
        parseJson(list, server_reply);
        puts("Server reply :");
        puts(server_reply);

        close(sock);
        break;
    }

    return 0;
}