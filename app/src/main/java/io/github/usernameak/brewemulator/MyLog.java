package io.github.usernameak.brewemulator;

import java.io.IOException;
import android.os.Environment;
import java.io.File;
import java.io.FileWriter;
import android.icu.util.Calendar;

public class MyLog
{
	public static void w(String text)
	{
		if (MainActivity.debugMode)
		{
			// Dapatkan instansiasi Calendar
			Calendar calendar = Calendar.getInstance();

			// Dapatkan jam, menit, detik, dan milidetik dari Calendar
			int jam = calendar.get(Calendar.HOUR_OF_DAY);
			int menit = calendar.get(Calendar.MINUTE);
			int detik = calendar.get(Calendar.SECOND);
			int milidetik = calendar.get(Calendar.MILLISECOND);

			String theTime = String.format("<font color=red><b>[</b></font><font color=green>%d</font>:<font color=green>%d</font>:<font color=green>%d</font>:<font color=green>%d</font><font color=red><b>]</b></font>", jam, menit, detik, milidetik);		

			try
			{
				File file = new File(Environment.getExternalStorageDirectory(), "MelangeBREW/my_debug.html");
				FileWriter fileWriter = new FileWriter(file, true); // Parameter true untuk menambahkan konten tanpa menggantikan
				fileWriter.write(theTime+"<br/><font color=#0000ff>"+text+"</font><br/>");
				fileWriter.flush();
				fileWriter.close();
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
		}	
	}
	
//	public static void d(String text)
//	{
//		if (MyLauncherActivity.debugUser)
//		{
//			// Dapatkan instansiasi Calendar
//			Calendar calendar = Calendar.getInstance();
//
//			// Dapatkan jam, menit, detik, dan milidetik dari Calendar
//			int jam = calendar.get(Calendar.HOUR_OF_DAY);
//			int menit = calendar.get(Calendar.MINUTE);
//			int detik = calendar.get(Calendar.SECOND);
//			int milidetik = calendar.get(Calendar.MILLISECOND);
//
//			String theTime = String.format("<font color=red><b>[</b></font><font color=green>%d</font>:<font color=green>%d</font>:<font color=green>%d</font>:<font color=green>%d</font><font color=red><b>]</b></font>", jam, menit, detik, milidetik);		
//
//			try
//			{
//				File file = new File(Environment.getExternalStorageDirectory(), "MelangeBREW/my_debug_user.html");
//				FileWriter fileWriter = new FileWriter(file, true); // Parameter true untuk menambahkan konten tanpa menggantikan
//				fileWriter.write(theTime+"<br/><font color=#0000ff>"+text+"</font><br/>");
//				fileWriter.flush();
//				fileWriter.close();
//			}
//			catch (IOException e)
//			{
//				e.printStackTrace();
//			}
//		}
//	}
	
	public static void clear()
	{
		try
		{
			File file = new File(Environment.getExternalStorageDirectory(), "MelangeBREW/my_debug.html");
			FileWriter fileWriter = new FileWriter(file);
			fileWriter.write("");
			fileWriter.flush();
			fileWriter.close();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}
	
//	public static void clear_user()
//	{
//		try
//		{
//			File file = new File(Environment.getExternalStorageDirectory(), "MelangeBREW/my_debug_user.html");
//			FileWriter fileWriter = new FileWriter(file);
//			fileWriter.write("");
//			fileWriter.flush();
//			fileWriter.close();
//		}
//		catch (IOException e)
//		{
//			e.printStackTrace();
//		}
//	}
}
