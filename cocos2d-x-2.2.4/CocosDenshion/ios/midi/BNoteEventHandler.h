//
//  BNoteEventHandler.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 21/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//


#include "BMidiNote.h"

@protocol BNoteEventHandler <NSObject>

@required
-(void) handleNoteEvent: (BMidiNote*) note;

@end