package com.sweetier.mrpg2kplayer.picker;

import com.sweetier.mrpg2kplayer.menu.SubMenuDownload;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

public interface PickerInterface {

	public void init(Activity activity, PickerMain parent);
	
	public void connect();
	
	public void initReceive(Intent intent, SubMenuDownload view);

	public void startReceive(SubMenuDownload view);
	
	public void stopReceive();
}
