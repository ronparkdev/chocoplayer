package com.sweetier.mrpg2kplayer.picker;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import com.sweetier.mrpg2kplayer.menu.SubMenuDownload;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.widget.Toast;

public class PickerFolder implements PickerInterface{

	private final static String TAG = "PickerFolder";
    
	private Activity mActivity;
	private PickerMain mParent;

	@Override
	public void init(Activity activity, PickerMain parent) {
		mActivity = activity;
		mParent = parent;
		
		connect();
	}

	@Override
	public void connect() {
		
	    Intent intent = new Intent(Intent.ACTION_GET_CONTENT); 
	    intent.setType("file/*"); 
	    intent.addCategory(Intent.CATEGORY_OPENABLE);
	    
	    mActivity.startActivityForResult(
                Intent.createChooser(intent, "Select zip file"),
                PickerMain.REQUEST_CODE_SELECT);
	}

	@Override
	public void initReceive(Intent data, SubMenuDownload view) {
		
		if(data == null)
			return;
		
		Uri uri = data.getData();
		
		String[] projection = { MediaStore.Images.Media.DATA };
		
		try{
			Cursor cur = mActivity.managedQuery(uri, projection, null, null, null);
			cur.moveToFirst();
			String path = cur.getString(cur.getColumnIndex(MediaStore.Images.Media.DATA));
	
			File file = new File(path);

			view.doInit(file.getName());
			
			copyFile(file, mParent.getPath() + file.getName());
	
			view.doFinish();
			
			Toast.makeText(mActivity, path + "=>" + mParent.getPath() + file.getName(), Toast.LENGTH_LONG).show();			
		}catch(Exception e){
			view.doFail(e);
		}
	}
	
    private boolean copyFile(File file , String save_file){
        boolean result;
        if(file!=null&&file.exists()){
            try {
                FileInputStream fis = new FileInputStream(file);
                FileOutputStream newfos = new FileOutputStream(save_file);
                int readcount=0;
                byte[] buffer = new byte[1024];
                while((readcount = fis.read(buffer,0,1024))!= -1){
                    newfos.write(buffer,0,readcount);
                }
                newfos.close();
                fis.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            result = true;
        }else{
            result = false;
        }
        return result;
    }

	@Override
	public void startReceive(SubMenuDownload mView){
		
	}
	
	@Override
	public void stopReceive() {

	}

}
