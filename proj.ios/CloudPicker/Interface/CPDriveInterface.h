//
//  CPDriveInterface.h
//  ios
//
//  Created by  A on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CPFolderInterface;

typedef void (^queryFinishHandler)();

@interface CPDriveInterface : NSObject{
    queryFinishHandler mQueryFinishHandler;
}

- (void)setQueryFinishHandler:(queryFinishHandler)handler;

- (BOOL)viewDidLoad:(UIViewController *)view;
- (BOOL)viewDidAppear:(UIViewController *)view;

- (void)logout;
- (BOOL)request:(CPFolderInterface *)folder;

- (CPFolderInterface *)getRootFolder;


@end
