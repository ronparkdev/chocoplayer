//
//  BMidiManager.h
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 02/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/MusicPlayer.h>
#import "BSequence.h"
#import "BMidiEvent.h"
#import "BMidiNote.h"
#import "BTempoEvent.h"
#import "BChannelEvent.h"

@class GameSessionManager;

@interface BMidiManager : NSObject {
    
    // Clock pulses per quarter note (crotchet)
    NSInteger _PPQN;
}

-(BSequence *) processMidiFile: (NSString *) path;

@end
