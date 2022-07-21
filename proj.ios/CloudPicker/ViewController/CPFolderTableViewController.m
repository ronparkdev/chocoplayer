//
//  CPFolderTableViewController.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 2..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFolderTableViewController.h"

#import "CPFileInterface.h"
#import "CPDownloadViewController.h"
#import "CPFolderInterface.h"

@interface CPFolderTableViewController ()

@end

@implementation CPFolderTableViewController

+ (void)show:(UIViewController *)oldView
      target:(CPDriveInterface *)drive
      folder:(CPFolderInterface *)folder
{
    CPFolderTableViewController *newView = [[CPFolderTableViewController alloc]initWithTarget:drive folder:folder];
    
    [oldView.navigationController pushViewController:newView animated:YES];
}

- (id)initWithTarget:(CPDriveInterface *)drive folder:folder{
    self = [super init];
    if(self != nil){
        mDrive = drive;
        mFolder = folder;
        mIsContentsLoaded = NO;
    }
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    if(mFolder == nil)
        mFolder = [mDrive getRootFolder];
    
    [self setTitle:[mFolder getName]];
    
    // Uncomment the following line to preserve selection between presentations.
    self.clearsSelectionOnViewWillAppear = NO;
    
    [self.tableView registerNib:[UINib nibWithNibName:@"CPTargetTableViewController" bundle:nil] forCellReuseIdentifier:@"reuseIdentifierss"];
    
    UIBarButtonItem *itemLogout = [[UIBarButtonItem alloc] initWithTitle:@"Logout" style:UIBarButtonItemStyleBordered target:self action:@selector(logout)];
    self.navigationItem.rightBarButtonItem = itemLogout;

    
    if([mDrive viewDidLoad:self])
        [self requestData];
        

}

- (void)logout{
    [mDrive logout];
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- (void)viewDidAppear:(BOOL)animated{
    
    if([mDrive viewDidAppear:self] && ![mFolder isLoaded]){
        [self requestData];
    }
}

- (void)requestData{
    if(mIsContentsLoaded)
        return;
    
    [mDrive setQueryFinishHandler:^{
        [self.tableView reloadData];
        [mHud hide:YES];
        
        mIsContentsLoaded = YES;
    }];
    
    [mDrive request:mFolder];
    
    if(mHud == nil){
        mHud = [[MBProgressHUD alloc] initWithView:self.view];
        [self.navigationController.view addSubview:mHud];
        mHud.labelText = @"Loading...";
        mHud.square = YES;
    }
    [mHud show:YES];
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
    return [mFolder getContentsCount];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cID = @"reuseIdentifier";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cID];
    
    // Configure the cell...
    
    if(cell == nil){
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cID];
    }
    
    CPFileInterface *node = [mFolder getContents:indexPath.row];
    
    cell.textLabel.text = [node getName];
    
    if([node isFolder]){
        [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];

        NSString *imgPath = [[NSBundle mainBundle] pathForResource:@"icon_folder" ofType:@"png"];
        [cell.imageView setImage:[[UIImage alloc]initWithContentsOfFile:imgPath]];
    }else{
        [cell setAccessoryType:UITableViewCellAccessoryNone];
        
        NSString *imgPath = [[NSBundle mainBundle] pathForResource:@"icon_file" ofType:@"png"];
        [cell.imageView setImage:[[UIImage alloc]initWithContentsOfFile:imgPath]];
    }
    return cell;
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    CPFileInterface *node = [mFolder getContents:indexPath.row];
    
    if([node isFolder]){
        CPFolderTableViewController *detailViewController =
            [[CPFolderTableViewController alloc] initWithTarget:mDrive folder:[mFolder getFolder:indexPath.row]];
        
        [self.navigationController pushViewController:detailViewController animated:YES];
    }else{
        CPDownloadViewController *detailViewController =
        [[CPDownloadViewController alloc] initWithTarget:mDrive from:[mFolder getDownload:indexPath.row]];
        
        [self.navigationController pushViewController:detailViewController animated:YES];
    }
}


@end
