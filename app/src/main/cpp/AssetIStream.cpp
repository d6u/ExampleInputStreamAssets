#include "AssetIStream.h"

AssetIStream::AssetIStream(AAssetManager *asset_manager, const char *filename, int mode)
        : std::istream(nullptr), buffer_(asset_manager, filename, mode) {
    init(&buffer_);
}
