//
//  CPDownloadGD.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadGD.h"

@implementation CPDownloadGD

- (id)initWithFile:(CPFileGD *)file{
    self = [super init];
    if(self != nil){
        mFile = file;
        mIsFinished = NO;
    }
    return self;
}

- (void)start:(CPDriveInterface *)drive{
    GTLServiceDrive *service = [((CPDriveGD *)drive) getService];
    
    NSString *url = [mFile getFile].downloadUrl;
    mFetched = [service.fetcherService fetcherWithURLString:url];
    
    __weak CPDownloadInterface *blockself = self;
    NSInteger total = [[mFile getFile].fileSize longValue];
    [mFetched setReceivedDataBlock:^(NSData *data) {
        if(mProgressHandler != nil)
            mProgressHandler(mFetched.downloadedLength, total);
    }];
    
    [mFetched beginFetchWithCompletionHandler:^(NSData *data, NSError *error) {
        if (error == nil) {
            mIsFinished = YES;
            
            [data writeToFile:[self getSavePath:[mFile getName]] atomically:YES];
            
            if(mFinishHandler != nil)
                mFinishHandler();
        } else {
            if(mErrorHandler != nil)
                mErrorHandler([error localizedDescription]);
            
            NSLog(@"An error occurred: %@", error);
        }
        mFetched = nil;
    }];
}

- (void)cancel{
    if(mFetched != nil){
        [mFetched stopFetching];
        mFetched = nil;
    }
}

- (CPFileInterface *)getFile{
    return mFile;
}

@end
