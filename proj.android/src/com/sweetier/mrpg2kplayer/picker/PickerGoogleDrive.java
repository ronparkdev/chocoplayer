package com.sweetier.mrpg2kplayer.picker;

import java.io.FileOutputStream;
import java.io.InputStream;

import com.dropbox.chooser.android.DbxChooser;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.common.api.GoogleApiClient.OnConnectionFailedListener;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.drive.DriveApi;
import com.google.android.gms.drive.DriveApi.DriveContentsResult;
import com.google.android.gms.drive.DriveContents;
import com.google.android.gms.drive.DriveFile;
import com.google.android.gms.drive.DriveFile.DownloadProgressListener;
import com.google.android.gms.drive.DriveFolder;
import com.google.android.gms.drive.DriveId;
import com.google.android.gms.drive.DriveResource;
import com.google.android.gms.drive.DriveResource.MetadataResult;
import com.google.android.gms.drive.Metadata;
import com.google.android.gms.drive.OpenFileActivityBuilder;
import com.sweetier.mrpg2kplayer.chocoplayer;
import com.sweetier.mrpg2kplayer.menu.SubMenuDownload;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class PickerGoogleDrive implements PickerInterface, ConnectionCallbacks, OnConnectionFailedListener{
    
	private final static String TAG = "PickerGoogleDrive";
    
    private final static String[] MIME_ZIP = {
		DriveFolder.MIME_TYPE, "application/zip", "application/x-zip", "application/x-zip-compressed", 
		"application/octet-stream", "application/x-compress", "application/x-compressed", "multipart/x-zip"
	};
	
	private Activity mActivity;
	private PickerMain mParent;
	private GoogleApiClient mGoogleApiClient;

	private boolean mIsDownload;

	private DriveFile mFileData;
	private Metadata mFileDataMeta;
	
	@Override
	public void init(Activity activity, PickerMain parent) {
		mActivity = activity;
		mParent = parent;
		
		connect();
	}

	@Override
	public void onConnected(Bundle bundle) {
        Log.i(TAG, "in onConnected() - we're connected, let's do the work in the background...");
//      build an intent that we'll use to start the open file activity
    	IntentSender intentSender = Drive.DriveApi
              .newOpenFileActivityBuilder()
//              these mimetypes enable these folders/files types to be selected
              .setMimeType(MIME_ZIP)
              .setActivityTitle("Select ZIP File")
              .build(mGoogleApiClient);
    	try {
			mActivity.startIntentSenderForResult(intentSender, PickerMain.REQUEST_CODE_SELECT, null, 0, 0, 0);
    	} catch (IntentSender.SendIntentException e) {
			Log.i(TAG, "Unable to send intent", e);
    	}
	}

    @Override
    public void onConnectionFailed(ConnectionResult result) {
        Log.i(TAG, "Connection failed - result: " + result.toString());
        if (!result.hasResolution()) {
//            display error dialog
            GooglePlayServicesUtil.getErrorDialog(result.getErrorCode(), mActivity, 0).show();
            return;
        }

        try {
            Log.i(TAG, "trying to resolve the Connection failed error...");
//            tries to resolve the connection failure by trying to restart this activity
            result.startResolutionForResult(mActivity, PickerMain.REQUEST_CODE_RESOLUTION);
        } catch (IntentSender.SendIntentException e) {
            Log.i(TAG, "Exception while starting resolution activity", e);
        }
    }
    
	@Override
	public void onConnectionSuspended(int i) {
        switch (i) {
        case 1:
            Log.i(TAG, "Connection suspended - Cause: " + "Service disconnected");
            break;
        case 2:
            Log.i(TAG, "Connection suspended - Cause: " + "Connection lost");
            break;
        default:
            Log.i(TAG, "Connection suspended - Cause: " + "Unknown");
            break;
        }
        
        mGoogleApiClient = null;
	}

	@Override
	public void connect() {
    	if (mGoogleApiClient == null) {
    		mGoogleApiClient = new GoogleApiClient.Builder(mActivity)
                    .addApi(Drive.API)
                    .addScope(Drive.SCOPE_FILE)
                    .addConnectionCallbacks(this)
                    .addOnConnectionFailedListener(this)
                    .build();
    	}
    	
    	if(mGoogleApiClient != null)
			mGoogleApiClient.connect();
	}

	@Override
	public void initReceive(Intent data, final SubMenuDownload view) {
		
		if(data == null)
			return;
		 
		DriveId driveId = (DriveId) data.getParcelableExtra(OpenFileActivityBuilder.EXTRA_RESPONSE_DRIVE_ID);
        final DriveFile selectedFile = Drive.DriveApi.getFile(mGoogleApiClient, driveId);

        mFileData = selectedFile;
        
        PendingResult<MetadataResult> selectedFileMetadata = selectedFile.getMetadata(mGoogleApiClient);
        
        selectedFileMetadata.setResultCallback(new ResultCallback<DriveResource.MetadataResult>() {
            @Override
            public void onResult(DriveResource.MetadataResult metadataResult) {

            	
                final Metadata fileMetadata = metadataResult.getMetadata();

                mFileDataMeta = fileMetadata;
                
                view.doInit(fileMetadata.getTitle());
            }
        });
	}

	@Override
	public void startReceive(final SubMenuDownload view) {

		if(mFileData == null || mFileDataMeta == null)
			return;
		
        final long fileSize = mFileDataMeta.getFileSize();
        
        final PendingResult<DriveContentsResult> process = mFileData.open(mGoogleApiClient, DriveFile.MODE_READ_ONLY, new DownloadProgressListener() {
			@Override
			public void onProgress(long bytesDownloaded, long bytesExpected) {
				view.doProgress(bytesDownloaded, fileSize);
			}
		});
        
        process.setResultCallback(new ResultCallback<DriveApi.DriveContentsResult>() {
			@Override
			public void onResult(DriveApi.DriveContentsResult result) {
				DriveContents contents = result.getDriveContents();
				
				try {
					int readed;
					byte[] buf = new byte[10240];
					InputStream is = contents.getInputStream();
					FileOutputStream os = new FileOutputStream(mParent.getPath() + mFileDataMeta.getTitle());
					
					mIsDownload = true;
					
					while(true){
						if(!mIsDownload)
							return;
						
						readed = is.read(buf);
						
						if(readed <= 0)
							break;
						
						os.write(buf, 0, readed);
					}

					os.close();
					is.close();
					
					view.doFinish();

					return;
				} catch (Exception e) {
					view.doFail(e);
				}

			}
		});
	}
	
	@Override
	public void stopReceive() {
		mIsDownload = false;
	}
}
