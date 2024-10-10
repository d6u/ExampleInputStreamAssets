#include <string>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include "AssetIStream.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_inputstreamassets_MainActivity_stringFromJNI(JNIEnv *env, jobject) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_inputstreamassets_MainActivity_init(JNIEnv *env, jobject, jobject jasset_manager) {
    AAssetManager *asset_manager = AAssetManager_fromJava(env, jasset_manager);

    // AAsset *asset = AAssetManager_open(asset_manager, "text/numbers.txt", AASSET_MODE_BUFFER);
    //
    // size_t asset_length = AAsset_getLength(asset);
    // char *buffer = new char[asset_length + 1];
    // buffer[asset_length] = '\0';
    //
    // AAsset_read(asset, buffer, asset_length);
    //
    // __android_log_print(ANDROID_LOG_DEBUG, "native", "length %zu %zu %s", asset_length,
    //                     strlen(buffer), buffer);
    //
    // AAsset_close(asset);

    // Use custom class

    AssetIStream input_stream(asset_manager, "text/numbers.txt");

    std::string line;
    std::getline(input_stream, line);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "line %s", line.c_str());
    std::getline(input_stream, line);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "line %s | peek %c", line.c_str(),
                        input_stream.peek());

    input_stream.seekg(11);
    std::getline(input_stream, line);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "line %s", line.c_str());
    std::getline(input_stream, line);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "line %s", line.c_str());

    AAsset *asset = AAssetManager_open(asset_manager, "text/numbers.txt", AASSET_MODE_BUFFER);
    size_t len = AAsset_getLength64(asset);
    const char *buf = (const char *) AAsset_getBuffer(asset);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "all %.*s", len, buf);
    AAsset_close(asset);
}
