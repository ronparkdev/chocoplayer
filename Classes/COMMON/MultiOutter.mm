#include <iostream>

#import <sys/utsname.h>
#include "../Data/TString.h"
#include "../MENU/MenuMain.h"

#import "AppController.h"

void MultiOutterIOS_OpenSomething(const char *url)
{
    NSString *strurl = [[NSString alloc]initWithUTF8String:url];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strurl]];
}

void MultiOutterIOS_GetModel(char *str, int strlen)
{
    struct utsname sysinfo;
    uname(&sysinfo);
    strcpy(str, sysinfo.machine);
}

void MultiOutterIOS_GetUniqueID(char *str, int strlen)
{

	NSString *tstr = @"iphone4";//[[UIDevice currentDevice] ];
	const char *cstr = [tstr UTF8String];
    strcpy(str, cstr);
}

TString MultiOutterIOS_GetTempFolder() {
    TString str;
    str.setTextUTF8([NSTemporaryDirectory() UTF8String]);
    
    return str;
}

void MultiOutterIOS_OpenMenu(char *stack) {
    NSString *nstack = [[NSString alloc]initWithCString:stack encoding:NSUTF8StringEncoding];
    [[AppController getInstance] showMenu:nstack];
}

void MultiOutterIOS_OpenCloudPicker() {
    [[AppController getInstance] showCloudPicker];
}

NSString* MultiOutterIOS_GetMenu(NSString *ncmd) {
    
    char cmd[512];
    TString tcmd;
    [ncmd getCString:cmd maxLength:511 encoding:NSUTF8StringEncoding];
    tcmd.setTextUTF8(cmd);
    
    TString result = MenuMain::getMenu(tcmd);
    return [[NSString alloc]initWithUTF8String:result.getTextUTF8()];
}

void MultiOutterIOS_removeFolder(char *path) {
    NSString *npath = [[NSString alloc]initWithCString:path encoding:NSUTF8StringEncoding];

	NSFileManager * fileManager = [NSFileManager defaultManager];
	if ([fileManager removeItemAtPath:npath error:NULL] == NO) {
	  //  delDir 폴더 삭제 실패
	} else {
	  // delDir 폴더 삭제 성공 
	} 
}
