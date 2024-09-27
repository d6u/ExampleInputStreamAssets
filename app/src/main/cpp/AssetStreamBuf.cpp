#include "AssetStreamBuf.h"

#include <android/log.h>

AssetStreamBuf::AssetStreamBuf(AAssetManager *asset_manager, const char *filename, int mode) {
    asset_ = AAssetManager_open(asset_manager, filename, mode);

    char *end = buffer_ + buffer_size;
    // Set beginning, next and end pointer.
    setg(end, end, end);
}

AssetStreamBuf::~AssetStreamBuf() {
    AAsset_close(asset_);
}

int AssetStreamBuf::underflow() {
    // gptr() returns pointer to current character of the get area
    // egptr() returns pointer to the end of the get area
    if (gptr() < egptr()) {
        __android_log_print(ANDROID_LOG_DEBUG, "native", "Buffer not exhausted");
        // Buffer not exhausted
        return traits_type::to_int_type(*gptr());
    }

    // eback() returns pointer to the beginning of the get area
    int num_putback = std::min<int>(gptr() - eback(), put_back_size);

    // memmove is the same as memcpy but handles overlapping memory region
    std::memmove(buffer_ + (put_back_size - num_putback),
                 gptr() - num_putback,
                 num_putback);

    int n = AAsset_read(asset_,
                        buffer_ + put_back_size,
                        buffer_size - put_back_size);

    if (n <= 0) {
        __android_log_print(ANDROID_LOG_DEBUG, "native", "EOF");
        // EOF or error
        return traits_type::eof();
    }

    setg(buffer_ + (put_back_size - num_putback),
         buffer_ + put_back_size,
         buffer_ + put_back_size + n);

    return traits_type::to_int_type(*gptr());
}
