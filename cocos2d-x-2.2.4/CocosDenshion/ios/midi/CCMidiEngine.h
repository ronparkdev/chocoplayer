//
//  CCMidiEngine.h
//  chocoplayer
//
//  Created by psm on 2015. 8. 20..
//
//

#import <Foundation/Foundation.h>

#import "BMidiManager.h"
#import "BSequence.h"
#import "BSequencePlayer.h"
#import "BMidiClock.h"
#import "AudioManager.h"
#import "BMidiNote.h"
#import "BNoteEventHandler.h"

@interface CCMidiEngine : NSObject <BNoteEventHandler, BTempoEventHandler>{
    BMidiManager * _midiManager;
    BSequencePlayer * _sequencePlayer;
    BMidiClock * _midiClock;
    AudioManager * _audioManager;
    NSThread * _audioThread;
    
    NSString *_lastPath;
    float _lastVolume;
    float _lastTempo;
    float _lastBalance;
    BOOL _isLoop;
}

- (void)play:(NSString*)path
      volume:(float)fvolume
       tempo:(float)ftempo
     balance:(float)fbalance
      isLoop:(BOOL)isloop;

- (void)pause;

- (void)resume;

- (void)stop;

- (void)setVolume:(float)fVolume;

@end
