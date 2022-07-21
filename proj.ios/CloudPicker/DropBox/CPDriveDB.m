//
//  CPDriveDB.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDriveDB.h"

#import "CPFolderDB.h"
#import "CPFileDB.h"
#import <stdlib.h>

@implementation CPDriveDB

- (id)init{
    self = [super init];
    if(self != nil){
        mIsAuthRequested = NO;
    }
    return self;
}

- (BOOL)viewDidLoad:(UIViewController *)view{
    
    mViewCtrl = view;
    
	// Set these variables before launching the app
    NSString* appKey = @"0d2jurwjf65gerg";
	NSString* appSecret = @"we74xpj6hcrgvtx";
	NSString *root = kDBRootDropbox; // Should be set to either kDBRootAppFolder or kDBRootDropbox
	// You can determine if you have App folder access or Full Dropbox along with your consumer key/secret
	// from https://dropbox.com/developers/apps
	
	// Look below where the DBSession is created to understand how to use DBSession in your app
	
	NSString* errorMsg = nil;
	if ([appKey rangeOfCharacterFromSet:[[NSCharacterSet alphanumericCharacterSet] invertedSet]].location != NSNotFound) {
		errorMsg = @"Make sure you set the app key correctly in DBRouletteAppDelegate.m";
	} else if ([appSecret rangeOfCharacterFromSet:[[NSCharacterSet alphanumericCharacterSet] invertedSet]].location != NSNotFound) {
		errorMsg = @"Make sure you set the app secret correctly in DBRouletteAppDelegate.m";
	} else if ([root length] == 0) {
		errorMsg = @"Set your root to use either App Folder of full Dropbox";
	} else {
		NSString *plistPath = [[NSBundle mainBundle] pathForResource:@"Info" ofType:@"plist"];
		NSData *plistData = [NSData dataWithContentsOfFile:plistPath];
		NSDictionary *loadedPlist =
        [NSPropertyListSerialization
         propertyListFromData:plistData mutabilityOption:0 format:NULL errorDescription:NULL];
		NSString *scheme = [[[[loadedPlist objectForKey:@"CFBundleURLTypes"] objectAtIndex:0] objectForKey:@"CFBundleURLSchemes"] objectAtIndex:0];
		if ([scheme isEqual:@"db-APP_KEY"]) {
			errorMsg = @"Set your URL scheme correctly in DBRoulette-Info.plist";
		}
	}
	
	DBSession* session =
    [[DBSession alloc] initWithAppKey:appKey appSecret:appSecret root:root];
	session.delegate = self; // DBSessionDelegate methods allow you to handle re-authenticating
	[DBSession setSharedSession:session];
	
	[DBRequest setNetworkRequestDelegate:self];
    
	if (errorMsg != nil) {
		[[[UIAlertView alloc]
		   initWithTitle:@"Error Configuring Session" message:errorMsg
		   delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil]
        show];
	}
    
    mRestClient = [[DBRestClient alloc] initWithSession:[DBSession sharedSession]];
    
    return [[DBSession sharedSession] isLinked];
}

- (BOOL)viewDidAppear:(UIViewController *)view{
    
    mViewCtrl = view;
    
    if (![[DBSession sharedSession] isLinked]) {
        if(!mIsAuthRequested){
            [[DBSession sharedSession] linkFromController:mViewCtrl];
            mIsAuthRequested = YES;
        }else{
            [view.navigationController popToRootViewControllerAnimated:YES];
        }
    } else {
        mIsAuthRequested = YES;
        return YES;
    }
    
    return NO;
}


- (void)logout{
    [[DBSession sharedSession] unlinkAll];
}


#pragma mark -
#pragma mark DBSessionDelegate methods

- (void)sessionDidReceiveAuthorizationFailure:(DBSession*)session userId:(NSString *)userId {
	relinkUserId = userId;
	[[[UIAlertView alloc]
	   initWithTitle:@"Dropbox Session Ended" message:@"Do you want to relink?" delegate:self
	   cancelButtonTitle:@"Cancel" otherButtonTitles:@"Relink", nil]
    show];
}

#pragma mark -
#pragma mark UIAlertViewDelegate methods

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)index {
	if (index != alertView.cancelButtonIndex) {
		[[DBSession sharedSession] linkUserId:relinkUserId fromController:mViewCtrl];
	}
	relinkUserId = nil;
}

- (CPFolderInterface *)getRootFolder{
    return [[CPFolderDB alloc] initWithName:@"DropBox" path:@"/"];
    //return [[CPFolderDB alloc] initWithName:@"" path:@"/"];
}



#pragma mark -
#pragma mark DBNetworkRequestDelegate methods

static int outstandingRequests;

- (void)networkRequestStarted {
	outstandingRequests++;
	if (outstandingRequests == 1) {
		[[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:YES];
	}
}

- (void)networkRequestStopped {
	outstandingRequests--;
	if (outstandingRequests == 0) {
		[[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:NO];
	}
}



#pragma mark 

- (BOOL)request:(CPFolderInterface *)folder{
    
    if(!mIsAuthRequested || mIsProcessing)
        return NO;
    
    mIsProcessing = YES;
    
    mResultFolder = (CPFolderGD *)folder;
    
    mRestClient.delegate = self;
    
    NSString *path = [folder getPath];
    
    [mRestClient loadMetadata:path];
    
    //[mRestClient loadMetadata:[folder getPath]];
    
    return YES;
}

- (void)restClient:(DBRestClient*)client loadedMetadata:(DBMetadata*)metadata {
    
    mIsProcessing = NO;
    
    CPFileDB *node;
    [mResultFolder.contents removeAllObjects];
    for (DBMetadata* file in metadata.contents) {
        node = [[CPFileDB alloc] initWithFile:file];
        [mResultFolder.contents addObject:node];
    }
    
    [mResultFolder.contents sortUsingComparator:^NSComparisonResult(id a, id b) {
        CPFileDB *first = (CPFileDB*)a;
        CPFileDB *second = (CPFileDB*)b;
        
        if(first.isFolder == second.isFolder)
            return [first.getName compare:second.getName];
        else{
            return second.isFolder;
        }
        
    }];
    
    if(mQueryFinishHandler != nil)
        mQueryFinishHandler();
}

- (void)restClient:(DBRestClient*)client loadMetadataFailedWithError:(NSError*)error {
    NSLog(@"mRestClient:loadMetadataFailedWithError: %@", [error localizedDescription]);
    
    if(mQueryFinishHandler != nil)
        mQueryFinishHandler();
}

- (DBRestClient *)getService{
    return mRestClient;
}

@end

