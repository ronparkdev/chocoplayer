//
//  CPFileInterface.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CPFileInterface : NSObject

- (BOOL)isFolder;
- (NSString *)getName;
- (NSInteger)getSize;

@end
