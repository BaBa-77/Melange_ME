package io.github.usernameak.brewemulator;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.ContextThemeWrapper;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TableLayout;
import android.widget.TableRow;
import java.io.File;
import android.os.Environment;
import java.io.IOException;
import android.widget.LinearLayout;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.view.View.OnTouchListener;
import java.util.HashMap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import java.util.Map;
import java.util.Scanner;
import androidx.appcompat.app.AppCompatActivity;
import java.util.Locale;
import android.widget.RelativeLayout;
import java.util.ArrayList;

public class EmulatorKeypad extends AppCompatActivity
{
	private static float newScaleX = 1.0f, newScaleY = 1.0f;
	private static int beforeID = 0;
	private static Long touchEnd = 0L;
	public static ArrayList<String> listRec_keypad;
	
    /*public static final String[][] KEYPAD_LABELS_ori = {
	 {"1", "2", "3", "-", "↑", "-"},
	 {"4", "5", "6", "←", "🆗", "→"},
	 {"7", "8", "9", "+", "↓", null},
	 {"*", "0", "#", "S", "C", "E"},
	 };

	 public static final int[][] KEYPAD_KEYCODES_ori = {
	 {0xE022, 0xE023, 0xE024, 0xE036, 0xE031, 0xE037},
	 {0xE025, 0xE026, 0xE027, 0xE033, 0xE035, 0xE034},
	 {0xE028, 0xE029, 0xE02A, 0xE083, 0xE032, 0xE010},
	 {0xE02B, 0xE021, 0xE02C, 0xE02F, 0xE030, 0xE02E},
	 };*/
	 
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

    @SuppressLint("ClickableViewAccessibility")
    public static ViewGroup buildKeypad(Context ctx, final IKeypadHandler handler, int typePad)
	{
		final Context theContext = ctx;
		TableLayout tl = new TableLayout(ctx);
		
		//get data from properties save-data
		HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);	
	
		//data label
		final Float styleSize = MyFormat.toFloat(myConfigSave.get(ENUM_keypad.labelSize.toString()), 25.00f);
		int styleColor = MyFormat.toColorInt(myConfigSave.get(ENUM_keypad.labelColor.toString()), Color.BLACK);
		
		//data margin
		int styleMarginTop = MyFormat.toInt(myConfigSave.get(ENUM_keypad.marginTop.toString()), 1);
		int styleMarginBottom = MyFormat.toInt(myConfigSave.get(ENUM_keypad.marginBottom.toString()), 1);
		int styleMarginRight = MyFormat.toInt(myConfigSave.get(ENUM_keypad.marginRight.toString()), 1);
		int styleMarginLeft = MyFormat.toInt(myConfigSave.get(ENUM_keypad.marginLeft.toString()), 1);
		//
		Boolean bgImage = MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useBgBtn.toString()));
		Boolean extKeypad = MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useExtLabel.toString()));	
	
		//load data, size of button
		int btnWidth = 125, btnHeight = 125;

		switch(typePad)
		{
			case R.id.keypad_potrait:
				int keypadWidth = MyFormat.toInt(myConfigSave.get(ENUM_keypad.buttonWidth.toString()), btnWidth);
				int keypadHeight = MyFormat.toInt(myConfigSave.get(ENUM_keypad.buttonHeigt.toString()), btnHeight);							
				btnWidth = keypadWidth;
				btnHeight = keypadHeight;
				break;

			case R.id.keypad_numpad:
				int keypadNumpadWidth = MyFormat.toInt(myConfigSave.get(ENUM_keypad.buttonNumpadWidth.toString()), btnWidth);
				int keypadNumpadHeight = MyFormat.toInt(myConfigSave.get(ENUM_keypad.buttonNumpadHeight.toString()), btnHeight);											
				btnWidth = keypadNumpadWidth;
				btnHeight = keypadNumpadHeight;
				break;

			case R.id.keypad_dpad:
				int keypadDpadWidth = MyFormat.toInt(myConfigSave.get(ENUM_keypad.buttonDpadWidth.toString()), btnWidth);
				int keypadDpadHeight = MyFormat.toInt(myConfigSave.get(ENUM_keypad.buttonDpadHeight.toString()), btnHeight);							
				btnWidth = keypadDpadWidth;
				btnHeight = keypadDpadHeight;
				break;
		}

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

		for (int row = 0; row < KEYPAD_LABELS.length; row++)
		{
			TableRow tr = new TableRow(ctx);

			for (int column = 0; column < KEYPAD_LABELS[row].length; column++)
			{
				String label = KEYPAD_LABELS[row][column];
				if (label != null)
				{
					Button btn = new Button(new ContextThemeWrapper(ctx, R.style.KeypadButton), null, 0);
					btn.setLayoutParams(new TableRow.LayoutParams(column));
					btn.setText(KEYPAD_LABELS[row][column]);
				
					//set label
					final int code = KEYPAD_KEYCODES[row][column];
					btn.setPadding(0, 0, 0, 0);
					btn.setTextSize(styleSize);
					btn.setTextColor(styleColor);
					
					//set background & size
					//make background for button
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
					
					//make listener for the button
					btn.setOnTouchListener(new OnTouchListener()
						{	
							private float lastTouchX, lastTouchY;
							private long pressStartTime;
							private boolean longPressed;
							Long touchStart;
						
							@Override
							public boolean onTouch(View view, MotionEvent event)
							{
								//get parent view
								View v = (View) view.getParent(); // 'tableRow' kolom tabel
								v = (View) v.getParent(); // 'viewGroup' keypad
								int parentID = v.getId();

								if (parentID != beforeID)
								{
									//scale
									newScaleX = v.getScaleX();
									newScaleY = v.getScaleY();

									// set/update beforeID
									beforeID = parentID;
								}

								//mendapatkan ukuran layar
								int screenWidth = MainActivity.fr.getWidth();
								int screenHeight = MainActivity.fr.getHeight();
								
								//mendapatkan ukuran view 'tl' (keypad)
								int keypadWidth = v.getWidth();
								int keypadHeight = v.getHeight();

								//new size keyoad
								float keypadWidthNew = (float) keypadWidth*newScaleX;
								float keypadHeightNew = (float) keypadHeight*newScaleY;

								//size (one-side) change 
								float changeWidth = (keypadWidthNew - keypadWidth) / 2.0f;
								float changeHeight = (keypadHeightNew - keypadHeight) / 2.0f;

								/*if (MainActivity.debugMode)
								{
									MainActivity.text1.setText("Screen: "+screenWidth+" , "+screenHeight);
									MainActivity.text2.setText("Keypad: "+keypadWidth+" , "+keypadHeight);								
									MainActivity.text3.setText(String.format("Scale: %.2f , %.2f", newScaleX, newScaleY));
									MainActivity.text4.setText("Keypad*(scale): "+keypadWidthNew+" , "+keypadHeightNew);
									MainActivity.text5.setText(String.format("Changed: %.2f , %.2f", changeWidth, changeHeight));
								}*/
								
								//int action = event.getAction();
								
								if (MyDataObject.getIntID() == R.id.menu_rec_cap)
								{
									if (event.getAction() == MotionEvent.ACTION_DOWN)
									{
										handler.onButtonDown(code);
										touchStart = System.currentTimeMillis();

										MyKeypad_AutoInput.listRec_keypad.add((touchStart-touchEnd)+"");
										/*if (listRec_keypad.size() >= 1)
										 {
										 int lastIndex = listRec_keypad.size()-1;
										 listRec_keypad.set(lastIndex, listRec_keypad.get(lastIndex)+(touchStart-touchEnd));
										 }*/

										view.setScaleX(0.8f);
										view.setScaleY(0.8f);
									}
									else if (event.getAction()==MotionEvent.ACTION_UP || event.getActionButton()==MotionEvent.ACTION_CANCEL)
									{									
										handler.onButtonUp(code);
										touchEnd = System.currentTimeMillis();

										MyKeypad_AutoInput.listRec_keypad.add(code+"-"+(touchEnd-touchStart)+"-");
										//listRec_keypad.add(code+"-"+(touchEnd-touchStart)+"-");

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
									//lock, no effect touching
								}
								//action for custom potition
								else if (MyDataObject.getIntID() == R.id.menu_custom_move)
								{
									float x = event.getRawX();
									float y = event.getRawY();

									switch (event.getAction())
									{
										case MotionEvent.ACTION_DOWN:
											//potition
											float posX = v.getX();
											float posY = v.getY();

											//centering
											if (newScaleX==1.0f && newScaleY==1.0f)
											{
												if (posX+keypadWidth > screenWidth || posX < 0)
												{
													v.setX((screenWidth/2)-(keypadWidth/2));							
												}

												if (posY+keypadHeight > screenHeight || posY < 0)
												{
													v.setY((screenHeight/2)-(keypadHeight/2));
												}
											}		

											lastTouchX = x;
											lastTouchY = y;

											pressStartTime = System.currentTimeMillis();
											longPressed = false;
											break;

										case MotionEvent.ACTION_MOVE:
											float deltaX = x - lastTouchX;
											float deltaY = y - lastTouchY;

											// Menyesuaikan perpindahan dengan faktor skala
											deltaX /= newScaleY;
											deltaY /= newScaleY;

											// Menggeser view
											float newX = (v.getX() + deltaX);
											float newY = (v.getY() + deltaY);

											if (newX >= 0+changeWidth && newX<=screenWidth-keypadWidthNew+changeWidth)
											{
												//set X
												v.setX(newX);
											}

											if (newY >= 0+changeHeight && newY <= screenHeight-keypadHeightNew+changeHeight)
											{
												//set Y
												v.setY(newY);
											}

											lastTouchX = x;
											lastTouchY = y;

											if (deltaX<20 && deltaY<20 && deltaX>-20 && deltaY>-20)
											{
												pressStartTime = System.currentTimeMillis();
											}
											break;

										case MotionEvent.ACTION_UP:
											long pressDuration = System.currentTimeMillis() - pressStartTime;

											if (pressDuration > 3000 && !longPressed) 
											{
												MyLauncherActivity.toastMe(theContext, "HoldON");
												longPressed = true;
											}

											//potition
											posX = v.getX();
											posY = v.getY();

											//new potition
											float newPosX = posX - changeWidth;
											float newPosY = posY - changeHeight;						

											Enum keyPos = null;

											switch (parentID)
											{
												case R.id.keypad_potrait:
													keyPos = ENUM_keypad.posKeypad;
													break;

												case R.id.keypad_numpad:
													keyPos = ENUM_keypad.posNumpad;													
													break;

												case R.id.keypad_dpad:
													keyPos = ENUM_keypad.posDpad;												
													break;
											}

											//simpan sementara posisi keypad										
											MyDataObject.setMap(keyPos.toString(), String.format(Locale.US, "%.2f,%.2f", newPosX, newPosY));										
											break;
									}
								}
								//action for custom size
								else if (MyDataObject.getIntID() == R.id.menu_custom_size)
								{
									float x = event.getRawX();
									float y = event.getRawY();

									switch(event.getActionMasked())
									{
										case MotionEvent.ACTION_DOWN:
											lastTouchX = x;
											lastTouchY = y;
											break;

										case MotionEvent.ACTION_MOVE:
											float deltaX = x - lastTouchX;
											float deltaY = lastTouchY - y;

											//menghitung untuk skala baru
											float i_scaleX = newScaleX + (deltaX / 1000.0f);
											float i_scaleY = newScaleY + (deltaY / 1000.0f);

											float maxScaleX = (float) screenWidth / keypadWidth;
											float maxScaleY = (float) screenHeight / keypadHeight;

											if (i_scaleX <= maxScaleX)
											{
												v.setScaleX(i_scaleX);
											}

											if (i_scaleY <= maxScaleY)
											{
												v.setScaleY(i_scaleY);
											}

											break;

										case MotionEvent.ACTION_UP:
											//menyimpan skala baru
											newScaleX = v.getScaleX();
											newScaleY = v.getScaleY();

											//potition
											float posX = v.getX();
											float posY = v.getY();

											//new size keyoad
											keypadWidthNew = (float) keypadWidth*newScaleX;
											keypadHeightNew = (float) keypadHeight*newScaleY;

											//size (one-side) change 
											changeWidth = (keypadWidthNew - keypadWidth) / 2.0f;
											changeHeight = (keypadHeightNew - keypadHeight) / 2.0f;

											//now size view (button)
											float newBtnWidth = view.getWidth() * newScaleX;
											float newBtnHeight = view.getHeight() * newScaleY;

											//new potition
											float newPosX =  posX - changeWidth;
											float newPosY = posY - changeHeight;	

											//set posisi jika ukuran skala keluar layar
											if (parentID == R.id.keypad_potrait)
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
											}

											Enum keyPos = null, keyW=null, keyH=null;

											switch (parentID)
											{
												case R.id.keypad_potrait:
													keyPos = ENUM_keypad.posKeypad;
													keyW = ENUM_keypad.buttonWidth;
													keyH = ENUM_keypad.buttonHeigt;
													break;

												case R.id.keypad_numpad:
													keyPos = ENUM_keypad.posNumpad;
													keyW = ENUM_keypad.buttonNumpadWidth;
													keyH = ENUM_keypad.buttonNumpadHeight;
													break;

												case R.id.keypad_dpad:
													keyPos = ENUM_keypad.posDpad;
													keyW = ENUM_keypad.buttonDpadWidth;
													keyH = ENUM_keypad.buttonDpadHeight;
													break;
											}
											
											//new label size
											Float newTextSizeLabel;
											
											if (newScaleX <= newScaleY)
											{
												newTextSizeLabel = styleSize * newScaleX;										
											}
											else
											{
												newTextSizeLabel = styleSize * newScaleY;
											}

											//simpan sementara										
											//pos
											MyDataObject.setMap(keyPos.toString(), String.format(Locale.US, "%.2f,%.2f", (newPosX),(newPosY)));
											//BtnWidth
											MyDataObject.setMap(keyW.toString(), String.format(Locale.US, "%.0f", newBtnWidth));
											//BtnHeight
											MyDataObject.setMap(keyH.toString(), String.format(Locale.US, "%.0f", newBtnHeight));	
											//labelSize
											MyDataObject.setMap(ENUM_keypad.labelSize.toString(), String.format(Locale.US, "%.2f", newTextSizeLabel));
											break;
									}
								}							
								else //action for control emulator
								{
//									switch(event.getAction())
//									{
//										case MotionEvent.ACTION_DOWN:
//											handler.onButtonDown(code);
//											view.getBackground().setColorFilter(Color.parseColor("#50000000"), PorterDuff.Mode.SRC_ATOP);
//											//view.invalidate();
//											view.setScaleX(0.9f);
//											view.setScaleY(0.9f);
//											break;
//
//										case MotionEvent.ACTION_CANCEL:
//											handler.onButtonUp(code);
//											view.getBackground().clearColorFilter();
//											//view.invalidate();
//											view.setScaleX(1.0f);
//											view.setScaleY(1.0f);
//											break;
//
//										case MotionEvent.ACTION_UP:
//											handler.onButtonUp(code);
//											view.getBackground().clearColorFilter();
//											//view.invalidate();
//											view.setScaleX(1.0f);
//											view.setScaleY(1.0f);
//											break;
//									}

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
								}

								return true;
							}
						}); //end of listener

					//set margin
					TableRow.LayoutParams btn_params = (TableRow.LayoutParams) btn.getLayoutParams();
					btn_params.setMargins(styleMarginLeft, styleMarginTop, styleMarginRight, styleMarginBottom);
					tr.addView(btn, btn_params);	
				} //end of label=null
			} //end of looping 'tr'

			tl.addView(tr);
		} //end of looping 'label length'

		return tl;
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
