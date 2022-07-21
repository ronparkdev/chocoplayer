//
//  BTempoEventProcessor.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 21/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BTempoEvent.h"

@protocol BTempoEventHandler <NSObject>

-(void) handleTempoEvent: (BTempoEvent*) tempoEvent;

@end
