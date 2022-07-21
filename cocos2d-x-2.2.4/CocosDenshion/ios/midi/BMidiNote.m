//
//  BMidiNote.m
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 14/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BMidiNote.h"

@implementation BMidiNote 

@synthesize channel;
@synthesize note;
@synthesize velocity;
@synthesize releaseVelocity;
@synthesize track;

- init {
    if((self=[super init])) {
        self.eventType = Note;
    }
    return self;
}

-(NSInteger) endTime {
    return _startTime + _duration;
}

-(BOOL) isWhiteNote {
    int nt = [self noteType];
    if (nt==0||nt==2||nt==4||nt==5||nt==7||nt==9||nt==11) {
        return YES;
    }
    return NO;
}

- (int) noteType {
    return ((int) note % 12);
}

-(void) setDuration: (float) duration {
    _duration = (int) roundf(duration);
}

-(NSInteger) getDuration {
    return _duration;
}

@end
