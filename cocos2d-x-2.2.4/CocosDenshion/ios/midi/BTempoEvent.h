//
//  BTempoEvent.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 14/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BMidiEvent.h"

typedef enum {
    BTimeSignature, BTempo, BPPNQ
} BTempoEventType;

@interface BTempoEvent : BMidiEvent


@property (nonatomic, readwrite) NSInteger timeSignatureNumerator;
@property (nonatomic, readwrite) NSInteger timeSignatureDenominator;
@property (nonatomic, readwrite) NSInteger ticksPerQtr;
@property (nonatomic, readwrite) NSInteger _32ndNotesPerBeat;
@property (nonatomic, readwrite) float BPM;
@property (nonatomic, readwrite) NSInteger PPNQ;

@property (nonatomic, readwrite) BTempoEventType type;

@end
