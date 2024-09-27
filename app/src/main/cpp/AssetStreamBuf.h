#pragma once

#include <streambuf>
#include <android/asset_manager.h>

class AssetStreamBuf : public std::streambuf {
public:
    explicit AssetStreamBuf(AAssetManager *asset_manager, const char *filename, int mode);

    ~AssetStreamBuf();
protected:
    int_type underflow() override;

private:
    static constexpr int put_back_size = 8;
    static constexpr int buffer_size = 5 + put_back_size;
    char buffer_[buffer_size]{};
    AAsset *asset_;
};
