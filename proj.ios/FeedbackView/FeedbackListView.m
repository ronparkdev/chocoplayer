//
//  FeedbackListViewController.m
//  chocoplayer
//
//  Created by psm on 2015. 8. 19..
//
//

#import "FeedbackListView.h"

@interface FeedbackListView ()

@end

@implementation FeedbackListView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if(self != NULL){
        m_tableView = [[UITableView alloc]
                     initWithFrame:CGRectMake(0, 0, 264, 120)
                             style:UITableViewStylePlain];
        m_tableView.delegate = self;
        m_tableView.dataSource = self;
        [self addSubview:m_tableView];
        m_data = NULL;
    }
    return self;
}

- (void)setData:(NSArray *)data{
    m_data = data;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    if(m_data == NULL)
        return 0;
    else
        return m_data.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell_Connections";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] ;
        cell.selectionStyle = UITableViewCellSelectionStyleGray;
        cell.detailTextLabel.textColor = [UIColor darkGrayColor];
    }
    
    cell.backgroundColor = [UIColor clearColor];
    cell.textLabel.font = [UIFont fontWithName:@"Helvetica-Bold" size:16];
    cell.textLabel.textColor = [UIColor blackColor];
    cell.textLabel.text = [m_data objectAtIndex:indexPath.row];
    cell.accessoryType = UITableViewCellAccessoryCheckmark;
    return cell;
}

@end
