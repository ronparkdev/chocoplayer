//
//  CPTargetTableViewController.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 2..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPTargetTableViewController.h"

#import "CPFolderTableViewController.h"
#import "CPDownloadViewController.h"

#import "CPDriveGD.h"
#import "CPDriveDB.h"
#import "CPDownloadURL.h"
@interface CPTargetTableViewController ()

@end

@implementation CPTargetTableViewController

+ (void)show:(UIViewController *)viewctl{
    UINavigationController *navi = [[UINavigationController alloc]init];
    navi.modalPresentationStyle = UIModalPresentationFormSheet;
    navi.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
    
    CPTargetTableViewController *table = [[CPTargetTableViewController alloc]initWithStyle:UITableViewStylePlain];
    
    [navi pushViewController:table animated:NO];
    [viewctl presentModalViewController:navi animated:YES];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self setTitle:@"Target"];
    
    
    // Uncomment the following line to preserve selection between presentations.
    self.clearsSelectionOnViewWillAppear = NO;
    
    [self.tableView registerNib:[UINib nibWithNibName:@"CPTargetTableViewController" bundle:nil] forCellReuseIdentifier:@"reuseIdentifierss"];

    
    UIBarButtonItem *itemQuit = [[UIBarButtonItem alloc] initWithTitle:@"Quit" style:UIBarButtonItemStyleBordered target:self action:@selector(quit)];
    self.navigationItem.rightBarButtonItem = itemQuit;
}

- (void)quit{
    [self dismissViewControllerAnimated:YES completion:nil ];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cID = @"reuseIdentifier";
    NSArray *arr = [[NSArray alloc]initWithObjects: @"GoogleDrive", @"DropBox", @"Url", nil];
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cID];
    
    // Configure the cell...
    
    if(cell == nil){
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cID];
    }
    
    cell.textLabel.text = [arr objectAtIndex:indexPath.row];
    
    return cell;
}


#pragma mark - Table view delegate

// In a xib-based application, navigation from a table can be handled in -tableView:didSelectRowAtIndexPath:

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    CPDriveInterface *drive = nil;
    
    switch(indexPath.row){
        case 0:drive = [[CPDriveGD alloc] init];break;
        case 1:drive = [[CPDriveDB alloc] init];break;
    }
    
    if(drive != nil){
        [CPFolderTableViewController show:self target:drive folder:nil];
    }else{
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"insert URL here"
                                                        message:nil
                                                       delegate:self
                                              cancelButtonTitle:@"Cancel"
                                              otherButtonTitles:@"Download", nil
                              ];
        
        alert.alertViewStyle = UIAlertViewStylePlainTextInput;
        [alert show];
        
        [alert textFieldAtIndex:0].placeholder = @"http://google.com/abc.zip";
        
    }
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
    if(buttonIndex == 1){
        NSString *url = [alertView textFieldAtIndex:0].text;
        
        CPDownloadURL *dURL = [[CPDownloadURL alloc] initWithURL:url];
        
        CPDownloadViewController *viewCtl = [[CPDownloadViewController alloc]initWithTarget:nil from:dURL];
        
        [self.navigationController pushViewController:viewCtl animated:YES];
    }
}

@end
