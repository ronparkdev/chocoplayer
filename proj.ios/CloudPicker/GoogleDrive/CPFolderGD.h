//
//  CPFolderGD.h
//  ios
//
//  Created by Sang Min Park on 2015. 9. 3..
//  Copyright (c) 2015년 Bridge4Biz. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CPFolderInterface.h"

#import "GTLDrive.h"

@interface CPFolderGD : CPFolderInterface{
    NSString *mIdentifier;
}

- (id)initWithName:(NSString *)name path:(NSString *)path identifier:(NSString *)identifier;

- (NSString *)getIdentify;

@end
