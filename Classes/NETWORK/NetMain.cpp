#include "NetMain.h"

#include "curl/curl.h"
#include "Base64Helper.h"
#include "zconf.h"

#define CONNECTION_URL "http://chocolatlsweet.cafe24.com/choco/proc/RequestApp.jsp" 

NetMain * NetMain::g_pInstance = NULL;

NetMain * NetMain::getInstance()
{
	if (g_pInstance == NULL)
		g_pInstance = new NetMain();

	return g_pInstance;
}

void NetMain::removeInstance()
{
	if (g_pInstance != NULL) {
		delete []g_pInstance;
		g_pInstance = NULL;
	}
}

NetMain::NetMain()
{
	serverHost = CONNECTION_URL;
	lastUserID = "";
}

NetMain::~NetMain()
{

}

size_t write_html(void *ptr, size_t size, size_t count, void *stream) { // 데이터 쓰기 함수
	((std::string*)stream)->append((char*)ptr, 0, size*count); // stream에 문자열을 추가한다.
	return size*count;
}

bool NetMain::getData(const TString &request, TString &html)
{
	CURL *curl;
	CURLcode res;
	std::string strHtml;

	if (curl = curl_easy_init()) {//"http://chocolatlsweet.cafe24.com/common/echo.jsp"
		curl_easy_setopt(curl, CURLOPT_URL, CONNECTION_URL); // url 변수를 GET 요청 주소로 사용
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.getTextUTF8());
		//curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.0.3705; .NET CLR 1.1.4322)");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_html); // 쓰기 함수에 write_html() 사용
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strHtml); // 쓰기 데이터에 html을 사용 (stream)

		/*
		char *last_use_url = NULL;
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, last_use_url);
		*/

		res = curl_easy_perform(curl);

		
		html.setTextUTF8(strHtml.c_str());
		curl_easy_cleanup(curl);
		
		return true; // 성공하면 true 리턴
	} else {
		return false; // 실패하면 false 리턴
	}
}

TString NetMain::getEncodedText( const TString &input )
{
	/*
	CURL *curl = curl_easy_init();
	TString output;
	if (curl) {
		char *str = curl_easy_escape(curl, input.getTextUTF8(), input.getSize());
		if (str) {
			output.setTextUTF8(str);
			curl_free(str);
			return output;
		}
	}
	*/

	TString result;

	int cipherTextLen = input.getSize();
	int b64ResLen = Base64EncodeGetRequiredLength(input.getSize());
	char *b64Res = new char[b64ResLen + 1];

	if (!Base64Encode((const Byte *)input.getTextUTF8(), cipherTextLen, b64Res, &b64ResLen)) {
		CCLOG("getEncodedText::Fail to encode result with Base64");
		result = input;
	} else {
		for(int i=0; i<b64ResLen; i++)
			if (b64Res[i] == '+')
				b64Res[i] = '-';

		b64Res[b64ResLen] = '\0';

		result = b64Res;
	}

	delete []b64Res;

	return result;

}
