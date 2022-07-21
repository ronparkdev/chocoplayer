package com.toystoy.navidialog;

import java.io.Serializable;

import com.sweetier.mrpg2kplayer.R;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class NaviDialogListNode implements Serializable{
	
	private static final long serialVersionUID = 1L;

	private int mDrawableID;
	private String mText;
	private NaviItemType mType;
	private NaviDialogPageNode mNextPage;
	private NaviDialogViewNode mNextViewNode;
	private OnNaviDialogListListener mListener;
	
	private boolean mValueChanging = false;
	
	public NaviDialogListNode(NaviItemType type, int drawableID, String content, OnNaviDialogListListener listener){
		mType = type;
		mDrawableID = drawableID;
		mText = content;
		mListener = listener;
	}
	
	public NaviDialogListNode(int drawableID, String content, NaviDialogPageNode page){
		mType = NaviItemType.Page;
		mDrawableID = drawableID;
		mText = content;
		mNextPage = page;
	}

	public NaviDialogListNode(int drawableID, String content, NaviDialogViewNode viewNode) {
		mType = NaviItemType.View;
		mDrawableID = drawableID;
		mText = content;
		mNextViewNode = viewNode;
	}

	public int getDrawableID() {
		return mDrawableID;
	}
	
	public String getText() {
		return mText;
	}
	
	public NaviDialogPageNode page(){
		return mNextPage;
	}

	public NaviDialogPageNode getNextPage() {
		return mNextPage;
	}

	public void updateView(final NaviDialogActivity parent, View v, final int position){
		Drawable drawable = null;
		TextView tv 	= (TextView)	v.findViewById(R.id.tv_navidialog_item);
		ImageView iv 	= (ImageView)	v.findViewById(R.id.iv_navidialog_item_next);
		CheckBox cb 	= (CheckBox)	v.findViewById(R.id.cb_navidialog_item);
		RadioButton rb 	= (RadioButton)	v.findViewById(R.id.rb_navidialog_item);
		SeekBar sb 		= (SeekBar)		v.findViewById(R.id.sb_navidialog_item);
		
		tv.setText(mText);
		sb.setMax(100);
		OnCheckedChangeListener btnOnCheckedChangeListener = new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				if(!mValueChanging && mListener != null){
					mListener.onClicked(parent, position, isChecked?1:0);
				}
			}
		};
		
		cb.setOnCheckedChangeListener(btnOnCheckedChangeListener);
		rb.setOnCheckedChangeListener(btnOnCheckedChangeListener);
		sb.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				if(!mValueChanging && mListener != null)
					mListener.onClicked(parent, position, seekBar.getProgress());
			}
			
			@Override public void onStartTrackingTouch(SeekBar seekBar) {}
			@Override public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {}
		});
		
		if(mDrawableID != -1)
			drawable = parent.getResources().getDrawable(mDrawableID);
		
		tv.setCompoundDrawablePadding((drawable != null)?20:0);
		tv.setCompoundDrawables(drawable, null, null, null);

		int val = -1;
		
		if(mListener != null){
			mValueChanging = true;
			val = mListener.onLoaded(parent, position);
			mValueChanging = false;
		}
		switch(mType){
		case OnlyText:
		{
			iv.setVisibility(View.GONE);
			cb.setVisibility(View.GONE);
			rb.setVisibility(View.GONE);
			sb.setVisibility(View.GONE);
			break;
		}
		case CallBack:
		case CallBackWeb:
		case Page:
		case View:
		{
			if(mType != NaviItemType.CallBackWeb)
				iv.setImageResource(R.drawable.ic_navidialog_next);
			else
				iv.setImageResource(R.drawable.ic_navidialog_web);
			iv.setVisibility(View.VISIBLE);
			cb.setVisibility(View.GONE);
			rb.setVisibility(View.GONE);
			sb.setVisibility(View.GONE);
			break;
		}
		case CheckBox:
		{
			iv.setVisibility(View.GONE);
			cb.setVisibility(View.VISIBLE);
			rb.setVisibility(View.GONE);
			sb.setVisibility(View.GONE);
			if(val != -1)
				cb.setChecked(val != 0);
			break;
		}
		case RadioBtn:
		{
			iv.setVisibility(View.GONE);
			cb.setVisibility(View.GONE);
			rb.setVisibility(View.VISIBLE);
			sb.setVisibility(View.GONE);
			if(val != -1)
				rb.setChecked(val != 0);
			break;
		}
		case SeekBar:
		{
			iv.setVisibility(View.GONE);
			cb.setVisibility(View.GONE);
			rb.setVisibility(View.GONE);
			sb.setVisibility(View.VISIBLE);
			if(val != -1)
				sb.setProgress(val);
			break;
		}
		}
	}

	public void onItemClick(NaviDialogActivity mParent, int position) {
		if(mNextPage != null)
			mParent.push(mNextPage);
		
		if(mListener != null)
			mListener.onClicked(mParent, position, 0);
		
		if(mNextViewNode != null)
			mParent.push(mNextViewNode);
	}
	
	
}
