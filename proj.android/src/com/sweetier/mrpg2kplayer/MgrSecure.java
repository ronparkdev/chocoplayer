package com.sweetier.mrpg2kplayer;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;

public class MgrSecure {
	
	public final static String PREFS_NAME = "r2k_secure";
	
	private final static String PREFS_CODE_USERID = "UserID";
	
	private static String g_userid = null;

	public static String getUserID() {
		chocoplayer parent;
		if((parent = chocoplayer.getInstance()) == null)
			return "";
		
		SharedPreferences prefs = parent.getSharedPreferences( PREFS_NAME , Activity.MODE_PRIVATE);

		
		if(g_userid != null)
			return g_userid;

		String code;
		
		code = prefs.getString( PREFS_CODE_USERID , "" );
		
		if(code.isEmpty()){
			TelephonyManager telephony;

			try{
				telephony =  (TelephonyManager)parent.getSystemService(Context.TELEPHONY_SERVICE);
				if(telephony != null){
					code = code + telephony.getDeviceId();
					code = code + telephony.getSubscriberId();
				}
				code = code + Secure.getString(parent.getContentResolver(), Secure.ANDROID_ID);
			}catch(Exception e){
				
			}
			
			code = MgrSecure.md5("Bt43t34nt#%" + code + "wet#%nWTN6");

			SharedPreferences.Editor ed = prefs.edit();
			ed.putString( PREFS_CODE_USERID , code ); 
			ed.commit();
		}
		
		g_userid = code;
		
		return code;
	}
	
	private static String md5(String s) { 
		try { 
        
	        // Create MD5 Hash 
	        MessageDigest digest = java.security.MessageDigest.getInstance("MD5"); 
	        digest.update(s.getBytes()); 
	        byte messageDigest[] = digest.digest();
	 
	         // Create Hex String
	         StringBuffer hexString = new StringBuffer();
	         for (int i=0; i<messageDigest.length; i++)
	             hexString.append(Integer.toHexString(0xFF & messageDigest[i]));
	        return hexString.toString();
 
	     } catch (NoSuchAlgorithmException e) {
	         e.printStackTrace();
	     }
     return "";
 
    }
}
