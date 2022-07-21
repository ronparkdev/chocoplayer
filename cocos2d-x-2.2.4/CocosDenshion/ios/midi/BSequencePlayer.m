//
//  BSequencePlayer.m
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 02/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BSequencePlayer.h"

@implementation BSequencePlayer

@synthesize sequence = _sequence;

-(id) init {
    if((self=[super init])) {
        _lastMidiPos = 0;
    }
    return self;
}

-(void) setTempoHandler:(id<BTempoEventHandler>)tempoHandler {
    _tempoHandler = tempoHandler;
}

-(void) setNoteHandler:(id<BNoteEventHandler>)noteHandler {
    _noteHandler = noteHandler;
}

-(void) restart{
    _lastMidiPos = 0;
    
    NSUInteger i, midiEventLen = [[_sequence getSequence] count];
    BMidiEvent * midiEvent;
    
    for(i = 0; i<midiEventLen; i++){
        midiEvent = [[_sequence getSequence] objectAtIndex:i];
        
        [midiEvent setPassed:NO];
    }
    
    _noMoreNode = NO;
}

// Play the next notes in the sequence
-(void) update: (NSInteger) timeInPulses{
    
    BOOL isFutureNode;
    
    NSUInteger i, c;
    NSUInteger lastPosNotHaveFutureNode = 0;
    NSUInteger midiEventLen = [[_sequence getSequence] count];
    BMidiEvent * midiEvent;

    for(i =_lastMidiPos, c=0; i<midiEventLen && c<20; i++){
        midiEvent = [[_sequence getSequence] objectAtIndex:i];
        isFutureNode = ([midiEvent getStartTime] > timeInPulses);
        
        if(isFutureNode == YES){
            if(lastPosNotHaveFutureNode == 0)
                lastPosNotHaveFutureNode = i;
            
            c++;
            continue;
        }
        
        if([midiEvent isPassed] == NO){
            if(midiEvent.eventType == Note) {
                if(_noteHandler  != Nil) {
                    NSLog(@"index%lu/%lu",
                          (unsigned long)i,
                          (unsigned long)midiEventLen);
                    [_noteHandler handleNoteEvent:(BMidiNote *) midiEvent];
                }
            }
            else if(midiEvent.eventType == Tempo ) {
                if(_tempoHandler != Nil) {
                    [_tempoHandler handleTempoEvent:(BTempoEvent *) midiEvent];
                }
            }
        
            [midiEvent setPassed:YES];
        }
    }
    
    _lastMidiPos = lastPosNotHaveFutureNode;
    
    if(i == midiEventLen && c == 0){
        _noMoreNode = YES;
    }
}

-(BOOL) isNoMoreNode{
    return _noMoreNode;
}

@end
