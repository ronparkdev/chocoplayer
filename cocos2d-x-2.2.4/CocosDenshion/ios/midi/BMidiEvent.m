//
//  BMidiEvent.m
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 14/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BMidiEvent.h"

@implementation BMidiEvent

@synthesize eventType;

- (NSComparisonResult)compare:(BMidiEvent *)otherObject {
    NSNumber * this = [NSNumber numberWithDouble:_startTime];
    NSNumber * other = [NSNumber numberWithDouble:[otherObject getStartTime]];
    return [this compare:other];
}

-(void) setStartTime: (float) startTime {
    _startTime = (int) roundf(startTime);
}

-(NSInteger) getStartTime {
    return _startTime;
}

-(BOOL) isPassed{
    return _isPassed;
}

-(void) setPassed:(BOOL)isPassed{
    _isPassed = isPassed;
}

@end
