//
//  CPDriveInterface.m
//  ios
//
//  Created by  A on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDriveInterface.h"

@implementation CPDriveInterface

- (void)setQueryFinishHandler:(queryFinishHandler)handler{
    mQueryFinishHandler = [handler copy];
}

- (BOOL)viewDidLoad:(UIViewController *)view{
    NSLog(@"CPDriveInterface viewDidLoad operation is gone");
    return NO;
}

- (BOOL)viewDidAppear:(UIViewController *)view{
    NSLog(@"CPDriveInterface viewDidAppear operation is gone");
    return NO;
}

- (void)logout{
    NSLog(@"CPDriveInterface logout operation is gone");
}

- (BOOL)request:(CPFolderInterface *)folder{
    NSLog(@"CPDriveInterface request operation is gone");
    return NO;
}

- (CPFolderInterface *)getRootFolder{
    NSLog(@"CPDriveInterface request operation is gone");
    return nil;
}

@end
