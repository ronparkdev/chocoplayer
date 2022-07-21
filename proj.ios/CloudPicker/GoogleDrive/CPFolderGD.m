//
//  CPFolderGD.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFolderGD.h"

#import "CPFileGD.h"
#import "CPFileInterface.h"
#import "CPDriveInterface.h"
#import "CPDownloadGD.h"

@implementation CPFolderGD

- (id)initWithName:(NSString *)name path:(NSString *)path identifier:(NSString *)identifier{
    self = [super initWithName:name path:path];
    if(self != nil){
        self.contents = [[NSMutableArray alloc] init];
        mIdentifier = identifier;
    }
    
    return self;
}

- (NSString *)getIdentify{
    return mIdentifier;
}

- (NSInteger)getContentsCount{
    return [self.contents count];
}

- (CPFileInterface *)getContents:(NSInteger)index{
    return [self.contents objectAtIndex:index];
}

- (CPFolderInterface *)getFolder:(NSInteger)index{
    CPFolderGD *folder;
    CPFileGD *node = [self.contents objectAtIndex:index];
    
    NSString *path = [NSString stringWithFormat:@"%@/%@", mPath, [node getName]];
    folder = [[CPFolderGD alloc]initWithName:[node getName] path:path identifier:[node getIdentifier]];
    
    return folder;
}

- (CPDownloadInterface *)getDownload:(NSInteger)index{
    CPFileGD *node = [self.contents objectAtIndex:index];
    
    return [[CPDownloadGD alloc] initWithFile:node];
}

@end
