package io.github.usernameak.brewemulator;

import android.os.Bundle;
import android.Manifest;
import android.widget.Toast;
import android.content.pm.PackageManager;
import android.view.View;
import android.content.Intent;
import android.os.Build;
import android.widget.EditText;
import java.util.Properties;
import java.io.File;
import android.os.Environment;
import java.io.IOException;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Switch;
import android.content.Context;
import android.widget.TextView;
import android.widget.Button;
import android.widget.AdapterView.OnItemClickListener;
import java.util.HashMap;
import android.content.SharedPreferences;
import java.util.ArrayList;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;
import android.widget.Spinner;
import android.widget.AdapterView;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AlertDialog;
import android.content.DialogInterface;
import androidx.appcompat.widget.Toolbar;
import android.widget.ArrayAdapter;
import android.widget.Adapter;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.RadioGroup;
import android.widget.LinearLayout;
import android.view.ViewGroup;
import android.text.InputType;
import android.graphics.Typeface;
import android.view.WindowManager;
import android.widget.ListView;
import androidx.appcompat.widget.PopupMenu;
import android.view.MenuInflater;
import android.graphics.Color;
import android.text.TextWatcher;
import android.text.Editable;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;


public class MyLauncherActivity extends AppCompatActivity
{
	//public static final Boolean debugUser = true;
	//MyLog.clear_user();
	
	//MyDataObject myDataObject = new MyDataObject();
	private final String PREF_NAME = "mySet";
	
	private Boolean barUI, fullScreen = null;
	private Boolean emu;//, skipIt;//, floatingConfig;

	private EditText bgColor, resoX,resoY, resoScale, labelSize, labelColor;//,marginTop,marginBottom,marginRight,marginLeft;
	private Switch barSwitch, switchFullscren, switchSkipMenu, switchDiagonal, switchBg, switchBgBtn, switchExtKeypad;
	private Spinner theKeypadLayout, theLib, theRotation;
	private Button btnProfile;
	
	Toolbar toolbar;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
		
		emu = getIntent().getBooleanExtra("emu", false);
		//floatingConfig = getIntent().getBooleanExtra("isFloating", false);
		
		if (emu)
		{
			loadDataConfig();
		}
		else
		{
			izin();
		}	
    }



	/******************************************************************************************************************************************************************************************************
					PERMISSION
	******************************************************************************************************************************************************************************************************/

	private void izin()
	{
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
		{
			if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED) 
			{
				ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
				Toast.makeText(this, "Need Permission", Toast.LENGTH_LONG).show();
			}
			else
			{
				Toast.makeText(this, "Welcome", Toast.LENGTH_LONG).show();
				began();
			}
		}
		else
		{
			began();
		}
	}

	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
	{
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);

		if (grantResults[0]==PackageManager.PERMISSION_GRANTED)
		{
			Toast.makeText(this, "Have fun!", Toast.LENGTH_LONG).show();
			began();
		}
		else
		{
			Toast.makeText(this, ";(", Toast.LENGTH_LONG).show();
			finishAffinity();
			System.exit(0);
		}
	}



	/******************************************************************************************************************************************************************************************************
					STARTING
	******************************************************************************************************************************************************************************************************/

	private void creteDirData()
	{
		File directory = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY);
		File directory2 = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY_AutoInput);
		
		if (!directory.exists())
		{
			directory.mkdirs();
		}
		
		if (!directory2.exists())
		{
			directory2.mkdirs();
		}
	}
	
	private void extractDatafromAsset()
	{
		String assetsPath = "sys"; // Path folder assets yang ingin disalin
		String destinationPath = Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"MelangeBREW/sys"; // Path tujuan di penyimpanan eksternal

		try
		{
			AssetsUtils.copyAssetsToExternalStorage(this, assetsPath, destinationPath);
			// Salin selesai
		}
		catch (IOException e)
		{
			e.printStackTrace();
			// Gagal menyalin
		}
	}

	private void began()
	{
		creteDirData();
		extractDatafromAsset();
		loadDataConfig();
	}

	public void startEmu(View v)
	{
		saveDataConfig();
		
		if (!emu)
		{
			//Toast.makeText(this, "Starting Emulator....", Toast.LENGTH_LONG).show();

			Intent in = new Intent(MyLauncherActivity.this, MainActivity.class);
			startActivity(in);
			finishAffinity();
		}
		else
		{
			//toastMe(this, getResourceString(R.string.saved));
			
			Intent in = new Intent(MyLauncherActivity.this, MainActivity.class);
			in.putExtra("newConfig", true);
			startActivity(in);
			finishAffinity();
		}
	}



	/******************************************************************************************************************************************************************************************************
					LOAD-GET DATA & SET EDIT FORM
	******************************************************************************************************************************************************************************************************/

	private void loadDataConfig() throws NumberFormatException
	{
		//get save-data SharedPreferences
        SharedPreferences sharedPreferences = getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        Boolean skipIt = Boolean.parseBoolean(sharedPreferences.getString(ENUM_general.skipMenu.toString(), "false"));
		String profileName_savedUsed = sharedPreferences.getString(ENUM_general.profileUse.toString(), "");
		
		if (!profileName_savedUsed.equals(""))
		{
			if (profileName_savedUsed.equals(PropertiesManager.PROPERTIES_profileName_default.toString()))
			{
				PropertiesManager.PROPERTIES_profileName = "";
			}
			else
			{
				PropertiesManager.PROPERTIES_profileName = profileName_savedUsed+"/";
			}	
		}
 		
		//if skip menu
		if (skipIt && !emu)
		{
			Intent in = new Intent(MyLauncherActivity.this, MainActivity.class);
			startActivity(in);
			finishAffinity();
		}	
		
		//Get save-data from file properties
		HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);
		
		//take save-data
		barUI = MyFormat.toBoolean(myConfigSave.get(ENUM_general.bar.toString()));
		fullScreen = MyFormat.toBoolean(myConfigSave.get(ENUM_general.fullscreen.toString()));
		
		setMyView();
		
		if (emu)
		{
			Button button = findViewById(R.id.btnStartEmu);
			button.setText(R.string.save);
			TextView textView = findViewById(R.id.msgMsg);
			textView.setText("");
		}
		
		btnProfile = findViewById(R.id.profile_name);
		theLib = findViewById(R.id.theLib);
		theRotation = findViewById(R.id.theRotation);
		switchSkipMenu = findViewById(R.id.switchSkipMenu);
		bgColor = findViewById(R.id.bgColor);
		switchBg = findViewById(R.id.switchBg);
		resoX = findViewById(R.id.resoWidth);
		resoY = findViewById(R.id.resoHeight);
		resoScale = findViewById(R.id.monitorScale);
		theKeypadLayout = findViewById(R.id.keypadLayout);
		labelSize = findViewById(R.id.labelSize);
		labelColor = findViewById(R.id.labelColor);
		//buttonX = findViewById(R.id.buttonWidth);
		//buttonY = findViewById(R.id.buttonHeight);
		//marginTop = findViewById(R.id.marginTop);
		//marginBottom = findViewById(R.id.marginBottom);
		//marginRight = findViewById(R.id.marginRight);
		//marginLeft = findViewById(R.id.marginLeft);
		switchFullscren = findViewById(R.id.switchFullscreen);
		barSwitch = findViewById(R.id.barSwitch);
		//strecthStatus = findViewById(R.id.switchScaleType);
		switchDiagonal = findViewById(R.id.switchDiagonal);
		switchBgBtn = findViewById(R.id.switchBgBtn);
		switchExtKeypad = findViewById(R.id.switchExtLabel);
		
		/* Menyesuaikan isi filed/form sesuai data yang sudah ada */
		
		//set profil
		if (profileName_savedUsed.equals(PropertiesManager.PROPERTIES_profileName_default.toString()))
		{
			btnProfile.setTypeface(Typeface.DEFAULT, Typeface.ITALIC);
		}
		
		if (profileName_savedUsed.equals(""))
		{
			btnProfile.setText(PropertiesManager.PROPERTIES_profileName_default.toString());
		}
		else
		{
			btnProfile.setText(profileName_savedUsed);
		}
		
		btnProfile.setOnClickListener(new OnClickListener()
			{
				ListView listView_profileName;
				
				@Override
				public void onClick(View view)
				{
					AlertDialog.Builder builder = new AlertDialog.Builder(MyLauncherActivity.this);
					View layout = getLayoutInflater().inflate(R.layout.list_profiles, null);
					builder.setView(layout);
					builder.setCancelable(false);
					
					builder.setPositiveButton(R.string.str_close, new DialogInterface.OnClickListener()
						{
							@Override
							public void onClick(DialogInterface p1, int p2)
							{
								p1.dismiss();
							}					
						});
					
					final AlertDialog dialog = builder.create();

					Window window = dialog.getWindow();
					if (window != null)
					{
						window.setDimAmount(0.0f);			 	
					}
				
					//set ListView 'Profiles'
					listView_profileName = layout.findViewById(R.id.theList);
					setListView();
					
					listView_profileName.setOnItemClickListener(new AdapterView.OnItemClickListener()
						{
							@Override
							public void onItemClick(AdapterView<?> p1, View p2, int p3, long p4)
							{
								String selected_profileName = (String) p1.getItemAtPosition(p3);						
								change_savedLoad_profileName(selected_profileName);							
								dialog.dismiss();
							}												
					});
					
					listView_profileName.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener()
						{
							@Override
							public boolean onItemLongClick(AdapterView<?> p1, View p2, int p3, long p4)
							{
								if (p3 != 0)
								{
									showPopupMenu(p2, p3);

									return true;
								}
								
								return false;
							}						
					});

					dialog.show();
					
					//button profile add
					Button btn_profile_add = layout.findViewById(R.id.btn_profile_add);
					btn_profile_add.setOnClickListener(new Button.OnClickListener()
						{
							//set profile
							private ArrayList<String> set_spinner_ProfileName(Spinner spinner)
							{
								//set list
								ArrayList<String> listProfile = create_list_profileName();
								ArrayAdapter<String> adapter = new ArrayAdapter<>(MyLauncherActivity.this, android.R.layout.simple_spinner_item, listProfile);	
								adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);										
								spinner.setAdapter(adapter);

								return listProfile;
							}
							
							@Override
							public void onClick(View p1)
							{
								AlertDialog.Builder builder_add = new AlertDialog.Builder(MyLauncherActivity.this);
								View layout_add = getLayoutInflater().inflate(R.layout.new_profile, null);
								builder_add.setView(layout_add);
								builder_add.setCancelable(false);

								//final LinearLayout theLayout = layout.findViewById(R.id.theLayout);
								RadioGroup radio = layout_add.findViewById(R.id.profileMode);
								TextView textTitle = layout_add.findViewById(R.id.theTitle);
								final EditText edit_profileName = layout_add.findViewById(R.id.theEdit);
								final Spinner spinner_profileName = layout_add.findViewById(R.id.theSpinner);

								//set default radio New
								radio.check(layout_add.findViewById(R.id.radio_new).getId());
								set_spinner_ProfileName(spinner_profileName);
								spinner_profileName.setVisibility(View.GONE);

								//set title
								textTitle.setText(R.string.new_profile_title);

								radio.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener()
									{
										//String newProfileName = edit_profileName.getText().toString();

										@Override
										public void onCheckedChanged(RadioGroup p1, int p2)
										{										
											if (p2 == R.id.radio_new) //new
											{											
												spinner_profileName.setVisibility(View.GONE);


											}
											else if( p2==R.id.radio_import) //import
											{											
												spinner_profileName.setVisibility(View.VISIBLE);
												//ArrayList<String> arr_profileName = 											
											}
										}									
									});

								builder_add.setPositiveButton(R.string.save, new DialogInterface.OnClickListener()
									{
										@Override
										public void onClick(DialogInterface p1, int p2)
										{
											//create profile directory
											String new_add_profileName = edit_profileName.getText().toString();
											File file = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY+PropertiesManager.PROPERTIES_DIRECTORY_PROFILES+new_add_profileName);

											if (!file.exists())
											{
												if (!file.mkdir())
												{
													MyLog.w("failed to create new profile: "+new_add_profileName);
												}
												else
												{
													//if using import
													if (spinner_profileName.getVisibility() == View.VISIBLE)
													{
														String import_profile = spinner_profileName.getSelectedItem().toString();
														
														//if import from '(default)' profile
														if (import_profile.equals(PropertiesManager.PROPERTIES_profileName_default.toString()))
														{
															import_profile = "";
														}
//MyLog.w("import: "+import_profile);														
														File file_import = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY+PropertiesManager.PROPERTIES_DIRECTORY_PROFILES+import_profile);
//MyLog.w("profile import dir: "+file_import.toString());														
														File[] scanFiles = file_import.listFiles();
														
														for (File theFile : scanFiles)
														{
															if (theFile.isFile())
															{
//MyLog.w("copy: "+theFile.toString());
//MyLog.w("to: "+file+"/"+theFile.getName());
																try
																{
																	InputStream inputStream = new FileInputStream(theFile);
																	OutputStream outputStream = new FileOutputStream(file+"/"+theFile.getName());

																	byte[] buffer = new byte[1024];
																	int length;

																	while ((length=inputStream.read(buffer))>0)
																	{
																		outputStream.write(buffer, 0, length);
																	}

																	inputStream.close();
																	outputStream.close();
																}
																catch (IOException e)
																{}
															}														
														}
													}
												}
											}
											else // if the new profile name already exists
											{
												toastMe("Profile name already exists");
											}
																					
											//update profile list
											setListView();
											
											p1.dismiss();
										}				
									});

								builder_add.setNeutralButton(R.string.str_cancel, new DialogInterface.OnClickListener()
									{
										@Override
										public void onClick(DialogInterface p1, int p2)
										{
											p1.dismiss();
										}					
									});

								final AlertDialog dialog_add = builder_add.create();
								dialog_add.show();
								dialog_add.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(false);
								
								edit_profileName.addTextChangedListener(new TextWatcher()
									{
										Button positiveButton = dialog_add.getButton(DialogInterface.BUTTON_POSITIVE);

										@Override
										public void beforeTextChanged(CharSequence p1, int p2, int p3, int p4)
										{
											// TODO: Implement this method
										}

										@Override
										public void onTextChanged(CharSequence p1, int p2, int p3, int p4)
										{
											// TODO: Implement this method
										}

										@Override
										public void afterTextChanged(Editable p1)
										{
											check_newProfileName(p1.toString(), positiveButton);
										}										
									});
							}		
						});
				}
				
				//make class 'Adapter'
				class CustomAdapter extends ArrayAdapter<String>
				{
					private int specialItemIndex; // Indeks item yang ingin diberi efek

					public CustomAdapter(Context context, int resource, ArrayList<String> objects, int specialItemIndex) 
					{
						super(context, resource, objects);
						this.specialItemIndex = specialItemIndex;
					}

					@Override
					public View getView(int position, View convertView, ViewGroup parent) 
					{
						View itemView = super.getView(position, convertView, parent);
						
						if (position > 0)
						{				
							((TextView) itemView).setTypeface(Typeface.MONOSPACE);
						}
						else
						{
							((TextView) itemView).setTypeface(Typeface.DEFAULT);
						}
						
						// Cek jika ini adalah item yang ingin diberi efek
						if (position == specialItemIndex) 
						{
							// Ubah tampilan item dengan efek yang Anda inginkan
							itemView.setBackgroundColor(ContextCompat.getColor(getContext(), R.color.abc_search_url_text_normal));
						} 
						else
						{
							// Reset tampilan item jika tidak perlu diberi efek
							itemView.setBackgroundColor(Color.TRANSPARENT);
						}

						return itemView;
					}
				}
				
				private ArrayList<String> create_list_profileName()
				{
					ArrayList<String> list_dir_name = new ArrayList<>();				
					File dir = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY+PropertiesManager.PROPERTIES_DIRECTORY_PROFILES);
					File[] listDir = dir.listFiles();

					list_dir_name.add(PropertiesManager.PROPERTIES_profileName_default.toString());

					for (File theDir : listDir)
					{
						if (theDir.isDirectory())
						{
							list_dir_name.add(theDir.getName());
						}
					}
					
					return list_dir_name;
				}
				
				private void setListView()
				{
					ArrayList<String> list = create_list_profileName();
					
					//choosed
					int profile_used = list.indexOf(PropertiesManager.PROPERTIES_profileName.replace("/", ""));
					if (profile_used == -1)
					{
						profile_used = 0;
					}
					
					CustomAdapter adapter = new CustomAdapter(MyLauncherActivity.this, android.R.layout.simple_list_item_activated_1, list, profile_used);
					listView_profileName.setAdapter(adapter);
				}
				
				private void change_savedLoad_profileName(String profileName)
				{
					profileName += "/";
					
					if (!PropertiesManager.PROPERTIES_profileName.equals(profileName))
					{
						//set profile to use
						/*if (!profileName.equals("(default)/"))
						{
							PropertiesManager.PROPERTIES_profileName = profileName;
						}*/

						//save profile name
						SharedPreferences sharedPreferences = getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
						SharedPreferences.Editor editor = sharedPreferences.edit();
						editor.putString(ENUM_general.profileUse.toString(), profileName.substring(0, profileName.length()-1));
						editor.apply();

						loadDataConfig();
					}
				}
				
				private void showPopupMenu(final View view, int position) 
				{
					PopupMenu popupMenu = new PopupMenu(MyLauncherActivity.this, view);
					MenuInflater inflater = popupMenu.getMenuInflater();
					inflater.inflate(R.menu.menu_profile, popupMenu.getMenu());

					popupMenu.setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() 
					{
							@Override
							public boolean onMenuItemClick(MenuItem item)
							{
								final String selected_profileName = ((TextView) view).getText().toString();
								
								switch (item.getItemId())
								{
									case R.id.menu_profile_load: //load the profile									
										change_savedLoad_profileName(selected_profileName);
										return true;
									case R.id.menu_profile_rename: //rename the profile
										AlertDialog.Builder builder_rename = new AlertDialog.Builder(MyLauncherActivity.this);
										builder_rename.setTitle("Profile, "+getResourceString(R.string.str_rename)+": "+selected_profileName);
										View layout_rename = getLayoutInflater().inflate(R.layout.profile_rename, null);
										builder_rename.setView(layout_rename);
										builder_rename.setCancelable(false);
										
										final EditText edit_profileName = layout_rename.findViewById(R.id.theEdit);
										
										edit_profileName.setText(selected_profileName);
																									
										builder_rename.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener()
											{
												@Override
												public void onClick(DialogInterface p1, int p2)
												{
													//renaming
													File file_old = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY+PropertiesManager.PROPERTIES_DIRECTORY_PROFILES+selected_profileName);
													File file_new = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY+PropertiesManager.PROPERTIES_DIRECTORY_PROFILES+edit_profileName.getText().toString());
												
													if (file_old.renameTo(file_new))
													{
														toastMe(MyLauncherActivity.this, "Succes renamed");
														
														if (PropertiesManager.PROPERTIES_profileName.equalsIgnoreCase(selected_profileName+"/"))
														{
															change_savedLoad_profileName(edit_profileName.getText().toString());
														}
													}
													else
													{
														toastMe(MyLauncherActivity.this, "ERROR");
														
														MyLog.w("fail to rename profile");
													}
													
													/*MyLog.w("profile now: "+PropertiesManager.PROPERTIES_profileName);
													MyLog.w("profile selected: "+selected_profileName);*/
																																			
													setListView();

													p1.dismiss();
												}										
											});

										builder_rename.setNeutralButton(android.R.string.cancel, new DialogInterface.OnClickListener()
											{
												@Override
												public void onClick(DialogInterface p1, int p2)
												{
													p1.dismiss();
												}										
											});
										
										final AlertDialog dialog_rename = builder_rename.create();
										dialog_rename.show();
										dialog_rename.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(false);
																			
										edit_profileName.addTextChangedListener(new TextWatcher()
											{
												Button positiveButton = dialog_rename.getButton(DialogInterface.BUTTON_POSITIVE);

												@Override
												public void beforeTextChanged(CharSequence p1, int p2, int p3, int p4)
												{
													// TODO: Implement this method
												}

												@Override
												public void onTextChanged(CharSequence p1, int p2, int p3, int p4)
												{
													// TODO: Implement this method
												}

												@Override
												public void afterTextChanged(Editable p1)
												{
													check_newProfileName(p1.toString(), positiveButton);
												}										
											});
										
										return true;
									case R.id.menu_profile_delete: //delete the profile																
										AlertDialog.Builder builder = new AlertDialog.Builder(MyLauncherActivity.this);
										builder.setTitle(getResourceString(R.string.str_delete)+": "+selected_profileName);
										builder.setMessage(getResourceString(R.string.msg_profile_delete));
										
										builder.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener()
											{
												@Override
												public void onClick(DialogInterface p1, int p2)
												{
													//deleting
													if (!delete_profile(selected_profileName))
													{
														MyLog.w("failed to delete profile: "+selected_profileName);
													}
													
													//update profile ListView
													
													if (PropertiesManager.PROPERTIES_profileName.equals(selected_profileName+"/"))
													{
														change_savedLoad_profileName(PropertiesManager.PROPERTIES_profileName_default.toString());
													}
													
													setListView();
													
													p1.dismiss();
												}										
										});
										
										/*builder.setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener()
											{
												@Override
												public void onClick(DialogInterface p1, int p2)
												{
													p1.dismiss();
												}										
											});*/
										
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
										
										return true;
									default:
										return false;
								}
							}
						});

					popupMenu.show();
				}
				
				private boolean delete_profile(String profilDirName)
				{
					File fileOrDirectory = new File(Environment.getExternalStorageDirectory(), PropertiesManager.PROPERTIES_DIRECTORY+PropertiesManager.PROPERTIES_DIRECTORY_PROFILES+profilDirName);
					if (fileOrDirectory.isDirectory())
					{
						File[] childFiles = fileOrDirectory.listFiles();
						if (childFiles!=null)
						{
							for (File child : childFiles)
							{
								child.delete();
							}
						}
					}
					
					return fileOrDirectory.delete();
				}
				
				private void check_newProfileName(String newProfileName, Button btn)
				{
					if (newProfileName.length()<1 || newProfileName.isEmpty())
					{
						btn.setEnabled(false);
					}
					else
					{
						ArrayList<String> list_dir_profileName = create_list_profileName();

						if (list_dir_profileName.size() > 0)
						{
							int indexMatch = -1;

							for (int i=0; i<list_dir_profileName.size(); i++)
							{
								if (list_dir_profileName.get(i).equalsIgnoreCase(newProfileName))
								{
									indexMatch = i;
									break;
								}
							}

							if (indexMatch != -1)
							{
								//toastMe(MyLauncherActivity.this, "Exists");
								btn.setEnabled(false);
							}
							else
							{
								//toastMe(MyLauncherActivity.this, getResourceString(android.R.string.ok));
								btn.setEnabled(true);
							}
						}
					}
				}			
		});
		
		//set selected spinner
		theLib.setSelection(MyFormat.toInt(myConfigSave.get(ENUM_general.libChoose.toString()), 0));
		theRotation.setSelection(MyFormat.toInt(myConfigSave.get(ENUM_general.orientation.toString()), 0));
		theKeypadLayout.setSelection(MyFormat.toInt(myConfigSave.get(ENUM_keypad.keypadLayout.toString()), 0));
		
		//men set EditText
		bgColor.setText(myConfigSave.get(ENUM_general.bgColor.toString()));
		resoX.setText(myConfigSave.get(ENUM_monitor.resoWidth.toString()));
		resoY.setText(myConfigSave.get(ENUM_monitor.resoHeight.toString()));
		resoScale.setText(myConfigSave.get(ENUM_monitor.scale.toString()));
		labelSize.setText(myConfigSave.get(ENUM_keypad.labelSize.toString()));
		labelColor.setText(myConfigSave.get(ENUM_keypad.labelColor.toString()));
		//buttonX.setText(keypad_save.get(ENUM_keypad.buttonWidth.toString()));
		//buttonY.setText(keypad_save.get(ENUM_keypad.buttonHeigt.toString()));
		/*marginLeft.setText(myConfigSave.get(ENUM_keypad.marginLeft.toString()));
		marginTop.setText(myConfigSave.get(ENUM_keypad.marginTop.toString()));
		marginRight.setText(myConfigSave.get(ENUM_keypad.marginRight.toString()));
		marginBottom.setText(myConfigSave.get(ENUM_keypad.marginBottom.toString()));*/

		//men set switch sesuai save-data
		switchBg.setChecked(MyFormat.toBoolean(myConfigSave.get(ENUM_general.useBg.toString())));
		switchSkipMenu.setChecked(skipIt);
		//strecthStatus.setChecked(MyFormat.toBoolean(myConfigSave.get(ENUM_monitor.stretch.toString())));
		barSwitch.setChecked(barUI);
		switchFullscren.setChecked(fullScreen);
		switchDiagonal.setChecked(MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.diagonalOn.toString())));
		switchBgBtn.setChecked(MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useBgBtn.toString())));
		switchExtKeypad.setChecked(MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useExtLabel.toString())));
	}
	
	
	
	
	

	/******************************************************************************************************************************************************************************************************
					SAVE EDIT FORM
	******************************************************************************************************************************************************************************************************/

	/*private ArrayList<String> batchKey()
	{
		ArrayList<String> keyIs = new ArrayList<>();
		
		keyIs.add(0, ENUM_general.bgColor.toString());
		keyIs.add(1, ENUM_general.orientation.toString());
		keyIs.add(2, ENUM_general.bar.toString());
		keyIs.add(3, ENUM_general.fullscreen.toString());
		
		return keyIs;
	}
	
	private ArrayList<String> batchValue()
	{
		ArrayList<String> valueIs = new ArrayList<>();

		valueIs.add(0, getTextField(bgColor));
		valueIs.add(1, getDataObjectInt());
		valueIs.add(2, String.valueOf(barSwitch.isChecked()));
		valueIs.add(3, String.valueOf(switchFullscren.isChecked()));

		return valueIs;
	}
	
	private void saveItBatch(String filename, ArrayList<String> key, ArrayList<String> value)
	{
		for (int loop=0; loop<key.size(); loop++)
		{
			PropertiesManager.saveDataProperties(filename, 
												 key.get(loop), 
												 value.get(loop));
		}
	}*/
	
	private String getTextField(EditText editText)
	{
		return editText.getText().toString();
	}

	//Overloading
	/*private String[] getTextField(EditText[] editText)
	{
		int arrLength = editText.length;
		String[] valueArray = new String[arrLength];

		for (int loop=0; loop<arrLength; loop++)
		{
			valueArray[loop] = editText[loop].getText().toString();
		}

		return valueArray;
	}*/
	
	/*private String getDataObjectInt()
	{
		int objInt = MyDataObject.getIntID();
		return ""+objInt;
	}*/

	//Overloading (Switch)
	/*private String[] getTextField(Switch[] theSwitch)
	{
		int arrLength = theSwitch.length;
		String[] valueArray = new String[arrLength];

		for (int loop=0; loop<arrLength; loop++)
		{
			valueArray[loop] = String.valueOf(theSwitch[loop].isChecked());
		}

		return valueArray;
	}*/

	private void saveDataConfig()
	{
		//skip menu
		SharedPreferences sharedPreferences = getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
		editor.putString(ENUM_general.skipMenu.toString(), String.valueOf(switchSkipMenu.isChecked()));
		editor.apply();
		
		//Library
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.libChoose.toString(), Integer.toString(theLib.getSelectedItemPosition()));
		
		//orientation
		//PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.orientation.toString(), getDataObjectInt());
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.orientation.toString(), Integer.toString(theRotation.getSelectedItemPosition()));
		
		//background color
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.bgColor.toString(), getTextField(bgColor));
		
		//background external
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.useBg.toString(), String.valueOf(switchBg.isChecked()));
		
		//resolution
		//'breConfig.toml'
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig, ENUM_breConfig.width.toString(), getTextField(resoX));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_breConfig, ENUM_breConfig.height.toString(), getTextField(resoY));
		//'myConfig.txt'
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.resoWidth.toString(), getTextField(resoX));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.resoHeight.toString(), getTextField(resoY));
		
		//scale type
		//PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.stretch.toString(), String.valueOf(strecthStatus.isChecked()));

		//scale
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_monitor.scale.toString(), getTextField(resoScale));

		//action bar
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.bar.toString(), String.valueOf(barSwitch.isChecked()));

		//fullscreen
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_general.fullscreen.toString(), String.valueOf(switchFullscren.isChecked()));

		//keypad
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.keypadLayout.toString(), Integer.toString(theKeypadLayout.getSelectedItemPosition()));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.labelSize.toString(), getTextField(labelSize));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.labelColor.toString(), getTextField(labelColor));
		//PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_keypad, ENUM_keypad.buttonWidth.toString(), getTextField(buttonX));
		//PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE_keypad, ENUM_keypad.buttonHeigt.toString(), getTextField(buttonY));
		/*PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.marginLeft.toString(), getTextField(marginLeft));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.marginTop.toString(), getTextField(marginTop));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.marginRight.toString(), getTextField(marginRight));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.marginBottom.toString(), getTextField(marginBottom));*/
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.diagonalOn.toString(), String.valueOf(switchDiagonal.isChecked()));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.useBgBtn.toString(), String.valueOf(switchBgBtn.isChecked()));
		PropertiesManager.saveDataProperties(PropertiesManager.PROPERTIES_FILE, ENUM_keypad.useExtLabel.toString(), String.valueOf(switchExtKeypad.isChecked()));
 	}



	/******************************************************************************************************************************************************************************************************
					GENERAL FUNCTION
	******************************************************************************************************************************************************************************************************/

	public static void toastMe(Context context, String s)
	{
		Toast.makeText(context, s, Toast.LENGTH_LONG).show();
	}
	
	public void toastMe(String s)
	{
		Toast.makeText(this, s, Toast.LENGTH_LONG).show();
	}
	
	private String getResourceString(int resource)
	{
		String str = getResources().getString(resource);
		
		return str;
	}



	/******************************************************************************************************************************************************************************************************
					BAR & MENU OPERATION
	******************************************************************************************************************************************************************************************************/

	private void setMyView()
	{
		if (fullScreen)
		{
			setTheme(R.style.fullscreenTheme);
		}
		
		setContentView(R.layout.main);
		
		toolbar = findViewById(R.id.myToolbarr);
        setSupportActionBar(toolbar);
        getSupportActionBar().setTitle("");
        getSupportActionBar().setDisplayHomeAsUpEnabled(false);
	
		if (!barUI)
		{
			hideBar();
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
		
		if (id == R.id.menu_item_exit)
		{
			AlertDialog.Builder dialogExit = new AlertDialog.Builder(this);
			dialogExit.setTitle(R.string.dialog_exit_title);
			dialogExit.setMessage(R.string.dialig_exit_msg);
			dialogExit.setPositiveButton(R.string.dialog_exit_btn_positive, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						// TODO: Implement this method
						finishAffinity();
						System.exit(0);
					}
				});
				
			dialogExit.setNeutralButton(R.string.dialog_exit_btn_neutral, new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						saveDataConfig();
						finishAffinity();
						System.exit(0);
						// TODO: Implement this method
					}
					
				
			});

			dialogExit.setNegativeButton(R.string.dialog_exit_btn_negative, new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface p1, int p2)
					{
						// TODO: Implement this method
					}	
				});
				
			AlertDialog dialog = dialogExit.create();
			dialog.show();
				
			return true;
		}
		
		// Tangani klik pada opsi menu ActionBar yang dipilih
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onBackPressed()
	{
		openOptionsMenu();
		
		/*new Handler().postDelayed(new Runnable() {
				@Override
				public void run()
				{
					hideBar();
				}		
		}, 3000);*/
	}

	

	
	
	
	
	/******************************************************************************************************************************************************************************************************
	 					WHEN ONCLICK
	 ******************************************************************************************************************************************************************************************************/
	
	 

}
