//
//  FeedbackViewController.h
//  ChocoPlayer
//
//  Created by  A on 2015. 8. 18..
//
//

#import <UIKit/UIKit.h>

@interface FeedbackViewController : UIViewController

@property (strong, nonatomic) IBOutlet UIView *viewAccordionMenu;

-(void)showMenu:(UIView *)targetView
       gameList:(NSArray *)list
      lastEmail:(NSString *)email;

-(void)closeMenu:(BOOL)animated;

@end
