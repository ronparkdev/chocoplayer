//
//  CPFolderInterface.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFolderInterface.h"

@implementation CPFolderInterface

- (id)initWithName:(NSString *)name path:(NSString *)path{
    self = [super init];
    if(self != nil){
        self.isLoaded = NO;
        
        mName = name;
        mPath = path;
    }
    return self;
}

- (NSString *)getPath{
    return mPath;
}


- (NSString *)getName{
    return mName;
}

- (NSInteger)getContentsCount{
    NSLog(@"CPFolderInterface getContentsCount operation is gone");
    return 0;
}

- (CPFileInterface *)getContents:(NSInteger)index{
    NSLog(@"CPFolderInterface getContents operation is gone");
    return nil;
}

- (CPFolderInterface *)getFolder:(NSInteger)index{
    NSLog(@"CPFolderInterface getFolder operation is gone");
    return nil;
}

- (CPDownloadInterface *)getDownload:(NSInteger)index{
    NSLog(@"CPFolderInterface getDownload operation is gone");
    return nil;
}

@end
