package io.github.usernameak.brewemulator;

import android.content.Context;
import android.widget.Button;
import android.view.MotionEvent;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.view.View;

public class MyKeypad_Control
{
	public interface IKeypadHandler
	{
        void onButtonDown(int code);
        void onButtonUp(int code);
    }

	public static void createKeypad_Control(Context theContext, int typePad, final IKeypadHandler handler)
	{
		String[][] labels = KeypadBuilder.getKeypadLabels(typePad);

		firts: for (int i=0; i<labels.length; i++)
		{
			second: for (int i2=0; i2<labels[0].length; i2++)
			{
				Button btn = KeypadBuilder.createButton(theContext, typePad, i, i2);

				if (btn == null)
				{
					continue;
				}
				
				final int code = btn.getId();
				
				btn.setOnTouchListener(new Button.OnTouchListener()
					{
						@Override
						public boolean onTouch(View view, MotionEvent event)
						{
							if (event.getAction()==MotionEvent.ACTION_DOWN)
							{
								handler.onButtonDown(code);
								view.getBackground().setColorFilter(Color.parseColor("#50000000"), PorterDuff.Mode.SRC_ATOP);
								//view.invalidate();
								view.setScaleX(0.9f);
								view.setScaleY(0.9f);
							}
							else if (event.getAction()==MotionEvent.ACTION_UP || event.getAction()==MotionEvent.ACTION_CANCEL)
							{
								handler.onButtonUp(code);
								view.getBackground().clearColorFilter();
								//view.invalidate();
								view.setScaleX(1);
								view.setScaleY(1);
							}
							
							return true;
						}				
				});	
				
			} //end loop 'second'
		} //end loop 'firts'
	}
}
