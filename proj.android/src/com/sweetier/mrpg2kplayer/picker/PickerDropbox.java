package com.sweetier.mrpg2kplayer.picker;

import com.dropbox.chooser.android.DbxChooser;
import com.sweetier.mrpg2kplayer.menu.SubMenuDownload;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class PickerDropbox implements PickerInterface{

	private final static String TAG = "PickerDropbox";
	private final static String APP_KEY = "0d2jurwjf65gerg";
	
	private Activity mActivity;
	private PickerMain mParent;
	private DbxChooser mChooser;
	
	private PickerDownload mDownload;
	
	private DbxChooser.Result mFileData;

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
            mChooser.forResultType(DbxChooser.ResultType.DIRECT_LINK)
            .launch(mActivity, PickerMain.REQUEST_CODE_SELECT);
		}
	}

	@Override
	public void initReceive(Intent data, final SubMenuDownload view) {

		if(data == null)
			return;
		
		final DbxChooser.Result result = new DbxChooser.Result(data);
		
        //Toast.makeText(mActivity, result.getLink() + "\n" + result.getName() + "\n" + result.getSize() + "byte", Toast.LENGTH_LONG).show();

        view.doInit(result.getName());
        
        mFileData = result;
	}

	@Override
	public void startReceive(final SubMenuDownload view){
		if(mFileData == null)
			return;

		final long fileSize = mFileData.getSize();
		
        mDownload = new PickerDownload();
        mDownload.setListener(new PickerDownloadListener() {
			@Override
			public void onSuccess() {
				view.doFinish();
			}
			
			@Override
			public void onProgress(long bytesDownloaded, long bytesExpected) {
				view.doProgress(bytesDownloaded, fileSize);
			}
			
			@Override
			public void onFail(Exception e) {
				view.doFail(e);
			}
		});
        mDownload.execute(mParent.getPath() + mFileData.getName(), mFileData.getLink().toString(), "");
	
        view.doStart();
	}
	
	@Override
	public void stopReceive() {
		if(mDownload != null)
			mDownload.cancel(true);
	}
}
