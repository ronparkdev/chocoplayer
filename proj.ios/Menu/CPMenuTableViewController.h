//
//  CPMenuListViewController.h
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 23..
//
//

#import <UIKit/UIKit.h>

@interface CPMenuTableViewController : UITableViewController<UITableViewDelegate, UIAlertViewDelegate>{
    NSString *curstack;
    NSDictionary *jdata;
}

- (id)initWithMenu: (NSString*)stack;

+ (void)show: (UIViewController *) viewctl
       stack: (NSString*)stack;

+ (void)showAndPushToNavi: (UINavigationController *)navi
                    stack: (NSString*)stack;

@end

