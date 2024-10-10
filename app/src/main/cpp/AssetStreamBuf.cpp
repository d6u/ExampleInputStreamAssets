#include "AssetStreamBuf.h"

#include <android/log.h>

AssetStreamBuf::AssetStreamBuf(AAssetManager *asset_manager, const char *filename) {
    asset_ = AAssetManager_open(asset_manager, filename, AASSET_MODE_STREAMING);

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
        __android_log_print(ANDROID_LOG_DEBUG, "native", "underflow: Buffer not exhausted");
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
        __android_log_print(ANDROID_LOG_DEBUG, "native", "underflow: EOF");
        // EOF or error
        return traits_type::eof();
    }

    setg(buffer_ + (put_back_size - num_putback),
         buffer_ + put_back_size,
         buffer_ + put_back_size + n);

    return traits_type::to_int_type(*gptr());
}

// Called by two arguments version of istream::seekg()
AssetStreamBuf::pos_type
AssetStreamBuf::seekoff(off_type off, std::ios::seekdir way, std::ios::openmode mode) {
    __android_log_print(ANDROID_LOG_DEBUG, "native", "seekoff %lld %d", off, way);

    switch (way) {
        case std::ios::beg: {
            __android_log_print(ANDROID_LOG_DEBUG, "native", "seekoff beg");
            char *end = buffer_ + buffer_size;
            setg(end, end, end);
            return AAsset_seek64(asset_, off, SEEK_SET);
        }
        case std::ios::cur: {
            __android_log_print(ANDROID_LOG_DEBUG, "native", "seekoff cur");
            if (gptr() + off >= eback() && gptr() + off <= egptr()) {
                __android_log_print(ANDROID_LOG_DEBUG, "native", "seekoff cur within range");
                setg(eback(), gptr() + off, egptr());
                return AAsset_getLength64(asset_) - AAsset_getRemainingLength64(asset_);
            } else {
                __android_log_print(ANDROID_LOG_DEBUG, "native", "seekoff cur out of range");
                char *end = buffer_ + buffer_size;
                setg(end, end, end);
                // Subtract two pointer first to invoke defined behavior of pointer subtraction
                off64_t offset = gptr() - egptr() + off;
                return AAsset_seek64(asset_, offset, SEEK_CUR);
            }
        }
        case std::ios::end: {
            __android_log_print(ANDROID_LOG_DEBUG, "native", "seekoff end");
            char *end = buffer_ + buffer_size;
            setg(end, end, end);
            return AAsset_seek64(asset_, off, SEEK_END);
        }
        default:
            return -1;
    }
}

// Called by single argument version of istream::seekg()
AssetStreamBuf::pos_type AssetStreamBuf::seekpos(pos_type sp, std::ios::openmode mode) {
    __android_log_print(ANDROID_LOG_DEBUG, "native", "seekpos %lld", sp - pos_type(0));
    return seekoff(sp - pos_type(0), std::ios_base::beg, mode);
}
