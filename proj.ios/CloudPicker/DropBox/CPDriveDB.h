//
//  CPDriveDB.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDriveInterface.h"

#import "CPFolderGD.h"

#import <DropboxSDK/DropboxSDK.h>

@class DBRestClient;

@interface CPDriveDB : CPDriveInterface<DBSessionDelegate, DBNetworkRequestDelegate, DBRestClientDelegate>{
    UIViewController *mViewCtrl;
    BOOL mIsAuthRequested;
    BOOL mIsProcessing;
    
    CPFolderGD *mResultFolder;
    
	NSString *relinkUserId;
    DBRestClient* mRestClient;
}

- (DBRestClient *)getService;

@end
