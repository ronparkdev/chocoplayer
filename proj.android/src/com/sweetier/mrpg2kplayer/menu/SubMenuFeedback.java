package com.sweetier.mrpg2kplayer.menu;

import org.json.JSONObject;

import com.sweetier.mrpg2kplayer.MgrSecure;
import com.sweetier.mrpg2kplayer.R;
import com.sweetier.mrpg2kplayer.chocoplayer;
import com.toystoy.navidialog.NaviDialogActivity;
import com.toystoy.navidialog.NaviDialogViewNode;

import android.app.Activity;
import android.content.SharedPreferences;
import android.view.View;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Toast;

public class SubMenuFeedback extends NaviDialogViewNode{
	
	private final static String PREFS_CODE_EMAIL = "email";
	
	private SharedPreferences prefs;
	
	private MgrMenu parent;
	private String stack;
	
	private String name, path, hash;
	private EditText etEmail;
	private EditText etExplain;
	private RelativeLayout rlProcess;
	
	public SubMenuFeedback(MgrMenu parent, String stack, String name, String path, String hash) {
		this.parent = parent;
		this.stack = stack;
		
		this.name = name;
		this.path = path;
		this.hash = hash;
	}

	@Override
	public int getLayoutID() {
		return R.layout.activity_menu_feedback;
	}
	
	@Override
	public void onCreate(NaviDialogActivity act, View v) {
		super.onCreate(act, v);

		prefs = act.getSharedPreferences( MgrSecure.PREFS_NAME , Activity.MODE_PRIVATE);
		
		etEmail = (EditText)v.findViewById(R.id.et_feedback_email);
		etExplain = (EditText)v.findViewById(R.id.et_feedback_detail);
		rlProcess = (RelativeLayout)v.findViewById(R.id.rl_feedback_process);
	
		etEmail.setHint("john@gmail.com");
		etEmail.setText(prefs.getString(PREFS_CODE_EMAIL, ""));
		rlProcess.setVisibility(View.GONE);
	}

	@Override
	public String getTitle() {
		return "FeedBack";
	}

	@Override
	public String getDoneName() {
		return "Send!";
	}

	@Override
	public boolean onDoneClicked() {

		String spath = path.replace("|", "(OR)");
		String shash = hash.replace("|", "(OR)");
		String sname = name.replace("|", "(OR)");
		String semail = etEmail.getText().toString().replace("|", "(OR)");
		String sdetail = etExplain.getText().toString().replace("|", "(OR)");
		
		String jsonRaw = parent.getMenu(String.format("%s|%s|%s|%s|%s|%s", stack, spath, shash, sname, semail, sdetail));

		String sdo = "FAILED";
		try{
			JSONObject jData = new JSONObject(jsonRaw);
			sdo = jData.getString("DO");
		}catch(Exception e){
			
		}
		
		if("SUCCESS".equals(sdo)){	
			Toast.makeText(mAct, "Thank you for your feedback\nI will check this ASAP", Toast.LENGTH_SHORT).show();
			mAct.pop(true);
			mAct.pop(true);
		}else{			
			Toast.makeText(mAct, "Send failed, please check network", Toast.LENGTH_SHORT).show();
		}
		
		return false;
	}
}



