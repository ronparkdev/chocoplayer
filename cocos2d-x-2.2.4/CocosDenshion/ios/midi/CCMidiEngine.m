//
//  CCMidiEngine.m
//  chocoplayer
//
//  Created by psm on 2015. 8. 20..
//
//

#import "CCMidiEngine.h"

@implementation CCMidiEngine
-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super's" return value
	if( (self=[super init]) ) {
        
        // Set up the note visualiser - the visualiser will add objects
        // to this layer
        // Create a new midi manager and load our midi file
        _midiManager = [BMidiManager new];
        
        // Initialise a new sequence player - this will actually play our
        // midi sequence
        _sequencePlayer = [BSequencePlayer new];
        
        // Add this class as a note processor - this means that notes will
        // be delivered to the addNote method of this class in realtime to
        // be handled
        [_sequencePlayer setNoteHandler:self];
        [_sequencePlayer setTempoHandler:self];
        
        // Create a new midi clock - this keeps track of MIDI time based on
        // midi properties such as BPM
        _midiClock = [BMidiClock new];
        _midiClock.tickResolution = 1;
        
        
        // Create a new audio manager this will vocalize the midi messages
        _audioManager = [AudioManager newAudioManager];
        
        
        _audioThread = nil;
	}
	return self;
}


- (void)play:(NSString*)path
      volume:(float)fvolume
       tempo:(float)ftempo
     balance:(float)fbalance
      isLoop:(BOOL)isloop;{
    
    NSLog(@"MIDI Play %@", path);
    
    if([path isEqualToString:_lastPath] == NO){
        
        if(_audioThread != nil){
            [_audioThread cancel];
            
            [NSThread sleepForTimeInterval:0.1f];
        }
        
        // The process midi file will return a our sequence of MIDI events
        BSequence * sequence = [_midiManager processMidiFile:path];
    
        // Add the sequence to the sequence player
        _sequencePlayer.sequence = sequence;

        // Load the default general midi instruments from the midi file
        [_audioManager configureForGeneralMidi:@"GeneralUser" withSequence:sequence];
    
        // Enable percussion
        [_audioManager enablePercussion:@"GeneralUser" withPatch:0 withVolume:1];
    
        // Add a silent default
        [_audioManager addDefaultVoice:@"GeneralUser" withPatch:0 withVolume:1];
    
        // Start the audio manager. After the audio manager has started you can't add any more
        // voices
        [_audioManager startAudioGraph];
        
        
        // Create the audio thread. This is a high priority thread
        // which will update the audio around 400 times per second
        
        _audioThread = [NSThread new];
        [_audioThread initWithTarget:self selector:@selector(audioLoop) object:Nil];
        [_audioThread setThreadPriority:1];
        [_audioThread start];
        
        [_midiClock restart];
        [_audioManager restart];
        [_sequencePlayer restart];
    }
    
    [_midiClock setTempo:ftempo];
    [_audioManager setVolume:fvolume];
    [_audioManager setBalance:fbalance];
    
    _lastPath = path;
    
    _lastVolume = fvolume;
    _lastTempo = ftempo;
    _lastBalance = fbalance;
    _isLoop = isloop;
}

- (void)pause{
    [_midiClock pause];
    [_audioManager setVolume:0.0f];
}

- (void)resume{
    [_midiClock resume];
    [_audioManager setVolume:_lastVolume];
}

- (void)stop{
    NSLog(@"MIDI Stop");
    
    _lastPath = @"";
    
    if(_audioThread != nil){
        [_audioThread cancel];
        
        [NSThread sleepForTimeInterval:0.1f];
        _audioThread = nil;
    }
    
    [_audioManager setVolume:0.0f];
}

- (void)setVolume:(float)fVolume{
    _lastVolume = fVolume;
    [_audioManager setVolume:fVolume];
    
    NSLog(@"%f", fVolume);
}

-(void) audioLoop {
    // Loop until the program terminates
    while ([[NSThread currentThread] isCancelled] == NO) {
        // Update the midi clock every loop
        [_midiClock update];
        
        // Only check for events if the required number of ticks
        // has elapsed - determined by _midiClock.tickResolution
        //if([_midiClock requiredTicksElapsed]) {
        [_sequencePlayer update:[_midiClock getDiscreteTime]];
        [_audioManager update:[_midiClock getDiscreteTime]];
        //}
        
        if([_audioManager isNoMoreNode] && [_sequencePlayer isNoMoreNode]){
            [self handleMidiFinish];
        }
    }
}

// What to do when a new note is activated - be very careful!! We're
// multi-threading
-(void) handleNoteEvent:(BMidiNote *)note {
    // Play the note in the audio manager
    [_audioManager playNote:note];
 }

// Handle any tempo events which occur
-(void) handleTempoEvent:(BTempoEvent *)tempoEvent {
    // Set the midi clock PPNQ
    if(tempoEvent.type == BPPNQ) {
        _midiClock.PPQN = tempoEvent.PPNQ;
        NSLog(@"Set PPQN: %i", tempoEvent.PPNQ);
    }
    // Set the midi clock BPM
    if (tempoEvent.type == BTempo) {
        _midiClock.BPM = tempoEvent.BPM;
        NSLog(@"Set BPM: %f", tempoEvent.BPM);
    }
    
    // Set the metronome rate
    if (tempoEvent.type == BTimeSignature) {
        // Set metronome rate. The metronome figure is measured as a fraction
        // over 24. So, a value of 24 means once per quarter note. 12 means
        // once every 1/8 note etc...
        //float metronomeRate = (float) tempoEvent.ticksPerQtr / 24.0;
        // Work out how many pulses this represents
        //_midiClock.metronomeFreq = (int) roundf(metronomeRate * _midiClock.PPQN);
        
        //[_midiClock setMetronomeFrequency:metronomeRate];
        
        
    }
}

-(void) handleMidiFinish{
    if(_isLoop == YES){
        [_midiClock restart];
        [_sequencePlayer restart];
    }else{
        [self stop];
    }
}


@end
