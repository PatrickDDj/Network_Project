//
//  configuration.h
//  20185697_final_project
//
//  Created by PatrickDD on 2020/12/2.
//

//#ifndef configuration_h
//#define configuration_h
//
//#include <stdio.h>
//
//#endif /* configuration_h */

#define VERSION 4   //ipv4
#define IHL 5
#define PACKAGE_MAX_SIZE 65535
#define DATA_MAX_SIZE PACKAGE_MAX_SIZE-IHL
#define SAERVICE 0x11
#define TTL 0x00FF    //255

#define TCP 0x0006  //6
#define UDP 0x0011  //17

#define FRAGMENT_MAX_SIZE 8

#define FRAME_MAX_SIZE 1536
#define PAYLOAD_MAX_SIZE 1500


typedef unsigned char byte;

static byte IDENTIFICATION[2] = {'D', 'D'};

static byte IP_ADDR_SENDER[4] = {192, 168, 0, 0};
static byte IP_ADDR_RECEIVER[4] = {192, 168, 1, 1};

static byte MAC_ADDR_SENDER[6] = {0x3a,0xaf,0x21,0x9e,0x5f,0x85};
static byte MAC_ADDR_RECEIVER[6] = {0x3b,0xaf,0x21,0x9e,0x5f,0x85};

 
