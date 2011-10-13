//
//  TeriesGame.h
//  cocos2d-ios
//
//  Created by onlyu on 11-10-3.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "cocos2d.h"
#import "SimpleAudioEngine.h"


// 方块
@interface Puzzle : NSObject {
@private
    int _x;
    int _y;
    int _style;
}

@property (nonatomic, readwrite)int x;
@property (nonatomic, readwrite)int y;

+ (id)puzzleWithRandomStyle;
+ (id)puzzleWithPuzzle:(Puzzle *) puzzle;

- (id)initWithPuzzle:(Puzzle *) puzzle;
- (id)initWithStyle:(int)style;
- (BOOL)isBlock:(int)x y:(int)y;
- (int *)getBlocks;

// 操作
- (void)left;
- (void)right;
- (void)down;
- (void)rotate;

@end

// 区域
@interface Area : NSObject {
@private
    int *_data;
}

-(BOOL) canPlace:(Puzzle *)puzzle;
-(void) placePuzzle:(Puzzle *)puzzle;
-(BOOL) isBlock:(int)x y:(int)y;
-(int) consumeFullLines;
@end


// 游戏状态
@interface Stats : NSObject {
@private
    Area *_area;
    Puzzle *_currentPuzzle;
    Puzzle *_nextPuzzel;
    NSInvocation *onGameOver;
    
    int _highScore;
    
    int _totalLine;
    int _score;
    int _level;
    BOOL _isSpeedUp;
    int _tick;
    
    BOOL _isPaused;
    BOOL _isRunning;
    
    CDSoundSource* _leftRightSound;
    CDSoundSource* _rotSound;
    CDSoundSource* _wrongSound;
    CDSoundSource* _speedUpSound;
    CDSoundSource* _addScoreSound;
}

@property (nonatomic,readwrite)int score;
@property (nonatomic,readwrite)int highScore;
@property (nonatomic,readwrite)int level;

- (void) speedup:(BOOL)flag;

- (void) up;
- (void) down;
- (void) left;
- (void) right;

- (BOOL) isBlock:(int)x y:(int)y;
- (BOOL) isNextBlock:(int)x y:(int)y;

// control
- (void) doPause:(BOOL)flag;
- (void) doRestart;
- (void) doStart;


@end

// 游戏
@interface Tetris : NSObject {
@private
    Stats *_stats;
}

@property (nonatomic,readonly)id stats;

@property (nonatomic,readonly)int score;

- (void)up;
- (void)down;
- (void)left;
- (void)right;
/*
- (void)pause;
- (void)restart;
- (void)start;
*/
// 判断是否是方块
- (BOOL) isBlock:(int)x y:(int)y;

@end

enum {
    LEFT =1,
    RIGHT,
    UP,
    DOWN,
}dir;

@interface TetrisGame : CCLayer {
    Tetris *_tetris;
    int _countTick;
    int _dirTick;
}

+ (TetrisGame *) sharedTetrisGame;

- (void) start;
- (void) pause:(BOOL) flag;
- (void) restart;
@end

