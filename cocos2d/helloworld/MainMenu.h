//
//  MainMenu.h
//  cocos2d-ios
//
//  Created by onlyu on 11-9-30.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "cocos2d.h"


@interface MainMenu : CCLayer {
    CCLabelTTF *_scoreLabel;
    CCLabelTTF *_levelLabel;
    CCLabelTTF *_highScoreLabel;
}

+ (id) sharedMainMenu;

- (void) setScore:(int)score;
- (void) setLevel:(int)level;
- (void) setHighScore:(int)highScore;

@end