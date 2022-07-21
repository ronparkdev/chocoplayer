//
//  BNoteProcessor.h
//  FirstGame
//
//  Created by Ben Smiley-Andrews on 02/04/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#include "BMidiNote.h"

@protocol BNoteProcessor <NSObject>
  
@required
-(void) addNote: (BMidiNote*) note;

@end

