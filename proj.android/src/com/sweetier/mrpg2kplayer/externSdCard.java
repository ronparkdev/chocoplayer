package com.sweetier.mrpg2kplayer;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import android.os.Environment;
import android.os.StatFs;

public class externSdCard {
	private static final long ONE_GIGABYTE = 1024 * 1024 * 1024;

	/**
	 * 추가 적인 외부 확장 SDCard path 얻기.
	 * 조건으로 걸러진 최종 String List size가 1이 아니면 null 리턴
	 * @return
	 */
	public static String getMicroSDCardDirectory() {
	    List<String> mMounts = readMountsFile();
	    List<String> mVold = readVoldFile();
	     
	    for (int i=0; i < mMounts.size(); i++) {
	        String mount = mMounts.get(i);
	         
	        if (!mVold.contains(mount)) {
	            mMounts.remove(i--);
	            continue;
	        }
	         
	        File root = new File(mount);
	        if (!root.exists() || !root.isDirectory()) {
	            mMounts.remove(i--);
	            continue;
	        }
	         
	        if (!isAvailableFileSystem(mount)) {
	            mMounts.remove(i--);
	            continue;
	        }
	         
	        if (!checkMicroSDCard(mount)) {
	            mMounts.remove(i--);
	        }
	    }
	     
	    if (mMounts.size() == 1) {
	        return mMounts.get(0);
	    }
	     
	    return null;
	}
	 
	private static List<String> readMountsFile() {
	    /**
	     * Scan the /proc/mounts file and look for lines like this:
	     * /dev/block/vold/179:1 /mnt/sdcard vfat rw,dirsync,nosuid,nodev,noexec,relatime,uid=1000,gid=1015,fmask=0602,dmask=0602,allow_utime=0020,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=remount-ro 0 0
	     *  
	     * When one is found, split it into its elements
	     * and then pull out the path to the that mount point
	     * and add it to the arraylist
	     */
	    List<String> mMounts = new ArrayList<String>();
	 
	    try {
	        Scanner scanner = new Scanner(new File("/proc/mounts"));
	         
	        while (scanner.hasNext()) {
	            String line = scanner.nextLine();
	             
	            if (line.startsWith("/dev/block/vold/")) {
	                String[] lineElements = line.split("[ \t]+");
	                String element = lineElements[1];
	                                     
	                mMounts.add(element);
	            }
	        }
	    } catch (Exception e) {
	        // Auto-generated catch block
	        e.printStackTrace();
	    }
	     
	    return mMounts;
	}
	 
	private static List<String> readVoldFile() {
	    /**
	     * Scan the /system/etc/vold.fstab file and look for lines like this:
	     * dev_mount sdcard /mnt/sdcard 1 /devices/platform/s3c-sdhci.0/mmc_host/mmc0
	     * 
	     * When one is found, split it into its elements
	     * and then pull out the path to the that mount point
	     * and add it to the arraylist
	     */
	     
	    List<String> mVold = new ArrayList<String>();
	     
	    try {
	        Scanner scanner = new Scanner(new File("/system/etc/vold.fstab"));
	         
	        while (scanner.hasNext()) {
	            String line = scanner.nextLine();
	             
	            if (line.startsWith("dev_mount")) {
	                String[] lineElements = line.split("[ \t]+");
	                String element = lineElements[2];
	                 
	                if (element.contains(":")) {
	                    element = element.substring(0, element.indexOf(":"));
	                }
	 
	                mVold.add(element);
	            }
	        }
	    } catch (Exception e) {
	        // Auto-generated catch block
	        e.printStackTrace();
	    }
	     
	    return mVold;
	}
	 
	private static boolean checkMicroSDCard(String fileSystemName) {
	    StatFs statFs = new StatFs(fileSystemName);
	     
	    long totalSize = (long)statFs.getBlockSize() * statFs.getBlockCount();
	     
	    if (totalSize < ONE_GIGABYTE) {
	        return false;
	    }
	         
	    return true;
	}
	 
	private static boolean isAvailableFileSystem(String fileSystemName) {
	    final String[]  unAvailableFileSystemList = {"/dev", "/mnt/asec", "/mnt/obb", "/system", "/data", "/cache", "/efs", "/firmware"};   // 알려진 File System List입니다.
	     
	    for (String name : unAvailableFileSystemList) {
	        if (fileSystemName.contains(name) == true) {
	            return false;
	        }
	    }
	     
	    if (Environment.getExternalStorageDirectory().getAbsolutePath().equals(fileSystemName) == true) {
	        /** 안드로이드에서 제공되는 getExternalStorageDirectory() 경로와 같은 경로일 경우에는 추가로 삽입된 SDCard가 아니라고 판단하였습니다. **/
	        return false;
	    }
	     
	    return true;
	}
}
