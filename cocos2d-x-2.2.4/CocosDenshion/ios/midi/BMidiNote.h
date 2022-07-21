//
//  BMidiNote.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 14/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BMidiEvent.h"

@interface BMidiNote : BMidiEvent {
    NSInteger _duration;
}

@property (nonatomic, readwrite) UInt8 channel;
@property (nonatomic, readwrite) UInt8 note;
@property (nonatomic, readwrite) UInt8 velocity;
@property (nonatomic, readwrite) UInt8 releaseVelocity;
@property (nonatomic, readwrite) NSInteger track;

-(NSInteger) endTime;
- (int) noteType;

-(void) setDuration: (float) duration;
-(NSInteger) getDuration;
-(BOOL) isWhiteNote;

@end
