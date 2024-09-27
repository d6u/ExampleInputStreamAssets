plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
}

val playcoreDir = file("../play-core-native-sdk")

android {
    namespace = "com.example.inputstreamassets"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.example.inputstreamassets"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags += "-std=c++17"
                // Define the PLAYCORE_LOCATION directive.
                arguments += listOf("-DANDROID_STL=c++_static", "-DPLAYCORE_LOCATION=$playcoreDir")
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro"
            )
            // Include Play Core Library proguard config files to strip unused code while retaining the Java symbols needed for JNI.
            proguardFile("$playcoreDir/proguard/common.pgcfg")
            proguardFile("$playcoreDir/proguard/gms_task.pgcfg")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }

    kotlinOptions {
        jvmTarget = "1.8"
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    buildFeatures {
        viewBinding = true
    }

    assetPacks += listOf(":example_asset_pack")
}

dependencies {
    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    implementation(libs.androidx.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)

    // Import these feature-specific AARs for each Google Play Core library.
    implementation("com.google.android.play:app-update:2.1.0")
    implementation("com.google.android.play:asset-delivery:2.2.2")
    implementation("com.google.android.play:integrity:1.4.0")
    implementation("com.google.android.play:review:2.0.1")

    // Import these common dependencies.
    implementation("com.google.android.gms:play-services-tasks:18.0.2")
    implementation(files("$playcoreDir/playcore-native-metadata.jar"))
}