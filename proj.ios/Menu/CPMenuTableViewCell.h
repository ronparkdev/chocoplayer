//
//  CPMenuTableViewCell.h
//  chocoplayer
//
//  Created by Sang Min Park on 2015. 12. 23..
//
//

#import <UIKit/UIKit.h>

@interface CPMenuTableViewCell : UITableViewCell{
    NSString *stack;
}

@property (nonatomic, weak) IBOutlet UILabel* textLabel2;
@property (nonatomic, weak) IBOutlet UIImageView* rightImage;
@property (nonatomic, weak) IBOutlet UISwitch* rightSwitch;

@end
