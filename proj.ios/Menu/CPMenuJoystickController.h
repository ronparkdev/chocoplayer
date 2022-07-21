//
//  CPMenuJoystickController.h
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 24..
//
//

#import <UIKit/UIKit.h>

@interface CPMenuJoystickController : UIViewController

@property (nonatomic, weak) IBOutlet UILabel *captionLabel;
@property (nonatomic, weak) IBOutlet UILabel *targetKeyLabel;
@property (nonatomic, weak) IBOutlet UILabel *joystickKeyLabel;
@property (nonatomic, weak) IBOutlet UIButton *prevButton;
@property (nonatomic, weak) IBOutlet UIButton *nextButton;

+ (void)showAndPushToNavi: (UINavigationController *)navi;

@end
