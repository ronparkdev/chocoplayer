package com.sweetier.mrpg2kplayer.menu;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.sweetier.mrpg2kplayer.chocoplayer;
import com.sweetier.mrpg2kplayer.picker.PickerMain;
import com.toystoy.navidialog.NaviDialogActivity;
import com.toystoy.navidialog.NaviDialogPageAdapter;
import com.toystoy.navidialog.NaviDialogPageNode;
import com.toystoy.navidialog.NaviItemType;
import com.toystoy.navidialog.OnNaviDialogActListener;
import com.toystoy.navidialog.OnNaviDialogListListener;

import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class MgrMenu {

	private final static String TAG = "MgrMenu";
	
	public final static int RST_JOYSTICK_KEYCODE 	= 120;
	public final static int RST_ETC_IS_VERTICAL 	= 165;

	public native int getValue(int type);
	public native void setValue(int type, int value);
	
	public native String getMenu(String cmd);
	
	public native boolean doBugReport( String path, String hash, String email, String detail);
	
	public boolean isVertical() {
		boolean isVert = (getValue(RST_ETC_IS_VERTICAL) == 1);
		boolean isPro = chocoplayer.isPro();
		
		if(!isPro)
			setValue(RST_ETC_IS_VERTICAL, 1);
		
		return isVert || !isPro;
	}

	public void showMenu(NaviDialogActivity act, String stack){
		showMenu(act, stack, -1);
	}
	
	public void showMenu(NaviDialogActivity act, String stack, int replacePos){
		final chocoplayer parent = chocoplayer.getInstance();
		if(parent == null)
			return;
		
		try {
			String jsonRaw = getMenu(stack);
			JSONObject jData = new JSONObject(jsonRaw);
			
			String sdo = jData.getString("DO");
			String stitle = "";
			if(!jData.isNull("TITLE"))
				stitle = jData.getString("TITLE");
			
			if("MENU".equals(sdo)){
				NaviDialogPageNode node = new NaviDialogPageNode(stitle);
				showMenuParse(node, stack, jData);
				
				if(act == null){
					Intent intent = new Intent(parent, NaviDialogActivity.class);
					NaviDialogActivity.setMenu(node);
					NaviDialogActivity.setListener(null);
					
					parent.startActivity(intent);
				}else if(replacePos == -1){
					act.push(node);
				}else{
					act.replace(replacePos, node);
				}
				
			}else if(act != null){
				if("CLOSE".equals(sdo)){
					act.finish();
				}else if("BACK".equals(sdo)){
					act.pop(true);
				}else if("BACK_REFRESH".equals(sdo)){
					String nstack = jData.getString("REFRESH_TO");
					int curPos = 0;
					
					for(int i=0; i<nstack.length(); i++)
						if(nstack.charAt(i) == '|')
							curPos++;

					if(nstack.startsWith("|MAINMENU"))
						showMenu(act, nstack, curPos - 1);
					else if(nstack.startsWith("|DETAILMENU"))
						showMenu(act, nstack, curPos - 3);
					
					act.pop(true);
				}else if("JOYSTICK".equals(sdo)){
					act.push(new SubMenuJoystick(this, stack));
				}else if("FEEDBACK".equals(sdo)){
					String stype, spath, sname, shash;
					JSONObject jfeed = jData.getJSONObject("FEEDBACK");
					stype = jfeed.getString("TYPE");
					if(jfeed.isNull("PATH")){
						spath = sname = shash = "";
					}else{
						spath = jfeed.getString("PATH");
						sname = jfeed.getString("NAME");
						shash = jfeed.getString("HASH");
					}
					if(!"EMUL".equals(stype)){
						sname = stype;
						spath = shash = "";
					}
					act.push(new SubMenuFeedback(this, stack, sname, spath, shash));
				}else if("MESSAGE".equals(sdo)){
					String smessage = jData.getString("MESSAGE");
					
					Toast.makeText(chocoplayer.getInstance(), smessage, Toast.LENGTH_LONG).show();
				}
			}
					
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	private void showMenuParse(NaviDialogPageNode node, String stack, JSONObject jData) throws JSONException{
		
		JSONArray jList = jData.getJSONArray("MENU");
		
		node.clear();
		for(int i=0; i<jList.length(); i++){
			JSONObject jItem = jList.getJSONObject(i);

			NaviItemType ntype;
			final String scaption = jItem.getString("C");
			final String sname = jItem.getString("N");
			final String stype = jItem.getString("T");
			int ctype = 0;
			int cval = 0;
			ctype = stype.charAt(0);
			if(stype.length() > 1)
				cval = stype.charAt(1);
			
			final int fctype = ctype;
			final int fcval = cval;
			
			switch(ctype){
				case 'N':ntype = NaviItemType.OnlyText;break;
				case 'A':ntype = NaviItemType.CallBack;break;
				case 'W':ntype = NaviItemType.CallBackWeb;break;
				case 'S':ntype = NaviItemType.CheckBox;break;
				default:ntype = NaviItemType.OnlyText;break;
			}
			
			final String thisStack = stack + "|" + sname;
			node.addItem(ntype, scaption, new OnNaviDialogListListener(){
				@Override
				public int onLoaded(NaviDialogActivity navi, int position) {
					if(fctype == 'S')
						return (fcval == 'Y')?1:0;
					else
						return 0;
				}

				@Override
				public void onClicked(NaviDialogActivity navi, int position, int value) {
					if(fctype == 'S')
						showMenu(navi, thisStack + "|" + ((value == 1)?"Y":"N"));
					else
						showMenu(navi, thisStack);
				}
			});
		}
	}
	
	public void showGameAddMenu(final PickerMain picker) {
		final Activity activity = chocoplayer.getInstance();
		if(activity == null)
			return;
		
		OnNaviDialogListListener listener = new OnNaviDialogListListener() {
			@Override
			public void onClicked(NaviDialogActivity navi, int position, int value) {
				int d;
				switch(position){
//				case 0:d = PickerMain.PICKER_URL;break;
//				case 1:d = PickerMain.PICKER_FOLDER;break;
				case 0:d = PickerMain.PICKER_DROPBOX;break;
				case 1:d = PickerMain.PICKER_GOOGLEDRIVE;break;
				default:return;
				}

				picker.startSelection(navi, d);
			}
		};
		
		NaviDialogPageNode page = new NaviDialogPageNode("GameAdd");
//		page.addItem(NaviItemType.CallBack, "URL", listener);
//		page.addItem(NaviItemType.CallBack, "Folder", listener);
		page.addItem(NaviItemType.CallBack, "Dropbox", listener);
//		page.addItem(NaviItemType.CallBack, "GoogleDrive", listener);
		
		Intent intent = new Intent(activity, NaviDialogActivity.class);
		NaviDialogActivity.setMenu(page);
		NaviDialogActivity.setListener(new OnNaviDialogActListener(){
			@Override
			public void onActivityResult(int requestCode, int resultCode, Intent data) {
				picker.onActivityResult(requestCode, resultCode, data);
			}			
		});
		activity.startActivity(intent);
	}
}
