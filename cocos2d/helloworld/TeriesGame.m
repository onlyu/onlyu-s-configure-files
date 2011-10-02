//
//  TeriesGame.m
//  cocos2d-ios
//
//  Created by onlyu on 11-10-3.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "TeriesGame.h"

const int numberOfRow = 17;
const int numberOfCulomn = 11;
const int blockSize = 40;

@implementation TeriesGame

static TeriesGame *gSharedTeriesGame = NULL;
+ (TeriesGame *) sharedTeriesGame
{
    if (!gSharedTeriesGame) {
        gSharedTeriesGame = [[TeriesGame alloc] init];
        [gSharedTeriesGame retain];
    }
    return gSharedTeriesGame;
}

- (id)init
{
    if ((self = [super init])) {
        self.visible = false;
    }
    return self;
}

- (void)draw
{
    CGPoint startPoint = CGPointMake(50, 50);
    CGSize size = CGSizeMake(blockSize * numberOfCulomn + 2, blockSize * numberOfRow + 2);
    
    // draw board
	CGPoint vertices[4]={
		ccp(startPoint.x, startPoint.y),ccp(size.width + startPoint.x,startPoint.y),
		ccp(startPoint.x + size.width, startPoint.y + size.height),ccp(startPoint.x, startPoint.y + size.height),
	};
	ccDrawPoly(vertices, 4, YES);
    //ccDrawSolidPoly(vertices, 4);
    
    // draw grids
    for (int i=0; i<numberOfRow; i++) {
        for (int j=0; j<numberOfCulomn; j++) {
            CGPoint v[4] = {
                ccp(startPoint.x + 1 + j*blockSize, startPoint.y + 1 + i*blockSize),
                ccp(startPoint.x + j*blockSize + blockSize, startPoint.y + 1 + i*blockSize),
                ccp(startPoint.x + j*blockSize + blockSize, startPoint.y + i*blockSize + blockSize),
                ccp(startPoint.x + 1 + j*blockSize, startPoint.y + i*blockSize + blockSize),
            };
            //ccDrawPoly(v, 4, YES);
            ccDrawSolidPoly(v, 4);
        }
    }
}

@end
