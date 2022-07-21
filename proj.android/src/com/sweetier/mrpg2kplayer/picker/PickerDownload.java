package com.sweetier.mrpg2kplayer.picker;

import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;

import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

class PickerDownload extends AsyncTask<String, String, String> {

	PickerDownloadListener mListener;

	public void setListener(PickerDownloadListener listener) {
		mListener = listener;
	}

	/**
	 * Before starting background thread Show Progress Bar Dialog
	 * */
	@Override
	protected void onPreExecute() {
		super.onPreExecute();
	}

	/**
	 * Downloading file in background thread
	 * */
	@Override
	protected String doInBackground(String... f_url) {
		int count;
		try {
			URL url = new URL(f_url[1]);
			URLConnection conection = url.openConnection();
			conection.connect();
			// getting file length
			int lenghtOfFile = conection.getContentLength();

			// input stream to read file - with 8k buffer
			InputStream input = new BufferedInputStream(url.openStream(), 8192);

			// Output stream to write file
			OutputStream output = new FileOutputStream(f_url[0]);

			byte data[] = new byte[1024];

			long total = 0;

			while ((count = input.read(data)) != -1) {
				total += count;
				// publishing the progress....
				// After this onProgressUpdate will be called
				if(mListener != null)
					mListener.onProgress(total, lenghtOfFile);
				//publishProgress("" + total, "" + lenghtOfFile);

				// writing data to file
				output.write(data, 0, count);
				
				if(isCancelled())
					return null;
			}

			// flushing output
			output.flush();

			// closing streams
			output.close();
			input.close();

			if(mListener != null)
				mListener.onSuccess();
			
		} catch (Exception e) {
			Log.e("Error: ", e.getMessage());
			
			if(mListener != null)
				mListener.onFail(e);
		}

		return null;
	}

	/**
	 * Updating progress bar
	 * */
	protected void onProgressUpdate(String... progress) {
		// setting progress percentage
	}

	/**
	 * After completing background task Dismiss the progress dialog
	 * **/
	@Override
	protected void onPostExecute(String file_url) {

	}

}