//
//  GameOverPanel.m
//  cocos2d-ios
//
//  Created by onlyu on 11-10-7.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "GameOverPanel.h"
#import "Button.h"
#import "TetrisGame.h"
#import "MainMenu.h"


@implementation GameOverPanel

static id _sharedGameOverPanel = nil;

+(GameOverPanel *) sharedGameOverPanel
{
    if (!_sharedGameOverPanel) {
        _sharedGameOverPanel = [[[GameOverPanel alloc] init] retain];
    }
    return _sharedGameOverPanel;
}

-(id) init
{
    if ((self = [super init])) {
        CCLabelTTF *label = [[CCLabelTTF labelWithString:@"Game Over" fontName:@"Marker Felt" fontSize:64] retain];
        label.position = ccp(500,400);
        [self addChild:label];
        
        Button *button = [[Button alloc] init];
        [button setText:@"Restart"];
        button.position = ccp(500, 300);
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onRestart:)];
        [self addChild:button];
        
        self.visible = NO;
    }
    return self;
}

- (void) onRestart:(id) sender
{
    [[TetrisGame sharedTetrisGame] restart];
    [TetrisGame sharedTetrisGame].visible = YES;
    [[MainMenu sharedMainMenu] setVisible:YES];
    self.visible = NO;
}

-(void)dealloc
{
    [super dealloc];
}
@end
