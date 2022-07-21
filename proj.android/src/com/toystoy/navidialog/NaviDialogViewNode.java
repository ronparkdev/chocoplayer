package com.toystoy.navidialog;

import android.view.View;

public class NaviDialogViewNode extends NaviDialogNodeInterface {
	
	protected NaviDialogActivity mAct;

	public void onCreate(NaviDialogActivity act, View v) {
		mAct = act;
	}

	public int getLayoutID() {
		return -1;
	}
	
	public void finish(){
		mAct.pop(true);
	}
}
