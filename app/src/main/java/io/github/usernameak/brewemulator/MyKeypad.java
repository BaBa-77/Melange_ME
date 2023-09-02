package io.github.usernameak.brewemulator;

import android.os.Bundle;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import java.util.HashMap;
import java.util.Map;
import java.io.File;
import android.os.Environment;
import java.util.Scanner;
import java.io.IOException;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import java.util.ArrayList;
import android.widget.SeekBar;
import android.view.View.OnClickListener;
import android.widget.SeekBar.OnSeekBarChangeListener;
import androidx.appcompat.app.AlertDialog;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.view.Window;
import android.widget.ImageButton;
import java.util.Locale;
import android.widget.EditText;
import android.content.DialogInterface;

public class MyKeypad extends AppCompatActivity
{
	//onTouchListener
	private static Float beginX=0f, beginY=0f, posX=0f, posY=0f;
	private static Long touchStart;
	private static Float currScaleX=1f, currScaleY=1f, newScaleX=1f, newScaleY=1f;
	private static Long touchEnd = 0L;
	public static ArrayList<String> listRec_keypad;
	
	public static float theMovement = 50f;
	public static float scaleUp = 0.100f;
	
	public static final String[][] KEYPAD_LABELS_NUM = {
		{"1", "2", "3"},
		{"4", "5", "6"},
		{"7", "8", "9"},
		{"*", "0", "#"},
	};

	public static final int[][] KEYPAD_KEYCODES_NUM = {
		{0xE022, 0xE023, 0xE024},
		{0xE025, 0xE026, 0xE027},
		{0xE028, 0xE029, 0xE02A},
		{0xE02B, 0xE021, 0xE02C},
	};

	public static final String[][] KEYPAD_LABELS_DPAD = {
		{"➖", "§", "➖"},
		{null, "↑", null},
		{"←", "🔘", "→"},
		{null, "↓", null},
		{"+", null, "©"},
	};

	public static final int[][] KEYPAD_KEYCODES_DPAD = {
		{0xE036, 0xE02F, 0xE037},
		{0, 0xE031, 0},
		{0xE033, 0xE035, 0xE034},
		{0, 0xE032, 0},
		{0xE083, 0, 0xE030},
	};

	public static final String[][] KEYPAD_LABELS_DPAD_PLUS = {
		{"➖", "§", "➖"},
		{"↖", "↑", "↗"},
		{"←", "🔘", "→"},
		{"↙", "↓", "↘"},
		{"+", null, "©"},
	};

	public static final int[][] KEYPAD_KEYCODES_DPAD_PLUS = {
		{0xE036, 0xE02F, 0xE037},
		{0x1, 0xE031, 0x2},
		{0xE033, 0xE035, 0xE034},
		{0x3, 0xE032, 0x4},
		{0xE083, 0, 0xE030},
	};

	public static final String[][] KEYPAD_LABELS_POTRAIT = {
		{"➖️", "⬅", "➡", "➖"},
		{"1", "2", "3", "⬆"},
		{"4", "5", "6", "🔘"},
		{"7", "8", "9", "⬇"},
		{"*", "0", "#", "©"},
    };

    public static final int[][] KEYPAD_KEYCODES_POTRAIT = {
		{0xE036, 0xE033, 0xE034, 0xE037},
		{0xE022, 0xE023, 0xE024, 0xE031},
		{0xE025, 0xE026, 0xE027, 0xE035},
		{0xE028, 0xE029, 0xE02A, 0xE032},
		{0xE02B, 0xE021, 0xE02C, 0xE030},
    };
	
	public interface IKeypadHandler
	{
        void onButtonDown(int code);
        void onButtonUp(int code);
    }

	public static void builderButton(final Context theContext, final IKeypadHandler handler, int typePad)
	{
		//get data from properties save-data
		HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);	

		//data label
		final Float styleSize = MyFormat.toFloat(myConfigSave.get(ENUM_keypad.labelSize.toString()), 25.00f);
		int styleColor = MyFormat.toColorInt(myConfigSave.get(ENUM_keypad.labelColor.toString()), Color.BLACK);
		Boolean bgImage = MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useBgBtn.toString()));
		Boolean extKeypad = MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useExtLabel.toString()));
		
		int screen_Width = MainActivity.fr.getWidth();
		int screen_Height = MainActivity.fr.getHeight();
		
		String[][] KEYPAD_LABELS = null;
		int[][] KEYPAD_KEYCODES = null;

		//get choosen of keypad
		if (typePad == R.id.keypad_potrait) //Potrait
		{
			if (extKeypad)
			{
				HashMap<String[][],int[][]> map = getExternalKeypadLabel(typePad);

				for (Map.Entry<String[][],int[][]> entryMap : map.entrySet())
				{
					KEYPAD_LABELS = entryMap.getKey();
					KEYPAD_KEYCODES = entryMap.getValue();
				}
			}
			else
			{
				KEYPAD_LABELS = KEYPAD_LABELS_POTRAIT;
				KEYPAD_KEYCODES = KEYPAD_KEYCODES_POTRAIT;
			}	
		}
		else if (typePad == R.id.keypad_numpad) //only numpad
		{
			if (extKeypad)
			{
				HashMap<String[][],int[][]> map = getExternalKeypadLabel(typePad);

				for (Map.Entry<String[][],int[][]> entryMap : map.entrySet())
				{
					KEYPAD_LABELS = entryMap.getKey();
					KEYPAD_KEYCODES = entryMap.getValue();
				}
			}
			else
			{
				KEYPAD_LABELS = KEYPAD_LABELS_NUM;
				KEYPAD_KEYCODES = KEYPAD_KEYCODES_NUM;
			}
		}
		else if (typePad == R.id.keypad_dpad) //only dpad
		{
			if (extKeypad)
			{
				HashMap<String[][],int[][]> map = getExternalKeypadLabel(typePad);

				for (Map.Entry<String[][],int[][]> entryMap : map.entrySet())
				{
					KEYPAD_LABELS = entryMap.getKey();
					KEYPAD_KEYCODES = entryMap.getValue();
				}
			}
			else
			{
				KEYPAD_LABELS = KEYPAD_LABELS_DPAD;
				KEYPAD_KEYCODES = KEYPAD_KEYCODES_DPAD;
			}
		}
		else if (typePad == R.id.keypad_dpad_plus) //only dpad + diagonal
		{
			if (extKeypad)
			{
				HashMap<String[][],int[][]> map = getExternalKeypadLabel(typePad);

				for (Map.Entry<String[][],int[][]> entryMap : map.entrySet())
				{
					KEYPAD_LABELS = entryMap.getKey();
					KEYPAD_KEYCODES = entryMap.getValue();
				}
			}
			else
			{
				KEYPAD_LABELS = KEYPAD_LABELS_DPAD_PLUS;
				KEYPAD_KEYCODES = KEYPAD_KEYCODES_DPAD_PLUS;
			}
		}

		firts: for (int i=0; i<KEYPAD_LABELS.length; i++)
		{
			second: for (int i2=0; i2<KEYPAD_LABELS[i].length; i2++)
			{
				if (KEYPAD_LABELS[i][i2] == null)
				{
					continue second;
				}			
				
				Button btn = new Button(theContext);
				btn.setText(KEYPAD_LABELS[i][i2]);
				btn.setTextSize(styleSize);
				btn.setTextColor(styleColor);
				btn.setTypeface(Typeface.SERIF, Typeface.BOLD);
				
				//set code
				final int code = KEYPAD_KEYCODES[i][i2];
				final String strCode = ENUM_keypad.keypad_.toString()+code;

				//give identity for each button
				btn.setId(code);
				
				//set visibility
				String theKey;
				if (MainActivity.theOrientation == 0)
				{
					theKey = strCode+ENUM_keypad._visibilityPotrait.toString();
				}
				else
				{
					theKey = strCode+ENUM_keypad._visibilityLandscape.toString();
				}
				
				String theVisibility = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, theKey);
				
				if (MyFormat.toInt(theVisibility,1) == 0)
				{
					btn.setVisibility(View.GONE);
				}
				
				//set alpha (transparant)
				if (MainActivity.theOrientation == 0)
				{
					theKey = strCode+ENUM_keypad._theAlphaPotrait.toString();
				}
				else
				{
					theKey = strCode+ENUM_keypad._theAlphaLandscape.toString();
				}
				
				String theAlpha = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, theKey);

				if (theAlpha != null)
				{
					int a = MyFormat.toInt(theAlpha, 100);

					btn.setAlpha((Float) (a/100.00f));
				}

				//set background & size
				//make background for button
				if (MainActivity.theOrientation == 0)
				{
					theKey = strCode+ENUM_keypad._theSizePotrait.toString();
				}
				else
				{
					theKey = strCode+ENUM_keypad._theSizeLandscape.toString();
				}
				
				String theSize = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, theKey);
				
				int btnWidth=100, btnHeight=100;
				
				if (theSize != null)
				{
					String[] s = theSize.split(",", 2);
					btnWidth = MyFormat.toInt(s[0], btnWidth);
					btnHeight = MyFormat.toInt(s[1], btnHeight);
				}				
				
				if (btnWidth < 1)
				{
					btnWidth = 100;
				}
				
				if (btnHeight < 1)
				{
					btnHeight = 100;
				}
				
				Drawable bgDrawable = null;

				if (bgImage) //if using external background for button
				{
					Bitmap bitmap = BitmapFactory.decodeFile("/sdcard/MelangeBREW/sys/bgBtn.png");
					Bitmap bitmap2 = Bitmap.createScaledBitmap(bitmap, btnWidth, btnHeight, true);
					bgDrawable = new BitmapDrawable(theContext.getResources(), bitmap2);
				}
				else
				{
					Bitmap bitmap = BitmapFactory.decodeResource(theContext.getResources(), R.drawable.keypad_button);
					Bitmap bitmap2 = Bitmap.createScaledBitmap(bitmap, btnWidth, btnHeight, true);
					bgDrawable = new BitmapDrawable(theContext.getResources(), bitmap2);
				}
				
				btn.setBackground(bgDrawable);
				
				/*RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
					RelativeLayout.LayoutParams.WRAP_CONTENT,
					RelativeLayout.LayoutParams.WRAP_CONTENT);*/
				RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
					btnWidth,
					btnHeight);
				//params.setMargins(10,10,10,10);
				
				if (MainActivity.theOrientation == 0)
				{
					theKey = strCode+ENUM_keypad._thePosPotrait.toString();
				}
				else
				{
					theKey = strCode+ENUM_keypad._thePosLandscape.toString();
				}
				
				String thePos = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, theKey);
				
				if (thePos != null)
				{
					String[] s = thePos.split(",", 2);
					try
					{
						float x = Float.parseFloat(s[0]);
						btn.setX(x);
					}
					catch (NumberFormatException e)
					{
						MyLog.w(theKey+":x: "+e.toString());
						thePos = null;
					}
					
					try
					{
						float y = Float.parseFloat(s[1]);
						btn.setY(y);
					}
					catch (NumberFormatException e)
					{
						MyLog.w(theKey+":y: "+e.toString());
						thePos = null;
					}
				}
			
				if (thePos == null)
				{
					//set position Y (vertical) button
					int btnPosY = screen_Height - (btnHeight * (KEYPAD_LABELS.length-i)) - 1;
					int btnPosX=0;
					
					//set position X (horizontal) button 
					switch (typePad)
					{
						case R.id.keypad_potrait:
							int totalWidth = btnWidth * KEYPAD_LABELS[0].length;
							int pos = screen_Width/2 - totalWidth/2;
							btnPosX = (btnWidth * i2) + pos;
							break;

						case R.id.keypad_numpad:
							btnPosX = screen_Width - (btnWidth*(KEYPAD_LABELS[0].length-i2)) - 1;
							break;

						case R.id.keypad_dpad:
							btnPosX = (btnWidth * i2) + 1;
							break;
					}

					//set button position
					btn.setX(btnPosX);
					btn.setY(btnPosY);
				}
				
				btn.setOnTouchListener(new OnTouchListener()
					{
						@Override
						public boolean onTouch(final View view, MotionEvent event)
						{
							if (MyDataObject.getIntID() == R.id.menu_rec_cap)
							{
								if (event.getAction() == MotionEvent.ACTION_DOWN)
								{
									handler.onButtonDown(code);
									touchStart = System.currentTimeMillis();

									listRec_keypad.add((touchStart-touchEnd)+"");

									view.setScaleX(0.8f);
									view.setScaleY(0.8f);
								}
								else if (event.getAction()==MotionEvent.ACTION_UP || event.getActionButton()==MotionEvent.ACTION_CANCEL)
								{									
									handler.onButtonUp(code);
									touchEnd = System.currentTimeMillis();

									listRec_keypad.add(code+"-"+(touchEnd-touchStart)+"-");

									view.setScaleX(1);
									view.setScaleY(1);									
								}
							}
							else if (MyDataObject.getIntID() == R.id.menu_rec_play)
							{
								//make nothing can do
							}
							else if (MyDataObject.getIntID() == 10101)
							{
							}
							//custom position
							else if (MyDataObject.getIntID() == R.id.menu_custom_move)
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
									case event.ACTION_DOWN:
										beginX = event.getRawX();
										beginY = event.getRawY();

										currScaleX = view.getScaleX();
										currScaleY = view.getScaleY();

										break;

									case event.ACTION_MOVE:
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

									case event.ACTION_UP:
										//set posisi jika ukuran skala keluar layar
										/*if (parentID == R.id.keypad_potrait)
										{
											if (newPosX+keypadWidthNew > screenWidth || newPosX < 0)
											{
												//center horizontal
												newPosX = (screenWidth-(keypadWidth))/2;
												v.setX(newPosX);
												newPosX -= changeWidth;
											}
										}
										else
										{
											if (newPosX+keypadWidthNew > screenWidth)
											{	
												newPosX = (screenWidth-keypadWidthNew)+changeWidth;
												v.setX((screenWidth-keypadWidthNew)+changeWidth);
												newPosX -= changeWidth;
											}
											else if (newPosX < 0)
											{	
												newPosX = changeWidth;
												v.setX(newPosX);
												newPosY -= changeWidth;
											}
										}

										//set Y align-buttom
										if (newPosY+(v.getHeight()*newScaleY) > screenHeight)
										{
											newPosY = (screenHeight-keypadHeightNew)+changeHeight;
											v.setY(newPosY);
											newPosY -= changeHeight;
										}
										//align-top
										else if (newPosY < 0) 
										{
											newPosY = changeHeight;
											v.setY(newPosY);
											newPosY -= changeHeight;
										}*/
										
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

										//new label size
										/*Float newTextSizeLabel;

										if (newScaleX <= newScaleY)
										{
											newTextSizeLabel = styleSize * newScaleX;										
										}
										else
										{
											newTextSizeLabel = styleSize * newScaleY;
										}*/
										
										//sizeButton
										MyDataObject.setMap(theKey, String.format("%.0f,%.0f", newWidth, newHeight));
										//labelSize
										//MyDataObject.setMap(ENUM_keypad.labelSize.toString(), String.format(Locale.US, "%.2f", newTextSizeLabel));
										
if (MainActivity.debugMode)
{
	MainActivity.text1.setText(((Button)view).getText().toString()+": "+theKey);
	MainActivity.text2.setText("size: "+view.getWidth()+","+view.getHeight());
	MainActivity.text3.setText("scale: "+view.getScaleX()+","+view.getScaleY());
	MainActivity.text4.setText("new size: "+String.format("%.0f,%.0f", newWidth, newHeight));											
}
										
										break;
								}
							}
							else if (MyDataObject.getIntID() == R.id.menu_custom_alphaKeypad)
							{
								int action = event.getAction();

								switch (action)
								{
									case event.ACTION_DOWN:

										break;

									case event.ACTION_MOVE:

										break;

									case event.ACTION_UP:
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
							//action control emulator
							else
							{
								if (event.getAction()==MotionEvent.ACTION_DOWN)
								{
									handler.onButtonDown(code);
									view.getBackground().setColorFilter(Color.parseColor("#50000000"), PorterDuff.Mode.SRC_ATOP);
									//view.invalidate();
									//view.setScaleX(0.9f);
									//view.setScaleY(0.9f);
								}
								else if (event.getAction()==MotionEvent.ACTION_UP || event.getAction()==MotionEvent.ACTION_CANCEL)
								{
									handler.onButtonUp(code);
									view.getBackground().clearColorFilter();
									//view.invalidate();
									//view.setScaleX(1);
									//view.setScaleY(1);
								}
							}
						
							return true;
						}
					});
					
				MainActivity.fr.addView(btn, params);
			}
		}
	}
	
	private static HashMap<String[][],int[][]> getExternalKeypadLabel(int keypadType)
	{
		String fileName=null;

		switch(keypadType)
		{
			case R.id.keypad_potrait:
				fileName = "keypad_potrait.txt";
				break;

			case R.id.keypad_numpad:
				fileName = "keypad_num.txt";
				break;

			case R.id.keypad_dpad:
				fileName = "keypad_dpad.txt";
				break;

			case R.id.keypad_dpad_plus:
				fileName = "keypad_dpad_diagonal.txt";
				break;
		}

		File labelFile = new File(Environment.getExternalStorageDirectory(),PropertiesManager.PROPERTIES_DIRECTORY+"keypad/"+fileName);

		String fileTeks = "";

		try
		{
			Scanner scan = new Scanner(labelFile);

			while (scan.hasNextLine())
			{
				String data = scan.nextLine();

				fileTeks = fileTeks + data;
			}

			scan.close();
		}
		catch (IOException e)
		{}

		fileTeks = fileTeks.replaceAll("\\s", "").replaceAll("^\\{", "").replaceAll("\\}$", ""); //menghapus spasi

		String[] rowLabel = fileTeks.split("\\}\\{");

        // Array untuk tipe data int
        String[][] arrLabel = new String[rowLabel.length][];

		// Array untuk tipe data String
        int[][] arrCode = new int[rowLabel.length][];

        // Memproses setiap bagian data untuk mengisi array intArray dan strArray
		for (int i = 0; i < rowLabel.length; i++)
		{
            String[] elements = rowLabel[i].split(",");

			arrLabel[i] = new String[elements.length];
			arrCode[i] = new int[elements.length];

			for (int j=0; j<elements.length; j++)
			{
				String[] s = elements[j].split("=");	

				if (s[0].contains("null"))
				{
					s[0] = null;
				}

				arrLabel[i][j] = s[0];
				arrCode[i][j] = Integer.decode(s[1]);	

			}
   	    }

		HashMap<String[][],int[][]> map = new HashMap<>();
		map.put(arrLabel, arrCode);

		return map;
	}
}
