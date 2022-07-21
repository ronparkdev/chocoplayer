//
//  CPMenuFeedbackController.m
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 24..
//
//

#import "CPMenuFeedbackController.h"

@interface CPMenuFeedbackController ()

@end

@implementation CPMenuFeedbackController

+ (void)showAndPushToNavi: (UINavigationController *)navi
{
    CPMenuFeedbackController *newView = [[CPMenuFeedbackController alloc]initWithNibName:@"CPMenuFeedbackController" bundle:nil];
    
    [navi pushViewController:newView animated:YES];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UIBarButtonItem *itemSend = [[UIBarButtonItem alloc] initWithTitle:@"Send" style:UIBarButtonItemStyleBordered target:self action:@selector(send)];
    self.navigationItem.rightBarButtonItem = itemSend;
}

- (void)send{
    [self dismissModalViewControllerAnimated:YES];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
