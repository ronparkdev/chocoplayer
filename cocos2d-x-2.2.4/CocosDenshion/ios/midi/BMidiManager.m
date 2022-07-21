//
//  BMidiManager.m
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 02/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BMidiManager.h"

/*
 * MidiManager Enapsulates the process of parsing MIDI.
 * It provides one public method - processMidiFile which
 * converts a midi file into an array containing MIDI 
 * event objects
 */
@implementation BMidiManager

/*
 * Extract the MidiEvents from a MIDI file and add them to a
 * BSequence object
 */
- (BSequence *) processMidiFile: (NSString *) path {
    
    // Create a new music sequence - this will store all the
    // data collected from the midi file
    MusicSequence s;
    
    // Initialise the music sequence - CoreMidi is procedural
    // but it tries to use some techniques from object orientated
    // programming. New music sequence is like initialising the
    // MusicSequence variable
    NewMusicSequence(&s);
    
    // Get a string to the path of the MIDI file which
    // should be located in the Resources folder
//    NSString *midiFilePath = [[NSBundle mainBundle]
//                              pathForResource:@"music0"
//                              ofType:@"mid"];
    
    // Create a new URL which points to the MIDI file
    NSURL * midiFileURL = [NSURL fileURLWithPath:path];
    
    // Load the MIDI file into the MusicSequence variable
    MusicSequenceFileLoad(s, (__bridge CFURLRef)midiFileURL, 0, 0);
    
    // You can use CAShow here to print a text version of the whole
    // midi file into the consol for debugging
    //CAShow(s);
    
    // The number of tracks
    UInt32 tracks;
    
    // Populate the track count
    MusicSequenceGetTrackCount(s, &tracks);
    
    // Create a track 
    MusicTrack track = NULL;
    
    // Create a pointer to the sequence. The sequence object
    // contains an array of midi events and some useful methods
    // for operating on the array
    BSequence * sequence = [BSequence new];
    
    // The tempo track is a track which only contains tempo
    // information. It contains no notes
    MusicTrack tempoTrack;
    
    // Retrieve the tempo track from the sequence 
    MusicSequenceGetTempoTrack(s, &tempoTrack);
    
    // Get the pulses per quarter note from the tempo track
    SInt16 resolution;
    UInt32 length;
    
    // The pulses per quarter note is stored as a property in the
    // track. This is accessed using the get property method
    MusicTrackGetProperty(tempoTrack, kSequenceTrackProperty_TimeResolution, &resolution, &length);
    
    _PPQN = resolution;
    
    // Add a ppnq event to the sequence at zero time
    BTempoEvent * ppnqEvent = [BTempoEvent new];
    [ppnqEvent setStartTime:0];
    // Set the event type to PPNQ
    ppnqEvent.type = BPPNQ;
    ppnqEvent.PPNQ = _PPQN;

    // Add the PPNQ event to our sequence
    [sequence addEvent: ppnqEvent];

    // Print the PPNQ for logging purposes
    NSLog(@"PPNQ: %i", _PPQN);

    // Process the note sequences
    for (NSInteger i=0; i<tracks; i++) {
        // Set the track variable to the current track number
        MusicSequenceGetIndTrack(s, i, &track);
        
        // Add the events from the track to the sequence object
        [self processTrack:track withTrackNumber:i withSequence: sequence];
    }
    
    // Process the events from the tempo track
    [self processTrack:tempoTrack withTrackNumber:99 withSequence: sequence];
    
    // Make sure the sequence is ordered by note start time ready to be played
    [sequence sortSequenceByStartTime];
    
    return sequence;
}

- (void) processTrack: (MusicTrack) track withTrackNumber: (NSInteger) trackNum withSequence: (BSequence * ) sequence {
    
    // Setup iterator - the iterator helps us loop over the events in the track
    MusicEventIterator iterator = NULL;
    NewMusicEventIterator(track, &iterator);
    
    // Values to be retrieved from event
    // Start time in quarter notes
    MusicTimeStamp timestamp = 0;
    // The MIDI message type
    MusicEventType eventType = 0;
    
    // The data contained in the message
    const void *eventData = NULL;
    UInt32 eventDataSize = 0;
    
    // Prepair variables for loop
    Boolean hasNext = YES;
    
    // Some variables to contain events which we'll use many times
    MIDINoteMessage * midiNoteMessage;
    MIDIMetaEvent * midiMetaEvent;
    
    while (hasNext) {
        
        // See if there are any more events
        MusicEventIteratorHasNextEvent(iterator, &hasNext);
        
        // Copy the event data into the variables we prepaired earlier
        MusicEventIteratorGetEventInfo(iterator, &timestamp, &eventType, &eventData, &eventDataSize);
        
        // Process Midi Note messages
        if(eventType==kMusicEventType_MIDINoteMessage) {
            // Cast the midi event data as a midi note message
            midiNoteMessage = (MIDINoteMessage*) eventData;
            
            // Create a new note event object
            BMidiNote * midiNote = [BMidiNote new];
            
            // Add the timestamp so we know when to play the note
            // Convert the start time and duration into clock pulses by multiplying by PPQN
            [midiNote setStartTime: timestamp * _PPQN];
            [midiNote setDuration:midiNoteMessage->duration * _PPQN];
            
            // Set the note velocity - how loud the note is played
            midiNote.velocity = midiNoteMessage->velocity;
            
            // The channel the note is played on - this defines the instrument
            midiNote.channel = midiNoteMessage->channel;
            midiNote.releaseVelocity = midiNoteMessage->releaseVelocity;
            
            midiNote.track = trackNum;
            
            // Add the midi message note - a number 0 - 127
            midiNote.note = midiNoteMessage->note;
            
            // Here I'm limiting the number of channels to be used to
            // 10. There seems to be a bug in CoreAudio which causes
            // audio distortions when more than 10 Midi samplers are
            // used. Remove this at your own risk!
            if( midiNote.channel < 5)
                [sequence addEvent: midiNote];
            
        }
        
        // Channel messages - control change / program change
        if(eventType == kMusicEventType_MIDIChannelMessage) {
            // Cast the event data as a channel message
            MIDIChannelMessage * channelMessage = (MIDIChannelMessage *) eventData;
            
            // Create a new channel event object
            BChannelEvent * channelEvent = [BChannelEvent new];
            
            // Set the start time in pulses
            [channelEvent setStartTime:timestamp * _PPQN];
            
            // set the channel and data - note the use of bitwise
            // operations to extract the channel
            channelEvent.channel = channelMessage->status & 0x0f;
            
            // For more information see BChannelEvent.h
            channelEvent.data1 = channelMessage->data1;
            channelEvent.data2 = channelMessage->data2;
            
            // Control Change - Used to set the value of a particular controller
            // Note the use of bitwise operations to extract the message type
            if(channelMessage->status >> 4 == 11) {
                channelEvent.type = Controller;
            }

            // Program Change - Used to assign an instrument to a particular
            // channel
            if(channelMessage->status >> 4 == 12) {
                channelEvent.type = Program;
            }
            
            // Add the events to the sequence
            [sequence addEvent:channelEvent];
            
        }
        
        // The extended tempo event happens on the tempo track
        // It contains a beats per minute value
        if(eventType == kMusicEventType_ExtendedTempo) {
            ExtendedTempoEvent * te = (ExtendedTempoEvent *) eventData;
            
            // Create a new tempo event
            BTempoEvent * tempoEvent = [BTempoEvent new];
            
            // Set the start time in pulses
            [tempoEvent setStartTime:timestamp * _PPQN];
            // Set the type to tempo
            tempoEvent.type = BTempo;
            // Set the bpm value
            tempoEvent.BPM = te->bpm;
            
            // Add the event to the sequence
            [sequence addEvent:tempoEvent];
        }
        
        // Process Meta events
        
        if(eventType == kMusicEventType_Meta) {
            
            midiMetaEvent = (MIDIMetaEvent*) eventData;
                        
            // Time signature information
            if(midiMetaEvent->metaEventType == 0x58) {
                
                // Create a new tempo event
                BTempoEvent * tempoEvent = [BTempoEvent new];
                
                // Set the start time in pulses
                [tempoEvent setStartTime: timestamp * _PPQN];
                
                // Set the type to time signature
                tempoEvent.type = BTimeSignature;
                
                // Set the numerator - beats per bar
                tempoEvent.timeSignatureNumerator = midiMetaEvent->data[0];
                // Which note value represents one beat 4 -> quarter note, 2 implies half note 
                tempoEvent.timeSignatureDenominator = powf(2, midiMetaEvent->data[1]);
                // Metronome ticks per quarter a value of 24 implies one tick per quarter note
                tempoEvent.ticksPerQtr = midiMetaEvent->data[2];
                // Used to vary rate of play of piece
                tempoEvent._32ndNotesPerBeat = midiMetaEvent->data[3];
                
                // Add event to the sequence
                [sequence addEvent:tempoEvent];

            }
        }
        // Update the iterator to the next event
        MusicEventIteratorNextEvent(iterator);
    }
}

@end
