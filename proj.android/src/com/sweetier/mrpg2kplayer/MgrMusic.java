package com.sweetier.mrpg2kplayer;

import android.media.MediaPlayer;
import android.util.Log;

public class MgrMusic {

	final static String TAG = "r2k_manager_music";
	
	//Music
	private MediaPlayer mpApp = null;
	private String mStr;
	
	private int mFadeFrameLeft, mFadeFrameTotal;
	
	enum MusicFade{
		FadeNone,
		FadeIn,
		FadeOut
	};
	
	private MusicFade mFadeUsed; // 1:Fadein 2:Fadeout
	
	private float mLVolume, mRVolume;

	public void init(){
		mStr = "";
		mFadeUsed = MusicFade.FadeNone;
		mFadeFrameLeft = mFadeFrameTotal = 0;
	}
	
	public void chk(int framedelta){
		if(mpApp != null && mFadeUsed != MusicFade.FadeNone){
			try{
				if(mFadeFrameLeft <= 0 || mFadeFrameTotal <= 0)
					return;
				
				mFadeFrameLeft -= framedelta;
				
				if(mFadeFrameLeft > 0){
					float p = ((float)(mFadeFrameTotal-mFadeFrameLeft)/mFadeFrameTotal);
					
					if(mFadeUsed == MusicFade.FadeOut)
						p = (1.0f-p);
					
					mpApp.setVolume(mLVolume*p, mRVolume*p);
				}else{
					if(mFadeUsed == MusicFade.FadeIn)
						mpApp.setVolume(mLVolume, mRVolume);
					else
						stop();

					mFadeUsed = MusicFade.FadeNone;
				}
			}catch(Exception e){
				
			}
		}
	}
	
	public void play(String path, int fadein, int vol, int tempo, int bal){

    	if(mStr != null && mStr.equals(path))
    		return;
    	
    	if(mpApp != null){
			try{
				mpApp.stop();
				mpApp.release();
				mStr = null;
			}catch(Exception e){
				
			}
		}
    	
    	try {
        	mpApp = new MediaPlayer();
			mpApp.setDataSource(path);
	    	
			int i = path.lastIndexOf('/');
			
			if(i + 1 < path.length() && path.charAt(i+1) == 'J')
				mpApp.setLooping(false);
			else
				mpApp.setLooping(true);

	    	mLVolume	= ((bal<=100)?1.0f:(200.0f-bal)/100.0f) * (vol/100.0f);
	    	mRVolume	= ((bal>=100)?1.0f:(bal/100.0f))	 	* (vol/100.0f);

			mpApp.setVolume(mLVolume, mRVolume);
			mpApp.prepare();
			mpApp.start();	
			
			if(fadein>0){
				mFadeUsed = MusicFade.FadeIn;
				mFadeFrameLeft = mFadeFrameTotal = fadein * 60 / 1000;
			}else{
				mFadeUsed = MusicFade.FadeNone;
			}

			mStr = path;
		} catch (Exception e) {
			e.printStackTrace();
		}
    	
    }
    
    public void fadeout(int time){
		mFadeUsed = MusicFade.FadeOut;
		mFadeFrameLeft = mFadeFrameTotal = time * 60 / 1000;
    }
    
    public void stop(){
		if(mpApp != null){
			try{
				mpApp.pause();
				mpApp.stop();
				mpApp.release();
			}catch(Exception e){
				e.printStackTrace();
			}

			mStr = null;
			mpApp = null;
		}
    }
    
    public void pause(){
    	try{
			if(mpApp != null && mpApp.isPlaying())
				mpApp.pause();
		}catch(Exception e){
			Log.e("SndSoundMgr", "doResume : " + e);
		}
    }

    public void resume(){
		try{
			if(mpApp != null)
				mpApp.start();
		}catch(Exception e){
			Log.e("SndSoundMgr", "doResume : " + e);
		}
    }
}
