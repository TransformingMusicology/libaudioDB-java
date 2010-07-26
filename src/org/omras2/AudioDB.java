package org.omras2;

import java.io.File;

public class AudioDB
{
	public native boolean audiodb_create(String path, int datasize, int ntracks, int datadim);
	public native boolean audiodb_open(String path, Mode mode);
	public native void audiodb_close();
	public native Status audiodb_status();
	public native boolean audiodb_insert_path(String key, String features, String power, String times);
	public native void audiodb_query_by_key(String key, Query config);

	public enum Mode { O_RDONLY, O_RDWR }

	private File path;
	private long adbHandle;

	public AudioDB(File path)
	{
		this.path = path;
	}

	public void close()
	{
		audiodb_close();
	}

	public boolean insert(File features)
	{
		return audiodb_insert_path(null, features.getPath(), null, null);
	}
	
	public boolean insert(String key, File features)
	{
		return audiodb_insert_path(key, features.getPath(), null, null);
	}
	
	public boolean insert(String key, File features, File power, File times)
	{
		return audiodb_insert_path(key, features.getPath(), (power == null ? null : power.getPath()), (times == null ? null : times.getPath()));
	}
	
	public boolean create(int datasize, int ntracks, int datadim)
	{
		return audiodb_create(path.toString(), datasize, ntracks, datadim);
	}

	public boolean open(Mode mode)
	{
		return audiodb_open(path.toString(), mode);
	}

	public void query(String key, Query config)
	{
		audiodb_query_by_key(key, config);	
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
		testDB.open(Mode.O_RDWR);
		testDB.insert(new File("testfiles/testfeature"));
		Status status = testDB.getStatus();
		System.out.println("Num files: "+status.getNumFiles());
		testDB.close();
	}
}


