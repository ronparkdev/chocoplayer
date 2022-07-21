package com.sweetier.mrpg2kplayer.picker;

public abstract class PickerDownloadListener {

	public abstract void onProgress(long bytesDownloaded, long bytesExpected);
	
	public abstract void onSuccess();
	
	public abstract void onFail(Exception e);
}
