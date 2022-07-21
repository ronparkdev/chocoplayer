//
//  CPFileURL.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 5..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFileURL.h"

@implementation CPFileURL

- (id)initWithURL:(NSString *)url{
    self = [super init];
    if(self != nil){
        mURL = url;
    }
    return self;
}

- (NSString *)getName{
    return mURL;
}

@end
