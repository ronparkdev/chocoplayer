//
//  BSequencePlayer.h
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 02/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BNoteEventHandler.h"
#import "BTempoEventHandler.h"
#import "BSequence.h"
#import "BMidiClock.h"
#import "BMidiEvent.h"
#import "BTempoEvent.h"

typedef enum {
    kPlaying,
    kStopped,
    kPaused
} Status;

// The sequence player plays a sequence of MIDI events
// it it loops over the events and when their start
// time is reached it hands the events to the appropriate
// event handler
@interface BSequencePlayer : NSObject {
    // A pointer to the current sequence
    BSequence * _sequence;
    
    // Locations to store handlers for the various types of event
    id<BNoteEventHandler> _noteHandler;
    id<BTempoEventHandler> _tempoHandler;
    
    NSUInteger _lastMidiPos;
    
    BOOL _noMoreNode;
}

@property (nonatomic, readwrite, retain) BSequence * sequence;
// Set handlers for various events
-(void) setNoteHandler: (id<BNoteEventHandler>) noteHandler;
-(void) setTempoHandler: (id<BTempoEventHandler>) tempoHandler;

-(BOOL) isNoMoreNode;

// The sequence should be updated with high frequency to ensure that
// events are dispatched when they should be
-(void) restart;
-(void) update: (NSInteger) timeInPulses;

@end
