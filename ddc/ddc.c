/*
 *  ddc.c
 *  ddc
 *
 *  Created by Jonathan Taylor on 07/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <assert.h>
#include <stdio.h>
#include "ddc.h"

IOI2CConnectRef display_connection(int display_id); 

//public

//number_of_displays - returns CGDisplayCount
//primary_display_id - returns CGDirectDisplayID
//ddc_write(monitor_id,DDCWriteCommand pointer)
//ddc_read(monitor_id,DDCReadCommand pointer)


//private

//display_connection

int ddc_write(int display_id, struct DDCWriteCommand * p_write ) {
    UInt8 data[128];
    IOI2CRequest request;
    kern_return_t kr;
    
    IOI2CConnectRef    connect = display_connection(display_id);
    
    bzero( &request, sizeof(request));
    
    request.commFlags = 0;
    request.sendAddress = 0x6e;
    request.sendTransactionType             = kIOI2CSimpleTransactionType;
	request.sendBuffer                      = (vm_address_t) &data[0];
	request.sendBytes                       = 7; //Dubios
    
    data[0] = 0x51;
    data[1] = 0x84; //Dubios
    data[2] = 0x03;
    data[3] = (*p_write).control_id;
    data[5] = (*p_write).new_value;
    data[4] = 0x1;
    data[6] = 0x6E ^ data[0] ^ data[1] ^ data[2] ^ data[3]^ data[4]^ data[5];
    
    request.replyTransactionType    = kIOI2CNoTransactionType;
	request.replyBytes                      = 0;
    
	kr = IOI2CSendRequest( connect, kNilOptions, &request );
	IOI2CInterfaceClose( connect, kNilOptions );
	
	
	assert( kIOReturnSuccess == kr );
	if( kIOReturnSuccess != request.result)
        return 0;
    
    return 1;	    
}

int ddc_read(int display_id, struct DDCReadCommand * p_read) {
    UInt8 data[128];
    IOI2CRequest request;
    kern_return_t kr;
    UInt8 reply_data[11];
    
    IOI2CConnectRef    connect; 
    
	int successful_reads = 0;
	
	for (int i=0; i<60; i++) {
	
		bzero( &request, sizeof(request));
		
		request.commFlags = 0;
		request.sendAddress = 0x6E;
		//request.sendTransactionType             = kIOI2CSimpleTransactionType;
		request.sendTransactionType             = kIOI2CDDCciReplyTransactionType;
		request.sendBuffer                      = (vm_address_t) &data[0];
		request.sendBytes                       = 5; //Dubios
		
		data[0] = 0x51;
		data[1] = 0x82; //Dubios - Number of packets
		data[2] = 0x01; // We want to read this time
		data[3] = (*p_read).control_id;
		
		data[4] = 0x6E ^ data[0] ^ data[1] ^ data[2] ^ data[3];
		
		request.replyAddress = 0x6f; //think this should be 6F
		//request.replyTransactionType    = kIOI2CSimpleTransactionType;
		request.replyTransactionType    = kIOI2CDDCciReplyTransactionType;
		//request.replyBuffer                     = (vm_address_t) &(*p_read).reply_buffer[0];
		//request.replyBytes                      = sizeof(*p_read).reply_buffer;
		
		

		request.replyBuffer = (vm_address_t) &reply_data[0] ;
		request.replyBytes = sizeof(reply_data);
		request.minReplyDelay = 50 * 10000;
		
		
		
		
		int calculated_checksum;
	
    
		bzero( &reply_data[0], request.replyBytes);
		
		connect = display_connection(display_id);
		kr = IOI2CSendRequest( connect, kNilOptions, &request );
		
		//EDIDDump( reply_data, request.replyBytes );
		
		calculated_checksum = 0x6f ^ 0x51 ^ reply_data[1] ^ reply_data[2] ^ reply_data[3] ^ reply_data[4]^ reply_data[5]^ reply_data[6]^ reply_data[7]^ reply_data[8]^ reply_data[9]; 

		//fprintf(stderr, "calculated checksum: ");
		//fprintf(stderr, "0x%02x,", calculated_checksum);
		//fprintf(stderr, "\nreturned checksum: ");
		//fprintf(stderr, "0x%02x,", reply_data[10]);
		//fprintf(stderr, "\n");
		
		if ((reply_data[10] == calculated_checksum) && reply_data[4] == data[3] ) {	
			successful_reads++;
			if (successful_reads> 1) {
                //printf("Tries required to get data: %d \n", i);
				//printf("Read address was %d", reply_data[4]);
				break;
			}
			
		}
		//fprintf(stderr, "READ ERROR\n");
		
	}
	
	IOI2CInterfaceClose( connect, kNilOptions );
	
    (*p_read).response.max_value = reply_data[7];
	
	
	
    (*p_read).response.current_value = reply_data[9];
	
	//EDIDDump( reply_data, request.replyBytes );

    //printf("reply buffer is ", reply_data);
	
	assert( kIOReturnSuccess == kr );
	if( kIOReturnSuccess != request.result)
	{
        printf("Error getting result\n");
        return 0;
    }
    
    return 1;
}



CGDisplayCount number_of_displays() {
	CGDisplayCount num_displays;
    CGGetActiveDisplayList(0, NULL, &num_displays);
	return num_displays;
}

int primary_display_id() {
	CGDirectDisplayID* display_ids;
	CGDisplayCount num_displays;
	
	num_displays = number_of_displays();
	
	display_ids = (CGDirectDisplayID*)malloc(sizeof(CGDirectDisplayID)*num_displays);
	CGGetActiveDisplayList(num_displays, display_ids, &num_displays);
	
	for(int i = 0; i < number_of_displays(); i++)
	{
		if (CGMainDisplayID() == display_ids[i]) {
			return i;
		}
    }
	return -1;
}



IOI2CConnectRef display_connection(int display_id) {
    kern_return_t kr;
	io_service_t  framebuffer, interface;
	IOOptionBits  bus;
	IOItemCount       busCount;
	
	
	//Hack to try and set this up for the secondary display
	
	CGDirectDisplayID priDisplayID;
	
	CGDirectDisplayID secDisplayID;
	
	
	CGDisplayCount num_displays;
	CGDirectDisplayID* display_ids;
	CGDisplayErr err = noErr;
	
	err = CGGetActiveDisplayList(0, NULL, &num_displays);
	//if (err != CGDisplayNoErr || num_displays == 0)
	//	return err;
	
	display_ids = (CGDirectDisplayID*)malloc(sizeof(CGDirectDisplayID)*num_displays);
	
	err = CGGetActiveDisplayList(num_displays, display_ids, &num_displays);
	//require_noerr(err,cleanup); 
	
	if (CGMainDisplayID () == display_ids[0])
	{
		priDisplayID = display_ids[0];
		secDisplayID = display_ids[1];
	}
	else
	{
		priDisplayID = display_ids[1];
		secDisplayID = display_ids[0];
		
	}

	
	
	framebuffer = CGDisplayIOServicePort(display_ids[display_id]);
	
	{
		io_string_t path;
		kr = IORegistryEntryGetPath(framebuffer, kIOServicePlane, path);
		assert( KERN_SUCCESS == kr );
		//printf("\nUsing device: %s\n", path);
		
		kr = IOFBGetI2CInterfaceCount( framebuffer, &busCount );
		assert( kIOReturnSuccess == kr );
		
		for( bus = 0; bus < busCount; bus++ )
		{
			IOI2CConnectRef  connect;
			
			kr = IOFBCopyI2CInterfaceForBus(framebuffer, bus, &interface);
			if( kIOReturnSuccess != kr)
				continue;
			
			kr = IOI2CInterfaceOpen( interface, kNilOptions, &connect );
			
			IOObjectRelease(interface);
			assert( kIOReturnSuccess == kr );
			if( kIOReturnSuccess != kr)
				continue;
			
			return connect;
			
			//IOI2CInterfaceClose( connect, kNilOptions );
		}
	}

}

SInt32 EDIDSum( const UInt8 * bytes, IOByteCount len )
{
    int i,j;
    UInt8 sum;

    for (j=0; j < len; j += 128)
    {
	sum = 0;
	for (i=0; i < 128; i++)
	    sum += bytes[j+i];
	if(sum)
	    return (j/128);
    }
    return (-1);
}

void EDIDDump( const UInt8 * bytes, IOByteCount len )
{
    int i;

    fprintf(stderr, "/*    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F */");
    for (i = 0; i < len; i++)
    {
        if( 0 == (i & 15))
            fprintf(stderr, "\n    ");
        fprintf(stderr, "0x%02x,", bytes[i]);
    }
    fprintf(stderr, "\n");
}


void EDIDRead( IOI2CConnectRef connect )
{
    kern_return_t	kr;
    IOI2CRequest	request;
    UInt8		data[128];
    int 		i;

    bzero( &request, sizeof(request) );

    request.commFlags	    		= 0;

    request.sendAddress			= 0xA0;
    request.sendTransactionType		= kIOI2CSimpleTransactionType;
    request.sendBuffer			= (vm_address_t) &data[0];
    request.sendBytes	    		= 0x01;
    data[0] 		    		= 0x00;

    request.replyAddress		= 0xA1;
    request.replyTransactionType	= kIOI2CSimpleTransactionType;
    request.replyBuffer	    		= (vm_address_t) &data[0];
    request.replyBytes	    		= sizeof(data);
    bzero( &data[0], request.replyBytes );

    kr = IOI2CSendRequest( connect, kNilOptions, &request );
    assert( kIOReturnSuccess == kr );
    fprintf(stderr, "/* Read result 0x%x, 0x%lx bytes */\n", request.result, request.replyBytes);
    if( kIOReturnSuccess != request.result)
        return;

    EDIDDump( data, request.replyBytes );

    i = EDIDSum( &data[0], request.replyBytes );

    if( i >= 0)
	fprintf(stderr, "/* Block %d checksum bad */\n", i);
    else
	fprintf(stderr, "/* Checksums ok */\n");

    
}

void edid_test(int display_id) {
    IOI2CConnectRef    connect = display_connection(display_id);
    
    EDIDRead(connect );
    
}
