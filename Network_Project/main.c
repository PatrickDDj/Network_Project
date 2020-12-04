//
//  main.c
//  20185697_final_project
//
//  Created by PatrickDD on 2020/12/3.
//

#include <stdio.h>
#include "transport_layer.h"


//int main(){
//    byte data[] = {'M','e','s','s','a','g','e', '-', '-','D', 'D', ' ', 'i', 's', ' ', 'S', 'm', 'a', 'r', 't'};
//
//    send_in_network_layer(IP_ADDR_SENDER, IP_ADDR_RECEIVER, UDP, data, 20);
//
//    byte res[DATA_MAX_SIZE];
//    receive_in_network_layer(res);
//    printf("%s\n", res);



//    FILE *file = fopen("temp.bin", "w");
//    byte *frame[FRAME_MAX_SIZE];
//    unsigned short len = create_frame(MAC_ADDR_SENDER, MAC_ADDR_RECEIVER, 20, data, 20, frame);
//    send_frame(frame, len, file);
//    fclose(file);
//
//    FILE *file1 = fopen("temp.bin", "r");
//    receive_frame(file1);
//}

int main(){
    byte data[] = {'M','e','s','s','a','g','e', '-', '-','D', 'D', ' ', 'i', 's', ' ', 'S', 'm', 'a', 'r', 't', '!'};

    send_in_transport_layer(PORT_SENDER, PORT_RECEIVER, data, 21);
//
    byte buf[DATAGRAM_MAX_SIZE];
//
    receive_in_transport_layer(buf);
//    UDP_datagram *udp_datagram = create_datagram(PORT_SENDER, PORT_RECEIVER, data, 21);
////    print_UDP_datagram(udp_datagram);
//
//
//    unsigned short len = serialize_UDP_datagram(udp_datagram, buf);
//
//    print_UDP_datagram(unserialize_UDP_datagram(buf));
}

