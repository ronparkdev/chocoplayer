//
//  CPFileGD.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CPFileInterface.h"

#import "GTLDrive.h"

@interface CPFileGD : CPFileInterface{
    GTLDriveFile *mFile;
}

- (id)initWithFile:(GTLDriveFile *)file;

- (NSString *)getIdentifier;

- (GTLDriveFile *)getFile;

@end
