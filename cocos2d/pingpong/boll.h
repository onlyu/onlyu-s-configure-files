//
//  boll.h
//  cocos2d-ios
//
//  Created by 兰 清 on 11-7-11.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "cocos2d.h"
#import "paddle.h"

@class Paddle;

@interface Boll : CCSprite {
    @private
    CGPoint velocity;
}

@property (nonatomic, readonly) float radius;

+(id) bollWithTexture:(CCTexture2D *)texture;

-(void)move:(ccTime) dt;
-(void)collideWithPaddle:(Paddle *)paddle;

@end
