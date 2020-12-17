#include <stdio.h>
#include "transport_layer.h"

int main(){
    char data[DATAGRAM_DATA_MAX_SIZE];

    printf("Welcome to the Sender Testing Part!\n");
    while(1){
        printf("\nInput Your Message, Please : \n");
        gets(data);
        // printf("%d\n", strlen(data));
        send_in_transport_layer(PORT_SENDER, PORT_RECEIVER, data, strlen(data));
        printf("[INFO] %s", get_time());
        printf("[INFO] Data Sent : %s\n",  data); 
    }
}