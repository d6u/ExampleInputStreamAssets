package com.example.inputstreamassets

import android.content.res.AssetManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.example.inputstreamassets.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()

        init(assets)
    }

    /**
     * A native method that is implemented by the 'inputstreamassets' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun init(assetManager: AssetManager)

    companion object {
        // Used to load the 'inputstreamassets' library on application startup.
        init {
            System.loadLibrary("inputstreamassets")
        }
    }
}