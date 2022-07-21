package com.sweetier.mrpg2kplayer.picker;

import com.sweetier.mrpg2kplayer.chocoplayer;
import com.sweetier.mrpg2kplayer.menu.SubMenuDownload;
import com.toystoy.navidialog.NaviDialogActivity;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;

public class PickerMain{

	public static final int PICKER_URL = 1000;
	public static final int PICKER_DROPBOX = 1001;
	public static final int PICKER_FOLDER = 1002;
	public static final int PICKER_GOOGLEDRIVE = 1003;
	public static final int PICKER_ONEDRIVE = 1004;

    public static final int REQUEST_CODE_SELECT = 102;
    public static final int REQUEST_CODE_RESOLUTION = 103;
    
    private NaviDialogActivity mNaviAct;
    
    private PickerInterface mPicker;
    
    private ProgressDialog mProgressDialog;
    
    private String mPath;
    
    public void startSelection(NaviDialogActivity naviAct, int picker){
    
    	mNaviAct = naviAct;
    	
    	switch(picker){
    	case PICKER_FOLDER:mPicker = new PickerFolder();break;
    	case PICKER_DROPBOX:mPicker = new PickerDropbox();break;
    	case PICKER_GOOGLEDRIVE:mPicker = new PickerGoogleDrive();break;
    	default:return;
    	}

    	mPicker.init(mNaviAct, this);
    }
    
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch(requestCode){
			case REQUEST_CODE_SELECT:
		    	SubMenuDownload mView = new SubMenuDownload(mPicker, data);
		    	mNaviAct.push(mView);
				break;
				
			case REQUEST_CODE_RESOLUTION:
		    	mPicker.connect();
				break;
		}
	}

    public void setPath(String path){
    	mPath = path;
    }
    
	public String getPath() {
		return mPath;
	}
}
