//
//  NSObject_MgrInApp.h
//  chocoplayer
//
//  Created by dev on 2016. 1. 11..
//
//

#import <Foundation/Foundation.h>

#import <StoreKit/SKPayment.h>
#import <StoreKit/SKProductsRequest.h>
#import <StoreKit/SKProduct.h>
#import <StoreKit/SKPaymentQueue.h>
#import <StoreKit/SKPaymentTransaction.h>
#import <curl.h>

@interface MgrInApp : NSObject<SKPaymentTransactionObserver, SKProductsRequestDelegate> {
    
}

- (void)buyProduct:(NSString *) productName;

- (void)isBoughtProduct:(NSString *) productName;

@end
