//
//  NSApplication.h
//
//  Created by Jonathan Taylor on 03/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSApplication : NSResponder {
    IBOutlet id delegate;
    IBOutlet NSMenu *dockMenu;
}
- (IBAction)activateContextHelpMode:(id)sender;
- (IBAction)arrangeInFront:(id)sender;
- (IBAction)hide:(id)sender;
- (IBAction)hideOtherApplications:(id)sender;
- (IBAction)miniaturizeAll:(id)sender;
- (IBAction)orderFrontCharacterPalette:(id)sender;
- (IBAction)orderFrontColorPanel:(id)sender;
- (IBAction)orderFrontDataLinkPanel:(id)sender;
- (IBAction)orderFrontStandardAboutPanel:(id)sender;
- (IBAction)runPageLayout:(id)sender;
- (IBAction)showHelp:(id)sender;
- (IBAction)stop:(id)sender;
- (IBAction)terminate:(id)sender;
- (IBAction)unhide:(id)sender;
- (IBAction)unhideAllApplications:(id)sender;
@end
