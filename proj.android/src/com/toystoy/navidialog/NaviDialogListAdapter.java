package com.toystoy.navidialog;

import com.sweetier.mrpg2kplayer.R;

import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class NaviDialogListAdapter extends BaseAdapter{
	
	private NaviDialogActivity mParent;
	private LayoutInflater mInflater;
	private NaviDialogPageNode mPage;
	
	public NaviDialogListAdapter(NaviDialogActivity parent, NaviDialogPageNode page){
		super();
		
		mParent = parent;
		mInflater = LayoutInflater.from(parent);
		mPage = page;
	}
	
	@Override
	public int getCount() {
		return mPage.getCount();
	}

	@Override
	public Object getItem(int position) {
		return mPage.getItem(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View cvtView, ViewGroup parent) {
		final NaviDialogListNode n = mPage.getItem(position);
				
		if(cvtView == null)
			cvtView = mInflater.inflate(R.layout.listview_navidialog, null);

		n.updateView(mParent, cvtView, position);
		
		return cvtView;
	}


	public void onItemClick(int position) {
		
		final NaviDialogListNode n = mPage.getItem(position);

		n.onItemClick(mParent, position);
	}

}
