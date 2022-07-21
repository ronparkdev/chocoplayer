//
//  BSequence.h
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 18/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BMidiEvent.h"

// BSequence is a simple class which stores a list of
// Midi events. It provides a convenience function
// to sort the list in ascending order of time
@interface BSequence : NSObject {
    // An array of midi events
    NSMutableArray * _sequence;
}

-(void) sortSequenceByStartTime;
-(NSMutableArray *) getSequence;
-(void) addEvent: (BMidiEvent *) event;
-(NSInteger) eventCount;

@end
