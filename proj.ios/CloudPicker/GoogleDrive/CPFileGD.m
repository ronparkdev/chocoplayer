//
//  CPFileGD.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFileGD.h"

@implementation CPFileGD

- (id)initWithFile:(GTLDriveFile *)file{
    self = [super init];
    if(self != nil){
        mFile = file;
    }
    return self;
}

- (BOOL)isFolder{
    return [mFile.mimeType isEqualToString:@"application/vnd.google-apps.folder"];
}

- (NSString *)getName{
    return mFile.title;
}

- (NSInteger)getSize{
    return mFile.fileSize;
}

- (NSString *)getIdentifier{
    return mFile.identifier;
}

- (GTLDriveFile *)getFile{
    return mFile;
}
@end
