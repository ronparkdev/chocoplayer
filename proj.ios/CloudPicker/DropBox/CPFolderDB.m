//
//  CPFolderDB.m
//  ios
//
//  Created by Sang Min Park on 2015. 9. 4..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import "CPFolderDB.h"

#import "CPFileDB.h"
#import "CPDownloadDB.h"

@implementation CPFolderDB

- (id)initWithName:(NSString *)name path:(NSString *)path{
    self = [super initWithName:name path:path];
    if(self != nil){
        self.contents = [[NSMutableArray alloc] init];
    }
    
    return self;
}

- (NSInteger)getContentsCount{
    return [self.contents count];
}

- (CPFileInterface *)getContents:(NSInteger)index{
    return [self.contents objectAtIndex:index];
}

- (CPFolderInterface *)getFolder:(NSInteger)index{
    CPFolderDB *folder;
    CPFileDB *node = [self.contents objectAtIndex:index];
    
    NSString *path = [NSString stringWithFormat:@"%@/%@", mPath, [node getName]];
    folder = [[CPFolderDB alloc]initWithName:[node getName] path:path];
    
    return folder;
}

- (CPDownloadInterface *)getDownload:(NSInteger)index{
    CPFileDB *node = [self.contents objectAtIndex:index];
    
    return [[CPDownloadDB alloc] initWithFile:node];
}

@end
