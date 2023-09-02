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
public class EmulatorSurfaceView_debug extends SurfaceView implements SurfaceHolder.Callback
{
	@Override
	public void surfaceCreated(SurfaceHolder p1)
	{
		// TODO: Implement this method
	}

	@Override
	public void surfaceChanged(SurfaceHolder p1, int p2, int p3, int p4)
	{
		// TODO: Implement this method
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder p1)
	{
		// TODO: Implement this method
	}
	
    public EmulatorSurfaceView_debug(Context context)
	{
        super(context);

        getHolder().addCallback(this);
    }
}
