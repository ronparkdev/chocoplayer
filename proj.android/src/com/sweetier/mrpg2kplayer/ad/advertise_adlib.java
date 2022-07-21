package com.sweetier.mrpg2kplayer.ad;

import android.app.Activity;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

import com.mocoplex.adlib.AdlibActivity;
import com.mocoplex.adlib.AdlibAdViewContainer;
import com.mocoplex.adlib.AdlibConfig;
import com.mocoplex.adlib.AdlibManager;

public class advertise_adlib implements advertise_interface{
	
	private static final String AD_CODE_ADLIB = "550e75020cf2833915d75467";

	private Activity m_act;
	
	private AdlibManager aManager;
	private AdlibAdViewContainer adView;
	
	boolean m_showing_line = false;

	@Override
	public void onCreate(Activity act){
		m_act = act;
		
		LayoutParams p;
		RelativeLayout layout;
		
		try{
	        AdlibConfig.getInstance().bindPlatform("ADMOB","com.sweetier.mrpg2kplayer.adlib.SubAdlibAdViewAdmob");
	        AdlibConfig.getInstance().bindPlatform("CAULY","com.sweetier.mrpg2kplayer.adlib.SubAdlibAdViewCauly");
	        
			AdlibActivity adact = (AdlibActivity)act;
			
			aManager = new AdlibManager(AD_CODE_ADLIB);
			aManager.onCreate(act);
		
			adact.setAdlibKey(AD_CODE_ADLIB);
			
			int width = m_act.getResources().getDisplayMetrics().heightPixels;
			p = new LayoutParams(width, dpToPx(50));
	    	p.addRule(RelativeLayout.CENTER_HORIZONTAL, 	RelativeLayout.TRUE);
	        p.addRule(RelativeLayout.ALIGN_TOP, 				RelativeLayout.TRUE);
		       
			adView = new AdlibAdViewContainer(adact);
	        adView.setBackgroundColor(Color.TRANSPARENT);
	        adView.setLayoutParams(p);
			adView.setVisibility(View.GONE);
	        
	        layout = new RelativeLayout(m_act);
	        layout.addView(adView);
	        layout.setBackgroundColor(Color.TRANSPARENT);
	        
	        adact.addContentView(layout, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
			adact.bindAdsContainer(adView);		

			aManager.bindAdsContainer(adView);
			
		
	        if(m_showing_line)
	        	showLine();
		}catch(Exception e){
			Log.e("R2KP_AD_ADLIB", "ErrorCreate");
			adView = null;
		}
	}

	@Override
	public void onResume() {
		aManager.onResume(m_act);
	    adView.onResume();
	}

	@Override
	public void onPause() {
		aManager.onPause(m_act);
	    adView.onPause();
	}
	
	@Override
	public void onDestroy() {
		aManager.onDestroy(m_act);
	    adView.onDestroy();	
	}

	@Override
	public void showLine(){
		
		if(adView == null)
			return;
		
		m_showing_line = true;

		m_act.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				adView.setVisibility(View.VISIBLE);
			}
		});
	}

	@Override
	public void hideLine(){
		
		if(adView == null)
			return;
		
    	m_showing_line = false;

		m_act.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				adView.setVisibility(View.GONE);
			}
		});
	}

	@Override
	public void showBig(){
		m_act.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AdlibActivity adact = (AdlibActivity)m_act;
				adact.loadFullInterstitialAd(new Handler() {
					public void handleMessage(Message message) {
			    		try
			    		{
			    			switch (message.what) {
				   			case AdlibManager.DID_SUCCEED:
				   				Log.d("HANA", "onReceivedAd " + (String)message.obj);
				   		    	break;
				   			case AdlibManager.DID_ERROR:
				   				Log.d("HANA", "onFailedAd " + (String)message.obj);
				   				break;
				   			case AdlibManager.INTERSTITIAL_CLOSED:
				   				Log.d("HANA", "onClosedAd " + (String)message.obj);
				   				break;
				   			case AdlibManager.INTERSTITIAL_FAILED:
				   				Log.d("HANA", "All Failed");
				    		}
			    		}
			    		catch(Exception e)
			    		{
			    			
			    		}
			    	}
				});
			}
		});
	}
	

    private int dpToPx(int dp) {
		return (int) (dp
				* m_act.getResources().getDisplayMetrics().density + 0.5f);
	}
}
