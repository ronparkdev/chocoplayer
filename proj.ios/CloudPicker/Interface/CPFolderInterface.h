//
//  CPFolderInterface.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CPFolderInterface.h"
#import "CPFileInterface.h"
#import "CPDownloadInterface.h"

@interface CPFolderInterface : NSObject{
    NSString *mPath;
    NSString *mName;
}

@property (nonatomic, strong) NSMutableArray *contents;
@property (nonatomic, assign) BOOL isLoaded;


- (id)initWithName:(NSString *)name path:(NSString *)path;

- (NSString *)getName;
- (NSString *)getPath;
- (NSInteger)getContentsCount;
- (CPFileInterface *)getContents:(NSInteger)index;
- (CPFolderInterface *)getFolder:(NSInteger)index;
- (CPDownloadInterface *)getDownload:(NSInteger)index;

@end
