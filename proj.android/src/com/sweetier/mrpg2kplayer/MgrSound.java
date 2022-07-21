package com.sweetier.mrpg2kplayer;

import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;

public class MgrSound {
	
	final static String TAG = "r2k_manager_sound";
	
    //Sound
	private int STREAM_MAX = 50;
	private int BUFFERLIFETIME = 15000;

	private String 		StreamName[] 	= new String[STREAM_MAX];
	private int 			StreamID[] 		= new int[STREAM_MAX];
	private long 		StreamEndTime[] = new long[STREAM_MAX];
	
	private SoundPool spSub;

	public void init(){
		spSub = new SoundPool(STREAM_MAX, AudioManager.STREAM_MUSIC, 0 );
		for(int i=0; i<STREAM_MAX; i++)
			StreamID[i] = -1;
	}
	
	public void chk(){
		long currentTime = System.currentTimeMillis();
    	int i;
    	for(i=0; i<STREAM_MAX; i++){
    		if(StreamID[i] == -1)
    			continue;
    		
    		if(StreamEndTime[i] > currentTime)
    			continue;
    		
    		try{
				spSub.stop(StreamID[i]);
				spSub.unload(StreamID[i]);
				Log.e("SndSoundMgr", "released : " + StreamName[i]);
			}catch(Exception e){
				
			}
			StreamID[i] = -1;
    	}
	}
	
    public void play(String path, int vol, int tempo, int bal){
    	Log.i("SndSoundMgr", "setPlaySound : " + path);
    	final int fvol = vol;
    	final int ftempo = tempo;
    	final int fbal = bal;
		try{
			long currentTime = System.currentTimeMillis();
	    	int i;
	    	for(i=0; i<STREAM_MAX; i++){
	    		if(StreamID[i] == -1)
	    			continue;
	    		
	    		if(StreamName[i].equals(path)){
	    			playOne(StreamID[i], vol, tempo, bal);
	    			StreamEndTime[i] 	= currentTime + BUFFERLIFETIME;
	    			return;
	    		}
	    	}
	    	
	    	for(i=0; i<STREAM_MAX; i++){
	    		if(StreamID[i] == -1)
	    			break;
	    	}
	    	
	    	if(i == STREAM_MAX)
	    		return;
	    	
	    	final int sid = spSub.load(path , 1);

			StreamName[i] 		= path;
			StreamID[i] 		= sid;
			StreamEndTime[i] 	= currentTime + BUFFERLIFETIME;

			spSub.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
			    @Override
			    public void onLoadComplete(SoundPool soundPool, int mySoundId, int status) {
			    	playOne(sid, fvol, ftempo, fbal);
			    }
			});
		}catch(Exception e){
			Log.e("SndSoundMgr", "setPlaySound : " + e);
		}
    }
    
    public void stop(){
		try{
			int i;
	    	for(i=0; i<STREAM_MAX; i++){
	    		if(StreamID[i] == -1)
	    			continue;
	    		
				spSub.stop(StreamID[i]);
	    	}
		}catch(Exception e){
			Log.e("SndSoundMgr", "setStopSound : " + e);
		}
    }

    private void playOne(int streamID, int vol, int tempo, int bal){
		float leftVolume	= ((bal<=100)?1.0f:(200.0f-bal)/100.0f) * (vol/100.0f);
		float rightVolume	= ((bal>=100)?1.0f:(bal/100.0f))	 	* (vol/100.0f);
		float rate = tempo/100.0f;

    	spSub.play(streamID, leftVolume, rightVolume, 0, 0, rate);//gM.tempo
	}
    
    public void pause(){
    	long currentTime = System.currentTimeMillis();
		for(int i=0; i<STREAM_MAX; i++){
    		if(StreamID[i] == -1)
    			continue;

    		try{
    			spSub.pause(StreamID[i]);
    		}catch(Exception e){
    			Log.e("SndSoundMgr", "doPause : " + e);
    		}
			StreamEndTime[i] = currentTime - StreamEndTime[i];
		}
    }

    public void resume(){
    	long currentTime = System.currentTimeMillis();
		for(int i=0; i<STREAM_MAX; i++){
    		if(StreamID[i] == -1)
    			continue;

    		try{
    			spSub.resume(StreamID[i]);
    		}catch(Exception e){
    			Log.e("SndSoundMgr", "doResume : " + e);
    		}
			StreamEndTime[i] = currentTime + StreamEndTime[i];
		}
    }
    
    public void release(){
    	try{
    		for(int i=0; i<STREAM_MAX; i++){
        		if(StreamID[i] == -1)
        			continue;

        		spSub.unload(StreamID[i]);
        		StreamID[i] = -1;
    		}
		}catch(Exception e){
			Log.e("SndSoundMgr", "doRelease : " + e);
		}
    }
}
