package com.sweetier.mrpg2kplayer.ad;

import android.app.Activity;

public interface advertise_interface {
	public void onCreate(Activity act);
	public void onDestroy();
	public void onPause();
	public void onResume();

	public void showLine();
	public void hideLine();
	
	public void showBig();
}
