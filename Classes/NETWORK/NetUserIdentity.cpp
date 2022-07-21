#include "NetMain.h"
#include <openssl/x509.h>
#include <openssl/ossl_typ.h>

#include "Base64Helper.h"
//#include <atlenc.h>

#include "../AppMulti.h"
#include "../COMMON/MultiOutter.h"
#include "../GameMain.h"

const TString& NetMain::getUserID()
{
	TString userInfo;
	if (lastUserID.isEmpty()) {
		
#if (SR_ANDROID == 1)
		userInfo = "ANDROID ";
#elif(SR_IPHONE == 1) 
		userInfo = "IOS ";
#else
		userInfo = "PC ";
#endif
		userInfo = userInfo + GameMain::getInstance()->getModel() + " " + GameMain::getInstance()->getUniqueID();

		getASCEncryptedString(userInfo, lastUserID);
	}

	return lastUserID;
}

const char *BASE64_PUB_KEY = 
	"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCE/yV45mX/bg"
	"Q+ucURI+4+Ju/gvUQ8nzMV/bEw2BND1qMa9iZBFhTvQ0DFxocv"
	"zf6cYH3KriUdL8GMuvkAhWGq1rN3AXL9TdbTnYkZRl+pQDdjoW"
	"6KaZg3p7S2XCKJcE7PIE1smaaev38SmP1fh60DwO7etNBsUTWI"
	"MaDYjDifNQIDAQAB";

bool NetMain::getASCEncryptedString(const TString &input, TString &output) {

	bool result = false;

	int b64PubKeyLen, pubKeyLen;
	unsigned char *pubKeyData;

	X509_PUBKEY * x509PubKey = NULL;
	EVP_PKEY *pubKey = NULL;
	RSA *rsa = NULL;

	int cipherTextLen;
	unsigned char *cipherText = NULL;

	int plainTextLen;
	const char *plainText = input.getTextUTF8();

	int resizedTextLen;
	char *resizedText = NULL;

	int b64ResLen;
	char *b64Res;

	b64PubKeyLen = strlen(BASE64_PUB_KEY);
	pubKeyLen = Base64DecodeGetRequiredLength(b64PubKeyLen);
	pubKeyData = new unsigned char[pubKeyLen];

	if (!Base64Decode(BASE64_PUB_KEY, b64PubKeyLen, pubKeyData, &pubKeyLen)) {
		CCLOG("NetUserIdentity::Fail to decode public key with Base64");
	} else {
		const unsigned char *cPubKeyData = pubKeyData;
		if (!d2i_X509_PUBKEY(&x509PubKey, &cPubKeyData, pubKeyLen) ||
			!(pubKey = X509_PUBKEY_get(x509PubKey)) ||
			!(rsa = EVP_PKEY_get1_RSA(pubKey))) {
				CCLOG("NetUserIdentity::Fail to open public key");
		} else {
			resizedTextLen = cipherTextLen = RSA_size(rsa);
			cipherText = new unsigned char[cipherTextLen + 1];

			plainTextLen = strlen(plainText);
			resizedText = new char[resizedTextLen+1];

			if (plainTextLen >= cipherTextLen) {
				CCLOG("NetUserIdentity::Fail cause by text size is too big");
			} else {
				strcpy(resizedText, plainText);
				memset(resizedText + plainTextLen, '\0', resizedTextLen - plainTextLen);

				cipherTextLen = RSA_public_encrypt(resizedTextLen, (const unsigned char*)resizedText, (unsigned char*)cipherText, rsa, RSA_NO_PADDING);

				if (cipherTextLen == -1) {
					CCLOG("NetUserIdentity::Fail to Encrypt data");
				} else {
					b64ResLen = Base64EncodeGetRequiredLength(cipherTextLen);
					b64Res = new char[b64ResLen + 1];

					if (!Base64Encode((const Byte *)cipherText, cipherTextLen, b64Res, &b64ResLen)) {
						CCLOG("NetUserIdentity::Fail to encode result with Base64");
					} else {
						for(int i=0; i<b64ResLen; i++)
							if (b64Res[i] == '+')
								b64Res[i] = '-';

						b64Res[b64ResLen] = '\0';

						output = b64Res;
						result = true;
					}

					delete []b64Res;
				}
			}

			delete []resizedText;
			delete []cipherText;

			RSA_free(rsa);
			EVP_PKEY_free(pubKey);
			X509_PUBKEY_free(x509PubKey);
		}
	}

	delete []pubKeyData;

	return result;
}

