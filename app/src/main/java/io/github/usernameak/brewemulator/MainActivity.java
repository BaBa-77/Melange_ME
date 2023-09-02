package io.github.usernameak.brewemulator;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;
import android.graphics.Color;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;
import android.view.Window;
import android.view.WindowManager;
import android.content.DialogInterface;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.Context;
import java.util.HashMap;
import android.widget.TextView;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;
import android.view.SubMenu;
import java.util.ArrayList;
import java.util.Map;
import android.graphics.Typeface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.graphics.drawable.BitmapDrawable;
import android.content.SharedPreferences;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.widget.Toolbar;
import java.util.Locale;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileReader;
import android.os.Handler;
import android.widget.FrameLayout;
import android.view.Choreographer.FrameCallback;
import android.view.Gravity;
import java.io.FileNotFoundException;
import java.util.Iterator;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.graphics.PorterDuff;
import android.transition.Visibility;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.CheckBox;
import android.widget.ScrollView;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.RadioGroup;
import android.widget.Switch;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.widget.EditText;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.text.TextWatcher;
import android.text.Editable;
import android.widget.RadioButton;

public class MainActivity extends AppCompatActivity
{
	public static final Boolean debugMode = false;
	
    static
	{
		if (!debugMode)
		{
			final int b = MyFormat.toInt(PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.libChoose.toString()), 0);

			/*final String resoW_config = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.resoWidth.toString());
			final String resoH_config = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.resoHeight.toString());
			final String resoW_saved = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig, ENUM_breConfig.width.toString());
			final String resoH_saved = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig, ENUM_breConfig.height.toString());

			final Handler waitReso = new Handler();
			waitReso.postDelayed(new Runnable()
				{
					@Override
					public void run()
					{
						if (resoW_config==resoW_saved && resoH_config==resoH_saved)
						{
							//done
							if (b == 0)
							{
								System.loadLibrary("brewemu");
							}
							else
							{
								System.loadLibrary("brewemu2");
							}
						}
						else
						{
							waitReso.postDelayed(this, 500);						
						}
					}
				}, 500L);*/
				
			if (b == 0)
			{
				System.loadLibrary("brewemu");
			}
			else
			{
				System.loadLibrary("brewemu2");
			}
		}
		else
		{
			MyLog.clear();
		}
    }
	
	/*public void brewEmuJNIStartup(){};
	public void brewEmuKeyDown(int i){Toast.makeText(this, ""+i, Toast.LENGTH_LONG).show();};
	public void brewEmuKeyUp(int i){}*/

	public native boolean brewEmuKeyUp(int keyCode);
	public native boolean brewEmuKeyDown(int keyCode);
	public native void brewEmuJNIStartup();
    //public static native void brewEmuJNIShutdown();
	
	public static int theOrientation;
	private int bgColor;

	private Boolean barUI, fullScreen, bgImage;
	private Boolean newConfig;
	
	public static RelativeLayout fr;
	private EmulatorSurfaceView surfaceView;
	private ViewGroup keypad, keypad_num, keypad_dpad;
	
	Toolbar toolbar;
	
	private ArrayList<Integer> listRec_code;
	private ArrayList<Long> listRec_hold;
	private ArrayList<Long> listRec_delay;
	private Handler mainHandler;
	private Runnable mainTask;
	private int numRun;
	
	private int keypadLayout = 0;
	private Boolean setDone = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);

		newConfig = getIntent().getBooleanExtra("newConfig", false);
		
		doSet();
		create_LayoutView();
		
		//set toolbar
		if (!barUI)
		{
			//hiding toolbar
			hideBar();
		}
				
		setUseLandscapeOrientation(true);
		
		final Handler waitOnCreate = new Handler();
		waitOnCreate.postDelayed(new Runnable()
			{
				@Override
				public void run()
				{
					if (setDone)
					//if (fr.getWidth() != 0)
					{
						setDone = null;
						
						final String resoW_config = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.resoWidth.toString());
						final String resoH_config = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.resoHeight.toString());
						final String resoW_saved = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig, ENUM_breConfig.width.toString());
						final String resoH_saved = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig, ENUM_breConfig.height.toString());

						//final Handler waitReso = new Handler();
						waitOnCreate.postDelayed(new Runnable()
							{
								@Override
								public void run()
								{
									if (resoW_config.equals(resoW_saved) && resoH_config.equals(resoH_saved))
									{																	
										//done
										create_Monitor();
										create_Controller();
										
										if (debugMode)
										{
											debugText();
										}
										
										if (!newConfig)
										{
											brewEmuJNIStartup();
										}
										
										//make ToolBar (ActionBar)
										toolbar = new Toolbar(MainActivity.this);
										toolbar.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT));
										fr.addView(toolbar);
										setSupportActionBar(toolbar);
										getSupportActionBar().setTitle("");
										getSupportActionBar().setDisplayHomeAsUpEnabled(false);
									}
									else
									{
										waitOnCreate.postDelayed(this, 100L);						
									}
								}
							}, 5L);
					}
					else
					{
						waitOnCreate.postDelayed(this, 500);
					}
				}	
		}, 500);
		
		setDone = true;
	}
	
	//melakukan konfigurasi pada aplikasi sesuai setelan config
	private void doSet() 
	{
		//Get save-data from file properties
		HashMap<String,String> myConfig_save = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);

		//set data from save-data
		theOrientation = MyFormat.toInt(myConfig_save.get(ENUM_general.orientation.toString()), 0);
		bgColor = MyFormat.toColorInt(myConfig_save.get(ENUM_general.bgColor.toString()), Color.BLUE);
		bgImage = MyFormat.toBoolean(myConfig_save.get(ENUM_general.useBg.toString()));
		barUI = MyFormat.toBoolean(myConfig_save.get(ENUM_general.bar.toString()));
		fullScreen = MyFormat.toBoolean(myConfig_save.get(ENUM_general.fullscreen.toString()));
		keypadLayout = MyFormat.toInt(myConfig_save.get(ENUM_keypad.keypadLayout.toString()), 0);
	}
	
	private void create_LayoutView() 
	{
		if (fullScreen)
		{
			setTheme(R.style.fullscreenTheme);
		}

		setContentView(R.layout.emu);
		fr = findViewById(R.id.emuLayout);
		fr.setFocusable(false);
		
		if (bgImage)
		{
			Bitmap bgImage = BitmapFactory.decodeFile("/sdcard/MelangeBREW/sys/bg.png");
			fr.setBackground(new BitmapDrawable(getResources(), bgImage));
			//lbgImage.recycle();
		}
		else
		{
			fr.setBackgroundColor(bgColor);
		}
	}
	
	private void create_Monitor() 
	{
		//Get save-data from file properties
		HashMap<String,String> reso_save = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig);
		HashMap<String,String> myConfig_save = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);

		//Boolean strecthOn = Boolean.parseBoolean(monitor_save.get(ENUM_monitor.stretch.toString()));
		int resoX = MyFormat.toInt(reso_save.get(ENUM_breConfig.width.toString()), 240);
		int resoY = MyFormat.toInt(reso_save.get(ENUM_breConfig.height.toString()), 320);
		final Float scaleXY = MyFormat.toFloat(myConfig_save.get(ENUM_monitor.scale.toString()), 100f);
		
		//membuat SurfaceView
		surfaceView = new EmulatorSurfaceView(this);
		
		int i_w = (int) (resoX*(scaleXY/100f));
		int i_h = (int) (resoY*(scaleXY/100f));
		surfaceView.setLayoutParams(new RelativeLayout.LayoutParams(i_w, i_h));
		
if (debugMode)
{
	surfaceView.setBackgroundColor(Color.parseColor("#ff00cc00"));
}

		//meletakkan posisi monitor
		if (theOrientation == 0)
		{
			String str_monitorXY_potrait  = myConfig_save.get(ENUM_monitor.posScreenPotrait.toString());		
			
			if (str_monitorXY_potrait != null)
			{
				String[] arrSplit = new String[2];
				arrSplit = str_monitorXY_potrait.split(",");

				Float monitorX = MyFormat.toFloat(arrSplit[0], 0f);
				Float monitorY = MyFormat.toFloat(arrSplit[1], 0f);

				surfaceView.setX(monitorX);
				surfaceView.setY(monitorY);
				
				fr.addView(surfaceView);
			}
			else
			{
				//set gravity of 'SurfaceView'
				RelativeLayout.LayoutParams surfaceParams = (RelativeLayout.LayoutParams) surfaceView.getLayoutParams();
				surfaceParams.addRule(RelativeLayout.ALIGN_TOP);
				surfaceParams.addRule(RelativeLayout.CENTER_HORIZONTAL);
				surfaceParams.setMargins(0,10,0,0);
				
				fr.addView(surfaceView, surfaceParams);
			}
			
			surfaceView.setId(R.id.monitor_potrait);
		}
		else if (theOrientation == 1)
		{
			String str_monitorXY_landscape  = myConfig_save.get(ENUM_monitor.posScreenLandscape.toString());
			
			if (str_monitorXY_landscape != null)
			{
				String[] arrSplit = new String[2];
				arrSplit = str_monitorXY_landscape.split(",");

				Float monitorX = MyFormat.toFloat(arrSplit[0], 0f);
				Float monitorY = MyFormat.toFloat(arrSplit[1], 0f);

				surfaceView.setX(monitorX);
				surfaceView.setY(monitorY);
				
				fr.addView(surfaceView);
			}
			else
			{
				//set gravity of 'SurfaceView'
				RelativeLayout.LayoutParams surfaceParams = (RelativeLayout.LayoutParams) surfaceView.getLayoutParams();
				surfaceParams.addRule(RelativeLayout.ALIGN_TOP);
				surfaceParams.addRule(RelativeLayout.CENTER_HORIZONTAL);
				surfaceParams.setMargins(0,10,0,0);
				
				fr.addView(surfaceView, surfaceParams);
			}
			
			surfaceView.setId(R.id.monitor_landscape);
		}
		
		//membuat posisi monitor bisa dipindah
		surfaceView.setOnTouchListener(new OnTouchListener()
			{
				private float i_rawX=0.0f, i_rawY=0.0f, i_posX=0.0f, i_posY=0.0f;
				private float lastTouchX, lastTouchY;
				private long pressStartTime;
				private boolean longPressed;
				float newScaleXY = 1.0f;
				
				@Override
				public boolean onTouch(View view, MotionEvent event)
				{
					//mendapatkan ukuran layar
					//View rootView = view.getRootView();
					int screenWidth = fr.getWidth();
					int screenHeight = fr.getHeight();

					//mendapatkan ukuran view
					int monitorWidth = view.getWidth();
					int monitorHeight = view.getHeight();

					//new size keyoad
					float monitorWidthNew = (float) monitorWidth*newScaleXY;
					float monitorHeightNew = (float) monitorHeight*newScaleXY;

					//size (one-side) change 
					float changeWidth = (monitorWidthNew - monitorWidth) / 2;
					float changeHeight = (monitorHeightNew - monitorHeight) / 2;
					
					/*if (MainActivity.debugMode)
					{
						MainActivity.text1.setText("Screen: "+screenWidth+" , "+screenHeight);
						MainActivity.text2.setText("monitor: "+monitorWidth+" , "+monitorHeight);								
						MainActivity.text3.setText(String.format("Scale: %.2f", newScaleXY));
						MainActivity.text4.setText("monitor*(scale): "+monitorWidthNew+" , "+monitorHeightNew);
						MainActivity.text5.setText(String.format("Changed: %.2f , %.2f", changeWidth, changeHeight));
					}*/
					
					//custom posisi
					if (MyDataObject.getIntID() == R.id.menu_custom_move)
					{
						{
							float x = event.getRawX();
							float y = event.getRawY();
							
							switch (event.getAction())
							{
								case MotionEvent.ACTION_DOWN:
									//potition
									float posX = view.getX();
									float posY = view.getY();

									//centering
									if (newScaleXY == 1.0f)
									{
										if (posX+monitorWidth > screenWidth || posX < 0)
										{
											view.setX((screenWidth/2)-(monitorWidth/2));							
										}

										if (posY+monitorHeight > screenHeight || posY < 0)
										{
											view.setY((screenHeight/2)-(monitorHeight/2));
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
									deltaX /= newScaleXY;
									deltaY /= newScaleXY;
									
									// Menggeser view
									float newX = (view.getX() + deltaX);
									float newY = (view.getY() + deltaY);

									if (newX >= 0+changeWidth && newX<=screenWidth-monitorWidthNew+changeWidth)
									{
										//set X
										view.setX(newX);
									}

									if (newY >= 0+changeHeight && newY <= screenHeight-monitorHeightNew+changeHeight)
									{
										//set Y
										view.setY(newY);
									}
									
									lastTouchX = x;
									lastTouchY = y;
									
									if (deltaX<20 && deltaY<20 && deltaX>-20 && deltaY>-20)
									{
										pressStartTime = System.currentTimeMillis();
										longPressed = false;
									}
									break;
									
								case MotionEvent.ACTION_UP:
									long pressDuration = System.currentTimeMillis() - pressStartTime;

									if (pressDuration > 3000 && !longPressed) 
									{
										// Aksi yang akan dijalankan saat terjadi tekan lama
										// Misalnya menampilkan pesan toast
										Toast.makeText(MainActivity.this, "Tombol ditekan lama!", Toast.LENGTH_LONG).show();
										longPressed = true;
									}
									
									String posMonitor = null;
									
									switch(theOrientation)
									{
										case 0:
											posMonitor = ENUM_monitor.posScreenPotrait.toString();
											break;
											
										case 1:
											posMonitor = ENUM_monitor.posScreenLandscape.toString();
											break;
									}
									
									//potition
									posX = view.getX();
									posY = view.getY();

									//new potition								
									float newPosX = posX - changeWidth;
									float newPosY = posY - changeHeight;	
									
									//simpan data									
									MyDataObject.setMap(posMonitor, String.format(Locale.US, "%.2f,%.2f", newPosX, newPosY));				
									break;
							}
						}
					}
					//atur skala monitor
					else if (MyDataObject.getIntID() == R.id.menu_custom_size)
					{
						switch(event.getActionMasked())
						{
							case MotionEvent.ACTION_DOWN:
								//posisi sentuh awal
								i_rawX = event.getRawX();
								i_rawY = event.getRawY();
								
								if (debugMode)
								{
									text5.setText("RAW: X="+i_rawX+" Y="+i_rawY);
								}			
								
								break;

							case MotionEvent.ACTION_MOVE:
								//posisi sentuhan setelah melakukan pergerakan
								float nowX = event.getRawX();
								float nowY = event.getRawY();
								
								if (debugMode)
								{
									text4.setText("raw: "+i_rawX+","+i_rawY+" | now: "+nowX+","+nowY);
								}

								//menghitung untuk skala baru
								float i_scaleX = ((nowX - i_rawX) / 1000);
								float i_scaleY = ((i_rawY - nowY) / 1000);
								float i_scaleXY = newScaleXY + i_scaleX + i_scaleY;							
								
								//menentukan batas maksimal skala
								float maxScaleX = (float) screenWidth/monitorWidth;
								float maxScaleY = (float) screenHeight/monitorHeight;
								float maxScaleXY;
								
								if (maxScaleX < maxScaleY)
								{
									maxScaleXY = maxScaleX;
								}
								else
								{
									maxScaleXY = maxScaleY;
								}
								
								//melakukan setScale selama skala tidak melebihi batas yang telah ditentukan
								if (i_scaleXY <= maxScaleXY)
								{
									view.setScaleX(i_scaleXY);
									view.setScaleY(i_scaleXY);
								}
								break;
								
							case MotionEvent.ACTION_UP:
								//menyimpan skala baru
								newScaleXY = view.getScaleX();
								
								//new size keyoad
								monitorWidthNew = (float) monitorWidth*newScaleXY;
								monitorHeightNew = (float) monitorHeight*newScaleXY;
								
								//size (one-side) change 
								changeWidth = (monitorWidthNew - monitorWidth) / 2;
								changeHeight = (monitorHeightNew - monitorHeight) / 2;
						
								//potition
								float posX = view.getX();
								float posY = view.getY();
								
								//new potition
								float newPosX = posX - changeWidth;
								float newPosY = posY - changeHeight;
								
								//centering
								if (newPosX+monitorWidthNew > screenWidth || newPosX < 0)
								{
									newPosX = (screenWidth-(view.getWidth()))/2;
									view.setX(newPosX);	
									newPosX -= changeWidth;
								}

								//set Y align-buttom
								if (newPosY+(view.getHeight()*newScaleXY) > screenHeight)
								{ 
									newPosY = (screenHeight-monitorHeightNew)+changeHeight;
									view.setY(newPosY);
									newPosY -= changeHeight;
								}
								//align-top
								else if (newPosY < 0) 
								{
									newPosY = changeHeight;
									view.setY(newPosY);
									newPosY -= changeHeight;
								}
								
								String posMonitor = null;

								switch(theOrientation)
								{
									case 0:
										posMonitor = ENUM_monitor.posScreenPotrait.toString();
										break;

									case 1:
										posMonitor = ENUM_monitor.posScreenLandscape.toString();
										break;
								}
															
								//simpan sementara data
								//posisi
								MyDataObject.setMap(posMonitor, String.format(Locale.US, "%.2f,%.2f", newPosX, newPosY));	
								//skala
								float saveScale = (scaleXY-100) + (newScaleXY*100);
								MyDataObject.setMap(ENUM_monitor.scale.toString(), String.format(Locale.US, "%.2f", saveScale));							
								break;
						}
					}

					return true;
				}
			});
	}
	
	private void create_Controller() 
	{
		//Get save-data from file properties
		HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);
		
		if (keypadLayout == 0)
		{
			if (theOrientation == 0) //keypad potrait
			{
				keypad = EmulatorKeypad.buildKeypad(this, new EmulatorKeypad.IKeypadHandler(){
						@Override
						public void onButtonDown(int code)
						{
							brewEmuKeyDown(code);
						}

						@Override
						public void onButtonUp(int code)
						{
							brewEmuKeyUp(code);
						}
					}, R.id.keypad_potrait);
				keypad.setId(R.id.keypad_potrait);

				keypad.setLayoutParams(new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));		

				String posKeypadXY = myConfigSave.get(ENUM_keypad.posKeypad.toString());

				if (posKeypadXY != null)
				{
					String[] str_keypadXY = new String[2];
					str_keypadXY = posKeypadXY.split(",");

					Float[] keypadXY = new Float[2];
					keypadXY[0] = MyFormat.toFloat(str_keypadXY[0], 0f);
					keypadXY[1] = MyFormat.toFloat(str_keypadXY[1], 0f);

					keypad.setX(keypadXY[0]);
					keypad.setY(keypadXY[1]);

					fr.addView(keypad);
				}
				else
				{
					//set gravity of 'keypad'
					RelativeLayout.LayoutParams keypadParams = (RelativeLayout.LayoutParams) keypad.getLayoutParams();
					keypadParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
					keypadParams.addRule(RelativeLayout.CENTER_HORIZONTAL);
					keypadParams.setMargins(0,0,0,10);
					fr.addView(keypad, keypadParams);
				}

				//set transparacy
				keypad.setAlpha((Float) (MyFormat.toFloat(myConfigSave.get(ENUM_keypad.transparacy.toString()), 100f) / 100.00f));
			}
			else if (theOrientation == 1) //keypad landscape
			{
				//numPad
				keypad_num = EmulatorKeypad.buildKeypad(this, new EmulatorKeypad.IKeypadHandler() {
						@Override
						public void onButtonDown(int code)
						{
							brewEmuKeyDown(code);
						}

						@Override
						public void onButtonUp(int code)
						{
							brewEmuKeyUp(code);
						}
					}, R.id.keypad_numpad);
				keypad_num.setId(R.id.keypad_numpad);

				//jika menggunakan dpad + diagonal
				if (Boolean.parseBoolean(myConfigSave.get(ENUM_keypad.diagonalOn.toString())))
				{
					//dpad + diagonal				
					keypad_dpad = EmulatorKeypad.buildKeypad(this, new EmulatorKeypad.IKeypadHandler() {
							@Override
							public void onButtonDown(int code)
							{
								if (code == 0x1)
								{
									brewEmuKeyDown(0xE031);
									brewEmuKeyDown(0xE033);
								}
								else if (code == 0x2)
								{
									brewEmuKeyDown(0xE031);
									brewEmuKeyDown(0xE034);
								}
								else if (code == 0x3)
								{
									brewEmuKeyDown(0xE032);
									brewEmuKeyDown(0xE033);
								}
								else if (code == 0x4)
								{
									brewEmuKeyDown(0xE032);
									brewEmuKeyDown(0xE034);
								}
								else
								{
									brewEmuKeyDown(code);
								}
							}

							@Override
							public void onButtonUp(int code)
							{
								if (code == 0x1)
								{
									brewEmuKeyUp(0xE031);
									brewEmuKeyUp(0xE033);
								}
								else if (code == 0x2)
								{
									brewEmuKeyUp(0xE031);
									brewEmuKeyUp(0xE034);
								}
								else if (code == 0x3)
								{
									brewEmuKeyUp(0xE032);
									brewEmuKeyUp(0xE033);
								}
								else if (code == 0x4)
								{
									brewEmuKeyUp(0xE032);
									brewEmuKeyUp(0xE034);
								}
								else
								{
									brewEmuKeyUp(code);
								}
							}
						}, R.id.keypad_dpad_plus);
					keypad_dpad.setId(R.id.keypad_dpad);
				}
				else
				{
					//dpad biasa				
					keypad_dpad = EmulatorKeypad.buildKeypad(this, new EmulatorKeypad.IKeypadHandler() {
							@Override
							public void onButtonDown(int code)
							{
								brewEmuKeyDown(code);
							}

							@Override
							public void onButtonUp(int code)
							{
								brewEmuKeyUp(code);
							}
						}, R.id.keypad_dpad);
					keypad_dpad.setId(R.id.keypad_dpad);
				}

				keypad_num.setLayoutParams(new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
				keypad_dpad.setLayoutParams(new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));

				//meletakkan posisi keypad
				String posDpadXY = myConfigSave.get(ENUM_keypad.posDpad.toString());	
				String posNumpadXY = myConfigSave.get(ENUM_keypad.posNumpad.toString());

				//numpad
				if (posNumpadXY != null)
				{
					String[] str_numpadXY = new String[2];
					str_numpadXY = posNumpadXY.split(",");

					Float[] numpadXY = new Float[2];
					numpadXY[0] = MyFormat.toFloat(str_numpadXY[0], 0f);
					numpadXY[1] = MyFormat.toFloat(str_numpadXY[1], 0f);

					keypad_num.setX(numpadXY[0]);
					keypad_num.setY(numpadXY[1]);

					fr.addView(keypad_num);
				}
				else
				{
					//set gravity of 'keypad_num'
					RelativeLayout.LayoutParams keypad_numParams = (RelativeLayout.LayoutParams) keypad_num.getLayoutParams();
					keypad_numParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
					keypad_numParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
					keypad_numParams.setMargins(0,0,0,50);

					fr.addView(keypad_num, keypad_numParams);
				}

				//dpad
				if (posDpadXY != null)
				{
					String[] str_dpadXY = new String[2];
					str_dpadXY = posDpadXY.split(",");

					Float[] dpadXY = new Float[2];
					dpadXY[0] = MyFormat.toFloat(str_dpadXY[0], 0f);
					dpadXY[1] = MyFormat.toFloat(str_dpadXY[1], 0f);

					keypad_dpad.setX(dpadXY[0]);
					keypad_dpad.setY(dpadXY[1]);

					fr.addView(keypad_dpad);
				}
				else
				{
					//set gravity of 'keypad_dpad'
					RelativeLayout.LayoutParams keypad_dpadParams = (RelativeLayout.LayoutParams) keypad_dpad.getLayoutParams();
					keypad_dpadParams.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
					keypad_dpadParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
					keypad_dpadParams.setMargins(0,0,0,10);

					fr.addView(keypad_dpad, keypad_dpadParams);
				}

				//set transparacy
				keypad_num.setAlpha((Float) (MyFormat.toFloat(myConfigSave.get(ENUM_keypad.transparacy.toString()), 100f) / 100.00f));
				keypad_dpad.setAlpha((Float) (MyFormat.toFloat(myConfigSave.get(ENUM_keypad.transparacy.toString()), 100f) / 100.00f));
			}
		}
		else if (keypadLayout == 1)
		{
			if (theOrientation == 0)
			{
				MyKeypad_Control.createKeypad_Control(this, R.id.keypad_potrait, new MyKeypad_Control.IKeypadHandler()
					{
						@Override
						public void onButtonDown(int code)
						{
							brewEmuKeyDown(code);
						}

						@Override
						public void onButtonUp(int code)
						{
							brewEmuKeyUp(code);
						}
					});
			}
			else if (theOrientation == 1)
			{	
				//build keypad num
				MyKeypad_Control.createKeypad_Control(this, R.id.keypad_numpad, new MyKeypad_Control.IKeypadHandler()
					{
						@Override
						public void onButtonDown(int code)
						{
							brewEmuKeyDown(code);
						}

						@Override
						public void onButtonUp(int code)
						{
							brewEmuKeyUp(code);
						}
					});
					
				if (MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.diagonalOn.toString())))
				{ //dpad + diagonal
					MyKeypad_Control.createKeypad_Control(this, R.id.keypad_dpad_plus, new MyKeypad_Control.IKeypadHandler()
						{
							@Override
							public void onButtonDown(int code)
							{
								if (code == 0x1)
								{
									brewEmuKeyDown(0xE031);
									brewEmuKeyDown(0xE033);
								}
								else if (code == 0x2)
								{
									brewEmuKeyDown(0xE031);
									brewEmuKeyDown(0xE034);
								}
								else if (code == 0x3)
								{
									brewEmuKeyDown(0xE032);
									brewEmuKeyDown(0xE033);
								}
								else if (code == 0x4)
								{
									brewEmuKeyDown(0xE032);
									brewEmuKeyDown(0xE034);
								}
								else
								{
									brewEmuKeyUp(code);
								}
							}

							@Override
							public void onButtonUp(int code)
							{
								if (code == 0x1)
								{
									brewEmuKeyUp(0xE031);
									brewEmuKeyUp(0xE033);
								}
								else if (code == 0x2)
								{
									brewEmuKeyUp(0xE031);
									brewEmuKeyUp(0xE034);
								}
								else if (code == 0x3)
								{
									brewEmuKeyUp(0xE032);
									brewEmuKeyUp(0xE033);
								}
								else if (code == 0x4)
								{
									brewEmuKeyUp(0xE032);
									brewEmuKeyUp(0xE034);
								}
								else
								{
									brewEmuKeyUp(code);
								}
							}
						});
				}
				else // dpad
				{
					
					MyKeypad_Control.createKeypad_Control(this, R.id.keypad_dpad, new MyKeypad_Control.IKeypadHandler()
						{
							@Override
							public void onButtonDown(int code)
							{
								brewEmuKeyDown(code);
							}

							@Override
							public void onButtonUp(int code)
							{
								brewEmuKeyUp(code);
							}
						});
				}
			} // end if 'theOrientation'
		} //end if 'keypadLayout'
	}
	
	private void setPosition_Monitor()
	{
		HashMap<String,String> myConfig_Save = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);
		
		surfaceView.setScaleX(1f);
		surfaceView.setScaleY(1f);
		
		int monitor_width = surfaceView.getWidth();
		//int monitor_height = surfaceView.getHeight();
		
		//meletakkan posisi monitor
		if (theOrientation == 0)
		{
			String str_monitorXY_potrait  = myConfig_Save.get(ENUM_monitor.posScreenPotrait.toString());		

			if (str_monitorXY_potrait != null)
			{
				String[] arrSplit = new String[2];
				arrSplit = str_monitorXY_potrait.split(",");

				Float monitorX = MyFormat.toFloat(arrSplit[0], 0f);
				Float monitorY = MyFormat.toFloat(arrSplit[1], 0f);

				surfaceView.setX(monitorX);
				surfaceView.setY(monitorY);
			}
			else
			{
				float y = 0;
				float x = (fr.getWidth()/2) - (monitor_width/2);
				
				surfaceView.setX(x);
				surfaceView.setY(y);
			}

			surfaceView.setId(R.id.monitor_potrait);
		}
		else if (theOrientation == 1)
		{
			String str_monitorXY_landscape  = myConfig_Save.get(ENUM_monitor.posScreenLandscape.toString());

			if (str_monitorXY_landscape != null)
			{
				String[] arrSplit = new String[2];
				arrSplit = str_monitorXY_landscape.split(",");

				Float monitorX = MyFormat.toFloat(arrSplit[0], 0f);
				Float monitorY = MyFormat.toFloat(arrSplit[1], 0f);

				surfaceView.setX(monitorX);
				surfaceView.setY(monitorY);
			}
			else
			{
				float y = 0;
				float x = (fr.getWidth()/2) - (monitor_width/2);

				surfaceView.setX(x);
				surfaceView.setY(y);
			}
		}
	}
	
	
	
	
	
	
	
	/*public void shutOff()
	{
		brewEmuJNIShutdown();
	}*/

    /*private void hideKeypad()
	{
        keypad.setVisibility(View.GONE);
    }*/

	/*private int translateKeycode(int keyCode)
	 {
	 int avk = 0xE010;
	 switch (keyCode)
	 {
	 case KeyEvent.KEYCODE_DPAD_DOWN: avk = 0xE032; break;
	 case KeyEvent.KEYCODE_DPAD_UP: avk = 0xE031; break;
	 case KeyEvent.KEYCODE_DPAD_LEFT: avk = 0xE033; break;
	 case KeyEvent.KEYCODE_DPAD_RIGHT: avk = 0xE034; break;

	 case KeyEvent.KEYCODE_DPAD_CENTER:
	 case KeyEvent.KEYCODE_ENTER:
	 avk = 0xE035; break;

	 case KeyEvent.KEYCODE_CLEAR:
	 case KeyEvent.KEYCODE_ESCAPE:
	 case KeyEvent.KEYCODE_C:
	 case KeyEvent.KEYCODE_DEL:
	 avk = 0xE030; break;

	 case KeyEvent.KEYCODE_MENU:
	 case KeyEvent.KEYCODE_W:
	 avk = 0xE036; break;

	 case KeyEvent.KEYCODE_BACK:
	 case KeyEvent.KEYCODE_E:
	 avk = 0xE037; break;

	 case KeyEvent.KEYCODE_X:
	 case KeyEvent.KEYCODE_ENDCALL:
	 avk = 0xE02E; break;

	 case KeyEvent.KEYCODE_Z:
	 case KeyEvent.KEYCODE_CALL:
	 avk = 0xE02F; break;

	 case KeyEvent.KEYCODE_0:
	 case KeyEvent.KEYCODE_NUMPAD_0:
	 avk = 0xE021; break;

	 case KeyEvent.KEYCODE_1:
	 case KeyEvent.KEYCODE_NUMPAD_1:
	 avk = 0xE022; break;

	 case KeyEvent.KEYCODE_2:
	 case KeyEvent.KEYCODE_NUMPAD_2:
	 avk = 0xE023; break;

	 case KeyEvent.KEYCODE_3:
	 case KeyEvent.KEYCODE_NUMPAD_3:
	 avk = 0xE024; break;

	 case KeyEvent.KEYCODE_4:
	 case KeyEvent.KEYCODE_NUMPAD_4:
	 avk = 0xE025; break;

	 case KeyEvent.KEYCODE_5:
	 case KeyEvent.KEYCODE_NUMPAD_5:
	 avk = 0xE026; break;

	 case KeyEvent.KEYCODE_6:
	 case KeyEvent.KEYCODE_NUMPAD_6:
	 avk = 0xE027; break;

	 case KeyEvent.KEYCODE_7:
	 case KeyEvent.KEYCODE_NUMPAD_7:
	 avk = 0xE028; break;

	 case KeyEvent.KEYCODE_8:
	 case KeyEvent.KEYCODE_NUMPAD_8:
	 avk = 0xE029; break;

	 case KeyEvent.KEYCODE_9:
	 case KeyEvent.KEYCODE_NUMPAD_9:
	 avk = 0xE02A; break;

	 case KeyEvent.KEYCODE_STAR:
	 case KeyEvent.KEYCODE_S:
	 case KeyEvent.KEYCODE_NUMPAD_MULTIPLY:
	 avk = 0xE02B; break;

	 case KeyEvent.KEYCODE_POUND:
	 case KeyEvent.KEYCODE_D:
	 case KeyEvent.KEYCODE_NUMPAD_DIVIDE:
	 avk = 0xE02C; break;
	 }

	 return avk;
	 }

	 @Override
	 public boolean dispatchKeyEvent(KeyEvent event)
	 {
	 int brewCode = translateKeycode(event.getKeyCode());
	 if (brewCode!=0xE010)
	 {
	 if (event.getAction()==KeyEvent.ACTION_DOWN)
	 {
	 brewEmuKeyDown(brewCode);
	 return true;
	 }
	 else if (event.getAction()==KeyEvent.ACTION_UP)
	 {
	 brewEmuKeyUp(brewCode);
	 return true;
	 }
	 }

	 return super.dispatchKeyEvent(event);
	 }*/
	 
    /*@Override
	 protected void onDestroy()
	 {
	 super.onDestroy();

	 //brewEmuJNIShutdown();
	 }*/

//    public void setUseLandscapeOrientation(boolean value)
//	{
//
//		if (value)
//		{
//			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
//		}
//		else
//		{
//			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
//		}
//    }
	 
	 
	 
	 
	 
	 
	 
	public void setUseLandscapeOrientation(boolean value)
	{
		if (theOrientation == 1)
		{
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		}
		else if (theOrientation == 0)
		{
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
		}
    }

	private void hideBar()
	{
		getSupportActionBar().hide();
		//toolbar.setVisibility(Visibility.MODE_OUT);
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu)
	{
		menu.clear();
		
		menu.add(menu.NONE, R.id.menu_item_screenshot, menu.NONE, R.string.str_screenshot);
		menu.add(menu.NONE, R.id.menu_item_stop, menu.NONE, R.string.str_stop);
		//menu.add(menu.NONE, R.id.menu_item_sett, menu.NONE, R.string.str_sett);
		
		//menu: custom
		if (!MyDataObject.getRec())
		{
			SubMenu itemCustom = menu.addSubMenu(menu.NONE, R.id.menu_custom, menu.NONE, R.string.str_custom);
			itemCustom.add(menu.NONE, R.id.menu_custom_move, menu.NONE, R.string.str_custom_move);
			itemCustom.add(menu.NONE, R.id.menu_custom_size, menu.NONE, R.string.str_custom_size);
			itemCustom.add(menu.NONE, R.id.menu_custom_alphaKeypad, menu.NONE, R.string.str_custom_alphaKeypad);
			
			if (keypadLayout ==1)
			{
				itemCustom.add(menu.NONE, R.id.menu_custom_visibility, menu.NONE, R.string.str_custom_visibility);
			}
		}
		
		if (MyDataObject.getCustom())
		{
			menu.add(menu.NONE, R.id.menu_custom_off, menu.NONE, R.string.str_custom_off);
		}
		
		//menu: Auto-Input
		if (!MyDataObject.getRec())
		{
			if (!MyDataObject.getCustom())
			{
				SubMenu autoInput = menu.addSubMenu(menu.NONE, R.id.menu_rec, menu.NONE, R.string.rec_input);
				autoInput.add(menu.NONE, R.id.menu_rec_cap, menu.NONE, R.string.rec_cap);
				
				if (getListAutoInput() != null)
				{
					autoInput.add(menu.NONE, R.id.menu_rec_play, menu.NONE, R.string.rec_play);
				}
			}
		}
		else
		{
			menu.add(menu.NONE, R.id.menu_rec_finish, menu.NONE, R.string.rec_finish);
		}
		
		//menu: exit
		menu.add(menu.NONE, R.id.menu_item_exit, menu.NONE, R.string.str_exit);
		
		// TODO: Implement this method
		return super.onPrepareOptionsMenu(menu);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		int id = item.getItemId();
		
		//if (ID) Custom
		if (id==R.id.menu_custom_move || id==R.id.menu_custom_size || id==R.id.menu_custom_alphaKeypad || id==R.id.menu_custom_visibility)
		{
			//rebuild keypad spesial for 'Custom'
			if (!MyDataObject.getCustom())
			{
				if (id != R.id.custom)
				{
					if (keypadLayout == 1)
					{
						clearKeypad();

						if (theOrientation == 0)
						{
							MyKeypad_Custom.createKeypad_Custom(this, R.id.keypad_potrait);
						}
						else if (theOrientation == 1)
						{	
							//Get save-data from file properties
							HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);

							//build keypad num
							MyKeypad_Custom.createKeypad_Custom(this, R.id.keypad_numpad);

							if (MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.diagonalOn.toString())))
							{ //dpad + diagonal
								MyKeypad_Custom.createKeypad_Custom(this, R.id.keypad_dpad_plus);
							}
							else
							{ //dpad					
								MyKeypad_Custom.createKeypad_Custom(this, R.id.keypad_dpad);
							}
						} 
					}
				}
			}
		}
		
		//menu: screenshot
		if (id == R.id.menu_item_screenshot)
		{
			MyLauncherActivity.toastMe(this, "Shot");
			
			brewEmuKeyDown(0xE02F);
			
			new Handler().postDelayed(new Runnable()
				{
					@Override
					public void run()
					{
						brewEmuKeyUp(0xE02F);
					}		
			}, 100);	
		}
		
		//menu: main menu
		else if (id == R.id.menu_item_stop)
		{
			AlertDialog.Builder dialog = new AlertDialog.Builder(this);
			dialog.setTitle(R.string.dialog_stop_title);
			dialog.setMessage(R.string.dialig_stop_msg);
			dialog.setPositiveButton(R.string.dialog_stop_btn_positive, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						brewEmuKeyDown(0xE02E);
						brewEmuKeyUp(0xE02E);
					}
				});

			dialog.setNegativeButton(R.string.dialog_stop_btn_negative, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						// TODO: Implement this method
					}	
				});

			AlertDialog dialogAlert = dialog.create();
			dialogAlert.show();

			return true;
		}
		
		//sub-menu: Custom potition
		else if (id == R.id.menu_custom_move)
		{
			MyDataObject.setCustom(true);
			MyDataObject.setIntID(R.id.menu_custom_move);
			
			if (keypadLayout == 1)
			{
				
				AlertDialog.Builder builder = new AlertDialog.Builder(this);
				View layout = getLayoutInflater().inflate(R.layout.custom_move, null);
				builder.setView(layout);
				builder.setTitle("Set for keypad separated");
				//builder.setCancelable(false);

				TextView textTitle = layout.findViewById(R.id.theTitle);
				final EditText perMove = layout.findViewById(R.id.perMove);
				Button btnSave = layout.findViewById(R.id.btnOk);

				//set title
				textTitle.setText(R.string.str_custom_move);

				String saveMovement = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, ENUM_keypad.movementXY.toString());

				if (saveMovement != null)
				{
					perMove.setText(saveMovement);
				}
				else
				{
					perMove.setText(MyKeypad.theMovement+"");
				}

				final AlertDialog dialog = builder.create();
				Window window = dialog.getWindow();

				if (window != null)
				{
					window.setLayout(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
					window.setDimAmount(0.0f);
					window.setGravity(Gravity.CENTER);
					window.setBackgroundDrawableResource(android.R.color.transparent);
				}

				btnSave.setOnClickListener(new OnClickListener()
					{
						@Override
						public void onClick(View p1)
						{
							float theMovement = MyFormat.toFloat(perMove.getText().toString(), 50f);					
							MyKeypad.theMovement = theMovement;

							PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, ENUM_keypad.movementXY.toString(), String.format(Locale.US, "%.1f", theMovement));

							dialog.dismiss();
						}																	
					});

				dialog.show();
			}		
		}
		
		//sub-menu: Custom size
		else if (id == R.id.menu_custom_size)
		{
			MyDataObject.setCustom(true);
			MyDataObject.setIntID(R.id.menu_custom_size);
			
			if (keypadLayout == 1)
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(this);
				View layout = getLayoutInflater().inflate(R.layout.custom_size, null);
				builder.setView(layout);
				builder.setTitle("Set for keypad separated");
				//builder.setCancelable(false);

				TextView textTitle = layout.findViewById(R.id.theTitle);
				final EditText perScale = layout.findViewById(R.id.perScale);
				Button btnSave = layout.findViewById(R.id.btnOk);

				//set title
				textTitle.setText(R.string.str_custom_size);

				String saveScaleUp = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, ENUM_keypad.scaleUp.toString());

				if (saveScaleUp != null)
				{
					perScale.setText(saveScaleUp);
				}
				else
				{
					perScale.setText(MyKeypad.scaleUp+"");
				}

				final AlertDialog dialog = builder.create();
				Window window = dialog.getWindow();

				if (window != null)
				{
					window.setLayout(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
					window.setDimAmount(0.0f);
					window.setGravity(Gravity.CENTER);
					window.setBackgroundDrawableResource(android.R.color.transparent);
				}

				btnSave.setOnClickListener(new OnClickListener()
					{
						@Override
						public void onClick(View p1)
						{
							float theScaleUp = MyFormat.toFloat(perScale.getText().toString(), 50f);					
							MyKeypad.scaleUp = theScaleUp;
							PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, ENUM_keypad.scaleUp.toString(), String.format(Locale.US, "%.3f", theScaleUp));					

							dialog.dismiss();					
						}																	
					});

				dialog.show();
			}
		}
		
		//sub-menu: Custom alpha keypad
		else if (id == R.id.menu_custom_alphaKeypad)
		{
			MyDataObject.setCustom(true);
			
			if (keypadLayout == 0)
			{
				MyDataObject.setIntID(10101);
				
				AlertDialog.Builder builder = new AlertDialog.Builder(this);	
				View layoutView = getLayoutInflater().inflate(R.layout.set_transparacy, null);					
				//LinearLayout layout = layoutView.findViewById(R.id.setTransparacyLayout);
				builder.setView(layoutView);			
				final AlertDialog dialog = builder.create();
				//dialog.setCancelable(false);

				Window window = dialog.getWindow();

				if (window != null)
				{
					window.setLayout(MainActivity.fr.getWidth()/2, RelativeLayout.LayoutParams.WRAP_CONTENT);
					window.setDimAmount(0.0f);
					window.setBackgroundDrawableResource(android.R.color.transparent);
				}

				dialog.show();

				final TextView infoSeek = layoutView.findViewById(R.id.infoSeek);
				SeekBar seekAlpha = layoutView.findViewById(R.id.theSeek);
				ImageButton btnClose = layoutView.findViewById(R.id.closeBtn);

				HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);

				int saveAlpha = MyFormat.toInt(myConfigSave.get(ENUM_keypad.transparacy.toString()), 100);
				
				infoSeek.setText(saveAlpha+"%");	
				seekAlpha.setMax(100);
				seekAlpha.setProgress(saveAlpha);
				
				btnClose.setOnClickListener(new OnClickListener()
					{
						@Override
						public void onClick(View p1)
						{
							dialog.dismiss();
						}
					});

				seekAlpha.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
					{

						@Override
						public void onProgressChanged(SeekBar p1, int p2, boolean p3)
						{
							if (theOrientation == 0)
							{
								keypad.setAlpha((Float) (p2/100.00f));
							}
							else
							{
								keypad_num.setAlpha((Float) (p2/100.00f));
								keypad_dpad.setAlpha((Float) (p2/100.00f));
							}

							infoSeek.setText(p2+"%");

							int width_seekbar = p1.getWidth();
							int width_info = infoSeek.getWidth();

							int perProgress = width_seekbar / p1.getMax();
							int infoX_center = width_info / 2;

							int posX = (p2 * perProgress) - infoX_center;

							if (posX > 0 && posX < fr.getWidth()-width_info-infoX_center)
							{
								infoSeek.setX(posX);
							}
						}

						@Override
						public void onStartTrackingTouch(SeekBar p1)
						{
							p1.getProgressDrawable().setColorFilter(Color.parseColor("#ff00ee00"), PorterDuff.Mode.SRC_IN);
							p1.getThumb().setColorFilter(Color.parseColor("#ff00dd00"), PorterDuff.Mode.SRC_IN);
							//p1.getBackground().setColorFilter(Color.DKGRAY, PorterDuff.Mode.SRC_IN);
						}

						@Override
						public void onStopTrackingTouch(SeekBar p1)
						{
							p1.getProgressDrawable().clearColorFilter();
							p1.getThumb().clearColorFilter();
							//p1.getBackground().clearColorFilter();

							MyDataObject.setMap(ENUM_keypad.transparacy.toString(), p1.getProgress()+"");
						}
					});
			}
			else if (keypadLayout == 1)
			{
				MyDataObject.setIntID(id);
			}
		}
		//special for keypad separrated
		else if (id == R.id.menu_custom_visibility)
		{
			MyDataObject.setCustom(true);
			
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			View layout = getLayoutInflater().inflate(R.layout.set_visibility, null);
			builder.setView(layout);
			builder.setCancelable(false);
			
			LinearLayout theLayout = layout.findViewById(R.id.visibilityLayout);
			TextView theTitle = layout.findViewById(R.id.theTitle);
			Button btnSave = layout.findViewById(R.id.btnOk);
			btnSave.setText(getResources().getString(android.R.string.ok));
			
			//set title
			theTitle.setText(R.string.str_custom_visibility);
			
			//set bg color for item
			final int bgVisible = Color.parseColor("#aa0000ff");
			final int bgGone = Color.BLACK;
			
			int count = fr.getChildCount();
			
			for (int i=1; i<count; i++)
			{
				View view = fr.getChildAt(i);
				
				if (view instanceof Button)
				{
					Button btn = (Button) view;
					String label = btn.getText().toString();
					
					LinearLayout layoutHorizontal = new LinearLayout(this);
					layoutHorizontal.setOrientation(LinearLayout.HORIZONTAL);
					if (btn.getVisibility() == View.GONE)
					{
						layoutHorizontal.setBackgroundColor(bgGone);
					}
					else
					{
						layoutHorizontal.setBackgroundColor(bgVisible);
					}
					
					layoutHorizontal.setId(i);
					layoutHorizontal.setGravity(Gravity.CENTER_HORIZONTAL);
					LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
					params.setMargins(3,2,3,2);
					theLayout.addView(layoutHorizontal, params);
					
					TextView text = new TextView(this);
					text.setText(label);
					text.setTextSize(35);
					if (btn.getVisibility() == View.GONE)
					{
						text.setTextColor(Color.RED);
					}
					else
					{
						text.setTextColor(Color.WHITE);
					}
					text.setTypeface(Typeface.SERIF, Typeface.BOLD);
					params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
					params.gravity = Gravity.CENTER_VERTICAL;
					layoutHorizontal.addView(text, params);
					
					layoutHorizontal.setOnClickListener(new OnClickListener()
						{
							@Override
							public void onClick(View view)
							{
								int id = view.getId();
								Button b = (Button) MainActivity.fr.getChildAt(id);
								int b_ID = b.getId();							
								String strCode = ENUM_keypad.keypad_.toString()+b_ID;
								
								TextView text = (TextView) ((LinearLayout) view).getChildAt(0);
								
								String theKey;
								if (theOrientation == 0)
								{
									theKey = strCode+ENUM_keypad._visibilityPotrait.toString();
								}
								else
								{
									theKey = strCode+ENUM_keypad._visibilityLandscape.toString();
								}
								
								Drawable d = view.getBackground();
								int c = ((ColorDrawable) d).getColor();
								
								if (c == bgVisible)
								{
									b.setVisibility(View.GONE);
									view.setBackgroundColor(bgGone);					
									text.setTextColor(Color.RED);
									
									MyDataObject.setMap(theKey, "0");
								}
								else
								{
									b.setVisibility(View.VISIBLE);
									view.setBackgroundColor(bgVisible);
									text.setTextColor(Color.WHITE);
									
									MyDataObject.setMap(theKey, "1");
								}
							}
					});
				}
			}
			
			builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						p1.dismiss();
						//saveDataCustom();
					}
			});
			
			final AlertDialog dialog = builder.create();
			dialog.setCancelable(false);

			Window window = dialog.getWindow();

			if (window != null)
			{
				window.setLayout(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
				window.setDimAmount(0.0f);
				window.setGravity(Gravity.CENTER);
				//window.setBackgroundDrawableResource(android.R.color.darker_gray);
			}
			
			btnSave.setOnClickListener(new OnClickListener()
				{
					@Override
					public void onClick(View p1)
					{
						dialog.dismiss();
						//saveDataCustom();
					}		
				});
			
			dialog.show();
		}
		
		//menu: keluar mode custom
		else if (id == R.id.menu_custom_off)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle(getResources().getString(R.string.save)+"?");

			builder.setPositiveButton(R.string.save, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						//simpan data sementara ke dalam file untuk jadi permanen								 
						HashMap<String,String> listMap = MyDataObject.getMap();

						for (Map.Entry<String,String> entery : listMap.entrySet())
						{
							String key = entery.getKey();

							if (key.contains(ENUM_keypad.keypad_.toString()))
							{
								PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_myKeypad, entery.getKey(), entery.getValue());
							}
							else
							{
								PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, entery.getKey(), entery.getValue());
							}			
						}

						theToast("Customizated!");	

						//reset data object
						MyDataObject.clearMapList();
						MyDataObject.setIntID(0);
						MyDataObject.setCustom(false);
						
						if (keypadLayout == 1)
						{
							clearKeypad();
							create_Controller();
						}

						p1.dismiss();
					}		
				});

			builder.setNegativeButton(R.string.str_close, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface p1, int p2)
					{

					}		
				});

			builder.setNeutralButton(android.R.string.cancel, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						theToast("Canceled Customize!");	

						//reset data object
						MyDataObject.clearMapList();
						MyDataObject.setIntID(0);
						MyDataObject.setCustom(false);

						//reset view to use saved configuration, and cancel the unsaved customization
						setPosition_Monitor();
						clearKeypad();
						create_Controller();

						p1.dismiss();
					}
				});

			AlertDialog dialog = builder.create();
			dialog.show();
		}	
		
		//menu: Keluar
		else if (id == R.id.menu_item_exit)
		{
			AlertDialog.Builder dialog = new AlertDialog.Builder(this);
			dialog.setTitle(R.string.dialog_exit_title);
			dialog.setMessage(R.string.dialig_exit_msg);
			
			//Ya, keluar
			dialog.setPositiveButton(R.string.dialog_exit_btn_positive, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						// TODO: Implement this method
						//brewEmuJNIShutdown();
						finishAffinity();
						System.exit(0);
					}
			});
		
			//Batal
			dialog.setNegativeButton(R.string.dialog_exit_btn_negative, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						// TODO: Implement this method
					}	
			});
			
			//masuk pengaturan
			dialog.setNeutralButton(R.string.str_sett, new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						//reset data object
						MyDataObject.clearMapList();
						MyDataObject.setCustom(false);
						MyDataObject.setIntID(0);
						
						//clear the view
						fr.removeAllViews();
						
						Intent i = new Intent(MainActivity.this, MyLauncherActivity.class);
						i.putExtra("emu", true);
						startActivity(i);
						finishAffinity();
					}
			});
			
			AlertDialog dialogAlert = dialog.create();
			dialogAlert.show();
			
			return true;
		}
		
		//menu: Pengaturan
		else if (id == R.id.menu_item_sett)
		{
			AlertDialog.Builder dialog = new AlertDialog.Builder(this);
			dialog.setTitle(R.string.dialog_sett_title);
			dialog.setMessage(R.string.dialig_sett_msg);
			
			dialog.setPositiveButton(R.string.dialog_sett_btn_positive, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						
					}
				});

			dialog.setNegativeButton(R.string.dialog_sett_btn_negative, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						// TODO: Implement this method
					}	
				});

			AlertDialog dialogAlert = dialog.create();
			dialogAlert.show();
			
			Intent i = new Intent(this, MyLauncherActivity.class);
			i.putExtra("emu", true);
			i.putExtra("isFloating", true);
			i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(i);
			//finishAffinity();
			
			return true;
		}
		
		else if (id == R.id.menu_rec_cap)
		{
			if (keypadLayout == 1)
			{
				clearKeypad();

				if (theOrientation == 0)
				{
					MyKeypad_AutoInput.createKeypad_AutoInput(this, R.id.keypad_potrait, new MyKeypad_AutoInput.IKeypadHandler()
						{
							@Override
							public void onButtonDown(int code)
							{
								brewEmuKeyDown(code);
							}

							@Override
							public void onButtonUp(int code)
							{
								brewEmuKeyUp(code);
							}
						});
				}
				else if (theOrientation == 1)
				{	
					//Get save-data from file properties
					HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);

					//build keypad num
					MyKeypad_AutoInput.createKeypad_AutoInput(this, R.id.keypad_numpad, new MyKeypad_AutoInput.IKeypadHandler()
						{
							@Override
							public void onButtonDown(int code)
							{
								brewEmuKeyDown(code);
							}

							@Override
							public void onButtonUp(int code)
							{
								brewEmuKeyUp(code);
							}
						});

					if (MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.diagonalOn.toString())))
					{ //dpad + diagonal
						MyKeypad_AutoInput.createKeypad_AutoInput(this, R.id.keypad_dpad_plus, new MyKeypad_AutoInput.IKeypadHandler()
							{
								@Override
								public void onButtonDown(int code)
								{
									if (code == 0x1)
									{
										brewEmuKeyDown(0xE031);
										brewEmuKeyDown(0xE033);
									}
									else if (code == 0x2)
									{
										brewEmuKeyDown(0xE031);
										brewEmuKeyDown(0xE034);
									}
									else if (code == 0x3)
									{
										brewEmuKeyDown(0xE032);
										brewEmuKeyDown(0xE033);
									}
									else if (code == 0x4)
									{
										brewEmuKeyDown(0xE032);
										brewEmuKeyDown(0xE034);
									}
									else
									{
										brewEmuKeyUp(code);
									}
								}

								@Override
								public void onButtonUp(int code)
								{
									if (code == 0x1)
									{
										brewEmuKeyUp(0xE031);
										brewEmuKeyUp(0xE033);
									}
									else if (code == 0x2)
									{
										brewEmuKeyUp(0xE031);
										brewEmuKeyUp(0xE034);
									}
									else if (code == 0x3)
									{
										brewEmuKeyUp(0xE032);
										brewEmuKeyUp(0xE033);
									}
									else if (code == 0x4)
									{
										brewEmuKeyUp(0xE032);
										brewEmuKeyUp(0xE034);
									}
									else
									{
										brewEmuKeyUp(code);
									}
								}
							});
					}
					else // dpad
					{
						
						MyKeypad_AutoInput.createKeypad_AutoInput(this, R.id.keypad_dpad, new MyKeypad_AutoInput.IKeypadHandler()
							{
								@Override
								public void onButtonDown(int code)
								{
									brewEmuKeyDown(code);
								}

								@Override
								public void onButtonUp(int code)
								{
									brewEmuKeyUp(code);
								}
							});
					}
				} 
			}
			
			MyKeypad_AutoInput.listRec_keypad = new ArrayList<>();
			
			MyDataObject.setIntID(R.id.menu_rec_cap);
			MyDataObject.setRec(true);
			
			MyLauncherActivity.toastMe(this, "Start Record");
		}
		
		else if (id == R.id.menu_rec_play)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle(getResources().getString(R.string.rec_input)+": Play");
			View layout = getLayoutInflater().inflate(R.layout.autoinput_play, null);
			builder.setView(layout);
			builder.setCancelable(false);
			
			final Spinner spinner_list_autoInput = layout.findViewById(R.id.theSpinner);
			final EditText edit_extratimesHold = layout.findViewById(R.id.autoinputplay_extraHold);
			final EditText edit_extratimesDelay = layout.findViewById(R.id.autoinputplay_extraDelay);
			
			
			ArrayList<String> list_autoinput = getListAutoInput();

			ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_spinner_item, list_autoinput);
			adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
			spinner_list_autoInput.setAdapter(adapter);		

			builder.setPositiveButton(R.string.rec_play, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						//clear keypad
						clearKeypad();

						MyDataObject.setIntID(R.id.menu_rec_play);
						MyDataObject.setRec(true);

						autoInput_play(spinner_list_autoInput, MyFormat.toLong(edit_extratimesHold.getText().toString(), 0L), MyFormat.toLong(edit_extratimesDelay.getText().toString(), 0L));

						p1.dismiss();
					}			
				});
			
			builder.setNeutralButton(android.R.string.cancel, new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						p1.dismiss();
					}			
			});
			
			AlertDialog dialog = builder.create();
			dialog.show();
		}
		
		else if (id == R.id.menu_rec_finish)
		{
			//finish from recording
			if (MyKeypad_AutoInput.listRec_keypad != null)
			{
				clearKeypad();
				
				if (MyKeypad_AutoInput.listRec_keypad.size() > 0)
				{
					AlertDialog.Builder builder = new AlertDialog.Builder(this);
					builder.setTitle(getResources().getString(R.string.rec_input)+": "+getResources().getString(R.string.save));
					View layout = getLayoutInflater().inflate(R.layout.autoinput, null);
					builder.setView(layout);
					
					final RadioGroup theRadio = layout.findViewById(R.id.autoInput_theRadio);
					final Spinner spinner_list_autoInput = layout.findViewById(R.id.autoInput_theSpinner);
					final EditText edit_newName_autoInput = layout.findViewById(R.id.autoinput_theEdit);
					final TextView text_extraTimeAppend = layout.findViewById(R.id.autoinputTextView1);
					final EditText edit_extraTimeAppend = layout.findViewById(R.id.autoInput_extraTimeAppend);
					final RadioButton radio_new = layout.findViewById(R.id.autoinput_new);
					final RadioButton radio_overwrite = layout.findViewById(R.id.autoinput_overwrite);
					final RadioButton radio_append = layout.findViewById(R.id.autoinput_append);
					
					ArrayList<String> autoInput_list = getListAutoInput();
					
					if (autoInput_list == null)
					{
						radio_overwrite.setVisibility(View.GONE);
						radio_append.setVisibility(View.GONE);
					}
					else
					{
						ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_spinner_item, autoInput_list);
						adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
						spinner_list_autoInput.setAdapter(adapter);	
					}
					
					theRadio.check(radio_new.getId());
					//spinner_list_autoInput.setVisibility(View.GONE);
					
					builder.setPositiveButton(R.string.save, new DialogInterface.OnClickListener()
						{
							@Override
							public void onClick(DialogInterface p1, int p2)
							{
								String newName = null;
								
								if (edit_newName_autoInput.getVisibility() == View.VISIBLE)
								{
									newName = edit_newName_autoInput.getText().toString(); 
								}
								else
								{
									newName = (String) spinner_list_autoInput.getSelectedItem();
								}
								
								try
								{
									File file = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY_AutoInput+newName);
									FileWriter fileWriter; // Parameter true untuk menambahkan konten tanpa menggantikan
									
									if (radio_append.isChecked())
									{
										fileWriter = new FileWriter(file, true);
										
										fileWriter.write("0-0-"+edit_extraTimeAppend.getText().toString()+"\n");
									}
									else
									{
										fileWriter = new FileWriter(file);
									}
									
									// add/make for last element
									MyKeypad_AutoInput.listRec_keypad.add("0");

									Iterator<String> iter = MyKeypad_AutoInput.listRec_keypad.iterator();

									iter.next(); //for skip firts element

									while (iter.hasNext())
									{
										fileWriter.write(iter.next()+iter.next()+"\n");
									}

									/*for (String s : EmulatorKeypad.listRec_keypad)
									 {
									 if (s.endsWith("-"))
									 {
									 //add '0' to element don't have value for delay
									 s = s + "0";
									 }

									 fileWriter.write(s+"\n");
									 }*/

									fileWriter.flush();
									fileWriter.close();
								}catch(IOException e){}

								//reset list
								MyKeypad_AutoInput.listRec_keypad.clear();
								MyKeypad_AutoInput.listRec_keypad = null;
								
								Toast.makeText(MainActivity.this, R.string.saved, Toast.LENGTH_LONG).show();
							}
					});
					
					/*builder.setNegativeButton(R.string.str_reset, new DialogInterface.OnClickListener()
						{
							@Override
							public void onClick(DialogInterface p1, int p2)
							{
								//Re-start recording
								MyKeypad_AutoInput.listRec_keypad.clear();
								MyKeypad_AutoInput.listRec_keypad = null;
								MyKeypad_AutoInput.listRec_keypad = new ArrayList<>();
								
								p1.dismiss();
							}					
					});*/
					
					builder.setNeutralButton(android.R.string.cancel, new DialogInterface.OnClickListener()
						{
							@Override
							public void onClick(DialogInterface p1, int p2)
							{
								if (keypadLayout == 1)
								{
									clearKeypad();
									create_Controller();
								}							
								
								p1.dismiss();
							}
					});
					
					final AlertDialog dialog = builder.create();
					dialog.setCanceledOnTouchOutside(false);
					dialog.show();
					dialog.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(false);
					
					final TextWatcher watch_text = new TextWatcher()
					{
						Button positiveButton = dialog.getButton(DialogInterface.BUTTON_POSITIVE);

						@Override
						public void beforeTextChanged(CharSequence p1, int p2, int p3, int p4)
						{
							//
						}

						@Override
						public void onTextChanged(CharSequence p1, int p2, int p3, int p4)
						{
							// TODO: Implement this method
						}

						@Override
						public void afterTextChanged(Editable p1)
						{
							//MyLog.w("text changed: "+p1.toString());

							if (edit_newName_autoInput.getVisibility() == View.VISIBLE)
							{
								//MyLog.w("edit:visible");

								if (p1.toString().isEmpty())
								{
									//MyLog.w("text empty");
									positiveButton.setEnabled(false);
								}
								else
								{
									//MyLog.w("text: "+p1.toString());
									ArrayList<String> list_autoInput = getListAutoInput();

									if (list_autoInput != null)
									{
										int indexMatch = -1;

										String newName = p1.toString();

										for (int i=0; i<list_autoInput.size(); i++)
										{
											if (list_autoInput.get(i).equalsIgnoreCase(newName))
											{
												indexMatch = i;
												break;
											}
										}

										if (indexMatch != -1)
										{
											positiveButton.setEnabled(false);
										}
										else
										{
											positiveButton.setEnabled(true);
										}
									}		
								}																											
							}
						}					
					};
					
					theRadio.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener()
						{
							Button positiveButton = dialog.getButton(DialogInterface.BUTTON_POSITIVE);
							
							@Override
							public void onCheckedChanged(RadioGroup p1, int p2)
							{
								if (p2 == R.id.autoinput_new)
								{
									edit_newName_autoInput.setVisibility(View.VISIBLE);
									spinner_list_autoInput.setVisibility(View.GONE);
									text_extraTimeAppend.setVisibility(View.GONE);
									edit_extraTimeAppend.setVisibility(View.GONE);
									
									positiveButton.setEnabled(false);
									edit_newName_autoInput.setEnabled(true);
									
									watch_text.afterTextChanged(edit_newName_autoInput.getText());
								}
								else if (p2==R.id.autoinput_overwrite || p2==R.id.autoinput_append)
								{
									edit_newName_autoInput.setVisibility(View.GONE);
									spinner_list_autoInput.setVisibility(View.VISIBLE);
								
									if (p2==R.id.autoinput_append)
									{
										text_extraTimeAppend.setVisibility(View.VISIBLE);
										edit_extraTimeAppend.setVisibility(View.VISIBLE);
									}
									else
									{
										text_extraTimeAppend.setVisibility(View.GONE);
										edit_extraTimeAppend.setVisibility(View.GONE);
									}
																
									positiveButton.setEnabled(true);
									edit_newName_autoInput.setEnabled(false);
								}							
							}				
						});
					
					edit_newName_autoInput.addTextChangedListener(watch_text);				
				}
			}	
			//finish from playing
			else if (MyDataObject.getIntID() == R.id.menu_rec_play)
			{
				mainHandler.removeCallbacksAndMessages(null);
				mainHandler = null;
				mainTask = null;
				
				listRec_code.clear();
				listRec_code = null;
				listRec_hold.clear();
				listRec_hold = null;
				listRec_delay.clear();
				listRec_delay = null;
				
				MyLauncherActivity.toastMe(this, "Stop Playing Auto-Input");
			}
			
			MyDataObject.setRec(false);
			MyDataObject.setIntID(0);
			
			create_Controller();
		} //END OF 'if (ID)'
			
		// Tangani klik pada opsi menu ActionBar yang dipilih
		return super.onOptionsItemSelected(item);
	}
	
	private void clearKeypad()
	{
		int childCount = fr.getChildCount();
		
		for (int i=childCount-1; i>0; i--)
		{
			View view = fr.getChildAt(i);
			
			if (view instanceof Button || view instanceof ViewGroup)
			{
				if (!view.toString().contains("Toolbar"))
				{
					view.setOnTouchListener(null);
					fr.removeViewInLayout(view);
					
					//MyLog.w("Romove: "+view.toString().substring(0, 25));
				}			
			}
		}
	}
	
	/*private void rebuildKeypad()
	{
		clearKeypad();
		create_Controller();
	}*/

	@Override
	public void onBackPressed()
	{
		openOptionsMenu();
	}
	
	//window notif/waring
	private void myNotif(String title, String message)
	{
		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle(title);
		dialog.setMessage(message);

		dialog.setPositiveButton("Close", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface p1, int p2){}
			});

		AlertDialog dialogAlert = dialog.create();
		dialogAlert.show();
	}
	
	private ArrayList<String>  getListAutoInput()
	{
		ArrayList<String> list_autoinput = new ArrayList<>();				
		File dir = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY_AutoInput);
		File[] listFile = dir.listFiles();

		if (listFile != null)
		{
			if (listFile.length > 0)
			{
				for (File theFile : listFile)
				{
					if (theFile.isFile())
					{
						list_autoinput.add(theFile.getName());
					}
				}
			}
			else
			{
				return null;
			}
		}
		else
		{
			return null;
		}
		
		return list_autoinput;
	}
	
	private void autoInput_play(Spinner selected, long extratimesHold, Long extratimesDelay)
	{
		listRec_code = new ArrayList<>();
		listRec_hold = new ArrayList<>();
		listRec_delay = new ArrayList<>();

		//put rec input to list
		try
		{
			String autoInput_selected = (String) selected.getSelectedItem();

			BufferedReader br = new BufferedReader(new FileReader(Environment.getExternalStorageDirectory().toString()+"/"+PropertiesManager.PROPERTIES_DIRECTORY_AutoInput+autoInput_selected));
			
			String line;

			while ((line=br.readLine()) != null)
			{
				String[] s = line.split("-", 3);

				listRec_code.add(Integer.parseInt(s[0]));
				
				if (s[0] != "0")
				{
					listRec_hold.add(Long.parseLong(s[1])+extratimesHold);
					listRec_delay.add(Long.parseLong(s[2])+extratimesDelay);
				}
				else
				{
					listRec_hold.add(Long.parseLong(s[1]));
					listRec_delay.add(Long.parseLong(s[2]));
				}
			}
		}
		catch (IOException e)
		{
			MyLog.w("AutoInput-Play: "+e);
		}

		final Iterator<Integer> iterator_code = listRec_code.iterator();
		final Iterator<Long> iterator_hold = listRec_hold.iterator();
		final Iterator<Long> iterator_delay = listRec_delay.iterator();

		//numRun = 1;
		
		mainTask = new Runnable() 
		{
			@Override
			public void run()
			{		
				if (iterator_code.hasNext())
				{
					final int keyCode = iterator_code.next();

					brewEmuKeyDown(keyCode);
					//text1.setText("Down("+keyCode+")");
					//text2.setText("Hold("+iterator_hold.next()+")");
					//text3.setText("Run: "+numRun);

					mainHandler.postDelayed(new Runnable()
					{
							@Override
							public void run()
							{
								brewEmuKeyUp(keyCode);
								//text1.setText("Up("+keyCode+")");
								//text2.setText("Wait("+iterator_delay.next()+")");
								
								//numRun++;

								//run again or back to main Handler, and add delay for between touching button
								mainHandler.postDelayed(mainTask, iterator_delay.next());
							}		
						}, iterator_hold.next()); //delay for holding button
				}//end of 'if' readLine()!=null
				else
				//complete
				{
					MyLauncherActivity.toastMe(MainActivity.this, "Playing Auto-Input is complete");

					MyDataObject.setRec(false);
					MyDataObject.setIntID(0);

					mainHandler.removeCallbacksAndMessages(null);
					mainHandler = null;
					mainTask = null;

					listRec_code.clear();
					listRec_code = null;
					listRec_hold.clear();
					listRec_hold = null;
					listRec_delay.clear();
					listRec_delay = null;

					//create keypad
					create_Controller();
				} 
			} //end of 'run()'
		}; //end of 'mainTask'

		//set for new handler
		mainHandler = new Handler();
		mainHandler.postDelayed(mainTask, 0);
	}

	
	
	
	
	
	
	
	
	//untuk keperluan debugging
	public void theToast(String teks)
	{
		Toast.makeText(MainActivity.this, teks, Toast.LENGTH_LONG).show();
	}
	
	RelativeLayout.LayoutParams param;
	RelativeLayout layar;
	public static TextView text1, text2, text3, text4, text5, text6;

	void param()
	{
		//layar = (RelativeLayout) findViewById(R.id.mainRelativeLayout1);
		layar = fr;
		param = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
	}
	
	private void debugText()
	{
		text1 = new TextView(this);
		param();
		param.addRule(layar.CENTER_HORIZONTAL);
		text1.setText("");
		text1.setTextColor(Color.parseColor("#ffff0000"));
		text1.setTextSize(30);
		text1.setTypeface(Typeface.MONOSPACE);
		text1.setBackgroundColor(Color.parseColor("#0000ffff"));
		text1.setPadding(0,10,0,10);
		text1.setId(71);
		layar.addView(text1, param);
		
		text2 = new TextView(this);
		param();
		param.addRule(layar.CENTER_HORIZONTAL);
		param.addRule(RelativeLayout.BELOW, text1.getId());
		text2.setText("");
		text2.setTextColor(Color.parseColor("#ffff0000"));
		text2.setTextSize(30);
		text2.setTypeface(Typeface.MONOSPACE);
		text2.setBackgroundColor(Color.parseColor("#0000ffff"));
		text2.setPadding(0,10,0,10);
		text2.setId(72);
		layar.addView(text2, param);
		
		text3 = new TextView(this);
		param();
		param.addRule(layar.CENTER_HORIZONTAL);
		param.addRule(RelativeLayout.BELOW, text2.getId());
		text3.setText("");
		text3.setTextColor(Color.parseColor("#ffff0000"));
		text3.setTextSize(30);
		text3.setTypeface(Typeface.MONOSPACE);
		text3.setBackgroundColor(Color.parseColor("#0000ffff"));
		text3.setPadding(0,10,0,10);
		text3.setId(73);
		layar.addView(text3, param);
		
		text4 = new TextView(this);
		param();
		param.addRule(layar.CENTER_HORIZONTAL);
		param.addRule(RelativeLayout.BELOW, text3.getId());
		text4.setText("");
		text4.setTextColor(Color.parseColor("#ffff0000"));
		text4.setTextSize(30);
		text4.setTypeface(Typeface.MONOSPACE);
		text4.setBackgroundColor(Color.parseColor("#0000ffff"));
		text4.setPadding(0,10,0,10);
		text4.setId(74);
		layar.addView(text4, param);
		
		text5 = new TextView(this);
		param();
		param.addRule(layar.CENTER_HORIZONTAL);
		param.addRule(RelativeLayout.BELOW, text4.getId());
		text5.setText("");
		text5.setTextColor(Color.parseColor("#ffff0000"));
		text5.setTextSize(30);
		text5.setTypeface(Typeface.MONOSPACE);
		text5.setBackgroundColor(Color.parseColor("#0000ffff"));
		text5.setPadding(0,10,0,10);
		text5.setId(75);
		layar.addView(text5, param);
		
		text6 = new TextView(this);
		param();
		param.addRule(layar.CENTER_HORIZONTAL);
		param.addRule(RelativeLayout.BELOW, text5.getId());
		text6.setText("");
		text6.setTextColor(Color.parseColor("#ffff0000"));
		text6.setTextSize(30);
		text6.setTypeface(Typeface.MONOSPACE);
		text6.setBackgroundColor(Color.parseColor("#0000ffff"));
		text6.setPadding(0,10,0,10);
		text6.setId(76);
		layar.addView(text6, param);
	}
	
	/*private float getDistance(MotionEvent event)
	{
		float deltaX = event.getX(0) - event.getX(1);
		float deltaY = event.getY(0) - event.getY(1);

		return (float) Math.sqrt(deltaX * deltaX + deltaY * deltaY);
	}*/
	/*float initialDistance = 0f;
	float initialScaleX = 1f;
	float initialScaleY = 1f;
	int i_rawX=0, i_rawY=0, i_posX=0, i_posY=0;
	@Override
    public boolean onTouchEvent(MotionEvent event)
	{
		/*int pointerCount = event.getPointerCount();

		if (pointerCount >= 2)
		{
			switch (event.getActionMasked())
			{
				case MotionEvent.ACTION_POINTER_DOWN:
					initialDistance = getDistance(event);
					initialScaleX = surfaceView.getScaleX();
					initialScaleY = surfaceView.getScaleY();
					break;
				case MotionEvent.ACTION_MOVE:
					float currentDistance = getDistance(event);
					float scaleDelta = currentDistance / initialDistance;
					surfaceView.setScaleX(initialScaleX * scaleDelta);
					surfaceView.setScaleY(initialScaleY * scaleDelta);
					break;
			}
		}
		
		/*if (MyDataObject.getIntID() == R.id.menu_custom_size)
		{
			switch(event.getActionMasked())
			{
				case MotionEvent.ACTION_DOWN:
					//posisi sentuh awal
					i_rawX = (int) event.getRawX();
					i_rawY = (int) event.getRawY();
					break;

				case MotionEvent.ACTION_MOVE:
					//posisi sentuhan setelah melakukan pergerakan
					int nowX = (int) event.getRawX();
					int nowY = (int) event.getRawY();

					//jumlah gerakan yang terjadi
					float i_moveX = 1f + (nowX - i_rawX) / 500f;
					float i_moveY = 1f + (i_rawY - nowY) / 500f;

					keypad.setScaleX(i_moveX);
					keypad.setScaleY(i_moveY);

					break;
			}
		}
        
		return true;
    }*/
}
