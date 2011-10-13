//
//  MainMenu.m
//  cocos2d-ios
//
//  Created by onlyu on 11-9-30.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "MainMenu.h"
#import "StartMenu.h"
#import "Button.h"
#import "TetrisGame.h"


@implementation MainMenu

static id sharedMainMenu_ = NULL;
+ (id) sharedMainMenu
{
    if (!sharedMainMenu_) {
        sharedMainMenu_ = [[MainMenu alloc] init];
    }
    return sharedMainMenu_;
}

- (id) init
{
    if ((self = [super init])) {
        Button *button = [[Button alloc] init];
        [button setText:@"Pause"];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(showMenuCallback:)];
		// ask director the the window size
		CGSize size = [[CCDirector sharedDirector] winSize];
        
		// position the label on the center of the screen
        
		button.position =  ccp(size.width - button.contentSize.width, size.height - button.contentSize.height);
		
        [self addChild:button];
        
        
        _scoreLabel = [[CCLabelTTF labelWithString:@"Score:0" fontName:@"Marker Felt" fontSize:32] retain];
        _scoreLabel.position = ccp(750, 700);
        [self addChild:_scoreLabel];
        
        _levelLabel = [[CCLabelTTF labelWithString:@"Level:1" fontName:@"Marker Felt" fontSize:32] retain];
        _levelLabel.position = ccp(750, 600);
        [self addChild:_levelLabel];
        
        _highScoreLabel = [[CCLabelTTF labelWithString:@"High Score:0" fontName:@"Marker Felt" fontSize:32] retain];
        _highScoreLabel.position = ccp(750, 500);
        [self addChild:_highScoreLabel];
        
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

- (void) showMenuCallback:(id)sender
{
    self.visible = false;
    [StartMenu sharedStartMenu].visible = true;
    TetrisGame *game = [TetrisGame sharedTetrisGame];
    game.visible = false;
    [game pause:YES];
}

- (void) setScore:(int)score
{
    [_scoreLabel setString:[NSString stringWithFormat:@"Score:%d", score]];
}

- (void) setLevel:(int)level
{
    [_levelLabel setString:[NSString stringWithFormat:@"Level:%d", level]];
}

- (void) setHighScore:(int)highScore
{
    [_highScoreLabel setString:[NSString stringWithFormat:@"High Score:%d", highScore]];
}

- (void)dealloc
{
    [_scoreLabel release];
    [_levelLabel release];
    [super dealloc];
}

@end
