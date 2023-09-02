package io.github.usernameak.brewemulator;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Color;
import android.graphics.Paint;

@SuppressLint("ViewConstructor")
public class EmulatorSurfaceView extends SurfaceView implements SurfaceHolder.Callback
{
    public EmulatorSurfaceView(Context context)
	{
        super(context);

        getHolder().addCallback(this);
    }

    private native void nSurfaceCreated(Surface surface);
    private native void nSurfaceChanged();
    private native void nSurfaceDestroyed(Surface surface);

    @Override
    public void surfaceCreated(SurfaceHolder holder)
	{
        System.out.println("surfaceCreated");

        nSurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
	{
        System.out.println("surfaceChanged");

        nSurfaceChanged();
        // nSurfaceDestroyed(holder.getSurface());
        // nSurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder)
	{
        System.out.println("surfaceDestroyed");

        nSurfaceDestroyed(holder.getSurface());
    }
}
