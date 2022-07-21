//
//  BMidiEvent.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 14/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>

// Define the different availble event types
typedef enum {
	Note,
	Tempo,
	Lyric,
    Channel
} BEventType;

@interface BMidiEvent : NSObject {
    NSInteger _startTime;
    BOOL _isPassed;
}

// In pulses
@property (nonatomic, readwrite) BEventType eventType;

-(void) setStartTime: (float) startTime;
-(NSInteger) getStartTime;

-(BOOL) isPassed;
-(void) setPassed:(BOOL)isPassed;

@end
