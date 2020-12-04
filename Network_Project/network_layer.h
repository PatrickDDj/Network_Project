//
//  main.c
//  20185697_final_project
//
//  Created by PatrickDD on 2020/12/2.
//

#include <stdio.h>
//#include "configuration.h"
#include "datalink_layer.h"
#include <string.h>
#include <stdlib.h>
//typedef unsigned char byte;


struct ip_package_s
{
    //header

    byte version_IHL;   //version - 4 bits, IHL - 4 bits
    byte service;   //differentiated service - 8 bits
    byte total_length[2];   //total length - 16 bits

    byte identification[2]; //identification - 16 bits
    byte _DF_MF_offset[2];  //DF - 1 bit, MF - 1 bit, offset - 13 bits

    byte ttl;   //time to live - 8 bits
    byte protocal;  //protocal - 8 bits
    byte checksum[2];   // check sum - 16 bits

    byte src_ip_addr[4];    //source ip address - 32 bits
    byte des_ip_addr[4];    //destination ip address - 32 bits


    //data

    byte data[DATA_MAX_SIZE-1];   //max data length

};

typedef struct ip_package_s ip_package;

static unsigned short get_header_length(byte version_IHL){
    return version_IHL & 0x0F;
}

static unsigned short get_total_length(byte *total_length){
    return (total_length[1] << 4) + total_length[0];
}

static unsigned short get_offset(byte *_DF_MF_offset){
    return ((_DF_MF_offset[1] & 0x1F) << 8) + _DF_MF_offset[0];
}

static unsigned short get_MF(byte *_DF_MF_offset){
    return (_DF_MF_offset[1] >> 5) & 0x01;
}

static unsigned short get_DF(byte *_DF_MF_offset){
    return (_DF_MF_offset[1] >> 6);
}

static void print_ip_package(ip_package *package){
//    printf("ID : %0x\n", package);
    printf("[IP package] %c%c\n", package->identification[0], package->identification[1]);
    printf("[INFO] Total Length : %d\n", get_total_length(package));
    printf("[INFO] Header Length : %d\n", get_header_length(package));
    printf("[INFO] Source IP Address : %d.%d.%d.%d\n",
           package->src_ip_addr[0], package->src_ip_addr[1],
           package->src_ip_addr[2], package->src_ip_addr[3]);
    printf("[INFO] Destination IP Address : %d.%d.%d.%d\n",
           package->des_ip_addr[0], package->des_ip_addr[1],
           package->des_ip_addr[2], package->des_ip_addr[3]);
    printf("[INFO] Protocal : %d\n", package->protocal);
    printf("[INFO] Time to Live : %d\n", package->ttl);
//    printf("[INFO] \n", get_offset(package));
    printf("[INFO] DF : %d\tMF : %d\tOffset : %d\n", get_DF(package->_DF_MF_offset), get_MF(package->_DF_MF_offset), get_offset(package->_DF_MF_offset));
    printf("[INFO] Data : %s\n", package->data);
    printf("- - - - - - - - - -\n");
}


static ip_package * create_ip_packeage(byte* src_ip_addr, byte* des_ip_addr, byte protocal, byte* data, unsigned short len, byte DF, byte MF, unsigned short offet ){
    ip_package *package = (ip_package *) malloc(sizeof(ip_package));

    package->version_IHL = (VERSION << 4) + IHL;
    package->service = SAERVICE;
    
    unsigned short total_length = IHL + len;

    memcpy(package->total_length, &total_length, 2);

    package->protocal = protocal;
    package->ttl = TTL;

    
    memcpy(package->identification, IDENTIFICATION, 2);
    
    memcpy(package->src_ip_addr, src_ip_addr, 4);
    memcpy(package->des_ip_addr, des_ip_addr, 4);
    
    unsigned short _DF_MF_offset = (DF << 14) + (MF << 13) + offet;
    
    memcpy(package->_DF_MF_offset, &_DF_MF_offset, 2);
    
    memcpy(package->data, data, len);

    return package;
}

static unsigned short serialize_ip_package(ip_package *package, byte* buf){
    
    memcpy(&buf[0], &package->version_IHL, 1);
    memcpy(&buf[1], &package->service, 1);
    memcpy(&buf[2], package->total_length, 2);
    
    
    memcpy(&buf[4], package->identification, 2);
    memcpy(&buf[6], package->_DF_MF_offset, 2);
    
    memcpy(&buf[8], &package->ttl, 1);
    memcpy(&buf[9], &package->protocal, 1);
    memcpy(&buf[10], &package->checksum, 2);
    
    memcpy(&buf[12], package->src_ip_addr, 4);
    memcpy(&buf[16], package->des_ip_addr, 4);
    
    
    int data_len = get_total_length(package->total_length) - get_header_length(package->version_IHL);
    
    memcpy(&buf[20], package->data, data_len);
    
    return 20 + data_len;
}

static ip_package* unserialize_ip_package(byte *buf){
    
    //get header length
    byte version_IHL;
    unsigned short IHL_s;
    memcpy(&version_IHL, &buf[0], 1);
    IHL_s = get_header_length(version_IHL);
    
    
    //get toal length
    byte total_length[2];
    unsigned short total_length_s;
    memcpy(total_length, &buf[2], 2);
    total_length_s = (total_length[1] << 4) + total_length[0];
    
    //get ip address (source/destination)
    byte src_ip_addr_s[4], des_ip_addr_s[4];
    memcpy(src_ip_addr_s, &buf[12], 4);
    memcpy(des_ip_addr_s, &buf[16], 4);
    
    //get protocal
    byte protocal_s;
    memcpy(&protocal_s, &buf[9], 1);
    
    
    //get DF MF offset
    byte DF, MF;
    unsigned short offset;
    byte _DF_MF_offset[2];
    memcpy(&_DF_MF_offset, &buf[6], 2);
    
    
    DF = get_DF(_DF_MF_offset);
    MF = get_MF(_DF_MF_offset);
    offset = get_offset(_DF_MF_offset);
    
    byte data_s[DATA_MAX_SIZE-1];
    unsigned short data_len_s = total_length_s - IHL_s;
    memcpy(data_s, &buf[20], data_len_s);
    
    ip_package *package = create_ip_packeage(src_ip_addr_s, des_ip_addr_s, protocal_s, data_s, data_len_s, DF, MF, offset);
    
    
    
//    print_ip_package(package);
    return package;

    
    
}



static void send_in_network_layer(byte* src_ip_addr, byte* des_ip_addr, byte protocal, byte* data, unsigned short len){
    
    ip_package *package;
    
    FILE *temp = fopen("temp.bin", "w");
    
    //no fragment
    if(len <= FRAGMENT_MAX_SIZE){
        package = create_ip_packeage(src_ip_addr, des_ip_addr, protocal, data, len, 0, 0, 0);
//        print_ip_package(package);
    }
    
    //more fragments
    else{
        int cur_len = len;
        unsigned short i = 0;
        while(cur_len > 0){
            
            //last fragment
            if(cur_len <= FRAGMENT_MAX_SIZE){
                package = create_ip_packeage(src_ip_addr, des_ip_addr, protocal, data+i*FRAGMENT_MAX_SIZE, cur_len, 1, 0, i);
            }
            else{
                package = create_ip_packeage(src_ip_addr, des_ip_addr, protocal, data+i*FRAGMENT_MAX_SIZE, FRAGMENT_MAX_SIZE, 1, 1, i);
            }
            byte buf[PACKAGE_MAX_SIZE];
//            print_ip_package(package);
//            printf("Data length : %d\n", serialize_ip_package(package, buf));
            unsigned short payload_len = serialize_ip_package(package, buf);
            
            
            byte frame[FRAME_MAX_SIZE];
            unsigned short frame_len = create_frame(MAC_ADDR_SENDER, MAC_ADDR_RECEIVER, payload_len, buf, payload_len, frame);
            
            send_frame(frame, frame_len, temp);
            
            
//            printf("After unserialization\n");
//            unserialize_ip_package(buf);
            cur_len -= FRAGMENT_MAX_SIZE;
            i++;
            
        }
    }
    fclose(temp);
}


static void receive_in_network_layer(byte *buf){
    FILE *temp = fopen("temp.bin", "r");
    ip_package *package;
    
    byte payload[PAYLOAD_MAX_SIZE];
    int payload_len;
    int cur = 0;
    int data_len;
    while(1){
        payload_len = receive_frame(temp, payload);
        if(payload_len){
            package = unserialize_ip_package(payload);
//            strcat(buf, package->data);
            data_len = get_total_length(package->total_length)-IHL;
            memcpy(buf+cur, package->data, data_len);
            cur += data_len;
//            print_ip_package(package);
        }
        else{
            break;
        }
    }
    fclose(temp);
    
    FILE *clear = fopen("temp.bin", "w");
    fclose(clear);
    
}




