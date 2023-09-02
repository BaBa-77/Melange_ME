package io.github.usernameak.brewemulator;

import android.graphics.Color;

public class MyFormat
{
	public static int toInt(String s, int theDefault)
	{
		int i = theDefault;
		
		try
		{
			i = Integer.parseInt(s);
		}
		catch (Exception e){}
		/*catch (NumberFormatException e){}
		catch (IndexOutOfBoundsException e){}
		catch (NullPointerException p){}*/		
		
		return i;
	}
	
	public static Long toLong(String s, Long theDefault)
	{
		Long l = theDefault;

		try
		{
			l = Long.parseLong(s);
		}
		catch (Exception e){}		

		return l;
	}
	
	public static Float toFloat(String s, Float theDefault)
	{
		Float f = theDefault;
		
		try
		{
			f = Float.parseFloat(s);
		}
		catch(Exception e){}
		/*catch (NullPointerException e){}
		catch (IndexOutOfBoundsException e){}
		catch (NumberFormatException e){}*/
		
		return f;
	}
	
	public static int toColorInt(String s, int theDefauult)
	{
		int ic = theDefauult;
		
		try
		{
			ic = Color.parseColor("#"+s);
		}
		catch (Exception e){}
		/*catch (NullPointerException e){}
		catch (IndexOutOfBoundsException e){}
		catch (NumberFormatException e){}*/
		
		return ic;
	}
	
	public static Boolean toBoolean(String s)
	{
		Boolean b = false;
		
		try
		{
			b = Boolean.parseBoolean(s);
		}
		catch (Exception e){}
		/*catch (NullPointerException e){}
		catch (IndexOutOfBoundsException e){}
		catch (NumberFormatException e){}*/	
		
		return b;
	}
}
