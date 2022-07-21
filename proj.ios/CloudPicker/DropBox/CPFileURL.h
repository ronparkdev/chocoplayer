//
//  CPFileURL.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 5..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFileInterface.h"

@interface CPFileURL : CPFileInterface{
    NSString *mURL;
}

- (id)initWithURL:(NSString *)url;

@end
