package com.toystoy.navidialog;

import com.sweetier.mrpg2kplayer.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;

public class NaviDialogActivity extends Activity implements OnPageChangeListener
{
	private TextView mTvTitle;
	private TextView mTvBack;
	private TextView mTvDone;
	
	private ViewPager mViewPager;
	private NaviDialogPageAdapter mViewPagerAdapter;
	private int mViewPagerIndex;
	
	private static NaviDialogPageNode mMenu = null;
	private static OnNaviDialogActListener mListener = null;
	
	public static void setMenu(NaviDialogPageNode menu){
		mMenu = menu;
	}

	public static void setListener(OnNaviDialogActListener listener){
		mListener = listener;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_navidialog);
		
		init();
		
		setViewPager();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		
		if(mListener != null)
			mListener.onActivityResult(requestCode, resultCode, data);
	}
	
	private void init(){
		BtnOnClickListener btnOnClickListener = new BtnOnClickListener();
		
		mTvTitle = (TextView)findViewById(R.id.tv_navidialog_title);
		mTvBack = (TextView)findViewById(R.id.tv_navidialog_back);
		mTvDone = (TextView)findViewById(R.id.tv_navidialog_done);

		mTvBack.setOnClickListener(btnOnClickListener);
		mTvDone.setOnClickListener(btnOnClickListener);
	}
	
	private void setViewPager() {
		mViewPager = (ViewPager)findViewById(R.id.vp_navidialog);

        mViewPagerIndex = -1;
		mViewPagerAdapter = new NaviDialogPageAdapter(this);
        mViewPager.setAdapter(mViewPagerAdapter);
        mViewPager.setOnPageChangeListener(this);
        
        push(mMenu);
		
		refreshTitle();
	}
	
	class BtnOnClickListener implements OnClickListener  {
		@Override
		public void onClick(View v) {
			if(v == mTvBack)
				pop(true);
			else if(v == mTvDone){
				NaviDialogNodeInterface node = mViewPagerAdapter.getTopItem();
				if(node != null && node.onDoneClicked())
					NaviDialogActivity.this.finish();
			}
		}
	};
	
	public void push(NaviDialogPageNode page){
		mViewPagerAdapter.push(page);
		
		if(mViewPagerIndex == -1){
			mViewPager.setCurrentItem(++mViewPagerIndex);
		}else
			mViewPager.setCurrentItem(++mViewPagerIndex, true);
	}
	
	public void push(NaviDialogViewNode nextViewNode) {
		mViewPagerAdapter.push(nextViewNode);
		
		if(mViewPagerIndex == -1){
			mViewPager.setCurrentItem(++mViewPagerIndex);
		}else
			mViewPager.setCurrentItem(++mViewPagerIndex, true);
	}

	public void pop(boolean withAni){
		if(mViewPagerIndex >= 1){
			mViewPager.setCurrentItem(--mViewPagerIndex, withAni);
			mViewPagerAdapter.pop();
		}
	}

	public void replace(int index, NaviDialogPageNode page){
		mViewPagerAdapter.replace(index, page);
	}
	
	public int getCount(){
		return mViewPagerAdapter.getCount();
	}

	public void setDoneVisible(boolean isVisible){
		mTvDone.setVisibility(isVisible?View.VISIBLE:View.INVISIBLE);
	}
	
	private void refreshTitle(){
		String strTitle = mViewPagerAdapter.getTitle();
		String strBackTitle = mViewPagerAdapter.getBackTitle();
		mTvTitle.setText(strTitle);
		if(strBackTitle.isEmpty()){
			mTvBack.setVisibility(View.INVISIBLE);
		}else{
			mTvBack.setVisibility(View.VISIBLE);
			mTvBack.setText(strBackTitle);	
		}

		NaviDialogNodeInterface node = mViewPagerAdapter.getTopItem();
		if(node != null)
			mTvDone.setText(node.getDoneName());
	}

	@Override public void onPageScrollStateChanged(int arg0) {
		if(arg0 == ViewPager.SCROLL_STATE_IDLE){
			if(mViewPager.getCurrentItem() < mViewPagerIndex)
				pop(false);
		}
		
		refreshTitle();
	}
	@Override public void onPageSelected(int arg0) {}
	@Override public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {}
}
