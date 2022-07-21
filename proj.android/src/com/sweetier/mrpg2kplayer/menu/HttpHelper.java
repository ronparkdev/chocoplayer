package com.sweetier.mrpg2kplayer.menu;

import java.net.HttpURLConnection;
import java.net.URL;

import org.json.JSONObject;

import android.os.AsyncTask;

public class HttpHelper extends AsyncTask<Object, Void, Boolean>{
	 
	private final static int DEFAULT_TIMEOUT = 60000;//60√ 
	
	public static void run(String url, String path, OnHttpHelperListener listener){
		new HttpHelper().execute(url, path, DEFAULT_TIMEOUT, listener);
	}
	
	public static void run(String url, String path, int timeout, OnHttpHelperListener listener){
		new HttpHelper().execute(url, path, timeout, listener);
	}
	
	OnHttpHelperListener mListener;

	@Override
	protected Boolean doInBackground(Object... params) {
		try {
			String sUrl = (String) params[0];
			String sPath = (String) params[1];
			int timeout = (Integer) params[3];
			mListener = (OnHttpHelperListener) params[4];
	
			URL url = new URL(sUrl);
			
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		
			
			
		}catch(Exception e){
			e.printStackTrace();
		}
			
		return false;
	}
 
}
