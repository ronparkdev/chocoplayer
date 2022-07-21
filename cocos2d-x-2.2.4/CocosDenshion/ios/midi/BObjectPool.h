//
//  BObjectPool.h
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 23/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol BPoolableObject <NSObject>

@required

-(void) activate: (id) object;
-(void) deactivate;
-(BOOL) isActive;

@end


@interface BObjectPool : NSObject {
    NSMutableArray * _pooledObjects;
}
    
-(id) getObject: (id) object;
-(NSInteger) availableObjects;
-(void) addObject: (id<BPoolableObject>) object;
-(NSMutableArray *) getActiveObjects;

@end
