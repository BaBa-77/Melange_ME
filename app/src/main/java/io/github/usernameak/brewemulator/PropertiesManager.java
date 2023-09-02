package io.github.usernameak.brewemulator;

import java.util.Properties;
import java.io.File;
import android.os.Environment;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.EnumMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.Arrays;

enum ENUM_general
{
	profileUse,
	orientation, bar, fullscreen, bgColor, skipMenu, useBg, libChoose
}

enum ENUM_breConfig
{
	width, height
}

enum ENUM_monitor
{
	resoWidth, resoHeight,
	scale, stretch, posScreenPotrait, posScreenLandscape
}

enum ENUM_keypad
{
	keypad_, keypadLayout,
	diagonalOn, labelSize, labelColor, transparacy, 
	buttonWidth, buttonHeigt, buttonDpadWidth, buttonDpadHeight, buttonNumpadWidth, buttonNumpadHeight,
	posDpad,  posNumpad, posKeypad,
	marginTop, marginBottom, marginRight, marginLeft,
	useBgBtn, useExtLabel,
	_thePosPotrait, _theSizePotrait, _theAlphaPotrait, _visibilityPotrait,
	_thePosLandscape, _theSizeLandscape, _theAlphaLandscape, _visibilityLandscape,
	movementXY, scaleUp
}

public class PropertiesManager
{
	public static final String PROPERTIES_DIRECTORY = "MelangeBREW/sys/";
	public static final String PROPERTIES_DIRECTORY_PROFILES = "profiles/";
	public static String PROPERTIES_profileName = "";
	public static String PROPERTIES_profileName_default = "(default)";
	public static final String PROPERTIES_FILE = "myConfig.txt";
	public static final String PROPERTIES_FILE_breConfig = "breConfig.toml";
	//public static final String PROPERTIES_FILE_breConfig_profile = "breConfig.toml";
	public static final String PROPERTIES_FILE_myKeypad = "myKeypad.txt";
	public static final String PROPERTIES_DIRECTORY_AutoInput = "MelangeBREW/AutoInput/";
	
	public static final String PROPERTIES_FILE_TEST = "test.txt";
	
    public static void saveProperties(Properties properties, String fileName)
	{
		File file;
		
		if (fileName == PROPERTIES_FILE_breConfig)
		{
			file = new File(Environment.getExternalStorageDirectory(), PROPERTIES_DIRECTORY+fileName);
		}
		else
		{
			file = new File(Environment.getExternalStorageDirectory(), PROPERTIES_DIRECTORY+PROPERTIES_DIRECTORY_PROFILES+PROPERTIES_profileName+fileName);
		}
        
		try
		{
            FileOutputStream fileOut = new FileOutputStream(file);
            properties.store(fileOut, null);
			fileOut.flush();
            fileOut.close();
        }
		catch (IOException e)
		{
            e.printStackTrace();
        }
    }

    public static Properties loadProperties(String fileName)
	{
        Properties properties = new Properties();
       
		String path;

		if (fileName == PROPERTIES_FILE_breConfig)
		{
			path = PROPERTIES_DIRECTORY;
		}
		else
		{
			path = PROPERTIES_DIRECTORY+PROPERTIES_DIRECTORY_PROFILES+PROPERTIES_profileName;
		}
		
		File thePath=new File(Environment.getExternalStorageDirectory(), path);
		if (!thePath.exists())
		{
			thePath.mkdirs();
		}
		
		File file = new File(Environment.getExternalStorageDirectory(), path+fileName);
		
		if (!file.exists())
		{
			try
			{
				file.createNewFile();
			}
			catch (IOException e)
			{
				MyLog.w(file.toString()+": not exists");
			}
		}
        
		try
		{
            FileInputStream fileIn = new FileInputStream(file);
            properties.load(fileIn);
            fileIn.close();
        }
		catch (IOException e)
		{
            e.printStackTrace();
        }
		
        return properties;
    }
	
	public static String getDataProperties(String fileName, String key)
	{
		Properties properties = new Properties();
		properties = loadProperties(fileName);

		return properties.getProperty(key);
	}
	
	//Overloading (Batch)
	public static HashMap<String,String> getDataProperties(String fileName)
	{
		HashMap<String,String> newMap = new HashMap<>();
		
		ArrayList<String> theKey = switchFilename(fileName);

		Properties properties = new Properties();
		properties = loadProperties(fileName);

		for (String key : theKey)
		{
			newMap.put(key, properties.getProperty(key));
		}

		return newMap;
	}
	
	public static void saveDataProperties(String fileName, String key, String value)
	{
		Properties properties = new Properties();
		properties = loadProperties(fileName);
		properties.setProperty(key, value);
		saveProperties(properties, fileName);
	}

	//Overloading (Batch)
	/*public static void saveDataProperties(String fileName, String[] key, String[] value)
	{
		for (int loop=0; loop<key.length; loop++)
		{
			Properties properties = new Properties();
			properties = PropertiesManager.loadProperties(fileName);
			properties.setProperty(key[loop], value[loop]);
			PropertiesManager.saveProperties(properties, fileName);
		}
	}*/
	
	private static ArrayList<String> switchFilename(String fileName)
	{
		ArrayList<String> theKey = new ArrayList<>();

		switch (fileName)
		{
			case PROPERTIES_FILE:
				for (ENUM_general isKey : ENUM_general.values())
				{
					theKey.add(isKey.toString());
				}
				
				for (ENUM_monitor isKey : ENUM_monitor.values())
				{
					theKey.add(isKey.toString());
				}
				
				for (ENUM_keypad isKey : ENUM_keypad.values())
				{
					theKey.add(isKey.toString());
				}
				break;

			case PROPERTIES_FILE_breConfig:
				for (ENUM_breConfig isKey : ENUM_breConfig.values())
				{
					theKey.add(isKey.toString());
				}
				break;
		}
		
		return theKey;
	}
	
	/*public static void setProfile(String profileName)
	{
		PropertiesManager.PROPERTIES_DIRECTORY_PROFILES += "/"+profileName+"/";
	}*/
}
