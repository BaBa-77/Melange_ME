package io.github.usernameak.brewemulator;

import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import java.util.Locale;
import androidx.appcompat.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.Window;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.SeekBar;
import android.widget.ImageButton;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.graphics.PorterDuff;
import android.view.View.OnClickListener;
import java.io.File;
import android.os.Environment;
import java.util.Scanner;
import java.io.IOException;
import android.content.Context;
import android.widget.Button;
import android.view.View;
import android.graphics.Color;

public class MyKeypad_Custom
{
	//onTouchListener
	private static Float beginX=0f, beginY=0f, posX=0f, posY=0f;
	private static Float currScaleX=1f, currScaleY=1f, newScaleX=1f, newScaleY=1f;

	public static float theMovement = 50f;
	public static float scaleUp = 0.100f;
	
	public static void createKeypad_Custom(final Context theContext, int typePad)
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
				final String strCode = ENUM_keypad.keypad_.toString()+code;

				btn.setOnTouchListener(new Button.OnTouchListener()
					{
						@Override
						public boolean onTouch(final View view, MotionEvent event)
						{
							//custom position
							if (MyDataObject.getIntID() == R.id.menu_custom_move)
							{
								//mendapatkan ukuran layar
								int screenWidth = MainActivity.fr.getWidth();
								int screenHeight = MainActivity.fr.getHeight();

								//mendapatkan ukuran view
								int keypadWidth = view.getWidth();
								int keypadHeight = view.getHeight();

								//get scale of view
								float scaleX = view.getScaleX();
								float scaleY = view.getScaleY();

								//new size keyoad
								float keypadWidthNew = (float) keypadWidth*scaleX;
								float keypadHeightNew = (float) keypadHeight*scaleY;

								//size (one-side) change 
								float changeWidth = (keypadWidthNew - keypadWidth) / 2.0f;
								float changeHeight = (keypadHeightNew - keypadHeight) / 2.0f;

								int action = event.getAction();

								switch (action)
								{
									case MotionEvent.ACTION_DOWN:
										//get position/cordinate, firts touch
										beginX = event.getRawX();
										beginY = event.getRawY();

										//get curreent position
										posX = view.getX();
										posY = view.getY();
										break;

									case MotionEvent.ACTION_MOVE:
										//get position/cordinate, end touch
										Float currX = event.getRawX();
										Float currY = event.getRawY();

										//calculate moving
										Float moveX = currX-beginX;

										if (moveX%theMovement <= 1 || moveX%theMovement >= -1)
										{
											Float newX = posX + (theMovement * ((int)(moveX/theMovement)));

											if (newX >= 0+changeWidth && newX<=screenWidth-keypadWidthNew+changeWidth)
											{
												//set X
												view.setX(newX);
											}
											else if (newX <= 0+changeWidth)
											{
												view.setX(0+changeWidth);
											}
											else if (newX >= screenWidth-keypadWidthNew+changeWidth)
											{
												view.setX(screenWidth-keypadWidthNew+changeWidth);
											}
										}

										Float moveY = currY-beginY;

										if (moveY%theMovement <= 1 || moveY%theMovement >= -1)
										{
											Float newY = posY + (theMovement * ((int)(moveY/theMovement)));

											if (newY >= 0+changeHeight && newY <= screenHeight-keypadHeightNew+changeHeight)
											{
												//set Y
												view.setY(newY);
											}
											else if (newY <= 0+changeHeight)
											{
												view.setY(0+changeHeight);
											}
											else if (newY >= screenHeight-keypadHeightNew+changeHeight)
											{
												view.setY(screenHeight-keypadHeightNew+changeHeight);
											}
										}
										break;

									case MotionEvent.ACTION_UP:
										float newX = view.getX() - changeWidth;
										float newY = view.getY() - changeHeight;

										String theKey;

										if (MainActivity.theOrientation == 0)
										{
											theKey = strCode+ENUM_keypad._thePosPotrait.toString();
										}
										else
										{
											theKey = strCode+ENUM_keypad._thePosLandscape.toString();
										}

										MyDataObject.setMap(theKey, String.format(Locale.US, "%.2f,%.2f", newX, newY));
										break;
								}
							}
							//custom scale
							else if (MyDataObject.getIntID() == R.id.menu_custom_size)
							{
								//mendapatkan ukuran layar
								int screenWidth = MainActivity.fr.getWidth();
								int screenHeight = MainActivity.fr.getHeight();

								//mendapatkan ukuran view 'tl' (keypad)
								int keypadWidth = view.getWidth();
								int keypadHeight = view.getHeight();

								int action = event.getAction();

								switch (action)
								{
									case MotionEvent.ACTION_DOWN:
										beginX = event.getRawX();
										beginY = event.getRawY();

										currScaleX = view.getScaleX();
										currScaleY = view.getScaleY();

										break;

									case MotionEvent.ACTION_MOVE:
										Float currX = event.getRawX();
										Float currY = event.getRawY();

										Float moveX = currX-beginX;
										Float moveY = beginY-currY;

										float scaleMove = 20f;

										Float thePerX = scaleUp*scaleMove;

										float maxScaleX = (float) screenWidth / keypadWidth;
										float maxScaleY = (float) screenHeight / keypadHeight;

										if (moveX%thePerX <= 1 && moveX%thePerX >= -1)
										{
											newScaleX = currScaleX + (scaleUp * (int)(moveX/scaleMove));

											if (newScaleX <= maxScaleX)
											{
												view.setScaleX(newScaleX);
											}
											else
											{
												view.setScaleX(maxScaleX);
											}
										}

										Float thetheMovement = scaleUp*scaleMove;

										if (moveY%thetheMovement <= 1 && moveY%thetheMovement >= -1)
										{
											newScaleY = currScaleY + (scaleUp * (int)(moveY/scaleMove));

											if (newScaleY <= maxScaleY)
											{
												view.setScaleY(newScaleY);
											}
											else
											{
												view.setScaleY(maxScaleY);
											}
										}

										break;

									case MotionEvent.ACTION_UP:
										//set posisi jika ukuran skala keluar layar

										float newWidth = view.getWidth() * view.getScaleX();
										float newHeight = view.getHeight() * view.getScaleY();

										String theKey;

										if (MainActivity.theOrientation == 0)
										{
											theKey = strCode+ENUM_keypad._theSizePotrait.toString();
										}
										else
										{
											theKey = strCode+ENUM_keypad._theSizeLandscape.toString();
										}

										//sizeButton
										MyDataObject.setMap(theKey, String.format("%.0f,%.0f", newWidth, newHeight));

										break;
								}
							}
							else if (MyDataObject.getIntID() == R.id.menu_custom_alphaKeypad)
							{
								int action = event.getAction();

								switch (action)
								{
									case MotionEvent.ACTION_DOWN:

										break;

									case MotionEvent.ACTION_MOVE:

										break;

									case MotionEvent.ACTION_UP:
										AlertDialog.Builder builder = new AlertDialog.Builder(theContext);	
										builder.setTitle("Set: "+((Button)view).getText().toString());
										View layoutView = LayoutInflater.from(theContext).inflate(R.layout.set_transparacy, null);
										builder.setView(layoutView);			
										final AlertDialog dialog = builder.create();
										//dialog.setCanceledOnTouchOutside(false);

										Window window = dialog.getWindow();
										if (window != null)
										{
											window.setLayout(MainActivity.fr.getWidth()/2, LinearLayout.LayoutParams.WRAP_CONTENT);
											window.setDimAmount(0.0f);
											window.setBackgroundDrawableResource(android.R.color.transparent);
										}

										if (dialog.isShowing())
										{
											dialog.dismiss();
										}

										final TextView infoSeek = layoutView.findViewById(R.id.infoSeek);
										SeekBar seekAlpha = layoutView.findViewById(R.id.theSeek);
										ImageButton btnClose = layoutView.findViewById(R.id.closeBtn);								

										int i = (int) (view.getAlpha() * 100);
										infoSeek.setText(i+"%");

										seekAlpha.setMax(100);
										seekAlpha.setProgress(i);

										seekAlpha.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
											{
												@Override
												public void onProgressChanged(SeekBar p1, int p2, boolean p3)
												{
													infoSeek.setText(p2+"%");
													view.setAlpha((float) p2/100.00f);
												}

												@Override
												public void onStartTrackingTouch(SeekBar p1)
												{
													p1.getProgressDrawable().setColorFilter(Color.parseColor("#ff00aa00"), PorterDuff.Mode.SRC_IN);
													p1.getThumb().setColorFilter(Color.parseColor("#ff00dd00"), PorterDuff.Mode.SRC_IN);
												}

												@Override
												public void onStopTrackingTouch(SeekBar p1)
												{
													p1.getProgressDrawable().clearColorFilter();
													p1.getThumb().clearColorFilter();

													String theKey;

													if (MainActivity.theOrientation == 0)
													{
														theKey = strCode+ENUM_keypad._theAlphaPotrait.toString();				
													}
													else
													{
														theKey = strCode+ENUM_keypad._theAlphaLandscape.toString();
													}

													MyDataObject.setMap(theKey, String.format("%d", p1.getProgress()));
												}						
											});

										btnClose.setOnClickListener(new OnClickListener()
											{
												@Override
												public void onClick(View view)
												{
													dialog.dismiss();
												}
											});

										dialog.show();
										break;
								}
							}
							
							
							return true;
						}
				});
			} //end loop 'second'
		} //end loop 'firts'		
		
		
		
		/*KeypadBuilder.buttonBuilder(theContext, typePad, new KeypadBuilder.MyTouchListener()
			{
				@Override
				public void MyTouch(final Button view, MotionEvent event, int code, final String strCode)
				{
					
		});*/
	}
}
