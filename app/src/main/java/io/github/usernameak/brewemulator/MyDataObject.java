package io.github.usernameak.brewemulator;
import java.util.ArrayList;
import java.util.HashMap;

public class MyDataObject
{
	private static int intID = 0;
	private static HashMap<String,String> map = new HashMap<>();
	private static Boolean inCustom = false;
	private static Boolean inRec = false;
	private static Long miliTimes;
	
	/////////////////////// SET ///////////////////////
	
	public static void setIntID(String theStringID) throws NumberFormatException
	{
 		MyDataObject.intID = Integer.parseInt(theStringID);
    }

	public static void setIntID(int theIntID)
	{
        MyDataObject.intID = theIntID;
    }
	
	/*public static void setListFloat(ArrayList<Float> arrList)
	{
		MyDataObject.theListFloat = arrList;
	}*/
	
	/*public static void setListInt(ArrayList<Integer> arrList)
	{
		MyDataObject.theListInt = arrList;
	}*/

	public static void setCustom(Boolean theBoolean)
	{
		MyDataObject.inCustom = theBoolean;
	}
	
	public static void setRec(Boolean b)
	{
		MyDataObject.inRec = b;
	}
	
	public static void setMap(String key, String value)
	{
		MyDataObject.map.put(key, value);
	}
	
	public static void clearMapList()
	{
		MyDataObject.map.clear();
	}
	
	public static void setMilitimes(Long mt)
	{
		MyDataObject.miliTimes = mt;
	}
	
	
	
	////////////////////////// GET /////////////////////
	
	public static int getIntID()
	{
		return MyDataObject.intID;
	}
	
	/*public static ArrayList<Float> getListFloat()
	{
		return MyDataObject.theListFloat;
	}*/
	
	/*public static ArrayList<Integer> getListInt()
	{
		return MyDataObject.theListInt;
	}*/
	
	public static Boolean getCustom()
	{
		return MyDataObject.inCustom;
	} 
	
	public static Boolean getRec()
	{
		return MyDataObject.inRec;
	}
	
	public static HashMap<String,String> getMap()
	{
		return MyDataObject.map;
	}
	
	public static Long getMilitimes()
	{
		return MyDataObject.miliTimes;
	}
}
