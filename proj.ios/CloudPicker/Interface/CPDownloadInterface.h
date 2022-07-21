//
//  CPDownloadInterface.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CPDriveInterface.h"
#import "CPFileInterface.h"

typedef void (^progressHandler)(unsigned long long curbyte, unsigned long long totbyte);
typedef void (^finishHandler)();
typedef void (^errorHandler)(NSString * error);

@interface CPDownloadInterface : NSObject{
    BOOL mIsFinished;
    
    progressHandler mProgressHandler;
    finishHandler mFinishHandler;
    errorHandler mErrorHandler;
}

- (void)setProgressHandler:(progressHandler)handler;
- (void)setFinishHandler:(finishHandler)handler;
- (void)setErrorHandler:(errorHandler)handler;

- (void)start:(CPDriveInterface *)drive;
- (void)cancel;

- (CPFileInterface *)getFile;
- (BOOL)isFinished;

- (NSString *)getSavePath:(NSString *)fileName;

@end
