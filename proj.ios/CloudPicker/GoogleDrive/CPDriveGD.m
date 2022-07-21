//
//  CPDriveGD.m
//  ios
//
//  Created by  A on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDriveGD.h"

#import "CPFileGD.h"

static NSString *const kKeychainItemName = @"Drive API";
static NSString *const kClientID = @"421629991605-2lo6g163td2hr75fglsvu3dkopj4l78n.apps.googleusercontent.com";
static NSString *const kClientSecret = @"H9W92vEqyIqApt7XZlbT2avq";

@implementation CPDriveGD

- (id)init{
    self = [super init];
    if(self != nil){
        mIsAuthRequested = NO;
        mIsProcessing = NO;
        mService = nil;
    }
    return self;
}

- (BOOL)viewDidLoad:(UIViewController *)view{
    
    mViewCtrl = view;
    
    //[GTMOAuth2ViewControllerTouch removeAuthFromKeychainForName:kKeychainItemName];
    
    if(mService == nil){
        mService = [[GTLServiceDrive alloc] init];
        mService.authorizer =
        [GTMOAuth2ViewControllerTouch authForGoogleFromKeychainForName:kKeychainItemName
                                                              clientID:kClientID
                                                          clientSecret:kClientSecret];
        
        return NO;
    }
    
    return mService.authorizer.canAuthorize;
}

- (BOOL)viewDidAppear:(UIViewController *)view{

    mViewCtrl = view;
    
    if (!mService.authorizer.canAuthorize) {
        // Not yet authorized, request authorization by pushing the login UI onto the UI stack.
        //[self presentViewController:[self createAuthController] animated:YES completion:nil];
        if(!mIsAuthRequested){
            [view presentViewController:[self createAuthController] animated:YES completion:nil];

            //[view.navigationController pushViewController:[self createAuthController] animated:YES];
            mIsAuthRequested = YES;
        }else{
            [view.navigationController popToRootViewControllerAnimated:YES];
        }
        
        return NO;
    } else {
        //[self fetchFiles];
        mIsAuthRequested = YES;
        
        return YES;
    }
}

- (GTMOAuth2ViewControllerTouch *)createAuthController {
    
    GTMOAuth2ViewControllerTouch *authController;
    NSArray *scopes = [NSArray arrayWithObjects:kGTLAuthScopeDrive, nil];
    authController = [[GTMOAuth2ViewControllerTouch alloc]
                      initWithScope:[scopes componentsJoinedByString:@" "]
                      clientID:kClientID
                      clientSecret:kClientSecret
                      keychainItemName:kKeychainItemName
                      delegate:self
                      finishedSelector:@selector(viewController:finishedWithAuth:error:)];
    return authController;
}

- (void)viewController:(GTMOAuth2ViewControllerTouch *)viewController
      finishedWithAuth:(GTMOAuth2Authentication *)authResult
                 error:(NSError *)error {

    if (error != nil) {
        
        switch(error.code){
            case -1000:
                break;
                
            default:
                [self showAlert:@"Authentication Error" message:error.localizedDescription];
        }
        mService.authorizer = nil;
    }
    else {
        mService.authorizer = authResult;
        [mViewCtrl dismissViewControllerAnimated:YES completion:nil];
    }
}

// Helper for showing an alert
- (void)showAlert:(NSString *)title message:(NSString *)message {
    UIAlertView *alert;
    alert = [[UIAlertView alloc] initWithTitle:title
                                       message:message
                                      delegate:nil
                             cancelButtonTitle:@"OK"
                             otherButtonTitles:nil];
    [alert show];
}

- (void)logout{
    [GTMOAuth2ViewControllerTouch removeAuthFromKeychainForName:kKeychainItemName];
}

- (BOOL)request:(CPFolderInterface *)folder{

    if(!mIsAuthRequested || mIsProcessing)
        return NO;
    
    mIsProcessing = YES;
    
    mResultFolder = (CPFolderGD *)folder;
    
    GTLQueryDrive *query = [GTLQueryDrive queryForFilesList];
    query.q = [NSString stringWithFormat:@"'%@' in parents", [mResultFolder getIdentify]];
    
    NSLog(query.q);
    
    //query.maxResults = 10;
    [mService executeQuery:query
                      delegate:self
             didFinishSelector:@selector(displayResultWithTicket:finishedWithObject:error:)];
    
    
    return YES;
}

// Process the response and display output.
- (void)displayResultWithTicket:(GTLServiceTicket *)ticket
             finishedWithObject:(GTLDriveFileList *)files
                          error:(NSError *)error {
    
    mIsProcessing = NO;
    
    if (error == nil) {
        [mResultFolder.contents removeAllObjects];
        CPFileGD *node;
        if (files.items.count > 0) {
            for (GTLDriveFile *file in files) {
                node = [[CPFileGD alloc] initWithFile:file];
                [mResultFolder.contents addObject:node];
            }
        }
        [mResultFolder setIsLoaded:YES];
    } else {
        [self showAlert:@"Error" message:error.localizedDescription];
    }
    
    [mResultFolder.contents sortUsingComparator:^NSComparisonResult(id a, id b) {
        CPFileGD *first = (CPFileGD*)a;
        CPFileGD *second = (CPFileGD*)b;
        
        if(first.isFolder == second.isFolder)
            return [first.getName compare:second.getName];
        else{
            return second.isFolder;
        }
            
    }];
    
    if(mQueryFinishHandler != nil)
        mQueryFinishHandler();
}

- (CPFolderInterface *) getRootFolder{
    return [[CPFolderGD alloc]initWithName:@"Google Drive" path:@"/" identifier:@"root"];
    //return [[CPFolderGD alloc]initWithName:@"" path:@"" identifier:@"root"];
}

- (GTLServiceDrive *)getService{
    return mService;
}

@end
