package com.sweetier.mrpg2kplayer;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

import com.android.vending.billing.IInAppBillingService;
import com.android.vending.util.IabException;
import com.android.vending.util.IabHelper;
import com.android.vending.util.IabResult;
import com.android.vending.util.Inventory;
import com.android.vending.util.Purchase;

public class MgrInapp {
	
	final static String TAG = "r2k_manager_inapp";

	final static String SKU_PRO = "no_advertise";
	final static String SET_PRO = "no_advertise";
	
	Activity mAct;
	MgrAdvertise mAD;
	
	IInAppBillingService mService;
	IabHelper mHelper;
	
	Timer mTimer;
	TimerTask mTimerTask;
	
	boolean m_helper_setup = false;
	
	boolean m_bought_item = false;


	
	public void onCreate(Activity act, MgrAdvertise ad) {
		mAct = act;
		mAD = ad;

		mAct.bindService(new Intent(
				"com.android.vending.billing.InAppBillingService.BIND"),
				mServiceConn, Context.BIND_AUTO_CREATE);

		String base64EncodiedPushedkey = 
				"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwcEv6Rv95j7OYYudsGW4dVOc/"+
				"Srmws5QYJ37npmfI6fmB0shZJdT0Ifj14EK+qhG8r2FakbVWtOVeZ1L8UfdKEL9Jgfl5" + 
				"JyAfDCoH3Duh8Q3qnNC7dn+aGQGgQECiWQxYuCqTJRVMvf7W8MV4lK3TJS0aBIJkQLB5G" +
				"nm0riTgYBab/8LPyQ3mnyv3IYOcs1hOuaJeS6Z3lwAvuOYaYUWpCTe5Cnb8r03z3TAML0" +
				"ENsUed7KJw96tsAlF6EkMfCre5o4p5Snh/6VSC27DmXxN3wIqXB9eOBkt36dpxrdzS00u" +
				"VzqpSwSre4LH82ORocFq3t5gwfVDxmGVY8GmS9KAMwIDAQAB";
		
		mHelper = new IabHelper(mAct, base64EncodiedPushedkey);
		
		//mHelper.enableDebugLogging(true, TAG);

		mTimer = new Timer();
		
		mTimerTask = new TimerTask() {
			
			@Override
			public void run() {
				if(m_helper_setup)
					return;
				
				mAct.runOnUiThread(new Runnable() {
					
					@Override
					public void run() {
						mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
							@Override
							public void onIabSetupFinished(IabResult result) {
								// TODO Auto-generated method stub
								boolean bInit = result.isSuccess();
								Log.d(TAG, "IAB Init " + bInit + result.getMessage());

								if (bInit == true) {
									Log.d(TAG, "Querying inventory.");
									try {
										mHelper.queryInventoryAsync(mGotInventoryListener);
										
										m_helper_setup = true;
										
										mTimerTask.cancel();
										mTimer.cancel();
									}
									catch(IllegalStateException ex){ //ADDED THIS CATCH
										
									}
								}
							}
						});
					}
				});
			}
		};
		
		mTimer.schedule(mTimerTask , 500 , 20000);
	}

	public void onDestroy() {
		if (mServiceConn != null) {
			mAct.unbindService(mServiceConn);
		}
	}
	
	public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.d(TAG, "onActivityResult(" + requestCode + "," + resultCode + "," + data);
		if (requestCode == 1001) {
			// Pass on the activity result to the helper for handling
			if (!mHelper.handleActivityResult(requestCode, resultCode, data)) {
				// not handled, so handle it ourselves (here's where you'd
				// perform any handling of activity results not related to
				// in-app
				// billing...
				return true;
			} else {
				Log.d(TAG, "onActivityResult handled by IABUtil.");
			}
		}
		return false;
	}

	ServiceConnection mServiceConn = new ServiceConnection() {

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mService = IInAppBillingService.Stub.asInterface(service);
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			mService = null;
		}

	};

	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
		public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
			if (result.isFailure()) {
				Log.d(TAG, "Failed to query inventory: " + result);
				return;
			}

			/*
			 * Check for items we own. Notice that for each purchase, we check
			 * the developer payload to see if it's correct! See
			 * verifyDeveloperPayload().
			 */

			List<String> inappList = inventory
					.getAllOwnedSkus(IabHelper.ITEM_TYPE_INAPP);
			// inappList.add("item01");

			//Toast.makeText(mAct, "AdFreeChkStart", Toast.LENGTH_LONG).show();

			
			boolean isAdFree = false;
			
			for (String inappSku : inappList) {
				Purchase purchase = inventory.getPurchase(inappSku);
				
				//Toast.makeText(mAct, "sku : " + purchase.getSku(), Toast.LENGTH_LONG).show();
				
				//mNet.sendBuyInfoToServer(mAct, purchase);
				
				if(verifyDeveloperPayload(purchase) && SKU_PRO.equals(purchase.getSku())){	
					
					isAdFree = true;
					
					break;
				}
			}

			SetAdFree(isAdFree);

			Log.d(TAG, "Query inventory was successful.");
		}
	};

	public void Buy(){
		Buy(SKU_PRO);
	}
	
	private void Buy(final String strItemId) {
		mAct.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				/*
				 * TODO: for security, generate your payload here for
				 * verification. See the comments on verifyDeveloperPayload()
				 * for more info. Since this is a SAMPLE, we just use an empty
				 * string, but on a production app you should carefully generate
				 * this.
				 */
				Log.d(TAG, "InAppBuyItem_U " + strItemId);
				try {
					String payload = "testpayload";
					
					Bundle buyIntentBundle = mService.getBuyIntent(3,
							mAct.getPackageName(), strItemId, IabHelper.ITEM_TYPE_INAPP, payload);
					PendingIntent pendingIntent = buyIntentBundle
							.getParcelable("BUY_INTENT");

					if (pendingIntent != null) {
						mHelper.launchPurchaseFlow(mAct, strItemId, 1001,
													mPurchaseFinishedListener, payload);
			
					} else {
						// 결제가 막혔다면
					}
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

			}
		});
	}

	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
		public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
			Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase); 
			// 결제 완료 되었을때 각종 결제 정보들을 볼 수 있습니다. 이정보들을 서버에
			// 저장해 놓아야 결제 취소시 변경된 정보를 관리 할 수 있을것 같습니다~
			
			
			
			if (purchase != null) {
//				if (!verifyDeveloperPayload(purchase)) {
//					Log.d(TAG, "Error purchasing. Authenticity verification failed.");
//				}
				
				Toast.makeText(mAct, mAct.getString(R.string.Inapp_BuySuccess), Toast.LENGTH_LONG).show();
				
				SetAdFree(true);
			} else {
				
				String str;
				
				switch(result.getResponse()){
				case IabHelper.IABHELPER_USER_CANCELLED :
					return;
					
				case IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED:
					str = mAct.getString(R.string.Inapp_BuyAlready);
					break;
				
				default:
					str = mAct.getString(R.string.Inapp_BuyFail) + result.getMessage() + "(" + result.getResponse() + ")";
				
				}

				Toast.makeText(mAct, str, Toast.LENGTH_LONG).show();
			}
		}
	};

	boolean verifyDeveloperPayload(Purchase p) {
		String payload = p.getDeveloperPayload();

		//Log.d(TAG, payload);

		return true;
	}
	
	private void SetAdFree(boolean val){
		try{
			if(val)
				mAD.hideLine();
			else
				mAD.showLine();
			
			SharedPreferences prefs = mAct.getSharedPreferences( "r2k_inapp" , Activity.MODE_PRIVATE);
			SharedPreferences.Editor ed = prefs.edit();
			ed.putBoolean( "AdFree" , val ); // value : 저장될 값, 
			ed.commit(); // 필수! 이것을 안해주면 저장이 안되요!
		}catch(Exception e){
			
		}
	}
	
	public boolean isPro(){
		try{
			SharedPreferences prefs = mAct.getSharedPreferences( "r2k_inapp" , Activity.MODE_PRIVATE);
			return prefs.getBoolean( "AdFree" , false ); // value : 저장될 값, 
		}catch(Exception e){
			return false;
		}
	}
}