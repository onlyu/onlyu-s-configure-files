//
//  MainUI.m
//  cocos2d-ios
//
//  Created by onlyu on 11-9-30.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "MainUI.h"
#import "StartMenu.h"
#import "MainMenu.h"

@implementation MainUI

static MainUI* sharedMainUI_ = NULL;
+ (MainUI *)sharedMainUI
{
    if (!sharedMainUI_) {
        sharedMainUI_ = [[self alloc] init];
    }
    return sharedMainUI_;
} 

- (id) init
{
    if ( (self = [super init]) ) {
        [self addChild:[StartMenu sharedStartMenu]];
        [self addChild:[MainMenu sharedMainMenu]];
    }
    return self;
}

@end
