//
//  CPFileDB.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFileInterface.h"

#import <DropboxSDK/DropboxSDK.h>

@interface CPFileDB : CPFileInterface{
    DBMetadata *mFile;
}

- (id)initWithFile:(DBMetadata *)file;

- (DBMetadata *)getFile;

@end
