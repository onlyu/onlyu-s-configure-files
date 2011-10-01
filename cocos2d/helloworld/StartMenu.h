//
//  StartMenu.h
//  cocos2d-ios
//
//  Created by onlyu on 11-9-30.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "cocos2d.h"

// 启动目录
@interface StartMenu : CCLayer {
    
}

+ (StartMenu *) sharedStartMenu;

- (void) startGameCallback:(id) sender;
- (void) quitGameCallBack:(id) sender;
- (void) aboutCallBack:(id) sender;

@end
