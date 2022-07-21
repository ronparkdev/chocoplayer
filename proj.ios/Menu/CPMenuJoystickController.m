//
//  CPMenuJoystickController.m
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 24..
//
//

#import "CPMenuJoystickController.h"

@interface CPMenuJoystickController ()

@end

@implementation CPMenuJoystickController

+ (void)showAndPushToNavi: (UINavigationController *)navi
{
    CPMenuJoystickController *newView = [[CPMenuJoystickController alloc]initWithNibName:@"CPMenuJoystickController" bundle:nil];
    
    [navi pushViewController:newView animated:YES];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self setTitle:@"Joystick"];
    
    UIBarButtonItem *itemQuit = [[UIBarButtonItem alloc] initWithTitle:@"Quit" style:UIBarButtonItemStyleBordered target:self action:@selector(quit)];
    self.navigationItem.rightBarButtonItem = itemQuit;
}

- (void)quit{
    [self dismissModalViewControllerAnimated:YES];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
