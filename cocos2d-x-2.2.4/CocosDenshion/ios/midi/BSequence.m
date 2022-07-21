//
//  BSequence.m
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 18/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BSequence.h"

@implementation BSequence

-(id) init {
    if((self=[super init])) {
        _sequence = [NSMutableArray new];
    }
    return self;
}

-(void) addEvent: (BMidiEvent *) event {
    [_sequence addObject:event];
}

-(NSMutableArray *) getSequence {
    return _sequence;
}

-(NSInteger) eventCount {
    return [_sequence count];
}

-(void) sortSequenceByStartTime {
    [_sequence sortUsingSelector:@selector(compare:)];
    
    NSInteger lastTime = 0;
    
    for(int i=0; i<_sequence.count; i++) {
        BMidiEvent * e = [_sequence objectAtIndex:i];
        
        if([e getStartTime] < lastTime) {
            NSLog(@"current: %i, last %i", [e getStartTime], lastTime);
        }
        
        lastTime = [e getStartTime];
        
    }
}

@end
