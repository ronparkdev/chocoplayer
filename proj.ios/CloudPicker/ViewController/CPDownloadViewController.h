//
//  CPDownloadViewController.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "CPDriveInterface.h"
#import "CPDownloadInterface.h"

@interface CPDownloadViewController : UIViewController<UINavigationBarDelegate>{
    CPDriveInterface *mDrive;
    CPDownloadInterface *mDownload;
    NSThread *mThread;
    BOOL mIsRunning;
    unsigned long long mLastByte;
}

@property (weak, nonatomic) IBOutlet UILabel *lbFileName;
@property (weak, nonatomic) IBOutlet UILabel *lbState;
@property (weak, nonatomic) IBOutlet UILabel *lbSpeed;
@property (weak, nonatomic) IBOutlet UIButton *btnCancel;
@property (weak, nonatomic) IBOutlet UIProgressView *pvDownlaod;
- (IBAction)onCancel:(id)sender;

- (id)initWithTarget:(CPDriveInterface *)drive from:(CPDownloadInterface *)download;


@end
