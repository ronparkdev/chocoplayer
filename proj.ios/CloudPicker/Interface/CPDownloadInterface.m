//
//  CPDownloadInterface.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadInterface.h"

@implementation CPDownloadInterface

- (id)init{
    self = [super init];
    if(self != nil){
        mIsFinished = NO;
    }
    return self;
}

- (void)setProgressHandler:(progressHandler)handler{
    mProgressHandler = [handler copy];
}
- (void)setFinishHandler:(finishHandler)handler{
    mFinishHandler = [handler copy];
}
- (void)setErrorHandler:(errorHandler)handler{
    mErrorHandler = [handler copy];
}

- (void)start:(CPDriveInterface *)drive{
    NSLog(@"CPDownloadInterface start operation is gone");
}

- (void)cancel{
    NSLog(@"CPDownloadInterface cancel operation is gone");
}

- (BOOL)isFinished{
    return mIsFinished;
}

- (CPFileInterface *)getFile{
    NSLog(@"CPDownloadInterface getFile operation is gone");
    return nil;
}

- (NSString *)applicationDocumentsDirectory {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    return basePath;
}

- (NSString *)getSavePath:(NSString *)fileName{
    NSString *path = [self applicationDocumentsDirectory];
    NSString *fullpath;
    
    //Check if dont have dot in filename
    NSUInteger dot = [fileName rangeOfString:@"." options:NSBackwardsSearch].location;
    
    if([path characterAtIndex:path.length-1] != '/')
        path = [NSString stringWithFormat:@"%@/", path];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    if (![fileManager fileExistsAtPath:path])
        [fileManager createDirectoryAtPath:path withIntermediateDirectories:NO attributes:nil error:nil];
    
    NSInteger i = 1;
    do{
        if(i == 1){
            fullpath = [NSString stringWithFormat:@"%@%@", path, fileName];
        }else{
            if(dot == NSNotFound){
                fullpath = [NSString stringWithFormat:@"%@%@(%d)", path, fileName, i];
            }else{
                fullpath = [NSString stringWithFormat:@"%@%@(%d)%@", path, [fileName substringToIndex:dot], i, [fileName substringFromIndex:dot]];
            }
        }
        
        if(![fileManager fileExistsAtPath:fullpath])
            break;
        i++;
    }while(true);
    
    NSLog(@"%@ + %@ => %@",path, fileName, fullpath);
    
    return fullpath;
}

@end
