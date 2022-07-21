#import <UIKit/UIKit.h>

#import "MgrInApp.h"


@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
    MgrInApp *inapp;
}

+ (AppController *)getInstance;

- (void)showCloudPicker;
- (void)showMenu: (NSString*)stack;


@end

