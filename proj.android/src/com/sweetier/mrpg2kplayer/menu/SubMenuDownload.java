package com.sweetier.mrpg2kplayer.menu;

import java.util.Timer;
import java.util.TimerTask;

import com.sweetier.mrpg2kplayer.R;
import com.sweetier.mrpg2kplayer.picker.PickerInterface;
import com.toystoy.navidialog.NaviDialogActivity;
import com.toystoy.navidialog.NaviDialogViewNode;

import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

public class SubMenuDownload extends NaviDialogViewNode {

	private Activity mAct;

	private TextView tvName;
	private TextView tvState;
	private TextView tvSpeed;
	private ProgressBar pbState;
	private Button btn;

	private PickerInterface mPicker;
	private Intent mPickerData;

	private long mReceivedByte;
	private long mTotalByte;

	private Timer mTimer = new Timer();
	
	private int mState;//0:NONE, 1:READY, 2:PREPARE 3:DOWNLOADING, 4:STOP
	
	public SubMenuDownload(PickerInterface picker, Intent data) {
		mPicker = picker;
		mPickerData = data;
		
		mReceivedByte = mTotalByte = -1;
	}

	@Override
	public int getLayoutID() {
		return R.layout.activity_menu_download;
	}

	@Override
	public void onCreate(NaviDialogActivity act, final View v) {
		super.onCreate(act, v);

		mAct = act;

		tvName = (TextView) v.findViewById(R.id.tv_download_name);
		tvState = (TextView) v.findViewById(R.id.tv_download_state);
		tvSpeed = (TextView) v.findViewById(R.id.tv_download_speed);
		pbState = (ProgressBar) v.findViewById(R.id.pb_download_state);
		btn = (Button) v.findViewById(R.id.btn_download);
		
		tvName.setText("FileName:?");
		tvState.setText("Receive info from cloud");
		tvSpeed.setText("");
		pbState.setProgress(0);
		btn.setVisibility(View.INVISIBLE);
		btn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if(mState == 1){
					mState = 2;
					mPicker.startReceive(SubMenuDownload.this);
				}else if(mState == 3){
					mState = 4;
					mPicker.stopReceive();
				}
					
			}
		});
		
		mState = 0;
		
		mPicker.initReceive(mPickerData, this);
	}

	@Override
	public void willDestroy() {
		mPicker.stopReceive();
		super.willDestroy();
	}

	public void doInit(final String filename) {
		mAct.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				tvName.setText(String.format("FileName:%s", filename));
				tvState.setText("Standby..");
				btn.setText("DOWNLOAD IT");
				btn.setVisibility(View.VISIBLE);
			}
		});
		mState = 1;
	}
	
	public void doStart(){
		mState = 3;

		mAct.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				tvState.setText("Downloading file");
				btn.setText("STOP IT");
				btn.setVisibility(View.VISIBLE);
			}
		});
		
		mTimer.schedule(new TimerTask() {
			@Override
			public void run() {
				if(mReceivedByte != -1){
					mAct.runOnUiThread(new Runnable() {
						@Override
						public void run() {
							if (mTotalByte <= 0) {
								tvState.setText(String.format("Downloading! %s", getSize(mReceivedByte)));
								pbState.setProgress(0);
							} else {
								float percent = 100.0f * mReceivedByte / mTotalByte;
	
								tvState.setText(String.format("Downloading! %s / %s (%3.1f)", getSize(mReceivedByte), getSize(mTotalByte), percent));
								pbState.setProgress((int) percent);
							}
						}
					});
				}
			}
		}, 500, 500);
	}

	private String getSize(long bytes){
		for(int i=0; i<5; i++){
			if(bytes < 1024 || (i==4)){
				switch(i){
				case 0:return String.format("%d B", bytes);
				case 1:return String.format("%d KB", bytes);
				case 2:return String.format("%d MB", bytes);
				case 3:return String.format("%d GB", bytes);
				case 4:return String.format("%d TB", bytes);
				}
			}
			bytes /= 1024;
		}
		return "";
	}
	
	public void doProgress(final long receivedByte, final long totalByte) {
		mReceivedByte = receivedByte;
		mTotalByte = totalByte;
	}

	public void doFinish() {
		mState = 4;
		
		mTimer.cancel();
		
		mAct.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				tvState.setText("Download Success!");
				pbState.setProgress(100);
			}
		});
	}

	public void doFail(final Exception e) {
		mState = 4;
		mAct.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				e.printStackTrace();
				tvState.setText("Failed..");
			}
		});
	}

}
