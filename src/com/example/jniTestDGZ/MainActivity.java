package com.example.jniTestDGZ;

import com.example.jni_test_dgz.R;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_main);
		
		/*TextView  tv = new TextView(this);
        tv.setText( stringFromJNI() );
        setContentView(tv);*/
		CallNativeC testNativeC =new CallNativeC();
		testNativeC.encryption();
		
		TextView tv=new TextView(this);
		tv.setText("生成100个128位秘钥所用时间为： "+Double.toString(testNativeC.encryption()));
		setContentView(tv);
	}
	
	

	/*@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
*/
}
