//
//  StartMenu.m
//  cocos2d-ios
//
//  Created by onlyu on 11-9-30.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "StartMenu.h"
#import "MainMenu.h"
#import "TetrisGame.h"
#import "AppController.h"


@implementation StartMenu

static StartMenu *sharedStartMenu_ = NULL;
+ (StartMenu *)sharedStartMenu
{
    if (!sharedStartMenu_) {
        sharedStartMenu_ = [[self alloc] init];
    }
    return sharedStartMenu_;
}

- (id) init
{
    if ((self = [super init])) {
        CCMenuItem *startGameItem = [CCMenuItemFont itemFromString: @"Start Game" target: self selector:@selector(startGameCallback:)];
        CCMenuItem *aboutItem = [CCMenuItemFont itemFromString:@"About" target:self selector:@selector(aboutCallBack:)];
        //CCMenuItem *quitGameItem = [CCMenuItemFont itemFromString:@"Quit Game" target:self selector:@selector(quitGameCallBack:)];
        
        CCMenu *menu = [CCMenu menuWithItems:startGameItem, aboutItem, nil];
        [menu alignItemsVertically];
        [self addChild:menu];
    }
    return self;  
}

- (void) startGameCallback:(id)sender
{
    self.visible = false;
    [[MainMenu sharedMainMenu] setVisible:true];
    TetrisGame *game = [TetrisGame sharedTetrisGame];
    game.visible = true;
    [game start];
    [game pause:NO];
}

- (void) aboutCallBack:(id)sender
{
    self.visible = false;
}

/*
- (void) quitGameCallBack:(id)sender
{
    [[AppController sharedAppController] applicationWillTerminate:(UIApplication *)[AppController sharedAppController]];
    exit(0);
}
*/
@end
