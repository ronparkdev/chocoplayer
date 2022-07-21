//
//  BObjectPool.m
//  co.deluge.advancedmidi
//
//  Created by Ben Smiley-Andrews on 23/08/2012.
//  Copyright (c) 2012 Deluge. All rights reserved.
//

#import "BObjectPool.h"

@implementation BObjectPool

-(id) init {
    if((self=[super init])) {
        _pooledObjects = [NSMutableArray new];
    }
    return self;
}

-(NSInteger) availableObjects {
    NSInteger i = 0;
    for(id<BPoolableObject> obj in _pooledObjects) {
        if (![obj isActive]) {
            i++;
        }
    }
    return i;
}

-(id) getObject: (id) object {
    for(id<BPoolableObject> obj in _pooledObjects) {
        if (![obj isActive]) {
            [obj activate:object];
            return obj;
        }
    }
    return Nil;
}

-(void) addObject: (id<BPoolableObject>) object {
    [_pooledObjects addObject:object];
}

-(NSMutableArray *) getActiveObjects {
    NSMutableArray * activeObjects = [NSMutableArray new];
    for(id<BPoolableObject> object in _pooledObjects) {
        if([object isActive])
            [activeObjects addObject:object];
    }
    return activeObjects;
}

@end
