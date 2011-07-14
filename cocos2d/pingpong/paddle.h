//
//  paddle.h
//  cocos2d-ios
//
//  Created by 兰 清 on 11-7-10.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "cocos2d.h"

typedef enum _PaddleState{
    GRABBED,
    UNGRABBED,
}PaddleState;

@interface Paddle : CCSprite <CCTargetedTouchDelegate> {
@private
    PaddleState state;
}

@property(nonatomic, readonly) CGRect rect;
@property(nonatomic, readonly) CGRect rectInPixel;

+(id)paddleWithTexture:(CCTexture2D *)texture;

@end
