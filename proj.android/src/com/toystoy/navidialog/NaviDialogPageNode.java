package com.toystoy.navidialog;

import java.io.Serializable;
import java.util.ArrayList;

public class NaviDialogPageNode extends NaviDialogNodeInterface implements Serializable{

	private static final long serialVersionUID = 1L;
	
	private String mTitle;
	private ArrayList<NaviDialogListNode> mList;
	
	public NaviDialogPageNode(String title){
		mList = new ArrayList<NaviDialogListNode>();
		mTitle = title;
	}
	
	@Override
	public String getDoneName() {
		return "¥¹";
	}

	@Override
	public boolean onDoneClicked() {
		return true;
	}

	@Override
	public String getTitle(){
		return mTitle;
	}
	
	public int getCount() {
		return mList.size();
	}

	public NaviDialogListNode getItem(int position) {
		return mList.get(position);
	}

	public void addItem(NaviItemType type, String content){
		addItem(type, -1, content, null);
	}

	public void addItem(NaviItemType type, int drawableID, String content){
		addItem(type, drawableID, content, null);
	}
	
	public void addItem(NaviItemType type, String content, OnNaviDialogListListener listener){
		addItem(type, -1, content, listener);
	}
	
	public void addItem(NaviItemType type, int drawableID, String content, OnNaviDialogListListener listener){
		mList.add(new NaviDialogListNode(type, drawableID, content, listener));
	}
	
	public void addItem(String content, NaviDialogPageNode page){
		addItem(-1, content, page);
	}
	
	public void addItem(int drawableID, String content, NaviDialogPageNode page){
		mList.add(new NaviDialogListNode(drawableID, content, page));
	}

	public void addItem(String content, NaviDialogViewNode subMenuJoystick) {
		addItem(-1, content, subMenuJoystick);
	}

	public void addItem(int drawableID, String content, NaviDialogViewNode viewNode) {
		mList.add(new NaviDialogListNode(drawableID, content, viewNode));
		
	}

	public void clear() {
		mList.clear();
	}
}
