//
//  datalink_layer.h
//  20185697_final_project
//
//  Created by PatrickDD on 2020/12/3.
//

#include <stdio.h>
#include "configuration.h"
#include <string.h>
//#include "network_layer.c"

 
static unsigned int crc32(const unsigned char *buf, unsigned int len)
{
    unsigned int crc = 0xFFFFFFFF;
        for (int i=0;i<len;i++) {
            crc = crc ^ buf[i];
            for (int j=7;j>=0;j--) {
                crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
            }
        }
        return ~crc;
}


//frame strcuture : destination address(6)  source address(6)  ether type(2)  payload(payloadlen) fcs bit(4)
static unsigned short create_frame(byte *dst, byte *src,unsigned short protocol,byte *payload, unsigned short payloadlen, byte *frame) {
    memcpy(&frame[0],dst,6);
    memcpy(&frame[6],src,6);

    memcpy(&frame[12], &protocol, 2);
    
    memcpy(&frame[14], payload, payloadlen);
    unsigned int crc32_result = crc32(frame, payloadlen+14);
    memcpy(&frame[14+payloadlen],&crc32_result,sizeof(crc32_result));
    return 18 + payloadlen;
}

static void send_frame(byte *frame_data,unsigned short len,FILE *file) {
    fwrite(&len, sizeof(len), 1, file);

    fwrite(frame_data, sizeof(byte), len, file);
    
}

static unsigned short receive_frame(FILE *file, byte *payload){
    unsigned short len;
    byte buf[FRAME_MAX_SIZE];
    
    
    byte dst[6];
    byte src[6];
    unsigned short protocol;
//    byte payload[PAYLOAD_MAX_SIZE];
    
    if(fread(&len, sizeof(len), 1, file) <= 0){
        return 0;
    }
    fread(buf, sizeof(byte), len, file);
    memcpy(&dst,buf,6);
    memcpy(&src,&buf[6],6);
    memcpy(&protocol,&buf[12],2);

    
    memcpy(payload, &buf[14], protocol);
    
    unsigned short payload_len = protocol;
    
    unsigned int crc32_result = crc32(buf, len-4);
    unsigned char crc32_result_c[4];
    memcpy(crc32_result_c,&crc32_result,sizeof(crc32_result));
    if (
        crc32_result_c[3] != buf[len-1] ||
        crc32_result_c[2] != buf[len-2] ||
        crc32_result_c[1] != buf[len-3] ||
        crc32_result_c[0] != buf[len-4]
    ) {
        printf("[ERROR] eth_frame crc32 check failed.\n");
//            continue;
    }
//    printf("%s\n", payload+20);
    return payload_len;
    
}
