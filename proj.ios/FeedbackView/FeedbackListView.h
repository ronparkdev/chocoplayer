//
//  FeedbackListViewController.h
//  chocoplayer
//
//  Created by psm on 2015. 8. 19..
//
//

#import <UIKit/UIKit.h>

@interface FeedbackListView : UIView<UITableViewDelegate, UITableViewDataSource>
{
    UITableView *m_tableView;
    
    NSArray *m_data;
}

- (void)setData:(NSArray *)data;

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;

- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath;


@end
