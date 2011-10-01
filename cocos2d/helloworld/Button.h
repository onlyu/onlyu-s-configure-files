//
//  Button.h
//  cocos2d-ios
//
//  Created by onlyu on 11-10-1.
//  Copyright 2011年 CSU. All rights reserved.
//

#import "cocos2d.h"
typedef enum  {
	EVENT_TOUCH_BEGIN,
	EVENT_TOUCH_ENDED,
    EVENT_TOUCH_CANCELED,
    EVENT_TOUCH_MOVED,
} EventCode;

@interface Button : CCLayer {
    CCNode<CCLabelProtocol, CCRGBAProtocol> *_label;
    BOOL _autoSize;
    NSInvocation *onClick;
};

- (void) setText:(NSString *) text;
- (void) addListener:(EventCode) event target:(id)target select:(SEL) selector;

@end
