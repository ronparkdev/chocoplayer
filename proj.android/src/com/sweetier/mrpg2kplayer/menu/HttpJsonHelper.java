package com.sweetier.mrpg2kplayer.menu;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Iterator;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

import org.json.JSONObject;

import android.os.AsyncTask;

public class HttpJsonHelper extends AsyncTask<Object, Void, JSONObject>{
 
	//url		:	https://google.com:1234/dir/helloworld.jsp 와 같이 프로토콜, 포트, 호스트, 파일명이 포함된 전체 URL
	//isPost	:	true일 경우		request의 값이 Json형식인 POST로 전송
	//				false일 경우	request의 값이 주소의 뒤에 붙어 GET으로 전송
	//				(단 GET을 이용할 경우 request 아래에는 String만 존재 해야하며, JSONObject 또는 JSONArray를 사용할 경우 비정상 작동을 할 수 있다.)
	//request	:	BSONObject 형식의 요청값
	//timeout	:	응답시간 설정(ms 단위, 기본값 60초)
	//listener	:	BSON 형태의 결과값이 파싱이 되어 이 콜백에 BSONObject 형식으로 반환

	private final static int DEFAULT_TIMEOUT = 60000;//60초
	
	public static void run(String url, boolean isPost, JSONObject request, OnHttpHelperListener listener){
		new HttpJsonHelper().execute(url, isPost, request, DEFAULT_TIMEOUT, listener);
	}
	
	public static void run(String url, boolean isPost, JSONObject request, int timeout, OnHttpHelperListener listener){
		new HttpJsonHelper().execute(url, isPost, request, timeout, listener);
	}
	
	OnHttpHelperListener mListener;
	int resultCode;
 
	@Override
	protected JSONObject doInBackground(Object... params) {
		try {
			//값 초기화
			String sUrl = (String) params[0];
			boolean isPost = (Boolean) params[1];
			JSONObject sBody = (JSONObject) params[2];
			int timeout = (Integer) params[3];
			mListener = (OnHttpHelperListener) params[4];
			
			URL url = new URL(sUrl);
			boolean isSequre = "https".equals(url.getProtocol());
			resultCode = -1;
			
			//컨넥션 오픈
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			
			//전송방법 선택 및 GET이라면 파라메터 입력
			if(isPost){
				conn.setRequestMethod("POST");
			}else{
				conn.setRequestMethod("GET");
				boolean isFirst = true;
				
				StringBuilder sb = new StringBuilder(sUrl);
				
				String key, val;
				Iterator<String> keys = sBody.keys();
				while(keys.hasNext()){
					key = keys.next();
					val = (String)sBody.get(key);
					
					if(val == null || val.isEmpty())
						continue;
					
					sb.append(isFirst?'?':'&');
					sb.append(key);
					sb.append('=');
					sb.append(sBody.get(key));
					
					isFirst = false;
				}
				
				url = new URL(sb.toString());
			}
			
			//기타 설정
			conn.setConnectTimeout(timeout);
			conn.setAllowUserInteraction(true);
			conn.setUseCaches(false);
			conn.setDoInput(true);
 
			conn.setRequestProperty("Cache-Control", "no-cache");
			conn.setRequestProperty("Content-Type", "application/json");
			//conn.setRequestProperty("Accept", "application/json");
			conn.setRequestProperty("Accept", "*");
			
			//https 통신이라면 모든 host를 허용하고(서버에서 host인증을 지원안해서..) 기본 통신 암호화(TSL)를 설정한다
			if(isSequre){
				TrustManager[] trustAllCerts = new TrustManager[] { new X509TrustManager() {
					public java.security.cert.X509Certificate[] getAcceptedIssuers() {return null;}
					public void checkClientTrusted(java.security.cert.X509Certificate[] certs, String authType) {}
					public void checkServerTrusted(java.security.cert.X509Certificate[] certs, String authType) {}
				} };
				
				SSLContext sslContext = SSLContext.getInstance("SSL");
				sslContext.init(null, trustAllCerts, null);
				
				HttpsURLConnection connS = (HttpsURLConnection)conn;
				connS.setSSLSocketFactory(sslContext.getSocketFactory());
				connS.setHostnameVerifier(new HostnameVerifier() {
					public boolean verify(String hostname, SSLSession session) {return true;}
				});
			}
 
			//POST면 request값을 JSON으로 변환하여 반환한다.
			if(isPost){
				conn.setDoOutput(true);
				OutputStreamWriter osw = new OutputStreamWriter(conn.getOutputStream());
				osw.write(sBody.toString());
				osw.flush();
				osw.close();
			}
 
			//정상적인 통신이 되었다면 BSON을 파싱한다
			resultCode = conn.getResponseCode();
			if (resultCode == HttpURLConnection.HTTP_OK) {
				InputStream in = conn.getInputStream();
				JSONObject result = new JSONObject(getStringFromInputStream(in));

				return result;
			}
 
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		return null;
	}
 
	@Override
	protected void onPostExecute(JSONObject result) {
		if (mListener != null){
			if (result != null) 
				mListener.onReceived(result);
			else 
				mListener.onError(resultCode);
		}
	}
	
	private static String getStringFromInputStream(InputStream is) {

		BufferedReader br = null;
		StringBuilder sb = new StringBuilder();

		String line;
		try {

			br = new BufferedReader(new InputStreamReader(is));
			while ((line = br.readLine()) != null) {
				sb.append(line);
			}

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (br != null) {
				try {
					br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		return sb.toString();

	}
}
