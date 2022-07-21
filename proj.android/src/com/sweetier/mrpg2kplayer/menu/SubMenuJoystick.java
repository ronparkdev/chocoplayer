
package com.sweetier.mrpg2kplayer.menu;

import com.sweetier.mrpg2kplayer.R;
import com.sweetier.mrpg2kplayer.chocoplayer;
import com.toystoy.navidialog.NaviDialogActivity;
import com.toystoy.navidialog.NaviDialogViewNode;

import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.widget.Button;
import android.widget.TextView;

public class SubMenuJoystick extends NaviDialogViewNode{
	
	private final static String KEY_NAMES[] = {
			"Up", "Down", "Left", "Right", 
			"A", "B", "Shift", 
			"Num 0", "Num 1", "Num 2", "Num 3", "Num 4",  
			"Num 5", "Num 6", "Num 7", "Num 8", "Num 9",
			"Special +", "Special -", "Special *", "Special /", "Special ¡Ý",
			"Menu", "Turbo", "QuickSave"
	};

	private MgrMenu parent;
	private String stack;
	
	private TextView tvExplain;
	private TextView tvKeyCur;
	private TextView tvKeyMapped;

	private Button btnPrev;
	private Button btnNext;
	
	private int keyId;
	
	public SubMenuJoystick(MgrMenu parent, String stack) {
		this.parent = parent;
		this.stack = stack;
	}
	
	@Override
	public int getLayoutID() {
		return R.layout.activity_menu_joystick;
	}
	
	@Override
	public String getTitle() {
		return "Joystick";
	}

	@Override
	public void onCreate(NaviDialogActivity act, View v) {
		super.onCreate(act, v);
		
		tvExplain = (TextView)v.findViewById(R.id.tv_joystick_explain);
		tvKeyCur = (TextView)v.findViewById(R.id.tv_joystick_current);
		tvKeyMapped = (TextView)v.findViewById(R.id.tv_joystick_mapped);
		btnPrev = (Button)v.findViewById(R.id.btn_joystick_prev);
		btnNext = (Button)v.findViewById(R.id.btn_joystick_next);
		
		ViewOnKeyListener viewOnKeyListener = new ViewOnKeyListener();
		v.setOnKeyListener(viewOnKeyListener);
		btnPrev.setOnKeyListener(viewOnKeyListener);
		btnNext.setOnKeyListener(viewOnKeyListener);
		v.setFocusable(true);
		
		BtnOnClickListener btnOnClickListener = new BtnOnClickListener();
		btnPrev.setOnClickListener(btnOnClickListener);;
		btnNext.setOnClickListener(btnOnClickListener);
		
		keyId = 0;
	}
	
	class BtnOnClickListener implements OnClickListener{
		@Override
		public void onClick(View v) {
			if(v == btnPrev)
				keyId = (keyId + (KEY_NAMES.length-1)) % KEY_NAMES.length;
			else if(v == btnNext)
				keyId = (keyId + 1) % KEY_NAMES.length;
			
			MgrMenu menu = chocoplayer.g_menu;
			int mappedKey = menu.getValue(MgrMenu.RST_JOYSTICK_KEYCODE + keyId);
			
			tvKeyCur.setText(KEY_NAMES[keyId]);
			tvKeyMapped.setText(KeyHelper.getKeyName(mappedKey));
		}
	}
	
	class ViewOnKeyListener implements OnKeyListener{
		@Override
		public boolean onKey(View v, int keyCode, KeyEvent event) {
			parent.getMenu(String.format("%s|%d|%d", stack, keyId, keyCode));
			
			tvKeyMapped.setText(KeyHelper.getKeyName(keyCode));
			
			return false;
		}
	}
}
