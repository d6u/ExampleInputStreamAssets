#pragma once

#include <istream>
#include <android/asset_manager.h>

#include "AssetStreamBuf.h"

class AssetIStream : public std::istream {
public:
    explicit AssetIStream(AAssetManager *asset_manager, const char *filename, int mode);
private:
    AssetStreamBuf buffer_;
};
