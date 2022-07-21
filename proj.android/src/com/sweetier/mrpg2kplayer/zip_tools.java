package com.sweetier.mrpg2kplayer;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import android.util.Base64;

public class zip_tools {

    static final int BUFFER_SIZE = 1024;
    
	public static void zip(String[] files, String zipFile) throws IOException {
	    BufferedInputStream origin = null;
	    ZipOutputStream out = new ZipOutputStream(new BufferedOutputStream(new FileOutputStream(zipFile)));
	    try { 
	        byte data[] = new byte[BUFFER_SIZE];

	        for (int i = 0; i < files.length; i++) {
	            FileInputStream fi = new FileInputStream(files[i]);    
	            origin = new BufferedInputStream(fi, BUFFER_SIZE);
	            try {
	                ZipEntry entry = new ZipEntry(files[i].substring(files[i].lastIndexOf("/") + 1));
	                out.putNextEntry(entry);
	                int count;
	                while ((count = origin.read(data, 0, BUFFER_SIZE)) != -1) {
	                    out.write(data, 0, count);
	                }
	            }
	            finally {
	                origin.close();
	            }
	        }
	    }
	    finally {
	        out.close();
	    }
	}
    
    public static boolean unzip(String zipFile, String location) throws IOException {
        try {
            File f = new File(location);
            if(!f.isDirectory()) {
                f.mkdirs();
            }
            ZipInputStream zin = new ZipInputStream(new FileInputStream(zipFile));
            try {
                ZipEntry ze = null;
                while ((ze = zin.getNextEntry()) != null) {
                    String path = location + ze.getName();

                    if (ze.isDirectory()) {
                        File unzipFile = new File(path);
                        if(!unzipFile.isDirectory()) {
                            unzipFile.mkdirs();
                        }
                    }
                    else {
                        FileOutputStream fout = new FileOutputStream(path, false);
                        try {
                            for (int c = zin.read(); c != -1; c = zin.read()) {
                                fout.write(c);
                            }
                            zin.closeEntry();
                        }
                        finally {
                            fout.close();
                        }
                    }
                }
            }finally {
                zin.close();
            }
        }
        catch (Exception e) {
        	return false;
        	//e.printStackTrace();
        }
        return true;
    }

    public static String Decrypt(String text, String key) throws Exception
    {
              Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
              byte[] keyBytes= new byte[16];
              byte[] b= key.getBytes("UTF-8");
              int len= b.length;
              if (len > keyBytes.length) len = keyBytes.length;
              System.arraycopy(b, 0, keyBytes, 0, len);
              SecretKeySpec keySpec = new SecretKeySpec(keyBytes, "AES");
              IvParameterSpec ivSpec = new IvParameterSpec(keyBytes);
              cipher.init(Cipher.DECRYPT_MODE,keySpec,ivSpec);

              byte [] results = cipher.doFinal(Base64.decode(text, Base64.DEFAULT));
              return new String(results,"UTF-8");
    }

    public static String Encrypt(String text, String key) throws Exception
    {
              Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
              byte[] keyBytes= new byte[16];
              byte[] b= key.getBytes("UTF-8");
              int len= b.length;
              if (len > keyBytes.length) len = keyBytes.length;
              System.arraycopy(b, 0, keyBytes, 0, len);
              SecretKeySpec keySpec = new SecretKeySpec(keyBytes, "AES");
              IvParameterSpec ivSpec = new IvParameterSpec(keyBytes);
              cipher.init(Cipher.ENCRYPT_MODE,keySpec,ivSpec);

              byte[] results = cipher.doFinal(text.getBytes("UTF-8"));
              return Base64.encodeToString(results, Base64.DEFAULT);
    }
}
