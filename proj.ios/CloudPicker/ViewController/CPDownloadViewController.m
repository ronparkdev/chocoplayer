//
//  CPDownloadViewController.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPDownloadViewController.h"

#import <GTLDrive.h>

@interface CPDownloadViewController ()

@end

@implementation CPDownloadViewController

- (id)initWithTarget:(CPDriveInterface *)drive from:(CPDownloadInterface *)download;
{
    self = [super init];
    if (self) {
        mDrive = drive;
        mDownload = download;
        mLastByte = 0;
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [mDownload start:mDrive];
    
    [self.lbFileName setText:[[mDownload getFile] getName]];
    [self.lbState setText:@"Downloading.."];
    [self.lbSpeed setText:@""];
    
    [self.pvDownlaod setProgress:0.0f];
    [self setRunning:YES];

    
    [mDownload setProgressHandler:^(unsigned long long curbyte, unsigned long long totbyte) {
        
        NSString *speedMsg;
        float speed = (curbyte - mLastByte) / 0.5f;
        mLastByte = curbyte;
        
        float progress = (float)curbyte / totbyte;
        
        [self.pvDownlaod setProgress:progress animated:YES];
        
    }];
    
    [mDownload setFinishHandler:^{
        [self.lbState setText:@"Download Complete!"];
        [self.pvDownlaod setProgress:1.0f animated:YES];
        [self setRunning:NO];

    }];
    
    [mDownload setErrorHandler:^(NSString *error) {
        [self.lbState setText:[NSString stringWithFormat:@"Download Failed! %@", error]];
        [self.pvDownlaod setProgress:0.0f animated:YES];
        [self setRunning:NO];

    }];
}

- (void)viewWillUnload{
    if(!mDownload.isFinished){
        [mDownload cancel];
        
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"CloudPicker" message:nil delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil,nil];
        [alertView setMessage:@"Download Canceled by User"];
        [alertView show];
    }
}

- (void)viewDidAppear:(BOOL)animated{
    mThread = [[NSThread alloc] initWithTarget:self selector:@selector(onRefreshProgress) object:nil];
    [mThread start];
}

- (void)viewDidDisappear:(BOOL)animated{
    if(mThread != nil){
        [mThread cancel];
        mThread = nil;
    }
}

- (void)onRefreshProgress{
    unsigned long long lastByte = 0;
    float speed;
    NSString *speedMsg;
    while([[NSThread currentThread] isCancelled] == NO){
        if(!mIsRunning){
            [[NSThread currentThread] cancel];
        }else{
            speed = (mLastByte - lastByte) / 1.0f;
            lastByte = mLastByte;
            
            if(speed > 1024 * 1024)
                speedMsg = [NSString stringWithFormat:@"%0.2f Mb/sec", (speed / (1024 * 1024))];
            else if(speed > 1024)
                speedMsg = [NSString stringWithFormat:@"%0.2f Kb/sec", (speed / 1024)];
            else
                speedMsg = [NSString stringWithFormat:@"%0.2f  b/sec", speed];
            
            [self performSelectorOnMainThread:@selector(onRefreshProgressUI:) withObject:speedMsg waitUntilDone:YES];
            
            
            [NSThread sleepForTimeInterval:1.0];
        }
    }
}

- (void)onRefreshProgressUI:(NSString *)speedMsg{
    if(speedMsg != nil)
        [self.lbSpeed setText:speedMsg];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onCancel:(id)sender {
    [self.lbState setText:@"Download Canceled"];
    [self setRunning:NO];
    [mDownload cancel];
}

- (void)setRunning:(BOOL)val{
    mIsRunning = val;
    [self.btnCancel setEnabled:val];
}

@end

