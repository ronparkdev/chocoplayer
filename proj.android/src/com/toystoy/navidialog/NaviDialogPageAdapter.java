package com.toystoy.navidialog;

import java.util.ArrayList;

import com.sweetier.mrpg2kplayer.R;

import android.content.Context;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;

public class NaviDialogPageAdapter extends PagerAdapter{

	private NaviDialogActivity mParent;
	private LayoutInflater mInflater;
	
	private ArrayList<NaviDialogNodeInterface> mPages;
	private ArrayList<View> mPageViews;

	private int mInitedPages;
	
	public NaviDialogPageAdapter(NaviDialogActivity parent){
		super();
		
		mParent = parent;
		mInflater = LayoutInflater.from(parent);
		mPages = new ArrayList<NaviDialogNodeInterface>();
		mPageViews = new ArrayList<View>();
		
		mInitedPages = -1;
	}
    
	@Override
	public int getCount() {
		return mPages.size();
	}

    @Override
	public int getItemPosition(Object object) {
    	int position = (Integer) ((View)object).getTag();
    	if(position < mPages.size() && position <= mInitedPages)
    		return super.getItemPosition(object);
    	else
        	return PagerAdapter.POSITION_NONE;
	}

	@Override
    public boolean isViewFromObject(View view, Object object) {
        return view == object;
    }


    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        ViewPager pager = (ViewPager) container;

        View v;
        final NaviDialogNodeInterface something = mPages.get(position);
        if(something instanceof NaviDialogViewNode){
        	NaviDialogViewNode node = (NaviDialogViewNode)something;
        	
        	v = mInflater.inflate(node.getLayoutID(), pager, false);
        	node.onCreate(mParent, v);
        	
        }else if(something instanceof NaviDialogPageNode){
        	NaviDialogPageNode node = (NaviDialogPageNode)something;
	        final NaviDialogListAdapter adapter = new NaviDialogListAdapter(mParent, node);
        	
            ListView lv;
	        
	        v = mInflater.inflate(R.layout.inflate_navidialog, pager, false);
	        lv = (ListView)v.findViewById(R.id.lv_navidialog);
	        lv.setAdapter(adapter);
	        lv.setChoiceMode(ListView.CHOICE_MODE_NONE);
	        lv.setCacheColorHint(0x00000000);
	        lv.setOnItemClickListener(new OnItemClickListener() {
				@Override
				public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
					adapter.onItemClick(position);
				}
			});
        }else{
        	Log.e("??????", "What the fuck... NaviDialogPageAdapter");
        	return null;
        }

    	v.setTag(position);
        mPageViews.add(v);
        pager.addView(v);
        
        mInitedPages = Math.max(position, mInitedPages);
        
        return v;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object view) {
        ViewPager pager = (ViewPager) container;
        pager.removeView((View) view);
    }
    
    public void push(NaviDialogPageNode page){
    	mPages.add(page);
    	this.notifyDataSetChanged();
    }

	public void push(NaviDialogViewNode nextViewNode) {
    	mPages.add(nextViewNode);
    	this.notifyDataSetChanged();
	}

	public void replace(int index, NaviDialogPageNode page) {
		mPages.remove(index);
		mPages.add(index, page);
		this.notifyDataSetChanged();
		mInitedPages = Math.min(index-1, mInitedPages);
	}
    
    public void pop(){
    	if(mPages.size() > 1){
    		int id = mPages.size()-1;
    		mPages.get(id).willDestroy();
    		mPages.remove(id);
    		this.notifyDataSetChanged();
    	}
    }
    
    public NaviDialogNodeInterface getTopItem(){
    	if(mPages.size() == 0)
    		return null;
    	else 
    		return mPages.get(mPages.size()-1);
    }

	public String getTitle() {
		if(mPages.size() < 1)
			return "";
		else
			return mPages.get(mPages.size()-1).getTitle();
	}

	public String getBackTitle() {
		if(mPages.size() < 2)
			return "";
		else
			return mPages.get(mPages.size()-2).getTitle();
	}
}
