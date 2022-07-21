//
//  CPFolderTableViewController.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 2..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "MBProgressHUD.h"
#import "CPDriveInterface.h"

@interface CPFolderTableViewController : UITableViewController<UITableViewDelegate>{
    CPDriveInterface *mDrive;
    CPFolderInterface *mFolder;
    
    BOOL mIsContentsLoaded;
    NSString *mPath;
    NSMutableArray *mContents;
    MBProgressHUD *mHud;
}



+ (void)show:(UIViewController *)oldView
      target:(CPDriveInterface *)drive
      folder:(CPFolderInterface *)folder;

@end
