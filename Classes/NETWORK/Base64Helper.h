
int Base64DecodeGetRequiredLength(int n);
int Base64Decode(const char* src, const int src_len, void* dst, int *dst_len) ;

int Base64EncodeGetRequiredLength(int n);
int Base64Encode(unsigned char const* src, int src_len, char* dst, int *dst_len);