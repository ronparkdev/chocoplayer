//
//  BTempoEvent.m
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 14/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BTempoEvent.h"

@implementation BTempoEvent

@synthesize timeSignatureNumerator;
@synthesize timeSignatureDenominator;
@synthesize ticksPerQtr;
@synthesize _32ndNotesPerBeat;
@synthesize BPM;
@synthesize PPNQ;
@synthesize type;

- init {
    if((self=[super init])) {
        self.eventType = Tempo;
    }
    return self;
}


@end
