//
//  paddle.m
//  cocos2d-ios
//
//  Created by 兰 清 on 11-7-10.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "paddle.h"
#import "cocos2d.h"


@implementation Paddle

-(CGRect) rect
{
    CGSize s = [texture_ contentSize];
    return CGRectMake(-s.width/2, -s.height/2, s.width, s.height);
}

-(CGRect) rectInPixel
{
    CGSize s = [texture_ contentSizeInPixels];
    return CGRectMake(-s.width/2, -s.height/2, s.width, s.height);
}


+ (id) paddleWithTexture:(CCTexture2D *)texture
{
    return [[[self alloc] initWithTexture:texture] autorelease];
}

- (id) initWithTexture:(CCTexture2D *)texture
{
    if((self = [super initWithTexture:texture])){
        state = UNGRABBED;
    }
    return self;
}

-(void) onEnter
{
    [[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:0 swallowsTouches:YES];
    [super onEnter];
}

-(void) onExit
{
    [[CCTouchDispatcher sharedDispatcher] removeDelegate:self];
    [super onExit];
}

-(BOOL)isCoverTouch:(UITouch *)touch
{
    CGPoint p = [self convertTouchToNodeSpaceAR:touch];
    CGRect r = [self rectInPixel];
    return CGRectContainsPoint(r, p);
}

-(BOOL)ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
    if(state != UNGRABBED) return NO;
    if( ![self isCoverTouch:touch] ) return NO;
    
    state = GRABBED;
    return YES;
}

-(void) ccTouchMoved:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSAssert(state == GRABBED, @"unexpected state!");
    CGPoint touchPoint = [touch locationInView:[touch view]];
    touchPoint = [[CCDirector sharedDirector] convertToGL:touchPoint];
    
    self.position = CGPointMake(touchPoint.x, self.position.y);
}

-(void) ccTouchEnded:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSAssert(state == GRABBED, @"unexpected state!");
    state = UNGRABBED;
}
@end
