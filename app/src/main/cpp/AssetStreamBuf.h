#pragma once

#include <streambuf>
#include <android/asset_manager.h>

class AssetStreamBuf : public std::streambuf {
public:
    explicit AssetStreamBuf(AAssetManager *asset_manager, const char *filename);

    ~AssetStreamBuf() override;

protected:
    int_type underflow() override;

    pos_type seekoff(off_type off, std::ios::seekdir way, std::ios::openmode which) override;

    pos_type seekpos(pos_type sp, std::ios_base::openmode which) override;

private:
    static constexpr int put_back_size = 8;
    static constexpr int buffer_size = 6 + put_back_size;

    char buffer_[buffer_size]{};
    AAsset *asset_;
};
