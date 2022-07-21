//
//  BMidiClock.m
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 11/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BMidiClock.h"

// i.e. 60FPS

int number_of_standard_ticks;

@implementation BMidiClock

@synthesize BPM;
@synthesize PPQN;
@synthesize tickResolution;
@synthesize startTime = _startTime;


-(id) init {
    if(( self = [super init])) {
        
        // Set default values
        BPM = 120;
        PPQN = 96;
        
        [self setMetronomeFrequency:1];
        
        // Perform processing every tick
        tickResolution = 1;
        
        _isPaused = NO;
        
        _tempo = 1.0f;
        
        [self restart];
    }
    return self;
}

-(void) setPPQN:(NSInteger) PPQN_ {
    self->PPQN = PPQN_;
    
    _metronomeFreq = _metronomeFreqInQtrNotes * PPQN_;
}

-(void) setTempo:(float) tempo{
    _tempo = tempo;
}

-(void) setMetronomeFrequency: (float) quarterNotes {
    _metronomeFreq = quarterNotes * PPQN;
    _metronomeFreqInQtrNotes = quarterNotes;
}

// Restart the clock
-(void) restart {
    _numberOfPulses = 0;
    _startTime = Nil;
    _numberOfMetronomeTicks = 0;
    _lastNumberOfPulses = 0;
}

-(void) setupClock {
    _startTime = [NSDate date];
    _previousTime = 0;
}



-(BOOL) requiredTicksElapsed {
    // If the required number of ticks have elapsed
    // reset the ticks elapsed counter and return true
    // otherwise return false
    if(_ticksElapsed >= tickResolution) {
        _ticksElapsed = 0;
        return YES;
    }
    return NO;
}

// Convert a time in seconds to a time in pulses
-(NSInteger) timeToPulses: (float) time {
    return (int) roundf(time / (60 / (BPM * PPQN)));
}

// Returns true if this tick is a metronome tick
// be careful using this from the render thread
// this method has to be called every pulse otherwise
// it can skip a beat
-(BOOL) isMetronomeTick {
    //printf("Number floor: %i, no ticks: %i \n",(NSInteger)floor((float)_numberOfPulses / (float)_metronomeFreq), _numberOfMetronomeTicks);
    if ((NSInteger)floor((float)_numberOfPulses / (float)_metronomeFreq) == _numberOfMetronomeTicks) {
        return NO;
    }
    else {
        _numberOfMetronomeTicks++;
        return YES;
    }
    //return _numberOfPulses % _metronomeFreq == 0;
}

-(void) pause {
    _isPaused = YES;
}

-(void) resume {
    _isPaused = NO;
    [self setupClock];
}

// Update the midi clock
-(void) update {
    
    if(_isPaused) {
        return;
    }
    

    // If this is the first time update is called or
    // if the game has been paused 
    if(_startTime == Nil) {
        // Set the start time to the current time
        [self setupClock];
    }

    // Get the time since the clock started
    NSTimeInterval interval = -[_startTime timeIntervalSinceNow];

//    // The animation has probably been paused
//    if(interval > 0.05) {
//        // reset the clock this avoids use counting
//        // the time the program was paused
//        [self setupClock];
//        interval = 0;
//    }
    
    // Use this time to get an accurate value for the
    // time since the clock last ticked
    _dt = interval - _previousTime;
    
    // Calculate the MIDI pulse duration 
    double tickDuration = (60.0 / BPM)/PPQN / _tempo;
    
    // If a pulse has happened update the current time in pulses
    if(_dt > tickDuration) {
        
        // Get the number of ticks which happened
        NSInteger numberOfTicks =  (int) floor(_dt/tickDuration);
        
        // Add this to the elapsed ticks
        _ticksElapsed += numberOfTicks;
        
        _lastNumberOfPulses = _numberOfPulses;
        
        // And the total number of pulses
        _numberOfPulses += numberOfTicks;
        
        // Calculate the previous time value. We calculate this
        // as number of ticks * tick duration so that we account
        // for the small discrepencies in time i.e. update will
        // normally be called a small fraction of time late
        // this stops us from starting to drift away from the real time
        _previousTime += numberOfTicks * tickDuration;
    }
    
    // Sleep the thread for about a third of a pulse to reduce CPU load but ensure
    // that a pulse is not missed
    [NSThread sleepForTimeInterval: 20/(BPM * PPQN)];

}

// Get the current time in pulses
-(NSInteger) getDiscreteTime {
    return _numberOfPulses;
}

-(NSInteger) getTimeSinceLastPulse {
    return _numberOfPulses - _lastNumberOfPulses;
}


@end
