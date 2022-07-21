//
//  BChannelEvent.m
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 18/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BChannelEvent.h"

@implementation BChannelEvent

@synthesize type;
@synthesize channel;
@synthesize data1;
@synthesize data2;

- init {
    if((self=[super init])) {
        self.eventType = Channel;
    }
    return self;
}

@end
