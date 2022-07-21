//
//  CPFileDB.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFileDB.h"

@implementation CPFileDB

- (id)initWithFile:(DBMetadata *)file{
    self = [super init];
    if(self != nil){
        mFile = file;
    }
    return self;
}

- (BOOL)isFolder{
    return mFile.isDirectory;
}

- (NSString *)getName{
    return mFile.filename;
}

- (NSInteger)getSize{
    return mFile.totalBytes;
}

- (DBMetadata *)getFile{
    return mFile;
}

@end
