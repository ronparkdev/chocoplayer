package com.sweetier.mrpg2kplayer;

import android.app.Activity;

import com.sweetier.mrpg2kplayer.ad.advertise_admob;
import com.sweetier.mrpg2kplayer.ad.advertise_interface;

public class MgrAdvertise {
	
	final static String TAG = "r2k_manager_advertise";
	
	public advertise_interface m_cur_ad;
	
	public void onCreate(){
		chocoplayer parent;
		if((parent = chocoplayer.getInstance()) == null)
			return;
		
		/*
	    String countryCode = act.getResources().getConfiguration().locale.getCountry();
	
	
	    if("KR".equals(countryCode)|| "KOR".equals(countryCode))
	    	m_cur_ad = new advertise_cauly();
	    else
	    	m_cur_ad = new advertise_admob();

		
		m_cur_ad = new advertise_adlib();
		*/
		
		m_cur_ad = new advertise_admob();
		
		m_cur_ad.onCreate(parent);
	}
	
	protected void onDestroy() {
		m_cur_ad.onDestroy();
	}

	protected void onPause() {
		m_cur_ad.onPause();
	}

	protected void onResume() {
		m_cur_ad.onResume();
	}
	
	public void showLine(){
		//Log.d("R2KP_AD", "showLine");
		m_cur_ad.showLine();
	}
	
	public void hideLine(){
		//Log.d("R2KP_AD", "hideLine");
    	m_cur_ad.hideLine();
	}
	
	public void showBig(){
		//Log.d("R2KP_AD", "showBig");
		m_cur_ad.showBig();
	}
}
