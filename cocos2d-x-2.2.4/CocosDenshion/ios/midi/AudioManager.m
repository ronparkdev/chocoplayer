//
//  AudioManager.m
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 24/03/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "AudioManager.h"

#import "VoiceDef.h"

NSString * PERCUSSION = @"percussion";
NSString * DEFAULT = @"default";

@implementation AudioManager

-(id) init {
    if ((self = [super init])) {
        _processingGraph = nil;
        
        _toDelete = [NSMutableArray new];
        _voiceChannels = [NSMutableDictionary new];
        _voiceDefs = [NSMutableArray new];
        _notesToPlay = [NSMutableArray new];
        
        _playingNotes = [NSMutableArray new];
        
        _started = NO;
        
        _percussionEnabled = NO;
        
        _noMoreNode = NO;
        
    }
    return self;
}

+(id) newAudioManager {
    return [[self alloc] initAudioManager];
}

-(id) initAudioManager {
    if ((self = [self init] )) {
    }
    return self;
}

- (void) configureForGeneralMidi: (NSString *) soundFont withSequence: (BSequence *) sequence {
    // For each program specified in the midi file create a new voice
    BChannelEvent * channelEvent;
    
    // Loop over all the events in the sequence
    for(BMidiEvent * event in [sequence getSequence]) {
        // If it's a channel event check which type
        if(event.eventType == Channel) {
            channelEvent = (BChannelEvent *) event;
            // If it's a program change event...
            if(channelEvent.type == Program) {
                
                // Print that a new voice has been added for logging
                //NSLog(@"New Voice Added: %@, patch: %i", [@"c" stringByAppendingFormat:@"%i", channelEvent.channel], channelEvent.data1);
                
                // Create a new channel mapping using the convention that the name is c + channel number
                [self addVoice:[@"c" stringByAppendingFormat:@"%i", channelEvent.channel]
                     withSound:soundFont withPatch:channelEvent.data1 withVolume:1];
            }
        }
    }
    
}

-(void) addDefaultVoice: (NSString *) soundFont withPatch: (UInt8) patch withVolume: (float) volume {
    [self addVoice:DEFAULT withSound:soundFont withPatch:patch withVolume:volume];
}

// In general midi channel 10 (in programming this is channel 9 because we start counting from 0 instead
// of 1) is reserved for percussion. This function sets up a percussion voice for our channel 9. 
-(void) enablePercussion: (NSString *) soundFont withPatch: (UInt8) patch withVolume: (float) volume {
    _percussionEnabled = YES;
    [self addVoice:PERCUSSION withSound:soundFont withPatch:patch withBank: kAUSampler_DefaultPercussionBankMSB withVolume:volume];
}

-(void) addVoice: (NSString*) name withSound: (NSString*) soundPath withPatch: (UInt8) patch withVolume: (float) volume {
    [self addVoice:name withSound: soundPath withPatch: patch withBank: kAUSampler_DefaultMelodicBankMSB withVolume: volume];
}

// Add a voice to the manager. Specify a name, sound font and patch number
-(BOOL) addVoice: (NSString*) name withSound: (NSString*) soundPath withPatch: (UInt8) patch withBank: (UInt8) bank withVolume: (float) volume {
    
    // Create struct
    VoiceDef *voiceStruct = [[VoiceDef alloc] init];
    
    voiceStruct.patch = patch;
    voiceStruct.soundPath = soundPath;
    voiceStruct.name = name;
    voiceStruct.volume = volume;
    voiceStruct.bank = bank;
    
    // Store the struct as an NSValue - only NS objects can be stored in an NSArray
    //NSValue * voiceValue = [NSValue value:&voiceStruct withObjCType:@encode(VoiceDef)];

    // The voice defs are added when the audio graph is setup
    [_voiceDefs addObject:voiceStruct];
    
    return YES;
}

// Create and start the audio graph
-(void) startAudioGraph {
    // If this project has already been initialised do nothing
    if(_started) {
        return;
    }
    
    // Set this project as initialised
    _started = YES;
    
    // Create the au graph
    if (![self createAUGraph]) {
    }
    
    // Start the AUGraph
    [self configureAndStartAudioProcessingGraph:_processingGraph];    
}

/*
 * This is a convenience method which will retrieve the correct sampler for a note. The sampler
 * retrieved depends on a number of factors. The highest priority is the voice assigned to a 
 * track. If a voice exists for the track to which the note belongs this voice will be used. 
 * The second level is channel. If a voice exists for the note's channel that will be used next.
 * The lowest level is default. Any note which has no voice assigned at track or channel level
 * will use the default voice. 
 */
-(AudioUnit) getSamplerForNote: (BMidiNote *) note {
    
    // Setup variable we need
    NSValue * value = Nil;
    AudioUnit sampler;
    
    // First check if the note's track has a sampler
    value = [_voiceChannels objectForKey:[@"t" stringByAppendingFormat:@"%i", note.track]];
    
    // Next check if the note's channel has a sampler
    if(value == Nil) {
        value = [_voiceChannels objectForKey:[@"c" stringByAppendingFormat:@"%i", note.channel]];
    }
    // Finally try to use the default voice
    if(value == Nil) {
        value = [_voiceChannels objectForKey:DEFAULT];
    }
    
    if(_percussionEnabled && note.channel == 9) {
        value = [_voiceChannels objectForKey:PERCUSSION];
    }

    
    // A default must be specified!
    // The code could be re-worked to allow certain tracks not to be played
    assert(value != Nil);

    [value getValue:&sampler];
    
    return sampler;
}

// Start any notes which have been added and stop any
// notes which have reached their duration
-(void) update: (NSInteger) timeInPulses {
    
    BMidiNote * note;
    AudioUnit sampler;
    
    // Stop any notes which should be stopped - loop over the playing notes
    // and if the current time is less than the note stop time send a stop
    // event to the midi sampler
    if([_playingNotes count] > 0) {
        for(int i=0; i<[_playingNotes count]; i++) {
            
            // Get a pointer to the note
            note = [_playingNotes objectAtIndex:i];
            
            // If the note should have stopped
            if([note endTime] <= timeInPulses) {

                sampler = [self getSamplerForNote:note];
                
                // Increase note duration to compensate for potential lag
                // Send a stop playing message to the sampler
                //MusicDeviceMIDIEvent(sampler, 0x80, note.note, 0, 0);
                MusicDeviceMIDIEvent(sampler, 0x80, note.note, 0, 0);

                // Delete the note from the list of active notes
                [_toDelete addObject:note];
            }
        }
    }
    
    // Delete the finished notes from the array of playing notes
    if([_toDelete count] > 0) {
        for(int i=0; i<[_toDelete count]; i++) {
            [_playingNotes removeObject:[_toDelete objectAtIndex:i]];
        }
        [_toDelete removeAllObjects];
    }
        
    // Play any new notes. Notes are added asynchronously. Each loop
    // any notes which have been added to be played are started here
    if ([_notesToPlay count] > 0) {
        for(int i=0; i<[_notesToPlay count]; i++) {
            
            // Get the note to play
            note = [_notesToPlay objectAtIndex:i];
            
            sampler = [self getSamplerForNote:note];
            
            // Play the note: 0x90 is a midi start command
            [self playMIDINote:0x90 withNote:note.note withVelocity:note.velocity withSampler:sampler];
            
            // Add the note to the list of playing notes
            [_playingNotes addObject:note];
            [_toDelete addObject:note];
        }
    }
    if([_toDelete count] > 0) {
        for(int i=0; i<[_toDelete count]; i++) {
            [_notesToPlay removeObject:[_toDelete objectAtIndex:i]];
        }
        [_toDelete removeAllObjects];
    }
    
    if([_playingNotes count] == 0)
        _noMoreNode = YES;
    else
        _noMoreNode = NO;
    
}

-(void) playNote: (BMidiNote*) note {
    [_notesToPlay addObject:note];
}

-(void) playNoteWithDefaultSampler {
    // Setup variable we need
    NSValue * value = Nil;
    AudioUnit sampler;
    
    value = [_voiceChannels objectForKey:DEFAULT];
    
    [value getValue:&sampler];
    
    [self playMIDINote:0x90 withNote:60 withVelocity:127 withSampler:sampler];
}


// An internal method which plays a note using the MusicDeviceMidiEvent method
-(void) playMIDINote: (Byte) status withNote: (Byte) note withVelocity: (Byte) velocity withSampler: (AudioUnit) sampler {
    // Play the note
    MusicDeviceMIDIEvent(sampler, status, note, velocity, 0);
}

// Create a new audio graph containing all the added voices
- (BOOL) createAUGraph {
    
	OSStatus result = noErr;
	AUNode ioNode, mixerNode;
    
    NSMutableArray * samplerNodes = [NSMutableArray new];
    
    // Specify the common portion of an audio unit's identify, used for both audio units
    // in the graph.
	AudioComponentDescription cd = {};
	cd.componentManufacturer     = kAudioUnitManufacturer_Apple;
    
    if(_processingGraph != nil)
        DisposeAUGraph(_processingGraph);
    
    // Instantiate an audio processing graph
	result = NewAUGraph (&_processingGraph);
    NSCAssert (result == noErr, @"Unable to create an AUGraph object. Error code: %d '%.4s'", (int) result, (const char *)&result);
    
	//Specify the Sampler unit, to be used as the first node of the graph
	cd.componentType = kAudioUnitType_MusicDevice;
	cd.componentSubType = kAudioUnitSubType_Sampler;
    
    // To store primative variables I'm using an NSValue. This wrapper variable is for convenience
    // when we want to extract something from the wrapper
    NSValue * wrapper;

    // Node which will be used to play our midi not with a particular sound font
    AUNode samplerNode;
    
    // Create nodes for all the voices
    for(int i=0; i<[_voiceDefs count]; i++) {
        
        // Create a new sampler note
        result = AUGraphAddNode (_processingGraph, &cd, &samplerNode);
        
        // Check for any errors
        NSCAssert (result == noErr, @"Unable to add the Sampler unit to the audio processing graph. Error code: %d '%.4s'", (int) result, (const char *)&result);
        
        // Encode the node and add it to the array of sampler notes for later
        wrapper = [NSValue value:&samplerNode withObjCType:@encode(AUNode)];
        [samplerNodes addObject:wrapper];
    }
    
    
	// Specify the Output unit, to be used as the second and final node of the graph	
	cd.componentType = kAudioUnitType_Output;
	cd.componentSubType = kAudioUnitSubType_RemoteIO;  
    
    // Add the Output unit node to the graph
	result = AUGraphAddNode (_processingGraph, &cd, &ioNode);
    NSCAssert (result == noErr, @"Unable to add the Output unit to the audio processing graph. Error code: %d '%.4s'", (int) result, (const char *)&result);
    
    // Add the mixer unit to the graph
    cd.componentType = kAudioUnitType_Mixer;
    cd.componentSubType = kAudioUnitSubType_MultiChannelMixer;
    
    result = AUGraphAddNode (_processingGraph, &cd, &mixerNode);
    NSCAssert (result == noErr, @"Unable to add the Output unit to the audio processing graph. Error code: %d '%.4s'", (int) result, (const char *)&result);
    
    
    // Open the graph
	result = AUGraphOpen (_processingGraph);
    NSCAssert (result == noErr, @"Unable to open the audio processing graph. Error code: %d '%.4s'", (int) result, (const char *)&result);
    
    // Now that the graph is open get references to all the nodes and store
    // them as audio units
    
    VoiceDef *voiceDef;
    AudioUnit samplerUnit;
    // Loop over the sampler notes
    for(int i=0; i<[samplerNodes count]; i++) {
        // Extract the sampler note from the NSValue into the samplerNode variable
        wrapper = [samplerNodes objectAtIndex:i];
        [wrapper getValue:&samplerNode];
        
        // Get a reference to the sampler node and store it in the samplerUnit variable
        result = AUGraphNodeInfo (_processingGraph, samplerNode, 0, &samplerUnit);
        NSCAssert (result == noErr, @"Unable to obtain a reference to the Sampler unit. Error code: %d '%.4s'", (int) result, (const char *)&result);
        
        // Extract the voice def from the NSValue - the voice def tells us how to configure the sound font for our sampler unit
        voiceDef = [_voiceDefs objectAtIndex:i];
        
        // Wrap our audio unit in the NSValue
        wrapper = [NSValue value:&samplerUnit withObjCType:@encode(AudioUnit)];

        // Load the soundfont four out sampler unit using the voice definition
        [self loadSoundFont:voiceDef.soundPath withPatch:voiceDef.patch withBank:voiceDef.bank withSampler:samplerUnit];
        
        // Store the audio unit for use indexed by the voice def name
        [_voiceChannels setObject:wrapper forKey:voiceDef.name];

    }


    // Create a new mixer unit. This is necessary because we have a number of sampler
    // units which we need to output through the speakers. Each of these channels needs
    // to be mixed together to create one output
	result = AUGraphNodeInfo (_processingGraph, mixerNode, 0, &_mixerUnit);
    NSCAssert (result == noErr, @"Unable to obtain a reference to the Sampler unit. Error code: %d '%.4s'", (int) result, (const char *)&result);

	// Obtain a reference to the I/O unit from its node
	result = AUGraphNodeInfo (_processingGraph, ioNode, 0, &_ioUnit);
    NSCAssert (result == noErr, @"Unable to obtain a reference to the I/O unit. Error code: %d '%.4s'", (int) result, (const char *)&result);
    
    // Define the number of input busses
    UInt32 busCount   = [_voiceDefs count];
    
    // Set the input channels property on the mixer unit
    result = AudioUnitSetProperty (
                                   _mixerUnit,
                                   kAudioUnitProperty_ElementCount,
                                   kAudioUnitScope_Input,
                                   0,
                                   &busCount,
                                   sizeof (busCount)
                                   );
    NSCAssert (result == noErr, @"AudioUnitSetProperty Set mixer bus count. Error code: %d '%.4s'", (int) result, (const char *)&result);

    // Connect the nodes to the mixer node
    for(int i=0; i<[samplerNodes count]; i++) {
        // Extract the sampler unit
        wrapper = [samplerNodes objectAtIndex:i];
        [wrapper getValue:&samplerNode];

        // Get the voice def to find out the volume
        voiceDef = [_voiceDefs objectAtIndex:i];
        
        
        // Connect the sampler unit to the mixer unit
        result = AUGraphConnectNodeInput(_processingGraph, samplerNode, 0, mixerNode, i);
    
        // Set the volume of the channel
        AudioUnitSetParameter(_mixerUnit, kMultiChannelMixerParam_Volume, kAudioUnitScope_Input, i, voiceDef.volume, 0);
        
        NSCAssert (result == noErr, @"Couldn't connect speech synth unit output (0) to mixer input (1). Error code: %d '%.4s'", (int) result, (const char *)&result);
    }
    
    // Connect the output of the mixer node to the input of he io node
    result = AUGraphConnectNodeInput (_processingGraph, mixerNode, 0, ioNode, 0);
    NSCAssert (result == noErr, @"Unable to interconnect the nodes in the audio processing graph. Error code: %d '%.4s'", (int) result, (const char *)&result);
    
    
    // Print a graphic version of the graph
    //CAShow(_processingGraph);
        
    return YES;
}

-(void) restart {
    [_toDelete removeAllObjects];
    [_notesToPlay removeAllObjects];
    
    NSValue * value;
    AudioUnit sampler;

    // Stop all currently playing notes
    for(NSInteger i=0; i<127; i++) {
        for(id voice in _voiceChannels) {
            value = [_voiceChannels objectForKey:voice];
            [value getValue:&sampler];
            [self playMIDINote:0x80 withNote:i withVelocity:0 withSampler:sampler];
        }
    }
}

// Set the mixer unit output volume
-(void) setVolume: (float) volume {
    AudioUnitSetParameter(_mixerUnit, kMultiChannelMixerParam_Volume, kAudioUnitScope_Output, 0, volume, 0);
}


// Set the output balance;
-(void) setBalance: (float) balance {
    
    NSLog(@"balance : %f", balance);
    
    AudioUnitSetParameter(_mixerUnit, kMultiChannelMixerParam_Pan, kAudioUnitScope_Output, 0, balance, 0);
}

// Return whether a voice with the given name exists
-(BOOL) containsVoice: (NSString*) voice {
    if([_voiceChannels objectForKey:voice]!=Nil) {
        return YES;
    }
    return NO;
}

// Starting with instantiated audio processing graph, configure its 
// audio units, initialize it, and start it.
- (void) configureAndStartAudioProcessingGraph: (AUGraph) graph {
    
    OSStatus result = noErr;
    
    if (graph) {
        
        // Initialize the audio processing graph.
        result = AUGraphInitialize (graph);
        
        NSAssert (result == noErr, @"Unable to initialze AUGraph object. Error code: %d '%.4s'", (int) result, (const char *)&result);
        
        // Start the graph
        result = AUGraphStart (graph);
        NSAssert (result == noErr, @"Unable to start audio processing graph. Error code: %d '%.4s'", (int) result, (const char *)&result);
        
    }
}

-(void) loadSoundFont: (NSString*) path withPatch: (int) patch withBank: (UInt8) bank withSampler: (AudioUnit) sampler {
    
    NSLog(@"Sound font: %@", path);
    
    NSURL *presetURL = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:path ofType:@"sf2"]];
    [self loadFromDLSOrSoundFont: (NSURL *)presetURL withBank: bank withPatch: patch  withSampler:sampler];
    //[presetURL relativePath];
    //[presetURL release];
}

// Load a SoundFont into a sampler
-(OSStatus) loadFromDLSOrSoundFont: (NSURL *)bankURL withBank: (UInt8) bank withPatch: (int)presetNumber withSampler: (AudioUnit) sampler {
    OSStatus result = noErr;
    
    // fill out a bank preset data structure
    AUSamplerBankPresetData bpdata;
    bpdata.bankURL  = (__bridge CFURLRef) bankURL;
    bpdata.bankMSB  = bank;
    bpdata.bankLSB  = kAUSampler_DefaultBankLSB;
    bpdata.presetID = (UInt8) presetNumber;
    
    // set the kAUSamplerProperty_LoadPresetFromBank property
    result = AudioUnitSetProperty(sampler,
                                  kAUSamplerProperty_LoadPresetFromBank,
                                  kAudioUnitScope_Global,
                                  0,
                                  &bpdata,
                                  sizeof(bpdata));
    
    // check for errors
    NSCAssert (result == noErr,
               @"Unable to set the preset property on the Sampler. Error code:%d '%.4s'",
               (int) result,
               (const char *)&result);
    
    return result;
}

-(OSStatus) loadFromDLSOrSoundFont: (NSURL *)bankURL withPatch: (int)presetNumber withSampler: (AudioUnit) sampler {
    return [self loadFromDLSOrSoundFont: bankURL withBank: kAUSampler_DefaultMelodicBankMSB withPatch: presetNumber withSampler: sampler];
}

-(BOOL) isNoMoreNode{
    return _noMoreNode;
}
@end
