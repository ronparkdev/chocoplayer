//
//  CPDownloadDB.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadDB.h"

#import "CPDriveDB.h"

@implementation CPDownloadDB

- (id)initWithFile:(CPFileDB *)file{
    self = [super init];
    if(self != nil){
        mFile = file;
    }
    return self;
}

- (void)start:(CPDriveInterface *)drive{
    mService = [((CPDriveDB *)drive) getService];
    
    NSString *url = [mFile getFile].path;
    
    mService.delegate = self;
    [mService loadFile:url intoPath:[self getSavePath:[mFile getName]]];
}

- (void)cancel{
    if(mService != nil)
        [mService cancelAllRequests];
}

- (CPFileInterface *)getFile{
    return mFile;
}

- (void)restClient:(DBRestClient *)client loadProgress:(CGFloat)progress forFile:(NSString *)destPath{
    if(mProgressHandler != nil)
        mProgressHandler(progress * [mFile getSize], [mFile getSize]);
}

- (void)restClient:(DBRestClient *)client loadedFile:(NSString *)destPath contentType:(NSString *)contentType{
    mIsFinished = YES;
    if(mFinishHandler != nil)
        mFinishHandler();
}

- (void)restClient:(DBRestClient *)client loadFileFailedWithError:(NSError *)error{
    if(mErrorHandler != nil)
        mErrorHandler([error localizedDescription]);
}

@end
