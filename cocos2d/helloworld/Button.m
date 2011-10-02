//
//  Button.m
//  cocos2d-ios
//
//  Created by onlyu on 11-10-1.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "Button.h"


@implementation Button


- (id) init
{
    if ((self = [super init])) {
        _label = [[CCLabelTTF labelWithString:@"" fontName:@"Marker Felt" fontSize:64] retain];
        _autoSize = YES;
        
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
		self.isTouchEnabled = YES;
#elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
		self.isMouseEnabled = YES;
#endif
        onTouchBegin = nil;
        onTouchCanceled = nil;
        onTouchEnded = nil;
        onTouchMoved = nil;
    }
    return self;
}

- (CGSize) contentSize
{
    if (_autoSize) {
        return _label.contentSize;
    }
    return contentSize_;
}

- (void) setContentSize:(CGSize)contentSize
{
    _autoSize = false;
    [super setContentSize:contentSize];
}

- (void) setText:(NSString *)text
{
    if (!text) {
        NSLog(@"invalid text");
        return;
    }
    [_label setString:text];
}

-(void) addListener:(EventCode)event target:(id)target select:(SEL)selector
{
    NSMethodSignature *sig = nil;
    if (event == EVENT_TOUCH_BEGIN) {
        if (target && selector) {
            sig = [[target class] instanceMethodSignatureForSelector:selector];
            
            onTouchBegin = nil;
            onTouchBegin = [NSInvocation invocationWithMethodSignature:sig];
            [onTouchBegin setTarget:target];
            [onTouchBegin setSelector:selector];
            if ([sig numberOfArguments] == 3) {
                [onTouchBegin setArgument:&self atIndex:2];
            }
            [onTouchBegin retain];
        }
    } else if (event == EVENT_TOUCH_CANCELED) {
        if (target && selector) {
            sig = [[target class] instanceMethodSignatureForSelector:selector];
            
            onTouchCanceled = nil;
            onTouchCanceled = [NSInvocation invocationWithMethodSignature:sig];
            [onTouchCanceled setTarget:target];
            [onTouchCanceled setSelector:selector];
            if ([sig numberOfArguments] == 3) {
                [onTouchCanceled setArgument:&self atIndex:2];
            }
            [onTouchCanceled retain];
        }
    } else if (event == EVENT_TOUCH_ENDED) {
        if (target && selector) {
            sig = [[target class] instanceMethodSignatureForSelector:selector];
            
            onTouchEnded = nil;
            onTouchEnded = [NSInvocation invocationWithMethodSignature:sig];
            [onTouchEnded setTarget:target];
            [onTouchEnded setSelector:selector];
            if ([sig numberOfArguments] == 3) {
                [onTouchEnded setArgument:&self atIndex:2];
            }
            [onTouchEnded retain];
        }
    } else if (event == EVENT_TOUCH_MOVED) {
        if (target && selector) {
            sig = [[target class] instanceMethodSignatureForSelector:selector];
            
            onTouchMoved = nil;
            onTouchMoved = [NSInvocation invocationWithMethodSignature:sig];
            [onTouchMoved setTarget:target];
            [onTouchMoved setSelector:selector];
            if ([sig numberOfArguments] == 3) {
                [onTouchMoved setArgument:&self atIndex:2];
            }
            [onTouchMoved retain];
        }
    }
}

- (void) draw
{
    CGSize s = self.contentSize;
	CGPoint vertices[4]={
		ccp(0,0),ccp(s.width,0),
		ccp(s.width,s.height),ccp(0,s.height),
	};
	ccDrawPoly(vertices, 4, YES);
    [_label draw];
}

- (void) dealloc
{
    if (onTouchBegin) {
        [onTouchBegin release];
        onTouchBegin = nil;
    }
    if (onTouchCanceled) {
        [onTouchCanceled release];
        onTouchCanceled = nil;
    }
    if (onTouchEnded) {
        [onTouchEnded release];
        onTouchEnded = nil;
    }
    if (onTouchMoved) {
        [onTouchMoved release];
        onTouchEnded = nil;
    }
    [_label release];
    [super dealloc];
}

#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
-(void) registerWithTouchDispatcher
{
	[[CCTouchDispatcher sharedDispatcher] addTargetedDelegate:self priority:INT_MIN+1 swallowsTouches:YES];
}


-(BOOL) ccTouchBegan:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch begin");
    CGPoint touchLocation = [touch locationInView: [touch view]];
	touchLocation = [[CCDirector sharedDirector] convertToGL: touchLocation];
    if ( self.visible ) {
        CGPoint local = [self convertToNodeSpace:touchLocation];
        
        CGRect r = CGRectMake(0, 0, self.contentSize.width, self.contentSize.height);
        r.origin = CGPointZero;
        
        if( CGRectContainsPoint( r, local ) ){
            if (onTouchBegin) {
                [onTouchBegin invoke];
            }
            
            return YES;
        }
            

    }
	return NO;
}

-(void) ccTouchEnded:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch ended");
    if (onTouchEnded) {
        [onTouchEnded invoke];
    }
    
	//NSAssert(state == kCCMenuStateTrackingTouch, @"[Menu ccTouchEnded] -- invalid state");
}

-(void) ccTouchCancelled:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch moved");
    if (onTouchCanceled) {
        [onTouchCanceled invoke];
    }
    
	//NSAssert(state == kCCMenuStateTrackingTouch, @"[Menu ccTouchCancelled] -- invalid state");
}

-(void) ccTouchMoved:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch moved");
    if (onTouchMoved) {
        [onTouchMoved invoke];
    }
	//NSAssert(state == kCCMenuStateTrackingTouch, @"[Menu ccTouchMoved] -- invalid state");
}

#endif
@end
