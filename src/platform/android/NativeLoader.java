package com.mercotui.ubahn;
public class NativeLoader extends android.app.NativeActivity {
    static {
        System.loadLibrary("main");
    }
}
