#import "MgrInApp.h"

@implementation MgrInApp

- (id) init{
    if((self = [super init]) != nil){
        if ([SKPaymentQueue canMakePayments]) {	// 스토어가 사용 가능하다면
            NSLog(@"Start Shop!");
            
            [[SKPaymentQueue defaultQueue] addTransactionObserver:self];	// Observer를 등록한다.
        }
        else
            NSLog(@"Failed Shop!");
    }
    
    return self;
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"SKPaymentTransactionStateRestored");
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"SKPaymentTransactionStateFailed");
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"SKPaymentTransactionStatePurchased");
    
    NSLog(@"Trasaction Identifier : %@", transaction.transactionIdentifier);
    NSLog(@"Trasaction Date : %@", transaction.transactionDate);
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)buyProduct:(NSString *) productName{
    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:productName]];
    request.delegate = self;
    [request setValue:@"buyIt" forKey:@"flag"];
    [request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    NSLog(@"SKProductRequest got response");
    
    NSString *flag = [request valueForKey:@"flag"];
    
    if( (flag != nil) && ([response.products count] > 0) ){
        SKProduct *product = [response.products objectAtIndex:0];
        
        NSLog(@"Title : %@", product.localizedTitle);
        NSLog(@"Description : %@", product.localizedDescription);
        NSLog(@"Price : %@", product.price);
        
        if([@"buyIt" isEqualToString:flag]){
            SKPayment *payment = [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }else{
            
        }
    }
    
    if( [response.invalidProductIdentifiers count] > 0 ) {
        NSString *invalidString = [response.invalidProductIdentifiers objectAtIndex:0];
        NSLog(@"Invalid Identifiers : %@", invalidString);
    }
}

- (void)isBoughtProduct:(NSString *) productName{
    // Load the receipt from the app bundle.
    NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receipt = [NSData dataWithContentsOfURL:receiptURL];
    if (receipt == nil) {
        NSLog(@"Cannot receive the receipt");
        return;
    }
    
    /* ... Send the receipt data to your server ... */

    
    // Create the JSON object that describes the request
    NSError *error;
    NSDictionary *requestContents = @{
                                      @"receipt-data": [receipt base64EncodedStringWithOptions:0]
                                      };
    NSData *requestData = [NSJSONSerialization dataWithJSONObject:requestContents
                                                          options:0
                                                            error:&error];
    
    if (!requestData) { /* ... Handle error ... */ }
    
    // Create a POST request with the receipt data.
    //NSURL *storeURL = [NSURL URLWithString:@"https://buy.itunes.apple.com/verifyReceipt"];
    NSURL *storeURL = [NSURL URLWithString:@"https://sandbox.itunes.apple.com/verifyReceipt"];
    NSMutableURLRequest *storeRequest = [NSMutableURLRequest requestWithURL:storeURL];
    [storeRequest setHTTPMethod:@"POST"];
    [storeRequest setHTTPBody:requestData];
    
    // Make a connection to the iTunes Store on a background queue.
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    [NSURLConnection sendAsynchronousRequest:storeRequest queue:queue
        completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
            if (connectionError) {
            } else {
                NSError *error;
                NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding ];
                NSLog(@"Query Result:%@", str);
                
                NSDictionary *jsonResponse = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
                if (!jsonResponse) {
                    /* ... Handle error ...*/
                }else{
                    /* ... Send a response back to the device ... */
                }
            }
        }];

}

@end