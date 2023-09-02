package io.github.usernameak.brewemulator;

import android.content.Context;
import java.util.HashMap;
import android.graphics.Color;
import java.util.Map;
import android.widget.Button;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.widget.RelativeLayout;
import android.view.View;
import java.io.File;
import android.os.Environment;
import java.util.Scanner;
import java.io.IOException;
import android.view.View.OnTouchListener;
import android.view.MotionEvent;

public class KeypadBuilder
{
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
	
	private static String[][] KEYPAD_LABELS = null;
	private static int[][] KEYPAD_KEYCODES = null;
	
	public interface MyTouchListener
	{
		void MyTouch(Button button, MotionEvent event, int code, String codeStr)
	}
	
	public static String[][] getKeypadLabels(int typePad)
	{
		//get data from properties save-data
		HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);	

		Boolean extKeypad = MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useExtLabel.toString()));
		
		
		
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
		
		return KEYPAD_LABELS;
	}
	
	public static Button createButton(Context theContext, int typePad, int i, int i2)
	{
		if (KEYPAD_LABELS[i][i2] == null)
		{
			return null;
		}
		
		//get data from properties save-data
		HashMap<String,String> myConfigSave = PropertiesManager.getDataProperties(PropertiesManager.PROPERTIES_FILE);	

		//data label
		final Float styleSize = MyFormat.toFloat(myConfigSave.get(ENUM_keypad.labelSize.toString()), 25.00f);
		int styleColor = MyFormat.toColorInt(myConfigSave.get(ENUM_keypad.labelColor.toString()), Color.BLACK);
		Boolean bgImage = MyFormat.toBoolean(myConfigSave.get(ENUM_keypad.useBgBtn.toString()));

		int screen_Width = MainActivity.fr.getWidth();
		int screen_Height = MainActivity.fr.getHeight();
	
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
		
		MainActivity.fr.addView(btn, params);
		
		return btn;
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
