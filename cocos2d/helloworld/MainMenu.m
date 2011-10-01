//
//  MainMenu.m
//  cocos2d-ios
//
//  Created by onlyu on 11-9-30.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "MainMenu.h"
#import "StartMenu.h"


@implementation MainMenu

static MainMenu *sharedMainMenu_ = NULL;
+ (MainMenu *) sharedMainMenu
{
    if (!sharedMainMenu_) {
        sharedMainMenu_ = [[MainMenu alloc] init];
    }
    return sharedMainMenu_;
}

- (id) init
{
    if ((self = [super init])) {
        CCLabelTTF *label = [CCLabelTTF labelWithString:@"Menu" fontName:@"Marker Felt" fontSize:64];
        
		// ask director the the window size
		CGSize size = [[CCDirector sharedDirector] winSize];
        
		// position the label on the center of the screen
		label.position =  ccp( size.width - label.contentSize.width , size.height - label.contentSize.height );
		
        [self addChild:label];
        
        self.visible = false;
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
		self.isTouchEnabled = YES;
#elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
		self.isMouseEnabled = YES;
#endif
        [self setContentSize:size];
    }
    return self;
}


-(BOOL) ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"MainMenu Touch");
    [StartMenu sharedStartMenu].visible = true;
	return YES;
}

@end
