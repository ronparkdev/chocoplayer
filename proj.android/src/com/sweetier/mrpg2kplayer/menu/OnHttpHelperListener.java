package com.sweetier.mrpg2kplayer.menu;

import org.json.JSONObject;

public abstract class OnHttpHelperListener {
	public abstract void onReceived(JSONObject data);
	public abstract void onError(int resultCode);
}
