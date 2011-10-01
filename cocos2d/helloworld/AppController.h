//
//  AppControler.h
//  cocos2d-ios
//
//  Created by onlyu on 11-10-1.
//  Copyright 2011年 CSU. All rights reserved.
//
// When you import this file, you import all the cocos2d classes
#import "cocos2d.h"

// Application Delegate class
@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate, UIApplicationDelegate>
{
	// main UIWindow
	// The OpenGL view will be a attached to this UIWindow
    UIWindow *window;
    
}

- (void)nextFrame:(ccTime) dt;

// Make the main UIWindow a property
@property (nonatomic, retain) UIWindow *window;
@end