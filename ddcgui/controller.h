//
//  controller.h
//
//  Created by Jonathan Taylor on 03/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface controller : NSObject {
    IBOutlet id brightnessValueLabel;
    IBOutlet id brightnessSlider;
	IBOutlet id contrastValueLabel;
    IBOutlet id contrastSlider;
	
}
- (IBAction)setBrightness:(id)sender;
- (IBAction)setContrast:(id)sender;
- (void)setControlAndUpdateLabel:(int)control :(id)slider :(id)label;
- (void)setControlsToCurrentValues;
- (int)readControlValue:(int)control;
- (void)setSliderValue:(id)slider :(int)value;
- (void)setLabelValue:(id)label :(int)value;
- (void)setControlValue:(int)control :(int)value;
	
@end
