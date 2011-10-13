//
//  TeriesGame.m
//  cocos2d-ios
//
//  Created by onlyu on 11-10-3.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "TetrisGame.h"
#import "Button.h"
#import "GameOverPanel.h"
#import "MainMenu.h"

const int numberOfRow = 17;
const int numberOfCulomn = 10;
const int blockSize = 40;
    
@implementation Puzzle

const int NUM_PUZZLE_STYLE = 24;
static int puzzles[][20] = {
    
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
        0,1,0,0,
        0,1,0,0,
    },

    {
        0,0,0,0,
        1,0,0,0,
        1,1,1,0,
        0,0,0,0,
    },    {
        0,0,0,0,
        0,1,0,0,
        0,1,0,0,
        1,1,0,0,
    },
    {
        0,0,0,0,
        0,0,0,0,
        1,1,1,0,
        0,0,1,0,
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

@synthesize x = _x;
@synthesize y = _y;

+ (id)puzzleWithRandomStyle
{
    id puzzle = [[Puzzle alloc] initWithStyle:rand()%NUM_PUZZLE_STYLE];
    return puzzle;
}

+(id)puzzleWithPuzzle:(Puzzle *)puzzle
{
    return [[Puzzle alloc] initWithPuzzle:puzzle];
}

- (id)initWithPuzzle:(Puzzle *)puzzle
{
    if ((self = [super init])) {
        _x = puzzle->_x;
        _y = puzzle->_y;
        _style = puzzle->_style;
    }
    return self;
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

-(int *)getBlocks
{
    static int blocks[8];
    int index = 0;
    for (int i=_x; i<_x+4; i++) {
        for (int j=_y; j<_y+4; j++) {
            if ([self isBlock:i y:j]) {
                blocks[index++] = i;
                blocks[index++] = j;
                if (index >= 8) {
                    return blocks;
                }
            }
        }
    }
    return blocks;
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


@interface Area(private) 
- (int) get_data:(int)x y:(int)y;
- (void) set_data:(int)x y:(int)y value:(int)value;
- (int) consumeLastFullLine;
@end

@implementation Area

- (id)init
{
    if ((self = [super init])) {
        _data = (int*)calloc(numberOfRow * numberOfCulomn, sizeof(int));
    }
    return self;
}

-(void)dealloc
{
    if (_data) {
        free(_data);
        _data = nil;
    }
    [super dealloc];
}

-(BOOL)canPlace:(Puzzle *)puzzle
{
    int *blocks = [puzzle getBlocks];
    for (int i=0; i<4; i++) {
        //NSLog(@"block %d(%d,%d) ret=%d", i, blocks[i*2], blocks[i*2+1],[self get_data:blocks[i*2] y:blocks[i*2+1]]);
        if (blocks[i*2+1]<0) {
            continue;
        }
        int ret = [self get_data:blocks[i*2] y:blocks[i*2+1]];
        if (ret != 0) {
            return NO;
        }
    }
    return YES;
}

-(void)placePuzzle:(Puzzle *)puzzle
{
    int *blocks = [puzzle getBlocks];
    for (int i=0; i<4; i++) {
        if (blocks[i*2+1]<0) {
            continue;
        }
        [self set_data:blocks[i*2] y:blocks[i*2+1] value:1];    
    }
}

-(BOOL)isBlock:(int)x y:(int)y
{
    return [self get_data:x y:y] == 1;
}

-(int) consumeFullLines
{
    int fullLineNumber = 0;
    while ([self consumeLastFullLine] == 1) {
        fullLineNumber ++;
    }
    return fullLineNumber;
}

// private methods
-(int) get_data:(int)x y:(int)y
{
    if (x<0 || y<0 || x>=numberOfCulomn || y>=numberOfRow) {
        return -1;
    }
    //NSLog(@"%d,%d,%d",x,y, _data[y*numberOfCulomn + x]);
    return _data[y*numberOfCulomn + x];
}

-(void) set_data:(int)x y:(int)y value:(int)value
{
    if (x<0 || y<0 || x>=numberOfCulomn || y>=numberOfRow) {
        return ;
    }
    _data[y*numberOfCulomn + x] = value;
}

-(int) consumeLastFullLine
{
    int full = -1;
    int i,j;
    for (j=numberOfRow-1; j>=0; j--) {
        BOOL flag = YES;
        for (i=0; i<numberOfCulomn; i++) {
            if ([self get_data:i y:j] != 1) {
                flag = NO;
                break;
            }
        }
        if (flag) {
            full = j;
            break;
        }
    }
    if (full != -1) {
        NSLog(@"full line:%d", full);
        for (j = full; j>0; j--) {
            for (int i=0; i<numberOfCulomn; i++) {
                [self set_data:i y:j value:[self get_data:i y:j-1]];
            }
        }
        return 1;
    }
    return 0;
}

@end

@interface Stats(private)
-(void)genNextPuzzle;
@end

@implementation Stats

@synthesize score = _score;
@synthesize highScore = _highScore;
@synthesize level = _level;

//15088063037

- (id)init
{
    if ((self = [super init])) {
        _area = [[[Area alloc] init] retain];
        //[self genNextPuzzle];
        [NSTimer scheduledTimerWithTimeInterval:0.05 target:self selector:@selector(onTick:) userInfo:nil repeats:YES];
        _isSpeedUp = NO;
        _level = 1;
        _tick = 0;
        
        _isPaused = NO;
        _isRunning = NO;
        _totalLine = 0;
        
        self.highScore = [[NSUserDefaults standardUserDefaults] integerForKey:@"highScore"];
        
        SimpleAudioEngine *sae = [SimpleAudioEngine sharedEngine];
        [[CDAudioManager sharedManager] setResignBehavior:kAMRBStopPlay autoHandle:YES];
        [sae preloadEffect:@"left_right.wav"];
        [sae preloadEffect:@"rot.wav"];
        [sae preloadEffect:@"wrong.wav"];
        [sae preloadEffect:@"speedup.wav"];
        [sae preloadEffect:@"add_score.wav"];
        
        _leftRightSound = [[sae soundSourceForFile:@"left_right.wav"] retain];
        _rotSound = [[sae soundSourceForFile:@"speedup.wav"] retain];
        _wrongSound = [_leftRightSound retain]; //[[sae soundSourceForFile:@"wrong.wav"] retain];
        _speedUpSound = [[sae soundSourceForFile:@"speedup.wav"] retain];
        _addScoreSound = [[sae soundSourceForFile:@"add_score.wav"] retain];
    }
    return self;
}

- (void)dealloc
{
    [_leftRightSound release];
    [_rotSound release];
    [_wrongSound release];
    [_speedUpSound release];
    [_addScoreSound release];
    [SimpleAudioEngine end];
    
    [_currentPuzzle release];
    [_area release];
    [super dealloc];
}

- (void) setHighScore:(int)highScore
{
    _highScore = highScore;
    [[NSUserDefaults standardUserDefaults] setInteger:highScore forKey:@"highScore"];
    [[MainMenu sharedMainMenu] setHighScore:highScore];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void) addScore:(int)lines
{
    if (lines <= 0) {
        return;
    }
    [_addScoreSound play];
    // 基本分
    self.score += lines * _level;
    // 奖励分
    if (lines == 2) {
        self.score += 1 * _level;
    } else if (lines == 3) {
        self.score += 3 * _level;
    } else if (lines == 4) {
        self.score += 5 * _level;
    }
    if (self.score > _highScore) {
        [self setHighScore:self.score];
    }
    // 等级提升
    _totalLine += lines;
    if (_totalLine >= _level * 50) {
        self.level ++;
    }
}

- (void) setScore:(int)score
{
    _score = score;
    [[MainMenu sharedMainMenu] setScore:self.score];
}

- (void) setLevel:(int)level
{
    _level = level;
    [[MainMenu sharedMainMenu] setLevel:level];
}

- (void) speedup:(BOOL)flag
{
    _isSpeedUp = flag;
    if (flag) {
        //[_speedUpSound play];
    }
}

- (void)up
{
    if (!_isRunning || _isPaused) return;
    Puzzle *newPuzzle = [Puzzle puzzleWithPuzzle:_currentPuzzle];
    [newPuzzle rotate];
    if ([_area canPlace:newPuzzle]) {
        [_currentPuzzle release];
        _currentPuzzle = [newPuzzle retain];
        [_rotSound play];
    } else {
        [_wrongSound play];
    }
}

- (void)down
{
    if (!_isRunning || _isPaused) return;
    Puzzle *newPuzzle = [Puzzle puzzleWithPuzzle:_currentPuzzle];
    [newPuzzle down];
    if ([_area canPlace:newPuzzle]) {
        [_currentPuzzle release];
        _currentPuzzle = [newPuzzle retain];
        if (_isSpeedUp) {
            //[_leftRightSound play];
        }

    }else{
        if (_currentPuzzle.y < 0) {
            NSLog(@"Game Over");
            _isRunning = NO;
            [GameOverPanel sharedGameOverPanel].visible = YES;
            [TetrisGame sharedTetrisGame].visible = NO;
            [[MainMenu sharedMainMenu] setVisible:NO];
            
            // new high score
            if (self.score > _highScore) {
                self.highScore = self.score;
            }
        } else {
            [_area placePuzzle:_currentPuzzle];
            int lines = [_area consumeFullLines];
            [self addScore:lines];
            [self genNextPuzzle];            
        }
    }
}

- (void)left
{
    if (!_isRunning || _isPaused) return;
    Puzzle *newPuzzle = [Puzzle puzzleWithPuzzle:_currentPuzzle];
    [newPuzzle left];
    if ([_area canPlace:newPuzzle]) {
        [_currentPuzzle release];
        _currentPuzzle = [newPuzzle retain];
        [_leftRightSound play];
    } else {
        [_wrongSound play];
    }
}

- (void)right
{
    if (!_isRunning || _isPaused) return;
    Puzzle *newPuzzle = [Puzzle puzzleWithPuzzle:_currentPuzzle];
    [newPuzzle right];
    if ([_area canPlace:newPuzzle]) {
        [_currentPuzzle release];
        _currentPuzzle = [newPuzzle retain];
        [_leftRightSound play];
    } else {
        [_wrongSound play];
    }
}


- (BOOL)isBlock:(int)x y:(int)y
{
    if ([_currentPuzzle isBlock:x y:y]) {
        return YES;
    }
    if ([_area isBlock:x y:y]) {
        return YES;
    }
    return NO;
}

- (BOOL)isNextBlock:(int)x y:(int)y
{
    return [_nextPuzzel isBlock:x y:y];
}

-(void) onTick:(NSTimer *)timer
{
    if (!_isRunning || _isPaused) return;
    if (_isSpeedUp) {
        [self down];
    } else {
        _tick ++;
        if (_tick%(20 - _level + 1) == 0) {
            [self down];
        }
    }
}

- (void) doPause:(BOOL)flag
{
    _isPaused = flag;
}

- (void) doStart
{
    _isRunning = YES;
    [self genNextPuzzle];
}

- (void) doRestart
{
    _isRunning = YES;
    self.score = 0;
    self.level = 0;
    [_area release];
    _area = [[[Area alloc] init] retain];
    [self genNextPuzzle];
}

// ---------------------- private Stats ----------------------

-(void)genNextPuzzle
{
    if (!_nextPuzzel) {
        _nextPuzzel = [[Puzzle puzzleWithRandomStyle] retain];
    }
    if (_currentPuzzle) {
        [_currentPuzzle release];
    }
    _currentPuzzle = _nextPuzzel;
    _currentPuzzle.y = -2;
    _currentPuzzle.x = (numberOfCulomn - 4)/2;
    _nextPuzzel = [[Puzzle puzzleWithRandomStyle] retain];
}
@end


// ---------------------- Tetris ----------------------
@implementation Tetris

@synthesize stats = _stats;
- (id)init
{
    if ((self = [super init])) {
        _stats = [[[Stats alloc] init] retain];
    }
    return self;
}

- (int)score
{
    return _stats.score;
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
        [button addListener:EVENT_TOUCH_BEGIN target:self select:@selector(onButtonDownPressed:)];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonDownReleased:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 100, 50);
        [self addChild:button];
        
        
        button = [[Button alloc] init];
        [button setText:@"left"];
        [button addListener:EVENT_TOUCH_BEGIN target:self select:@selector(onButtonLeftPressed:)];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonLeftReleased:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 200, 50);
        [self addChild:button];
        
        
        button = [[Button alloc] init];
        [button setText:@"right"];
        [button addListener:EVENT_TOUCH_BEGIN target:self select:@selector(onButtonRightPressed:)];
        [button addListener:EVENT_TOUCH_ENDED target:self select:@selector(onButtonRightReleased:)];
        button.contentSize = CGSizeMake(100, 100);
		button.position =  ccp(size.width - button.contentSize.width - 0, 50);
        [self addChild:button];
        [self schedule:@selector(onTick:) interval:(ccTime)0.05];
        
        _dirTick = 0;
        _countTick = 0;
    }
    return self;
}

- (void) start
{
    [_tetris.stats doStart];
}

- (void) pause:(BOOL)flag
{
    [_tetris.stats doPause:flag];
}

- (void) restart
{
    [_tetris.stats doRestart];
}

- (void) onTick:(ccTime)dt
{
    if (_dirTick) {
        if (_countTick >= 5) {
            if (_dirTick == LEFT) {
                [_tetris left];
            } else if (_dirTick == RIGHT) {
                [_tetris right];
            } else if (_dirTick == UP) {
                [_tetris up];
            }
        } else {
            _countTick ++;
        }
    }
  
}

- (void) onButtonUpPressed:(id)sender
{
    [_tetris up];
}

- (void) onButtonDownPressed:(id)sender
{
    [_tetris.stats speedup:YES];
}

- (void) onButtonDownReleased:(id)sender
{
    [_tetris.stats speedup:NO];
}

- (void) onButtonLeftPressed:(id)sender
{
    [_tetris left];
    _countTick = 0;
    _dirTick = LEFT;
}

- (void) onButtonLeftReleased:(id)sender
{
    _dirTick = 0;
}

- (void) onButtonRightPressed:(id)sender
{
    [_tetris right]; 
    _countTick = 0;
    _dirTick = RIGHT;
}

- (void) onButtonRightReleased:(id)sender
{
    _dirTick = 0;
}

- (void) draw
{
    CGPoint startPoint = CGPointMake(250, 50);
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
    
    CGPoint nextPuzzel = CGPointMake(50, 550);
    CGPoint nextVertices[4] = {
        ccp(nextPuzzel.x - blockSize, nextPuzzel.y - blockSize),
        ccp(nextPuzzel.x + 4 * blockSize + 2, nextPuzzel.y - blockSize),
        ccp(nextPuzzel.x + 4 * blockSize + 2, nextPuzzel.y + 4 * blockSize + 2),
        ccp(nextPuzzel.x - blockSize, nextPuzzel.y + 4 * blockSize + 2),
    };
    ccDrawPoly(nextVertices, 4, YES);
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {   
            if (![_tetris.stats isNextBlock:i y:4-j-1]){
                continue;
            }
            CGPoint p = CGPointMake(nextPuzzel.x + i * blockSize + 2, nextPuzzel.y + j*blockSize + 2);
            CGPoint v[4] = {
                ccp(p.x, p.y),
                ccp(p.x + blockSize - 1, p.y),
                ccp(p.x + blockSize - 1, p.y + blockSize - 1),
                ccp(p.x, p.y + blockSize - 1),                
            };
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
