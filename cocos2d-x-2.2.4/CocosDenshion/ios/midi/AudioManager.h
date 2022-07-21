//
//  AudioManager.h
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 24/03/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <AudioToolbox/MusicPlayer.h>
#import "BMidiNote.h"
#import "BSequence.h"
#import "BChannelEvent.h"
#import "BMidiClock.h"


@interface AudioManager : NSObject {
    
    // An io unit is responsible for sending sound to the iPhone speakers
    AudioUnit _ioUnit;
    
    // A mixer unit mixes a number of channels into one
    AudioUnit _mixerUnit;
    
    // Voice definitions - each specifies a sound font and patch number
    NSMutableArray * _voiceDefs;
    
    // Maps user defined voice name to the voices AUSampler
    NSMutableDictionary * _voiceChannels;

    // An array of notes which are pending being played
    NSMutableArray * _notesToPlay;
    
    // A pointer to the audio graph
    AUGraph _processingGraph;
    
    NSMutableArray * _toDelete;
    
    // Notes currently playing
    NSMutableArray * _playingNotes;
    
    // If enabled track 9 will be used for percussion
    BOOL _percussionEnabled;
    
    // Has this class already been initialised
    BOOL _started;
    
    BOOL _noMoreNode;
    
}

// Create a new audio manager instance
+(id) newAudioManager;

// Called every cycle on the audio loop - used to start and stop notes
-(void) update: (NSInteger) timeInPulses;

// Used to play a note through the audio graph
-(void) playNote: (BMidiNote*) note;

// Configure the AUGraph for general midi. This function loops over the
// sequence looking for program change events. For each event it creates
// a sampler using the patch number specified in the midi file. Instruments
// use the naming convention c[channel] i.e. channel 1 is called c1
- (void) configureForGeneralMidi: (NSString *) soundFont withSequence: (BSequence *) sequence;

// Add a voice which is used if no instrument is specified for a notes channel OR track number
-(void) addDefaultVoice: (NSString *) soundFont withPatch: (UInt8) patch withVolume: (float) volume;

// Enable percussion and provide the bank in which the percussion library exists
// this function will use the default percussion bank of the sound font
-(void) enablePercussion: (NSString *) soundFont withPatch: (UInt8) patch withVolume: (float) volume;

// Manually add a sound font voice. If you want to override which instrument a track uses you can use the
// convention t[track number]
-(void) addVoice: (NSString*) name withSound: (NSString*) soundPath withPatch: (UInt8) patch withVolume: (float) volume;

// Starts the AUGraph. This must be called after all voices have been added
-(void) startAudioGraph;

// Set the output volume
-(void) setVolume: (float) volume;

// Set the output balance;
-(void) setBalance: (float) balance;

// Check if a particular voice name is already registered
-(BOOL) containsVoice: (NSString*) voice;

// Restart the audio manager
-(void) restart;

-(void) playNoteWithDefaultSampler;

-(BOOL) isNoMoreNode;
@end
