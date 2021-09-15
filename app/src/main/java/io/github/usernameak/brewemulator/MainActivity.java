package io.github.usernameak.brewemulator;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.View;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("brewemu");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getActionBar().hide();

        EmulatorMainContext mainContext = new EmulatorMainContext();
        mainContext.mainActivity = this;

        EmulatorSurfaceView surfaceView = new EmulatorSurfaceView(getApplicationContext(), mainContext);
        surfaceView.setLayerType(View.LAYER_TYPE_SOFTWARE, null);

        setContentView(surfaceView);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        return brewEmuKeyUp(keyCode);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return brewEmuKeyDown(keyCode);
    }

    public native boolean brewEmuKeyUp(int keyCode);
    public native boolean brewEmuKeyDown(int keyCode);

    public native void brewEmuJNIStartup(Surface surface);
}