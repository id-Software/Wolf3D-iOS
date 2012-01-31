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

#include <stdbool.h>

/*
===============================================
Expose a C interface to the In App Purchase functionality.

This interface only supports non-consumable products, such as one-time purchases of extra levels
or expansion packs. It uses NSUserDefaults internally to keep track of purchases the user has
made. This is Apple's recommended method, and it means that purchases will automatically be
backed up with the app in iTunes, should the user choose to do so. Each product is a boolean
value stored in NSUserDefaults, with a key equal to the product ID string specified in iTunes
connect. This is the same string that needs to passed to InAppPurchaseStartPurchase and similar
functions.

Usage:

InAppPurchaseInit needs to be called during app launch to complete any transactions that
were not finished last time.

Call InAppPurchaseStartPurchase with the product ID string from iTunes connect to begin the
purchase process. This will launch Apple's interface for logging in with an Apple ID and
comfirming the purchase.

Call InAppPurchaseHasPurchased to see if the user has purchased a specific product. The string
parameter is the same product ID used in iTunes Connect and InAppPurchaseStartPurchase.

===============================================
*/

void InAppPurchaseInit( void );
void InAppPurchaseShutdown( void );
void InAppPurchaseStartPurchase( const char * productIdentifier );

typedef enum {
	IAP_SUCCESS,
	IAP_FAILURE
} InAppPurchaseResult;

typedef void (*InAppPurchaseCallbackPtr)( InAppPurchaseResult result );

void InAppPurchaseSetCallback( InAppPurchaseCallbackPtr function );

bool InAppPurchaseIsInProgress( void );

bool InAppPurchaseHasPurchased( const char * productIdentifier );

