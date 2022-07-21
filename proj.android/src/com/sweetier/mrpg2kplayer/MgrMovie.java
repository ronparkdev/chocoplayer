package com.sweetier.mrpg2kplayer;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class MgrMovie {

	final static String TAG = "r2k_manager_movie";
	
	ArrayList<Hashtable<String, Object>> m_data;
	String m_targetVideo = "비디오경로";
	Intent m_playVideo;
	PackageManager m_pm;
	
	public boolean doRun = false;
	
	public void show(String path){ 
		chocoplayer parent;
		if((parent = chocoplayer.getInstance()) == null)
			return;
		
		m_targetVideo = path;
		if(m_targetVideo != null){
			parent.runOnUiThread(new Runnable(){
	            @Override
	             public void run() {
	            	showList();
	            }
			});
		}
		
	}
	
	private void showList(){
		try{
			final chocoplayer parent;
			if((parent = chocoplayer.getInstance()) == null)
				return;
			
			//m.setContentView(new TextView(m));
			m_playVideo = new Intent(Intent.ACTION_VIEW);
			Uri uri = Uri.parse(m_targetVideo);
			m_playVideo.setDataAndType(uri, "video/*");
			if(m_pm == null)m_pm = parent.getPackageManager();
			List<ResolveInfo> list = m_pm.queryIntentActivities(m_playVideo,
					PackageManager.GET_META_DATA);
			m_data = new ArrayList<Hashtable<String, Object>>();
			for (ResolveInfo resolveInfo : list) {
				try {
					Hashtable<String, Object> tmp = new Hashtable<String, Object>();
					tmp.put("activity", resolveInfo.activityInfo.name);
					tmp.put("package", resolveInfo.activityInfo.packageName);
					tmp.put("appname",
							m_pm.getApplicationLabel(
									resolveInfo.activityInfo.applicationInfo)
									.toString());
					tmp.put("res", resolveInfo.activityInfo.loadIcon(m_pm));
					m_data.add(tmp);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			
			AlertDialog.Builder alert = new Builder(parent);
			alert.setTitle("Select Player");
			ListView listView = new ListView(parent);
			alert.setView(listView);
			alert.setNegativeButton("SKIP", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					dialog.dismiss();
				}
			});
			
			final Dialog dial = alert.show();
			
			listView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
			listView.setBackgroundColor(0xFFFFFFFF);
			listView.setPadding(5, 5, 5, 5);
			listView.setAdapter((ListAdapter) new RunListAdapter(parent));
			listView.setOnItemClickListener(new OnItemClickListener() {

				@Override
				public void onItemClick(AdapterView<?> arg0, View arg1,
						int arg2, long arg3) {
					try{
						dial.dismiss();
						//파라미터를 넘기는 방법입니다. 
						ComponentName cm = new ComponentName((String) m_data.get(arg2).get("package"), (String) m_data.get(arg2).get("activity"));
						Intent intent = new Intent(Intent.ACTION_VIEW);
						intent.setComponent(cm);
						intent.setDataAndType(Uri.parse(m_targetVideo), "video/*");
						parent.startActivity(intent);
					}catch(Exception e){
						e.printStackTrace();
					}
				}
			});
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	class RunListAdapter extends BaseAdapter implements ListAdapter{

		Context mContext;
		LayoutInflater mInflater;
		
		public RunListAdapter(Context context){
			mContext = context;
			mInflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		}
		
		public int getCount() {
			return m_data.size();
		}

		public Object getItem(int position) {
			return m_data.get(position);
		}

		public long getItemId(int position) {
			return 0;
		}

		public String getPkgActivity(int position){
			return (String) m_data.get(position).get("activity");
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			View v = mInflater.inflate(R.layout.movie_selector, parent);
			ImageView img = (ImageView) v.findViewById(R.id.movSelectIcon);
			TextView tx = (TextView) v.findViewById(R.id.movSelectLabel);
			
			tx.setText((CharSequence) m_data.get(position).get("appname"));
			img.setImageDrawable((Drawable) m_data.get(position).get("res"));
			
			return v;
		}
	}
}
