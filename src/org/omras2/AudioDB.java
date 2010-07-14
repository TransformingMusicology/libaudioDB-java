package org.omras2;

import java.io.File;

public class AudioDB
{
	public native boolean audiodb_create(String path, int datasize, int ntracks, int datadim);
	public native boolean audiodb_open(String path, Mode mode);
	public native Status audiodb_status();

//	public native void insert();
//	public native void query();

	public enum Mode { O_RDONLY, O_RDRW }

	private File path;
	private long adbHandle;

	public AudioDB(File path)
	{
		this.path = path;
	}

	public boolean create(int datasize, int ntracks, int datadim)
	{
		return audiodb_create(path.toString(), datasize, ntracks, datadim);
	}

	public boolean open(Mode mode)
	{
		return audiodb_open(path.toString(), mode);
	}

	public Status getStatus() 
	{
		return audiodb_status();
	}

	static {
		System.loadLibrary("AudioDB_JNI");
	}


	public static void main(String args[])
	{
		AudioDB testDB = new AudioDB(new File("test.adb"));
		testDB.create(5, 5, 12);
//		testDB.open(Mode.O_RDRW);
		Status status = testDB.getStatus();
		System.out.println("Num files: "+status.getNumFiles());
	}
}


