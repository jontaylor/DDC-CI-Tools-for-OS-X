//
//  ddcguiAppDelegate.h
//  ddcgui
//
//  Created by Jonathan Taylor on 03/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ddcguiAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
    IBOutlet id controller_object;
}

@property (assign) IBOutlet NSWindow *window;



@end
