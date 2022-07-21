//
//  FeedbackViewController.m
//  ChocoPlayer
//
//  Created by  A on 2015. 8. 18..
//
//

#import "FeedbackViewController.h"

#define ANIMATION_DURATION  0.25    // The duration of the animation.

@interface FeedbackViewController (){
    CGFloat menuViewWidth;
    CGFloat menuViewHeight;
}

@end

@implementation FeedbackViewController

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
    // Do any additional setup after loading the view from its nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)showMenu:(UIView *)targetView
       gameList:(NSArray *)list
      lastEmail:(NSString *)email
{
    // STEP 1: THE STATUS BAR OFFSET
    CGFloat statusBarOffset;
    
    if (![[UIApplication sharedApplication] isStatusBarHidden]) {
        // If the status bar is not hidden then we get its height and keep it to the statusBarOffset variable.
        // However, there is a small trick here that needs to be done.
        // In portrait orientation the status bar size is 320.0 x 20.0 pixels.
        // In landscape orientation the status bar size is 20.0 x 480.0 pixels (or 20.0 x 568.0 pixels on iPhone 5).
        // We need to check which is the smallest value (width or height). This is the value that will be kept.
        // At first we get the status bar size.
        CGSize statusBarSize = [[UIApplication sharedApplication] statusBarFrame].size;
        if (statusBarSize.width < statusBarSize.height) {
            // If the width is smaller than the height then this is the value we need.
            statusBarOffset = statusBarSize.width;
        }
        else{
            // Otherwise the height is the desired value that we want to keep.
            statusBarOffset = statusBarSize.height;
        }
    }
    else{
        // Otherwise set it to 0.0.
        statusBarOffset = 0.0;
    }
    
    
    // STEP 2: CALCULATE DIMENSIONS
    
    // Declare the following variables that will take their values
    // depending on the orientation.
    CGFloat width, height, offsetX, offsetY;
    
    if ([[UIApplication sharedApplication] statusBarOrientation] == UIInterfaceOrientationLandscapeLeft ||
        [[UIApplication sharedApplication] statusBarOrientation] == UIInterfaceOrientationLandscapeRight) {
        // If the orientation is landscape then the width
        // gets the targetView's height value and the height gets
        // the targetView's width value.
        width = targetView.frame.size.height;
        height = targetView.frame.size.width;
        
        offsetX = -statusBarOffset;
        offsetY = 0.0;
    }
    else{
        // Otherwise the width is width and the height is height.
        width = targetView.frame.size.width;
        height = targetView.frame.size.height;
        
        offsetX = 0.0;
        offsetY = -statusBarOffset;
    }
    
    
    // STEP 3 : SETUP THE SELF.VIEW
    
    // Set the self.view's frame, set its alpha value to 0.0 and add it
    // to the target view.
    [self.view setFrame:CGRectMake(targetView.frame.origin.x, targetView.frame.origin.y, width, height)];
    [self.view setFrame:CGRectOffset(self.view.frame, offsetX, offsetY)];
    [self.view setAlpha:0.0];
    [targetView addSubview:self.view];
    
    
    
    // STEP 4: SETUP THE MENU VIEW
    
    // Set the menuViewWidth variable regarding the width of the menu view.
    // It can be set according to your desire.
    int menuViewWidth = 260.0;
    // The height is the height of each row multiplied by the number
    // of options.
    int menuViewHeight = 500.0;
   
    [_viewAccordionMenu setFrame:CGRectMake(0.0, 0.0, menuViewWidth, menuViewHeight)];
    
    // Set its center to the self.view's center.
    [_viewAccordionMenu setCenter:self.view.center];
    
    // Store temporarily the current y origin point of the menu view.
    CGFloat yPoint = _viewAccordionMenu.frame.origin.y;
    // Now set the center.y point as the y origin point of the menu view and its height to 0.0.
    [_viewAccordionMenu setFrame:CGRectMake(_viewAccordionMenu.frame.origin.x, _viewAccordionMenu.center.y, _viewAccordionMenu.frame.size.width, 0.0)];
    // Add the menu view to the targetView view.
    [targetView addSubview:_viewAccordionMenu];
    
    
    // STEP 5: ANIMATE
    
    // Begin the animation.
    [UIView beginAnimations:@"" context:nil];
    [UIView setAnimationDuration:ANIMATION_DURATION];
    [UIView setAnimationCurve:UIViewAnimationCurveLinear];
    
    // Set the self.view's alpha to 0.5, so it works as a cover for any subviews
    // existing at the back.
    [self.view setAlpha:0.5];
    
    // Set the yPoint value as the y origin point of the menu view
    // and the tempMenuHeight value as its height.
    // This way the accordion effect will be achieved.
    [_viewAccordionMenu setFrame:CGRectMake(_viewAccordionMenu.frame.origin.x,
                                            yPoint,
                                            _viewAccordionMenu.frame.size.width,
                                            menuViewHeight)];
    
    [UIView commitAnimations];
}


-(void)closeAccordionMenuAnimated:(BOOL)animated{
    // At first check if the menu should be closed using animation.
    if (animated) {
        // In that case begin a new animation.
        [UIView beginAnimations:@"" context:nil];
        [UIView setAnimationDuration:ANIMATION_DURATION];
        [UIView setAnimationCurve:UIViewAnimationCurveLinear];
        
        // Set the self.view's alpha to 0.0.
        [self.view setAlpha:0.0];
        
        // Set the y origin point of the menu view to the center.y and
        // its height to 0.0.
        [_viewAccordionMenu setFrame:CGRectMake(_viewAccordionMenu.frame.origin.x, _viewAccordionMenu.center.y, _viewAccordionMenu.frame.size.width, 0.0)];
        
        [UIView commitAnimations];
        
        // Remove both the accordion menu view and the self.view from the parent view.
        // Make sure that the animation is over before removing the views.
        // Set the delay to ANIMATION_DURATION value plus 0.5 second.
        [_viewAccordionMenu performSelector:@selector(removeFromSuperview) withObject:nil afterDelay:ANIMATION_DURATION + 0.5];
        [self.view performSelector:@selector(removeFromSuperview) withObject:nil afterDelay:ANIMATION_DURATION + 0.5];
    }
    else{
        // In case that the user doesn't want animated closing
        // just remove the views from the superview.
        [_viewAccordionMenu removeFromSuperview];
        [self.view removeFromSuperview];
    }
}

@end
