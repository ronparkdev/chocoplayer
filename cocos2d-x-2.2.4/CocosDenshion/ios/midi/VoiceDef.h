//
//  VoiceDef.h
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 9. 5..
//
//

#import "CPFileInterface.h"

// Defines a voice (instrument sound effect)
@interface VoiceDef : NSObject
// Path to the sound font file
@property(nonatomic, strong) NSString * soundPath;
// Name chosen by the user to index the voice
@property(nonatomic, strong) NSString * name;
// Sound font patch number
@property(nonatomic, assign) UInt8 patch;

// If the sounds are stored in a different bank
@property(nonatomic, assign) UInt8 bank;
// Relative volume: 0 - 1
@property(nonatomic, assign) float volume;

@end;