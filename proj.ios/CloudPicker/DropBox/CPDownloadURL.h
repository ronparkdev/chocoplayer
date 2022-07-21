//
//  CPDownloadURL.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 5..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadInterface.h"

@interface CPDownloadURL : CPDownloadInterface<NSURLConnectionDataDelegate, NSURLConnectionDataDelegate, NSURLConnectionDownloadDelegate>{
    NSString *mURL;
    NSMutableData *mData;
    NSURLConnection *mConnection;
    unsigned long long mFileSize;
    unsigned long long mReceivedSize;
    NSString *mFileName;
}

- (id)initWithURL:(NSString *)url;

@end
