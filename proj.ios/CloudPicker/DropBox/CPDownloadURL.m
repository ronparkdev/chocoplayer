//
//  CPDownloadURL.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 5..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadURL.h"

#import "CPFileURL.h"

@implementation CPDownloadURL

- (id)initWithURL:(NSString *)url{
    self = [super init];
    if(self != nil){
        mURL = url;
    }
    return self;
}

- (void)start:(CPDriveInterface *)drive{
    
    NSURL *url = [NSURL URLWithString:mURL];
    NSURLRequest *request = [NSURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringLocalCacheData timeoutInterval:10];
    mData = [[NSMutableData alloc] initWithLength:0];
    mConnection = [[NSURLConnection alloc] initWithRequest:request delegate:self startImmediately:YES];
    [mConnection start];
}

- (void)cancel{
    if(mConnection != nil){
        [mConnection cancel];
        mConnection = nil;
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response{
    mReceivedSize = 0;
    mFileSize = response.expectedContentLength;
    mFileName = response.textEncodingName;
    [mData setLength:0];
}

- (void)connection:(NSURLConnection *)connection didWriteData:(long long)bytesWritten totalBytesWritten:(long long)totalBytesWritten expectedTotalBytes:(long long)expectedTotalBytes{
    
    mReceivedSize = totalBytesWritten;
    
    if(mFileSize < 0 && expectedTotalBytes > 0)
        mFileSize = expectedTotalBytes;
    
    if(mProgressHandler != nil)
        mProgressHandler(mReceivedSize, mFileSize);
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data{
    [mData appendData:data];
    
    mReceivedSize += data.length;
    
    if(mProgressHandler != nil)
        mProgressHandler(mReceivedSize, mFileSize);
}

- (void)connectionDidFinishDownloading:(NSURLConnection *)connection destinationURL:(NSURL *)destinationURL{
    
    [mData writeToFile:[self getSavePath:mFileName] atomically:YES];
    
    if(mFinishHandler != nil)
        mFinishHandler();
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error{
    
    if(mErrorHandler != nil)
        mErrorHandler(error.localizedDescription);
}

- (CPFileInterface *)getFile{
    CPFileURL *file = [[CPFileURL alloc]initWithURL:mURL];
    return file;
}
@end
