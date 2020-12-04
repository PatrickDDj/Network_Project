//
//  transport_layer.h
//  Network_Project
//
//  Created by PatrickDD on 2020/12/4.
//

#include "network_layer.h"


struct UDP_datagrame_s{
    byte src_port[2], des_port[2];
    byte total_length[2], checksum[2];
    
    byte data[DATAGRAM_DATA_MAX_SIZE];
};

typedef struct UDP_datagrame_s UDP_datagram;

static unsigned short get_datagram_length(byte *a){
    return (a[1] << 8) + a[0];
}

static unsigned short get_port(byte *port){
    return (port[0] << 8) + port[1];
}


static UDP_datagram* create_datagram(byte *src_port, byte *des_port, byte *data, unsigned short len){
    UDP_datagram *udp_datagram = (UDP_datagram *) malloc(sizeof(UDP_datagram));
    
    memcpy(udp_datagram->src_port, src_port, 2);
    memcpy(udp_datagram->des_port, des_port, 2);
    
    unsigned short total_length = len + DHL;
    memcpy(udp_datagram->total_length, &total_length, 2);
    memcpy(udp_datagram->data, data, len);
    
    return udp_datagram;
}

static void print_UDP_datagram(UDP_datagram *udp_datagram){
    printf("[UDP Datagram]\n");
    printf("[INFO] Total Length : %d\n", get_datagram_length(udp_datagram->total_length));
    printf("[INFO] Source Port : %d\n", get_port(udp_datagram->src_port));
    printf("[INFO] Destination Port : %d\n", get_port(udp_datagram->des_port));
    printf("[INFO] Data : %s\n", udp_datagram->data);
    printf("- - - - - - - - - -\n\n");
}

static unsigned short serialize_UDP_datagram(UDP_datagram *udp_datagram, byte* buf){
    memcpy(&buf[0], udp_datagram->src_port, 2);
    memcpy(&buf[2], udp_datagram->des_port, 2);
    
    memcpy(&buf[4], udp_datagram->total_length, 2);
    memcpy(&buf[6], udp_datagram->checksum, 2);
    
    unsigned short total_length = get_datagram_length(udp_datagram->total_length);
    
    memcpy(&buf[8], udp_datagram->data, total_length-8);
    
    return total_length;
}

static UDP_datagram* unserialize_UDP_datagram(byte *buf){
    byte src_port[2], des_port[2];
//    byte data[DATAGRAM_MAX_SIZE];
    
    memcpy(src_port, &buf[0], 2);
    memcpy(des_port, &buf[2], 2);
    
    unsigned short total_length = get_datagram_length(&buf[4]);
    
    byte *data[DATAGRAM_DATA_MAX_SIZE-DHL];
    unsigned short data_length = total_length - DHL;
    memcpy(data, &buf[8], data_length);
    
    UDP_datagram * udp_datagram = create_datagram(src_port, des_port, data, data_length);
    
    return udp_datagram;
}


static void send_in_transport_layer(byte *src_port, byte *des_port, byte *data, unsigned short len){
    UDP_datagram *udp_datagram = create_datagram(src_port, des_port, data, len);
    // printf("----------\n");
    // printf("[INFO] UDP Datagram sent\n");
    
    // print_UDP_datagram(udp_datagram);


    byte buf[DATAGRAM_MAX_SIZE];
    
    unsigned short datagram_len = serialize_UDP_datagram(udp_datagram, buf);
    
    send_in_network_layer(IP_ADDR_SENDER, IP_ADDR_RECEIVER, UDP, buf, datagram_len);
}


static unsigned short receive_in_transport_layer(byte *buf){
    receive_in_network_layer(buf);
    
    UDP_datagram *udp_datagram = unserialize_UDP_datagram(buf);
    
    if(get_datagram_length(udp_datagram->total_length) == 0){
        return 0;
    }
    // printf("----------\n");
    // printf("[INFO] UDP Datagram received\n");
    
    
    // print_UDP_datagram(udp_datagram);
    return 1;
}


