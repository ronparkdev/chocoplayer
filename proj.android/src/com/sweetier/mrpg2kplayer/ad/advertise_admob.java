package com.sweetier.mrpg2kplayer.ad;

import android.app.Activity;
import android.graphics.Color;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.sweetier.mrpg2kplayer.chocoplayer;

public class advertise_admob implements advertise_interface{

	final static String TAG = "r2k_manager_advertise_admob";
	
	private static final String AD_CODE_ADMOB = "ca-app-pub-8847915882688572/2282532246";
	private static final String AD_CODE_ADMOB_INTER = "ca-app-pub-8847915882688572/4299432242";

	private Activity m_act;
	
	private AdView adView;
	private InterstitialAd interstitial;

	boolean m_showing_line = false;

	@Override
	public void onCreate(Activity act) {
		m_act = act;
		
		LayoutParams p;
		RelativeLayout layout;
		AdRequest adRequest;
		
		try{
			adView = new AdView(m_act);
			adView.setAdUnitId(AD_CODE_ADMOB);
			adView.setAdSize(AdSize.BANNER);
			
			
			DisplayMetrics size = m_act.getResources().getDisplayMetrics();
	        
	        if(!chocoplayer.isLastVertical()){		  
		        p = new LayoutParams(size.heightPixels, dpToPx(50));
		    	p.addRule(RelativeLayout.CENTER_HORIZONTAL, 	RelativeLayout.TRUE);
		        p.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 	RelativeLayout.TRUE);
		        p.addRule(RelativeLayout.ALIGN_BOTTOM, 			RelativeLayout.TRUE);
	        	p.setMargins(0, 0, 0, 0);
		        adView.setBackgroundColor(Color.TRANSPARENT);
	        }else{
	        	int minWidth = Math.min(size.widthPixels, size.heightPixels);
	        	int maxWidth = Math.max(size.widthPixels, size.heightPixels);
	        	int height = dpToPx(50);
				p = new LayoutParams(minWidth, height);
		    	p.addRule(RelativeLayout.CENTER_HORIZONTAL, 	RelativeLayout.TRUE);
		        p.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 	RelativeLayout.TRUE);
		        p.addRule(RelativeLayout.ALIGN_BOTTOM, 			RelativeLayout.TRUE);
	        	p.setMargins(0, 0, 0, 0);
		        adView.setBackgroundColor(Color.BLACK);
	        }

	        adView.setLayoutParams(p);
	        
	        layout = new RelativeLayout(m_act);
	        layout.addView(adView);
	        layout.setBackgroundColor(Color.TRANSPARENT);
	        
	        m_act.addContentView(layout, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

	        adRequest = new AdRequest.Builder().build();
	        adView.loadAd(adRequest);
	        
	        interstitial = new InterstitialAd(m_act);
	        interstitial.setAdUnitId(AD_CODE_ADMOB_INTER);

	        interstitial.setAdListener(new AdListener() {
	             @Override
	             public void onAdLoaded() {
	 	        	if (interstitial.isLoaded())
		        	      interstitial.show();
	             }

	             @Override
	             public void onAdClosed() {

	             }
			});
	        
	        if(m_showing_line)
	        	showLine();
	        
		}catch(Exception e){
			Log.e(TAG, "ErrorCreate");
			adView = null;
		}

	}

	@Override
	public void onResume() {
	    adView.resume();
	}

	@Override
	public void onPause() {
	    adView.pause();
	}
	
	@Override
	public void onDestroy() {
	    adView.destroy();	
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
	public void showBig() {
		m_act.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				AdRequest adRequest = new AdRequest.Builder().build();
		        interstitial.loadAd(adRequest);	
			}
		});
		
	}

    private int dpToPx(int dp) {
		return (int) (dp
				* m_act.getResources().getDisplayMetrics().density + 0.5f);
	}
}
