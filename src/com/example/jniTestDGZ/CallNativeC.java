package com.example.jniTestDGZ;

public class CallNativeC {
	static{
		System.loadLibrary("encryption");
	}
	
	public native double encryption();
}
