//
//  BChannelEvent.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 18/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BMidiEvent.h"

typedef enum {
    Controller,
	Program
} BChannelEventType;

@interface BChannelEvent : BMidiEvent

@property (nonatomic, readwrite) BChannelEventType type;
@property (nonatomic, readwrite) UInt8 channel;

// For Program type: instrument patch number to assign to channel
// For Control type: control number affected by message
@property (nonatomic, readwrite) UInt8 data1;

// For Program type: n/a
// For Control type: The value to which the controller should be set
@property (nonatomic, readwrite) UInt8 data2;

@end
