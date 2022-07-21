package com.sweetier.mrpg2kplayer.picker;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.widget.Toast;

import com.dropbox.chooser.android.DbxChooser;
import com.sweetier.mrpg2kplayer.menu.SubMenuDownload;

public class PickerOneDrive implements PickerInterface{

	private final static String TAG = "PickerOneDrive";
	private final static String APP_KEY = "0000000048169939";
	
	private Activity mActivity;
	private PickerMain mParent;
	private DbxChooser mChooser;
	
	private PickerDownload mDownload;

	@Override
	public void init(Activity activity, PickerMain parent) {
		mActivity = activity;
		mParent = parent;
		
		connect();
	}

	@Override
	public void connect() {
		if(mChooser == null){
		    mChooser = new DbxChooser(APP_KEY);
            mChooser.forResultType(DbxChooser.ResultType.PREVIEW_LINK)
            .launch(mActivity, PickerMain.REQUEST_CODE_SELECT);
		}
	}

	@Override
	public void initReceive(Intent data, final SubMenuDownload view) {

		if(data == null)
			return;
		
		final DbxChooser.Result result = new DbxChooser.Result(data);
        Toast.makeText(mActivity, result.getLink() + "\n" + result.getName() + "\n" + result.getSize() + "byte", Toast.LENGTH_LONG).show();
	
        view.doInit(result.getName());
        
	}

	@Override
	public void startReceive(final SubMenuDownload view){
//        mDownload = new PickerDownload();
//        mDownload.setListener(new PickerDownloadListener() {
//			@Override
//			public void onSuccess() {
//				view.doFinish();
//			}
//			
//			@Override
//			public void onProgress(long bytesDownloaded, long bytesExpected) {
//				view.doProgress(bytesDownloaded, bytesExpected);
//			}
//			
//			@Override
//			public void onFail(Exception e) {
//				view.doFail(e);
//			}
//		});
//        mDownload.execute(mParent.getPath() + result.getName(), result.getLink().toString(), "");
	}

	@Override
	public void stopReceive() {
		// TODO Auto-generated method stub
		
	}
}
