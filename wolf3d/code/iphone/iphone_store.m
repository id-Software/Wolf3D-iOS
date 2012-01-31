/*
 
 Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company. 

 This file is part of the WOLF3D iOS v2.1 GPL Source Code. 

 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 
 */

#import <Foundation/Foundation.h>

#import <StoreKit/StoreKit.h>
#import <StoreKit/SKPaymentTransaction.h>
#import <StoreKit/SKError.h>

#import <UIKit/UIAlertView.h>

#import "iphone_store.h"


@interface MyStoreObserver : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver> {
	
}

// SKProductsRequestDelegate methods
- (void)productsRequest: (SKProductsRequest *)request didReceiveResponse: (SKProductsResponse *)response;

// SKPaymentTransactionObserver methods
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions;

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;

// MyStoreObserver methods
- (void)completeTransaction:(SKPaymentTransaction *)transaction;
- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;
- (void)finalizeTransaction:(SKPaymentTransaction *)transaction;

@end

// The single instance of the store observer. This is created in InAppPurchaseInit and released
// in InAppPurchaseShutdown.
static MyStoreObserver *storeObserver;

// Flag to indicate whether a purchase is in progress or not. This is set to true in
// InAppPurchaseStartPurchase and false when the request either succeeds or fails.
static bool isInProgress = false;

static InAppPurchaseCallbackPtr clientCallback;

@implementation MyStoreObserver

/*
========================
productsRequest:didReceiveResponse

Called by the system when the application gets product information about an In-App Purchase.
========================
*/
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	NSArray *myProduct = response.products;
    
	// For now, immediately request payment for any items the user has requested.
	for (SKProduct *product in myProduct)
	{
		SKPayment *payment = [SKPayment paymentWithProductIdentifier:[product productIdentifier]];
		[[SKPaymentQueue defaultQueue] addPayment:payment];
	}
	
	// We should handle the case of an empty array, this seems to occur if we send a request
	// with a nonexistant product ID, but there may be other cases.
	// TODO: Need a timeout to be even more robust.
	if ( [myProduct count] == 0 ) {
		if ( clientCallback ) {
			clientCallback( IAP_FAILURE );
		}
	}
	
    [request autorelease];
}

/*
========================
paymentQueue:updatedTransactions

Called by the system when purchase requests are updated.
========================
*/
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

- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{

}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{

}


- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{

}

/*
========================
completeTransaction

Called by updatedTransactions when a request is completed.
========================
*/
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
	[self finalizeTransaction:transaction];
}

/*
========================
failedTransaction

Called by updatedTransactions when a request fails.
========================
*/
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
	if (transaction.error.code != SKErrorPaymentCancelled)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"In-App Purchase error"
												  message:[transaction.error localizedDescription]
												  delegate:nil
												  cancelButtonTitle:@"OK"
												  otherButtonTitles:nil];
												  
		[alert show];
		[alert release];
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
	
	isInProgress = false;
	
	if ( clientCallback ) {
		clientCallback( IAP_FAILURE );
	}
}

/*
========================
restoreTransaction

Called by updatedTransactions when a request is restored. This can behave identically to
completeTransaction.
========================
*/
- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
	[self finalizeTransaction:transaction.originalTransaction];
}

/*
========================
finalizeTransaction

This method actually delivers the purchased item to the user. Currently, this means setting
a key in NSUserDefaults equal to the product ID to TRUE.
========================
*/
- (void)finalizeTransaction:(SKPaymentTransaction *)transaction
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	
	NSString *productID = transaction.payment.productIdentifier;
	
	[defaults setBool:TRUE forKey:productID];

	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
	
	isInProgress = false;
	
	if ( clientCallback ) {
		clientCallback( IAP_SUCCESS );
	}
}

@end





/*
========================
InAppPurchaseInit

Creates the instance of the payment observer and adds it to the payment queue. This should be
called at application launch.
========================
*/
void InAppPurchaseInit(void) {
	if ( !storeObserver ) {
		storeObserver = [[MyStoreObserver alloc] init];
		
		// Add the observer here in case purchase requests were interrupted last time.
		[[SKPaymentQueue defaultQueue] addTransactionObserver:storeObserver];
	}
	
}

void InAppPurchaseShutdown(void) {
	[storeObserver release];
	storeObserver = nil;
}

/*
========================
InAppPurchaseStartPurchase

If payments are enabled, starts an In-App Purchase transaction for the given product identifier.
If payments are not enabled, reminds the user that they can be enabled.
If a request is currently in progress, nothing happens, as only one request at a time is
allowed.
========================
*/
void InAppPurchaseStartPurchase( const char * productIdentifier ) {
	// Only allow one request at a time.
	if ( isInProgress ) {
		return;
	}
	
	if ( [SKPaymentQueue canMakePayments] ) {
		// Start transaction.
		if ( storeObserver == nil ) {
			printf( "In-App Purchase system not initialized. Can't purchase anything!\n" );
			return;
		}
		
		isInProgress = true;
		
		// convert the C string to an NSString.
		NSString *productKey = [[NSString alloc] initWithCString:productIdentifier
												 encoding:NSASCIIStringEncoding];
		
		SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: [NSSet setWithObject: productKey]];
		
		// The set retains the string, so we can release it now.
		[productKey release];
		
		request.delegate = storeObserver;
		[request start];
	} else {
		// User has disabled In-App purchases in settings. Kindly remind him that he can
		// enable purchases again.
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"In-App Purchases are disabled"
			message:@"You can enable In-App purchases in your device's settings."
			delegate:nil
			cancelButtonTitle:@"OK"
			otherButtonTitles:nil];
			
		[alert show];
		[alert release];
	}

}

/*
========================
InAppPurchaseHasPurchased

Queries the standard NSUserDefaults object for a boolean key equal to the product identifier.
If this key is true, the user has purchased the product. If the key is false, the user has
not purchased the product.
========================
*/
bool InAppPurchaseHasPurchased( const char * productIdentifier ) {
	// Convert the C string to an NSString for use with NSUserDefaults.
	NSString *productKey = [[NSString alloc] initWithCString:productIdentifier
											 encoding:NSASCIIStringEncoding];
	
	// Purchase records are stored in the standardUserDefaults, with a key equal to the product
	// identifier.
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    BOOL hasProduct = [defaults boolForKey:productKey];
	
	// This string was alloced above, but since it isn't needed anymore, it must be released.
	[productKey release];
	
	// Convert Objective-C BOOL type to standard C99 _Bool type.
	return ( hasProduct )? true: false;
}

/*
========================
InAppPurchaseIsInProgress

Returns true if an In-App Purchase request is currently pending. Only one request can be active
at a time.
========================
*/
bool InAppPurchaseIsInProgress( void ) {
	return isInProgress;
}

/*
========================
InAppPurchaseSetCallback

Sets a functino pointer to be called when the in-app purchase transaction in complete, either
successfully or unsuccessfully.
========================
*/
void InAppPurchaseSetCallback( InAppPurchaseCallbackPtr function ) {
	clientCallback = function;
}

