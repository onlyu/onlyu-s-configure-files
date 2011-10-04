//
//  TeriesGame.h
//  cocos2d-ios
//
//  Created by onlyu on 11-10-3.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "cocos2d.h"


// 方块
@interface Puzzle : NSObject {
@private
    int _x;
    int _y;
    int _style;
}

+ (id)getRandomPuzzel;

- (id)initWithStyle:(int)style;

- (BOOL)isBlock:(int)x y:(int)y;

// 操作
- (void)left;
- (void)right;
- (void)down;
- (void)rotate;

@end

// 区域
@interface Area : NSObject {
@private
}
@end


// 游戏状态
@interface Stats : NSObject {
@private
    Area *_area;
    Puzzle *_currentPuzzle;
    Puzzle *_nextPuzzel;
}

- (void)up;
- (void)down;
- (void)left;
- (void)right;

- (BOOL)isBlock:(int)x y:(int)y;
@end

// 游戏
@interface Tetris : NSObject {
@private
    Stats *_stats;
}

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

@interface TetrisGame : CCLayer {
    Tetris *_tetris;
}

+ (TetrisGame *) sharedTetrisGame;

-(void)onButtonUpPressed:(id)sender;
-(void)onButtonDownPressed:(id)sender;
-(void)onButtonLeftPressed:(id)sender;
-(void)onButtonRightPressed:(id)sender;

@end

