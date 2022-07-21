//
//  BMidiClock.h
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 11/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BMidiClock : NSObject {
    
    // The total number of pulses which have occurred
    NSInteger _numberOfPulses;
    NSInteger _lastNumberOfPulses;
    
    // Our base time - all times are measured from this base
    NSDate * _startTime;
    
    // When the clock last ticked
    NSTimeInterval _previousTime;
    
    // Time elasped since last tick
    NSTimeInterval _dt;
    
    // Used to determine if the required number of ticks have elapsed
    // see requiredTicksElapsed function
    NSInteger _ticksElapsed;
    
    BOOL _isPaused;
    
    NSInteger _metronomeFreq;
    float _metronomeFreqInQtrNotes;
    
    NSInteger _numberOfMetronomeTicks;
    
    Float32 _tempo;

}

// Beats per minute
@property (nonatomic, readwrite) double BPM;

// Pulses per quarter note
@property (nonatomic, readwrite) NSInteger PPQN;

// The frequency with whigh the requiredTicksElapsed function
// will return true. A value of 2 means the function will
// return true every second tick
@property (nonatomic, readwrite) NSInteger tickResolution;

@property (nonatomic, readwrite, retain) NSDate * startTime;


// Must be called on the audio loop at high frequency
-(void) update;

// Resets the clock
-(void) restart;

// Returns the time in pulses
-(NSInteger) getDiscreteTime;

// The requiredTicksElapsed function allows us to perform tasks at a lower rate.
// If we set this value of tickResolution to 2 the requiredTicksElapsed function
// will return true every second tick this is useful if we want to perform a
//certain piece of processing every two ticks rather than every tick
-(BOOL) requiredTicksElapsed;

// Returns true if this tick is also a metronome tick
-(BOOL) isMetronomeTick;

-(void) setMetronomeFrequency: (float) quarterNotes;

-(void) pause;

-(void) resume;

-(void) setTempo:(float) tempo;
@end
