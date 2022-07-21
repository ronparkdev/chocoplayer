package com.toystoy.navidialog;

public class NaviDialogNodeInterface {
	public String getTitle(){
		return "";
	}
	
	public String getDoneName(){
		return "";
	}
	
	public boolean onDoneClicked(){
		return false;
	}

	public void willDestroy() {
		
	}
}
