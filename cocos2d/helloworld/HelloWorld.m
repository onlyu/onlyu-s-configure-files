//
// cocos2d Hello World example
// http://www.cocos2d-iphone.org
//

// IMPORTANT:
//  This example ONLY shows the basic steps to render a label on the screen.
//  Some advanced options regarding the initialization were removed to simplify the sample.
//  Once you understand this example, read "HelloActions" sample.

// Needed for UIWindow, NSAutoReleasePool, and other objects
#import <UIKit/UIKit.h>
// Import the interfaces
#import "HelloWorld.h"

// HelloWorld implementation
@implementation HelloWorld

// on "init" you need to initialize your instance
-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	if( (self=[super init] )) {        
        sprite = [CCSprite spriteWithFile:@"umbella.png"];
        sprite.position = ccp(100,100);
        sprite.color = ccc3(255, 0, 0);
        [self addChild:sprite];
        
        [self schedule:@selector(nextFrame:) interval:0];

	}
	return self;
}

-(void) nextFrame:(ccTime)dt
{
#define speed 3
    static int dx=speed,dy=speed;

    CGSize size = [[CCDirector sharedDirector] winSize];
    if (sprite) {
        if (sprite.position.x + dx > size.width) {
            dx = -speed;
        }else if(sprite.position.x + dx < 0){
            dx = speed;
        }
        
        if (sprite.position.y + dy > size.height) {
            dy = -speed;
        }else if(sprite.position.y + dy < 0){
            dy = speed;
        }
        
        sprite.position = ccp(sprite.position.x + dx, sprite.position.y + dy);
    }
}

// on "dealloc" you need to release all your retained objects
- (void) dealloc
{
	// in case you have something to dealloc, do it in this method
	// in this particular example nothing needs to be released.
	// cocos2d will automatically release all the children (Label)
	
	// don't forget to call "super dealloc"
	[super dealloc];
}
@end


