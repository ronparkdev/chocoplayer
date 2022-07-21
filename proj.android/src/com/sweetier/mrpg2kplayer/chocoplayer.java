/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.sweetier.mrpg2kplayer;

import java.io.File;
import java.io.IOException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import com.sweetier.mrpg2kplayer.menu.MgrMenu;
import com.sweetier.mrpg2kplayer.picker.PickerMain;

public class chocoplayer extends Cocos2dxActivity {
	
	private static MgrMovie g_movie = new MgrMovie();
	private static MgrMusic g_music = new MgrMusic();
	private static MgrSound g_sound = new MgrSound();
	private static MgrAdvertise g_ad = new MgrAdvertise();
	private static MgrInapp g_inapp = new MgrInapp();
	public static MgrMenu	g_menu = new MgrMenu();
	
	private static PickerMain g_picker = new PickerMain();

	
	/*-----------------------------------------------*/
	/*-------------------In Game---------------------*/
	/*-----------------------------------------------*/

	private static chocoplayer g_pInstance = null;
	private static Cocos2dxGLSurfaceView g_glSurfaceView;

	static {
		System.loadLibrary("cocos2dcpp");
	}
	
	public static chocoplayer getInstance(){
		return g_pInstance;
	}
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		g_pInstance = this;
	
		if((g_isVertical = g_menu.isVertical()))
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

		g_ad.onCreate();
		g_sound.init();
		g_inapp.onCreate(this, g_ad);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}
	
	public Cocos2dxGLSurfaceView onCreateView() {
		g_glSurfaceView = new Cocos2dxGLSurfaceView(this);
		
		// chocoplayer should create stencil buffer
		g_glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		return g_glSurfaceView;
	}

	@Override
	protected void onResume() {
		g_ad.onResume();
		super.onResume();
	}

	@Override
	protected void onPause() {
		g_ad.onPause();
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		g_ad.onDestroy();
		g_inapp.onDestroy();
		super.onDestroy();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		
		g_inapp.onActivityResult(requestCode, resultCode, data);
		g_picker.onActivityResult(requestCode, resultCode, data);
	}

	public static String chkPath(String path) {
		if (path == null || path.length() == 0)
			return "";

		if (path.charAt(path.length() - 1) == '/')
			return path;
		else
			return path + '/';
	}

	public static String getGamesPath() {
		String inner_path = "", extern_path = "", rtp_path = "", tmp_path = "";

		inner_path = chkPath(Environment.getExternalStorageDirectory().getAbsolutePath());
		extern_path = chkPath(externSdCard.getMicroSDCardDirectory());

		if (inner_path.length() > 0)
			inner_path += "RPGL/";

		if (extern_path.length() > 0)
			extern_path += "RPGL/";
		
		g_picker.setPath(inner_path);

		String thePackageName = g_pInstance.getPackageName();
		int thePackageVer = 84;// info.versionCode;
		rtp_path = "/mnt/sdcard/Android/obb/" + thePackageName + "/main." + thePackageVer + "." + thePackageName + ".obb";

		Log.i("chocoplayer", "RTP_PATH : " + rtp_path);

		// tmp_path = m_act.getCacheDir().getAbsolutePath();

		tmp_path = inner_path + "TEMP/";

		return inner_path + '#' + extern_path + '#' + rtp_path + '#' + tmp_path;
	}

	/*-----------------------------------------------*/
	/*--------------Screen Orientation---------------*/
	/*-----------------------------------------------*/
	
	private static boolean g_isVertical;
	
	public native boolean isVertical();
	
	public static boolean isLastVertical(){
		return g_isVertical;
	}
	
	/*-----------------------------------------------*/
	/*-----------------Music/Sound-------------------*/
	/*-----------------------------------------------*/
	
	public static void doCheck(int framedelta) {
		g_music.chk(framedelta);
		g_sound.chk();
	}

	public static void doPlaySound(String path, int vol, int tempo, int bal) {
		g_sound.play(path, vol, tempo, bal);
	}

	public static void doStopSound() {
		g_sound.stop();
	}

	public static void doPlayMusic(String path, int fadein, int vol, int tempo, int bal) {
		g_music.play(path, fadein, vol, tempo, bal);
	}

	public static void doFadeoutMusic(int time) {
		g_music.fadeout(time);
	}

	public static void doStopMusic() {
		g_music.stop();
	}
	
	public static void doPause() {
		g_music.pause();
		g_sound.pause();
	}

	public static void doResume() {
		g_music.resume();
		g_sound.resume();
	}

	public static void doRelease() {
		g_music.stop();
		g_sound.stop();
		g_sound.release();
	}

	/*-----------------------------------------------*/
	/*-------------------Advertise-------------------*/
	/*-----------------------------------------------*/
	
	public static void doShowAdLine() {
		if (!g_inapp.isPro())
			g_ad.showLine();
	}

	public static void doHideAdLine() {
		g_ad.hideLine();
	}

	public static void doShowAdBig() {
		if (!g_inapp.isPro())
			g_ad.showBig();
	}

	public static boolean isPro() {
		return g_inapp.isPro();
	}

	public static void doBuyPro() {
		g_inapp.Buy();
	}

	/*-----------------------------------------------*/
	/*----------------ETC Functions------------------*/
	/*-----------------------------------------------*/
	
	public final static int INCOMING_CMD_OPEN_VKEY 			= 0;
	public final static int INCOMING_CMD_GET_GAMES_INFO 	= 1;
	public final static int INCOMING_CMD_GET_SUPPORT_MENUS 	= 2;
	
	public final static int INCOMING_CMD_CLOSE_GAME_ADD 	= 50;
	
	public final static int INCOMING_CMD_GOTO_TITLE 		= 100;
	public final static int INCOMING_CMD_GOTO_GAMELIST 		= 101;
	public final static int INCOMING_CMD_GOTO_SAVE 			= 102;
	public final static int INCOMING_CMD_QUIT_APP 			= 103;
	
	public native String doAction(int type);

	public static String getModel(){
		return Build.MODEL;
	}
	
	public static String getUserID() {
		return MgrSecure.getUserID();
	}
	
	public static String getAppVersion(){
		String version = "";
		try {
		   PackageInfo i = g_pInstance.getPackageManager().getPackageInfo(g_pInstance.getPackageName(), 0);
		   version = i.versionName;
		} catch(NameNotFoundException e) { }
		
		return version;
	}
	
	public static void doOpenMenu(String stack){
		g_menu.showMenu(null, stack);
	}
	
	public static void doOpenGameAdd(){
		g_menu.showGameAddMenu(g_picker);
	}
	
	public static void doOpenWeb(String url) {
		if (g_glSurfaceView == null)
			return;

		Intent browse = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		browse.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		g_glSurfaceView.getContext().startActivity(browse);
	}

	public static void doOpenMovie(String path) {
		if (g_glSurfaceView == null)
			return;

		g_movie.show(path);
	}
	
	public static void doDeleteFolder(String path){
		try{
			File f = new File(path);
			f.delete();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}
