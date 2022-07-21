//
//  CPMenuFeedbackController.h
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 24..
//
//

#import <UIKit/UIKit.h>

@interface CPMenuFeedbackController : UIViewController

@property (nonatomic, weak) IBOutlet UILabel *emailLabel;
@property (nonatomic, weak) IBOutlet UITextField *emailText;
@property (nonatomic, weak) IBOutlet UILabel *detailLabel;
@property (nonatomic, weak) IBOutlet UITextView *detailText;

+ (void)showAndPushToNavi: (UINavigationController *)navi;

@end
