//
//  CPDriveGD.h
//  ios
//
//  Created by  A on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CPDriveInterface.h"
#import "CPFolderInterface.h"
#import "CPFolderGD.h"

#import "GTMOAuth2ViewControllerTouch.h"
#import "GTLDrive.h"

@interface CPDriveGD : CPDriveInterface{
    UIViewController *mViewCtrl;
    BOOL mIsAuthRequested;
    BOOL mIsProcessing;

    CPFolderGD *mResultFolder;

    GTLServiceDrive *mService;
}

- (GTLServiceDrive *)getService;

@end
