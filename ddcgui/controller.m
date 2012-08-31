//
//  controller.m
//
//  Created by Jonathan Taylor on 03/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "controller.h"
#import "ddc.h"

@implementation controller

- (void)setControlAndUpdateLabel:(int)control :(id)slider :(id)label {
	//[label takeIntValueFrom: slider];
	[self setLabelValue:label :[slider intValue]];
	[self setControlValue:control :[slider intValue]];
}

- (void)setSliderValue:(id)slider :(int)value {
	[slider setIntValue:value];
}

- (void)setLabelValue:(id)label :(int)value {
	[label setIntValue:value];
}

- (void)setControlValue:(int)control :(int)value {
	struct DDCWriteCommand write_command;                                                         
	write_command.control_id = control;
	write_command.new_value = value;
	ddc_write(0, &write_command);
}


- (int)readControlValue:(int)control {
	struct DDCReadCommand read_command;
	read_command.control_id = control;
    
	ddc_read(0, &read_command);
	return ((int)read_command.response.current_value);
}

- (void)setControlsToCurrentValues{
	//Brightness
	int brightness_value = [self readControlValue:0x10];
	[self setSliderValue:brightnessSlider :brightness_value];
	[self setLabelValue:brightnessValueLabel :brightness_value];
	
	//Contrast
	int contrast_value = [self readControlValue:0x12];
	[self setSliderValue:contrastSlider :contrast_value];
	[self setLabelValue:contrastValueLabel :contrast_value];
	
	
}

- (IBAction)setBrightness:(id)sender {
	[self setControlAndUpdateLabel:0x10 :sender :brightnessValueLabel];
	
}

- (IBAction)setContrast:(id)sender {
	[self setControlAndUpdateLabel:0x12 :sender :contrastValueLabel];
	
}

@end
