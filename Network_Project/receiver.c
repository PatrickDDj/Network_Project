#include <stdio.h>
#include "transport_layer.h"

int main(){
    byte buf[DATAGRAM_MAX_SIZE];

    printf("Welcome to the Receiver Testing Part!\n");
    while(1){
        memset(buf, 0, sizeof(buf));
        if(receive_in_transport_layer(buf)){
            printf("[INFO] Data Received : %s\n", buf + DHL);
        }
    }
}