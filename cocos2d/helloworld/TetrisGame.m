//
//  TeriesGame.m
//  cocos2d-ios
//
//  Created by onlyu on 11-10-3.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "TetrisGame.h"
#import "Button.h"

const int numberOfRow = 17;
const int numberOfCulomn = 10;
const int blockSize = 40;
    
@implementation Puzzle

const int NUM_PUZZLE_STYLE = 24;
static int puzzles[][16] = {
    
    //-----------------------
    {
        0,0,0,0,
        1,1,1,1,
        0,0,0,0,
        0,0,0,0,
    },
    {
        0,1,0,0,
        0,1,0,0,
        0,1,0,0,
        0,1,0,0,
    },
    {
        0,0,0,0,
        1,1,1,1,
        0,0,0,0,
        0,0,0,0,
    },
    
    {
        0,1,0,0,
        0,1,0,0,
        0,1,0,0,
        0,1,0,0,
    },
    //-----------------------
    {
        0,0,0,0,
        0,1,0,0,
        1,1,1,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        0,1,0,0,
        1,1,0,0,
        0,1,0,0,
    },
    {
        0,0,0,0,
        0,0,0,0,
        1,1,1,0,
        0,1,0,0,
    },
    
    {
        0,0,0,0,
        0,1,0,0,
        0,1,1,0,
        0,1,0,0,
    },
    //-----------------------
    {
        0,0,0,0,
        0,0,1,0,
        1,1,1,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        1,1,0,0,
        0,1,0,0,
        0,1,0,0,
    },
    {
        0,0,0,0,
        0,0,0,0,
        1,1,1,0,
        1,0,0,0,
    },
    
    {
        0,0,0,0,
        0,1,0,0,
        0,1,0,0,
        0,1,1,0,
    },
    //-----------------------
    {
        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        0,1,1,0,
        0,1,1,0,
        0,0,0,0,
    },
    //-----------------------
    {
        0,0,0,0,
        0,1,1,0,
        1,1,0,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        1,0,0,0,
        1,1,0,0,
        0,1,0,0,
    },
    {
        0,0,0,0,
        0,0,0,0,
        0,1,1,0,
        1,1,0,0,
    },
    {
        0,0,0,0,
        0,1,0,0,
        0,1,1,0,
        0,0,1,0,
    },
    //-----------------------
    {
        0,0,0,0,
        1,1,0,0,
        0,1,1,0,
        0,0,0,0,
    },
    {
        0,0,0,0,
        0,1,0,0,
        1,1,0,0,
        1,0,0,0,
    },
    {
        0,0,0,0,
        0,0,0,0,
        1,1,0,0,
        0,1,1,0,
    },
    {
        0,0,0,0,
        0,0,1,0,
        0,1,1,0,
        0,1,0,0,
    },
    //-----------------------
};

+ (id)getRandomPuzzel
{
    id puzzle = [[Puzzle alloc] initWithStyle:rand()%NUM_PUZZLE_STYLE];
    return puzzle;
}

- (id)initWithStyle:(int)style
{
    if ((self = [super init])) {
        _x = 0;
        _y = 0;
        _style = style;
        NSLog(@"%d",_style);
    }
    return self;
}

- (id)init
{
    if ((self = [super init])) {
        _x = 0;
        _y = 0;
        _style = 0;
    }
    return self;
}

- (BOOL)isBlock:(int)x y:(int)y
{
    int dx = x-_x;
    int dy = y-_y;
    int index = dy*4 + dx;
    if (dx < 4 && dy < 4 && dx >= 0 && dy >= 0) {
        return puzzles[_style][index] == 1;
    }
    return NO;
}

- (void)left
{
    _x --;
}

- (void)right
{
    _x ++;
}

- (void)down
{
    _y ++;
}

- (void)rotate
{
    int lowStyle = _style & 0x03;
    int hiStyle = _style - lowStyle;
    lowStyle = (lowStyle - 1)&0x03;
    _style = hiStyle + lowStyle;
}
@end

@implementation Area

- (id)init
{
    if ((self = [super init])) {
        ;
    }
    return self;
}

@end

@implementation Stats

- (id)init
{
    if ((self = [super init])) {
        _area = [[[Area alloc] init] retain];
        _currentPuzzle = [[Puzzle getRandomPuzzel] retain];
    }
    return self;
}

- (void)up
{
    [_currentPuzzle rotate];
}

- (void)down
{
    [_currentPuzzle down];
}

- (void)left
{
    [_currentPuzzle left];
}

- (void)right
{
    [_currentPuzzle right];
}


- (BOOL)isBlock:(int)x y:(int)y
{
    if ([_currentPuzzle isBlock:x y:y]) {
        return YES;
    }
    return NO;
}

- (void)dealloc
{
    [_currentPuzzle release];
    [_area release];
    [super dealloc];
}
@end

@implementation Tetris

- (id)init
{
    if ((self = [super init])) {
        _stats = [[[Stats alloc] init] retain];
    }
    return self;
}

- (int)score
{
    return 0;
}

- (void)up
{
    [_stats up];
}

- (void)down
{
    [_stats down];
}

- (void)left
{
    [_stats left];
}

- (void)right
{
    [_stats right];
}

- (BOOL)isBlock:(int)x y:(int)y
{
    return [_stats isBlock:x y:y];
}

- (void)dealloc
{
    [_stats release];
    [super dealloc];
}

@end

@implementation TetrisGame

static TetrisGame *gSharedTetrisGame = NULL;
+ (TetrisGame *) sharedTetrisGame
{
    if (!gSharedTetrisGame) {
        gSharedTetrisGame = [[[TetrisGame alloc] init] retain];
        
    }
    return gSharedTetrisGame;
}

- (id)init
{
    if ((self = [super init])) {
        self.visible = false;

        _tetris = [[[Tetris alloc] init] retain];
        
		CGSize size = [[CCDirector sharedDirector] winSize];
        
        Button *button = [[Button alloc] init];
        [button setText:@"up"];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonUpPressed:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 100, 150);
        [self addChild:button];
        
        
        button = [[Button alloc] init];
        [button setText:@"down"];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonDownPressed:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 100, 50);
        [self addChild:button];
        
        
        button = [[Button alloc] init];
        [button setText:@"left"];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonLeftPressed:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 200, 50);
        [self addChild:button];
        
        
        button = [[Button alloc] init];
        [button setText:@"right"];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonRightPressed:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 0, 50);
        [self addChild:button];
        
    }
    return self;
}

- (void)onButtonUpPressed:(id)sender
{
    [_tetris up];
}

-(void)onButtonDownPressed:(id)sender
{
    [_tetris down];
}

-(void)onButtonLeftPressed:(id)sender
{
    [_tetris left];
}

-(void)onButtonRightPressed:(id)sender
{
    [_tetris right];
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
            if (![_tetris isBlock:j y:numberOfRow-i-1]) {
                continue;
            }
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

- (void)dealloc
{
    [_tetris release];
    [super dealloc];
}

@end
