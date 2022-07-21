//
//  CPDownloadGD.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadInterface.h"

#import "CPDriveGD.h"
#import "CPFileGD.h"

#import "GTLDrive.h"

@interface CPDownloadGD : CPDownloadInterface{
    CPFileGD *mFile;
    GTMHTTPFetcher *mFetched;
}

- (id)initWithFile:(CPFileGD *)file;

- (GTLDriveFile *)getFile;

@end
