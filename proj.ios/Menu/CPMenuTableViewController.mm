//
//  CPMenuListViewController.m
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 23..
//
//

#import "CPMenuTableViewController.h"

#import "CPMenuTableViewCell.h"
#import "CPMenuFeedbackController.h"
#import "CPMenuJoystickController.h"

NSString* MultiOutterIOS_GetMenu(NSString *ncmd);

@interface CPMenuTableViewController ()

@end

@implementation CPMenuTableViewController

+ (void)show: (UIViewController *) viewctl
       stack: (NSString*)stack
{
    UINavigationController *navi = [[UINavigationController alloc]init];
    navi.modalPresentationStyle = UIModalPresentationFormSheet;
    navi.modalTransitionStyle = UIModalTransitionStyleCoverVertical;
    
    CPMenuTableViewController *table = [[CPMenuTableViewController alloc]initWithMenu:stack];
    
    [navi pushViewController:table animated:NO];
    [viewctl presentModalViewController:navi animated:YES];
}

+ (void)showAndPushToNavi: (UINavigationController *)navi
                    stack: (NSString*)stack
{
    CPMenuTableViewController *newView = [[CPMenuTableViewController alloc]initWithMenu:stack];
    
    [navi pushViewController:newView animated:YES];
}

- (id)initWithMenu: (NSString*)stack{
    self = [super initWithStyle:UITableViewStylePlain];
    
    if(self != nil){
        jdata = nil;
        curstack = nil;
        
        curstack = [NSString stringWithString:stack];
        
        NSString *jsonRaw = MultiOutterIOS_GetMenu(stack);
        NSData *jsonData = [jsonRaw dataUsingEncoding:NSUTF8StringEncoding];
        
        if(jsonData){
            NSError* jsonParsingError = nil;
            jdata = [NSJSONSerialization JSONObjectWithData:jsonData
                          options:NSJSONReadingAllowFragments
                            error:&jsonParsingError];
        }
        
        //NSLog(@"%@>%@;", stack, jsonRaw);
    }
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    if(jdata != nil){
        NSString *stitle = [jdata objectForKey:@"TITLE"];
        if(stitle != nil)
            [self setTitle:stitle];
        
    }

    self.clearsSelectionOnViewWillAppear = NO;
    
    [self.tableView registerNib:[UINib nibWithNibName:@"CPMenuTableViewController" bundle:nil] forCellReuseIdentifier:@"reuseIdentifierss"];
    
    UIBarButtonItem *itemQuit = [[UIBarButtonItem alloc] initWithTitle:@"Quit" style:UIBarButtonItemStyleBordered target:self action:@selector(quit)];
    self.navigationItem.rightBarButtonItem = itemQuit;
}

- (void)quit{
    [self dismissViewControllerAnimated:YES completion:nil ];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if(jdata == nil || ![((NSString *)[jdata objectForKey:@"DO"]) isEqualToString:@"MENU"])
        return 0;
    else
        return [((NSArray *)[jdata objectForKey:@"MENU"]) count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CPMenuTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"myCell"];
    CPMenuTableViewCell *cellform;
    
    // Configure the cell...
    
    if(cell == nil)
        cellform = cell = [[CPMenuTableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"myCell"];
    
    NSArray *topLevelObjects = [[NSBundle mainBundle] loadNibNamed:@"CPMenuTableViewCell" owner:cell options:nil];
    
    for(id currentObject in topLevelObjects){
        if([currentObject isKindOfClass:[CPMenuTableViewCell class]]){
            cellform = (CPMenuTableViewCell *)currentObject;
            break;
        }
    }
    
    
    if(jdata != nil){
        NSDictionary *jmenu = [(NSArray *)[jdata objectForKey:@"MENU"] objectAtIndex:indexPath.row];
        
        NSString *scaption = [jmenu objectForKey:@"C"];
        NSString *stype = [jmenu objectForKey:@"T"];
        unichar type, value;
        type = value = '\0';
        if(stype.length >= 1)type = [stype characterAtIndex:0];
        if(stype.length >= 2)value = [stype characterAtIndex:1];
        
        [cell.textLabel2 setText:scaption];
        
        switch(type){
            case 'N':
                [cell.rightImage setHidden:TRUE];
                [cell.rightSwitch setHidden:TRUE];
                break;
                
            case 'A':
            case 'W':
                [cell.rightImage setHidden:FALSE];
                [cell.rightSwitch setHidden:TRUE];
                break;
                
            case 'S':
                [cell.rightImage setHidden:TRUE];
                [cell.rightSwitch setHidden:FALSE];
                [cell.rightSwitch setTag:indexPath.row];
                [cell.rightSwitch setOn:(value == 'Y')];
                [cell.rightSwitch addTarget:self action:@selector(onSwitchChanged:) forControlEvents:UIControlEventValueChanged];
                break;
                
        }
        
    }
    
    return cellform;
}

- (void)onSwitchChanged:(id)sender
{
    NSString *val = [sender isOn]?@"Y":@"N";
    
    NSDictionary *jmenu = [(NSArray *)[jdata objectForKey:@"MENU"] objectAtIndex:[sender tag]];
    NSString *sname = [jmenu objectForKey:@"N"];
    NSString *stack = [NSString stringWithFormat:@"%@|%@|%@", curstack, sname, val, nil];
    
    MultiOutterIOS_GetMenu(stack);
}


#pragma mark - Table view delegate

// In a xib-based application, navigation from a table can be handled in -tableView:didSelectRowAtIndexPath:

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(jdata != nil){
        NSDictionary *jmenu = [(NSArray *)[jdata objectForKey:@"MENU"] objectAtIndex:indexPath.row];
        
        NSString *sname = [jmenu objectForKey:@"N"];
        NSString *stack = [NSString stringWithFormat:@"%@|%@", curstack, sname, nil];
        
        NSString *jsonRaw = MultiOutterIOS_GetMenu(stack);
        NSData *jsonData = [jsonRaw dataUsingEncoding:NSUTF8StringEncoding];
        
        if(jsonData){
            NSError* jsonParsingError = nil;
            NSDictionary *jnewdata = [NSJSONSerialization JSONObjectWithData:jsonData
                                                    options:NSJSONReadingAllowFragments
                                                      error:&jsonParsingError];
            
            if(jnewdata != nil){
                NSString *sdo = [jnewdata objectForKey:@"DO"];
                if ([@"MENU" isEqualToString:sdo]) {
                    [CPMenuTableViewController showAndPushToNavi:self.navigationController stack:stack];
                }else if([@"CLOSE" isEqualToString:sdo]){
                    [self dismissModalViewControllerAnimated:YES];
                }else if([@"BACK" isEqualToString:sdo]){
                    [self.navigationController popViewControllerAnimated:TRUE];
                }else if([@"JOYSTICK" isEqualToString:sdo]){
                    [CPMenuJoystickController showAndPushToNavi:self.navigationController];
                }else if([@"FEEDBACK" isEqualToString:sdo]){
                    [CPMenuFeedbackController showAndPushToNavi:self.navigationController];                }
            }
        }
    }
}

@end
