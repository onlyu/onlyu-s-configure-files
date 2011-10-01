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
    if (event == EVENT_TOUCH_BEGIN) {
        NSMethodSignature *sig = nil;
        if (target && selector) {
            sig = [[target class] instanceMethodSignatureForSelector:selector];
            
            onClick = nil;
            onClick = [NSInvocation invocationWithMethodSignature:sig];
            [onClick setTarget:target];
            [onClick setSelector:selector];
            if ([sig numberOfArguments] == 3) {
                [onClick setArgument:&self atIndex:2];
            }
            [onClick retain];
        }
    }
}

- (void) draw
{
    [_label draw];
}

- (void) dealloc
{
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
            if (onClick) {
                [onClick invoke];
            }
            return YES;
        }
            

    }
	return NO;
}

-(void) ccTouchEnded:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch ended");
	//NSAssert(state == kCCMenuStateTrackingTouch, @"[Menu ccTouchEnded] -- invalid state");
}

-(void) ccTouchCancelled:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch moved");
	//NSAssert(state == kCCMenuStateTrackingTouch, @"[Menu ccTouchCancelled] -- invalid state");
}

-(void) ccTouchMoved:(UITouch *)touch withEvent:(UIEvent *)event
{
    NSLog(@"touch moved");
	//NSAssert(state == kCCMenuStateTrackingTouch, @"[Menu ccTouchMoved] -- invalid state");
}

#endif
@end
