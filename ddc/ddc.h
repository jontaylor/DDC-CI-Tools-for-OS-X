/*
 *  ddc.h
 *  ddc
 *
 *  Created by Jonathan Taylor on 07/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif 

#include <IOKit/IOKitLib.h>
#include <ApplicationServices/ApplicationServices.h>
#include <IOKit/i2c/IOI2CInterface.h>

#define RESET 0x04
#define RESET_BRIGHTNESS_AND_CONTRAST 0x05
#define RESET_GEOMETRY 0x06
#define RESET_COLOR 0x08	
#define BRIGHTNESS 0x10  //OK
#define CONTRAST 0x12 //OK
#define RED_GAIN 0x16
#define GREEN_GAIN 0x18
#define BLUE_GAIN 0x1A
#define AUTO_SIZE_CENTER 0x1E	
#define WIDTH 0x22
#define HEIGHT 0x32
#define VERTICAL_POS	0x30
#define HORIZONTAL_POS 0x20
#define PINCUSHION_AMP 0x24
#define PINCUSHION_PHASE 0x42
#define KEYSTONE_BALANCE 0x40
#define PINCUSHION_BALANCE 0x26
#define TOP_PINCUSHION_AMP 0x46
#define TOP_PINCUSHION_BALANCE 0x48
#define BOTTOM_PINCUSHION_AMP 0x4A
#define BOTTOM_PINCUSHION_BALANCE 0x4C
#define VERTICAL_LINEARITY 0x3A
#define VERTICAL_LINEARITY_BALANCE 0x3C
#define HORIZONTAL_STATIC_CONVERGENCE 0x28
#define VERTICAL_STATIC_CONVERGENCE 0x28
#define MOIRE_CANCEL 0x56
#define INPUT_SOURCE 0x60
#define AUDIO_SPEAKER_VOLUME 0x62
#define RED_BLACK_LEVEL 0x6C
#define GREEN_BLACK_LEVEL 0x6E
#define BLUE_BLACK_LEVEL 0x70

#define SETTINGS 0xB0                  //unsure on this one	
#define ON_SCREEN_DISPLAY 0xCA
#define OSD_LANGUAGE 0xCC
#define DPMS 0xD6
#define MAGIC_BRIGHT 0xDC //unsure
#define VCP_VERSION 0xDF
#define COLOR_PRESET 0xE0
#define POWER_CONTROL 0xE1	
	
#define TOP_LEFT_SCREEN_PURITY 0xE8
#define TOP_RIGHT_SCREEN_PURITY 0xE9
#define BOTTOM_LEFT_SCREEN_PURITY 0xE8
#define BOTTOM_RIGHT_SCREEN_PURITY 0xEB	
	
	

    struct DDCWriteCommand
    {
    	UInt8 control_id;
    	UInt8 new_value;
    };

    struct DDCReadResponse
    {
    	UInt8 max_value;
    	UInt8 current_value;
    };

    struct DDCReadCommand 
    {
    	UInt8 control_id;
        size_t reply_bytes;
        unsigned char*  reply_buffer;
    	struct DDCReadResponse response;
    };


    CGDisplayCount number_of_displays();
    int primary_display_id();
    int ddc_write(int display_id, struct DDCWriteCommand * p_write );
    int ddc_read(int display_id, struct DDCReadCommand * p_read);
    
    void edid_test(int display_id);

#ifdef __cplusplus
}
#endif 