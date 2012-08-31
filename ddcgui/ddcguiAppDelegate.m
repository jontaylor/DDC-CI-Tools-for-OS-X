//
//  ddcguiAppDelegate.m
//  ddcgui
//
//  Created by Jonathan Taylor on 03/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ddcguiAppDelegate.h"

@implementation ddcguiAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application
	[controller_object setControlsToCurrentValues];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) theApplication {
	return YES;
}


@end
