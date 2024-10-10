#include "AssetIStream.h"

AssetIStream::AssetIStream(AAssetManager *asset_manager, const char *filename)
        : std::istream(nullptr), buffer_(asset_manager, filename) {
    init(&buffer_);
}
