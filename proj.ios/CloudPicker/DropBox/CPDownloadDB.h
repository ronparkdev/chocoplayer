//
//  CPDownloadDB.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadInterface.h"

#import <DropboxSDK/DropboxSDK.h>

#import "CPFileDB.h"

@interface CPDownloadDB : CPDownloadInterface<DBRestClientDelegate>{
    CPFileDB *mFile;
    DBRestClient *mService;
}

- (id)initWithFile:(DBMetadata *)file;

@end
