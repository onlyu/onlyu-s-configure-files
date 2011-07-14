//
//  boll.m
//  cocos2d-ios
//
//  Created by 兰 清 on 11-7-11.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "boll.h"


@implementation Boll

-(float) radius
{
    return self.contentSize.width / 2;
}

+(id) bollWithTexture:(CCTexture2D *)texture
{
    return [[self alloc] initWithTexture:texture];
}

-(id) initWithTexture:(CCTexture2D *)texture
{
    if((self = [super initWithTexture:texture])){
        velocity = CGPointMake(100, 100);
    }
    return self;
}

-(void)move:(ccTime)dt
{
    self.position = ccpAdd(self.position, ccpMult(velocity, dt));

    if (self.position.y > 768 - self.radius) {
        velocity.y *= -1;
    }
    if( self.position.x < self.radius || self.position.x > 1024-self.radius){
        velocity.x *= -1;
    }
}

-(void)collideWithPaddle:(Paddle *)paddle
{
	CGRect paddleRect = paddle.rect;
	paddleRect.origin.x += paddle.position.x;
	paddleRect.origin.y += paddle.position.y;
	
	float lowY = CGRectGetMinY(paddleRect);
	float midY = CGRectGetMidY(paddleRect);
	float highY = CGRectGetMaxY(paddleRect);
	
	float leftX = CGRectGetMinX(paddleRect);
	float rightX = CGRectGetMaxX(paddleRect);
	
	if (self.position.x > leftX && self.position.x < rightX) {
        
		BOOL hit = NO;
		float angleOffset = 0.0f; 
		
		if (self.position.y > midY && self.position.y <= highY + self.radius) {
			self.position = CGPointMake(self.position.x, highY + self.radius);
			hit = YES;
			angleOffset = (float)M_PI / 2;
		}
        
		else if (self.position.y < midY && self.position.y >= lowY - self.radius) {
			self.position = CGPointMake(self.position.x, lowY - self.radius);
			hit = YES;
			angleOffset = -(float)M_PI / 2;
		}
		
		if (hit) {
			float hitAngle = ccpToAngle(ccpSub(paddle.position, self.position)) + angleOffset;
			
			float scalarVelocity = ccpLength(velocity) * 1.05f;
			float velocityAngle = -ccpToAngle(velocity) + 0.5f * hitAngle;
			
			velocity = ccpMult(ccpForAngle(velocityAngle), scalarVelocity);
		}
	}	
    
}
@end