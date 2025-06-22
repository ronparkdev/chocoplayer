package com.sweetier.mrpg2kplayer;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.WindowManager;
import org.cocos2dx.lib.Cocos2dxActivity;

import com.sweetier.mrpg2kplayer.menu.MgrMenu;
import com.sweetier.mrpg2kplayer.picker.PickerMain;

import java.io.File;


public class ChocoplayerActivity extends Cocos2dxActivity {

    private static MgrMovie g_movie = new MgrMovie();
    private static MgrMusic g_music = new MgrMusic();
    private static MgrSound g_sound = new MgrSound();
    private static MgrAdvertise g_ad = new MgrAdvertise();
    private static MgrInapp g_inapp = new MgrInapp();
    public static MgrMenu g_menu = new MgrMenu();
    private static PickerMain g_picker = new PickerMain();

    private static ChocoplayerActivity g_pInstance = null;

    public native boolean isVertical();
    public native String doAction(int type);

    static {
        System.loadLibrary("cocos2dcpp");
    }

    public static ChocoplayerActivity getInstance() {
        return g_pInstance;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        g_pInstance = this;

        // For v3, EGL config is usually not set this way.
        // If stencil buffer is needed, it's often a renderer option in C++
        // or requires a custom SurfaceView factory.

        if (isVertical()) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            g_isVerticalState = true;
        } else {
            // Set to landscape if not vertical, matching original behavior (sensorLandscape from manifest)
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
            g_isVerticalState = false;
        }

        // Initialize managers
        // These managers (MgrAdvertise, MgrInapp, MgrSound, MgrMusic) might need updates
        // to work with new SDKs (especially Ads and IAP) and the new Activity lifecycle.
        // They also need to correctly handle the Activity context.
        if(g_ad != null) g_ad.init(this);
        if(g_sound != null) g_sound.init(this);
        if(g_inapp != null) g_inapp.onCreate(this, g_ad);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }


    @Override
    protected void onResume() {
        super.onResume();
        if (g_ad != null) g_ad.onResume();
        // Assuming these managers have their own resume logic
        if (g_music != null) g_music.resume();
        if (g_sound != null) g_sound.resume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (g_ad != null) g_ad.onPause();
        // Assuming these managers have their own pause logic
        if (g_music != null) g_music.pause();
        if (g_sound != null) g_sound.pause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (g_ad != null) g_ad.onDestroy();
        if (g_inapp != null) g_inapp.onDestroy();
        if (g_sound != null) g_sound.release();
        g_pInstance = null;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (g_inapp != null) g_inapp.onActivityResult(requestCode, resultCode, data);
        if (g_picker != null) g_picker.onActivityResult(requestCode, resultCode, data);
    }

    private static String chkPath(String path) {
        if (path == null || path.length() == 0) return "";
        return path.endsWith("/") ? path : path + "/";
    }

    public static String getGamesPath() {
        if (g_pInstance == null) return "null#null#null#null";

        String inner_path_default = "";
        File primaryExternalStorage = g_pInstance.getExternalFilesDir(null);
        if (primaryExternalStorage != null) {
            inner_path_default = chkPath(primaryExternalStorage.getAbsolutePath());
        } else {
            // Fallback to internal storage if external is not available/mounted
            inner_path_default = chkPath(g_pInstance.getFilesDir().getAbsolutePath());
        }
        inner_path_default += "RPGL/";


        // externSdCard.getMicroSDCardDirectory() is non-standard and platform-dependent.
        // Modern Android makes accessing arbitrary secondary storage paths difficult.
        // This part would need significant rework for compatibility and permissions.
        // For now, returning an empty string for extern_path.
        String extern_path = "";

        if (g_picker != null) g_picker.setPath(inner_path_default);

        // OBB path construction might also need adjustment based on current Android practices.
        String rtp_path = "";
        // Example: Could use g_pInstance.getObbDir() if targeting API level that supports it well.
        // String thePackageName = g_pInstance.getPackageName();
        // int thePackageVer = getAppVersionCode(); // Using dynamic version code
        // if (thePackageVer > 0) {
        //    rtp_path = chkPath(g_pInstance.getObbDir().getAbsolutePath()) + "main." + thePackageVer + "." + thePackageName + ".obb";
        // }


        String tmp_path = chkPath(g_pInstance.getCacheDir().getAbsolutePath()) + "TEMP/";

        return inner_path_default + '#' + extern_path + '#' + rtp_path + '#' + tmp_path;
    }

    private static boolean g_isVerticalState;

    public static boolean isLastVertical(){
        return g_isVerticalState;
    }

    // This method could be called from C++ after isVertical() to update Activity orientation
    // if the C++ side determines the orientation dynamically after initial load.
    public static void setScreenOrientation(boolean isVertical) {
        g_isVerticalState = isVertical;
        if (g_pInstance != null) {
            final int orientation = isVertical ? ActivityInfo.SCREEN_ORIENTATION_PORTRAIT : ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
            g_pInstance.runOnUiThread(() -> g_pInstance.setRequestedOrientation(orientation));
        }
    }

    public static void doCheck(int framedelta) {
        if (g_music != null) g_music.chk(framedelta);
        if (g_sound != null) g_sound.chk();
    }

    public static void doPlaySound(String path, int vol, int tempo, int bal) {
        if (g_sound != null) g_sound.play(path, vol, tempo, bal);
    }

    public static void doStopSound() {
        if (g_sound != null) g_sound.stop();
    }

    public static void doPlayMusic(String path, int fadein, int vol, int tempo, int bal) {
        if (g_music != null) g_music.play(path, fadein, vol, tempo, bal);
    }

    public static void doFadeoutMusic(int time) {
        if (g_music != null) g_music.fadeout(time);
    }

    public static void doStopMusic() {
        if (g_music != null) g_music.stop();
    }

    public static void doPauseAudio() {
        if (g_music != null) g_music.pause();
        if (g_sound != null) g_sound.pause();
    }

    public static void doResumeAudio() {
        if (g_music != null) g_music.resume();
        if (g_sound != null) g_sound.resume();
    }

    public static void doReleaseAudio() {
        if (g_music != null) g_music.stop();
        if (g_sound != null) {
            g_sound.stop();
            g_sound.release();
        }
    }

    public static void doShowAdLine() {
        if (g_inapp != null && !g_inapp.isPro() && g_ad != null && g_pInstance != null) {
            g_pInstance.runOnUiThread(() -> g_ad.showLine());
        }
    }

    public static void doHideAdLine() {
        if (g_ad != null && g_pInstance != null) {
            g_pInstance.runOnUiThread(() -> g_ad.hideLine());
        }
    }

    public static void doShowAdBig() {
        if (g_inapp != null && !g_inapp.isPro() && g_ad != null && g_pInstance != null) {
            g_pInstance.runOnUiThread(() -> g_ad.showBig());
        }
    }

    public static boolean isPro() {
        return g_inapp != null && g_inapp.isPro();
    }

    public static void doBuyPro() {
        if (g_inapp != null && g_pInstance != null) {
             g_pInstance.runOnUiThread(() -> g_inapp.Buy());
        }
    }

    public static String getModelName(){
        return Build.MODEL;
    }

    public static String getUserID() {
        // MgrSecure.getUserID() should be checked. Assuming it might need context.
        return (g_pInstance != null) ? MgrSecure.getUserID(g_pInstance) : "";
    }

    public static String getAppVersionName(){
        if (g_pInstance == null) return "";
        try {
            PackageInfo i = g_pInstance.getPackageManager().getPackageInfo(g_pInstance.getPackageName(), 0);
            return i.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            Log.e("ChocoplayerActivity", "Error getting version name", e);
            return "";
        }
    }

    public static int getAppVersionCode(){
        if (g_pInstance == null) return 0;
        try {
            PackageInfo i = g_pInstance.getPackageManager().getPackageInfo(g_pInstance.getPackageName(), 0);
            return i.versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            Log.e("ChocoplayerActivity", "Error getting version code", e);
            return 0;
        }
    }

    public static void doOpenMenu(String stack) {
        if (g_menu != null && g_pInstance != null) {
            g_pInstance.runOnUiThread(() -> g_menu.showMenu(g_pInstance, stack));
        }
    }

    public static void doOpenGameAdd() {
        if (g_menu != null && g_picker != null && g_pInstance != null) {
            g_pInstance.runOnUiThread(() -> g_menu.showGameAddMenu(g_pInstance, g_picker));
        }
    }

    public static void doOpenWeb(String url) {
        if (g_pInstance == null) return;
        try {
            Intent browse = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
            g_pInstance.startActivity(browse);
        } catch (Exception e) {
            Log.e("ChocoplayerActivity", "Error opening web: " + url, e);
        }
    }

    public static void doOpenMovie(String path) {
        if (g_movie != null && g_pInstance != null) {
             g_pInstance.runOnUiThread(() -> g_movie.show(g_pInstance, path));
        }
    }

    public static void doDeleteFolder(String path) {
        if (path == null || path.isEmpty()) return;
        try {
            File f = new File(path);
            deleteRecursive(f);
        } catch (Exception e) {
            Log.e("ChocoplayerActivity", "Error deleting folder: " + path, e);
        }
    }

    private static void deleteRecursive(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory()) {
            File[] children = fileOrDirectory.listFiles();
            if (children != null) {
                for (File child : children) {
                    deleteRecursive(child);
                }
            }
        }
        fileOrDirectory.delete();
    }
}
