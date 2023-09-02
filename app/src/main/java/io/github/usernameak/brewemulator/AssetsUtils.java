package io.github.usernameak.brewemulator;

import android.content.Context;
import android.content.res.AssetManager;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class AssetsUtils
{
	public static void copyAssetsToExternalStorage(Context context, String assetsPath, String destinationPath) throws IOException
	{
		AssetManager assetManager = context.getAssets();
		String[] files = assetManager.list(assetsPath);

		if (files!=null && files.length>0)
		{
			File destinationDirectory = new File(destinationPath);
			
			if (!destinationDirectory.exists())
			{
				destinationDirectory.mkdirs();
			}

			for (String fileName : files)
			{
				String sourceFilePath = assetsPath+File.separator+fileName;
				String destinationFilePath = destinationPath+File.separator+fileName;
				
				if (!new File(destinationFilePath).exists())
				{
					copyFile(assetManager, sourceFilePath, destinationFilePath);
				}

				if (isDirectory(assetManager, sourceFilePath))
				{
					copyAssetsToExternalStorage(context, sourceFilePath, destinationFilePath);
				}
				/*else
				{
					copyFile(assetManager, sourceFilePath, destinationFilePath);
				}*/
			}
		}
	}

	private static boolean isDirectory(AssetManager assetManager, String filePath) throws IOException
	{
		InputStream inputStream = assetManager.open(filePath);
		boolean isDirectory = inputStream.available()==0;
		inputStream.close();
		
		return isDirectory;
	}

	private static void copyFile(AssetManager assetManager, String sourceFilePath, String destinationFilePath) throws IOException
	{
		InputStream inputStream = assetManager.open(sourceFilePath);
		OutputStream outputStream = new FileOutputStream(destinationFilePath);

		byte[] buffer = new byte[1024];
		int length;
		
		while ((length=inputStream.read(buffer))>0)
		{
			outputStream.write(buffer, 0, length);
		}

		inputStream.close();
		outputStream.close();
	}
}
