package io.github.usernameak.brewemulator;

import android.content.Context;
import android.widget.Button;
import android.view.MotionEvent;
import java.util.ArrayList;
import android.view.View;

public class MyKeypad_AutoInput
{
	private static Long touchStart;
	private static Long touchEnd = 0L;
	public static ArrayList<String> listRec_keypad;
	
	public interface IKeypadHandler
	{
        void onButtonDown(int code);
        void onButtonUp(int code);
    }
	
	public static void createKeypad_AutoInput(Context theContext, int typePad, final IKeypadHandler handler)
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
							int action = event.getAction();
							
							if (action == MotionEvent.ACTION_DOWN)
							{
								//handler.onButtonDown(code);
								touchStart = System.currentTimeMillis();
								handler.onButtonDown(code);
								
								//delay between touch
								listRec_keypad.add((touchStart-touchEnd)+"");

								view.setScaleX(0.8f);
								view.setScaleY(0.8f);
							}
							else if (action==MotionEvent.ACTION_UP)// || event.getActionButton()==MotionEvent.ACTION_CANCEL)
							{									
								//handler.onButtonUp(code);
								touchEnd = System.currentTimeMillis();
								handler.onButtonUp(code);
								
								//time for touching
								listRec_keypad.add(code+"-"+(touchEnd-touchStart)+"-");

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
